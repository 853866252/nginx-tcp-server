nginx tcp server
===================================  
	����nginx�ĸ����ܵ�TCP������ģ�顣����Э��(makecontext)ʵ�ִ��첽��
	�첽֧��mysql,redis�ȳ��õĻ���TCP��������
	��ģ�����nginx��stream������ֻ֧��1.9.x���ϵİ汾��

* [ChangeLogs](./ChangeLogs.md "ChangeLogs")

����Э�̵��첽ʵ�֣�
-----------------------------------
	����Э�̵��첽ʵ�ֻ���ԭ���ǣ�
	ͨ����дϵͳ��connect,recv,send,read,write���������IO������
	ÿһ���������ʱ������һ���µ�Э�̣������Э���У������µ�����
	����ʱ(connect)����д��connect�����Զ���socket����Ϊ��������
	����Э���н��ж�дʱ�����socketδ����(���ɶ��򲻿�д)��
	��д��send,recv,read,write�����Զ������ǰЭ�̣�
	���ص������������������ڹ���ǰЭ�̵�ͬʱ���
	��socket��Ӷ���д���¼���������������������ʱ(�ɶ����д)��
	�ٻ���֮ǰ�������Э�̡�
��������˵��
```

stream {
	server{
		listen 2014;
		tcp_server; # ����tcp_serverģ�顣
		# tcp_server�����������ã���Ҫ�����ݿ⣬�����������á�
		appcfgfile conf/testcfg.ini;
		
		bizlog on; #��ҵ����־��Ĭ��Ϊoff
		# ��־����error,warn,info,debug,debug2,all, Ĭ��Ϊinfo
		log_level debug; 
		# ERROR,WARN,INFO������־�ļ�
		logfile  logs/test.log; 
		# DEBUG, DEBUG2������־�ļ�
		debugfile logs/test.debug;

		use_async on; #�Ƿ����첽��Ĭ��Ϊon
		tcp_nodelay on; # ��������Ϊnodelayģʽ, Ĭ��Ϊon
		so_keepalive off; # �������ӵ�keepalive��Ĭ��Ϊoff

		#���ӳ�ʱʱ�䣬�������ӳ�����ʱ��δ���κ�����ʱ����ر����ӡ�Ĭ��Ϊ10m;
		timeout 10m; 
		# ��Ϣ���ճ�ʱʱ�䣬Ĭ��Ϊ3s
		timeout_recv 3s;
		# ��Ϣ���ͳ�ʱʱ�䣬Ĭ��Ϊ2s
		timeout_send 2s;
		# �������ʱ�����ͳ�ʱʱ�䣬Ĭ��Ϊ5s
		backend_timeout_send 5s;
		# �������ʱ�����ճ�ʱʱ�䣬Ĭ��Ϊ10s
		backend_timeout_recv 10s;
	}
}
```

�����𲽣�
-----------------------------------
    # �������Ŀ¼
	cd path/to/nginx-tcp/
	# ��װĿ¼����
	export TCP_SERVER=/usr/local/nginx-tcp-server
	# ������򣬼�Demo�������
	./configure --prefix=$TCP_SERVER \
    --with-debug --without-pcre --without-http \
    --with-stream  --add-module=src/tcp_svr \
    --add-module=src/tcp_svr/demo
	make -j 4
	make install
	#�����������á�
	cp -f src/tcp_svr/demo/conf/nginx.conf $TCP_SERVER/conf/
	cp -f src/tcp_svr/demo/conf/testcfg.ini $TCP_SERVER/conf/
	#��������
	$TCP_SERVER/sbin/nginx
	#������Կͻ���
	cd src/tcp_svr/demo/client/
	make
	#�鿴���԰�����
	./testcli -?
	>./testcli -h [host] -t [threads] -r [request count] -f [function]  -?
	>-f function: ######## functions ##########
    >      1:ADD 2:SUB 3:QUERY 4:SLEEP

	#����֮ǰ�����а�װ������mysql,��ȷ������(testcfg.ini)��ȷ��
	#�����ۼ�
	./testcli -h 127.0.0.1 -t 4 -r 1000 -f 1
	#�����ۼ�
	./testcli -h 127.0.0.1 -t 4 -r 1000 -f 2
	
	############# ����ͬ�����첽�Ĳ�� #############
	#����ͬ���ķ�����
	```
	�޸�$TCP_SERVER/conf/nginx.conf���ã�
	worker_processes  1; #ȷ��ֻ��һ��worker.
	use_async off;		 #�ر��첽
	#����nginx
	$TCP_SERVER/sbin/nginx -s stop
	$TCP_SERVER/sbin/nginx
	#���в��Գ���(-h ָ��Host, -t ָ���߳�����-rָ����������-fָ�����Թ���)
	./testcli -h 127.0.0.1 -t 4 -r 10 -f 4 
	#����������������£�
	requests,error,threads,totaltimes,  QPS
          10,    0,      4,    10.007,  1.00
	************** All Test Is OK **************
	#���ڷ������ǵ����̣����̲߳���ÿ����������1�롣����10��������Ҫ10����ɣ�������QPSֻ��1��
	#(��ʹ�ͻ�����4���߳�Ҳ�����,���Գ���ʱʱ��Ϊ10s,̫�̻ᵼ�º��������ʱ)��
	#��˵����ͬ��������£����̱��������ʱ��û�а취������������

	#�����첽�ķ�����
	�޸�$TCP_SERVER/conf/nginx.conf���ã�
	worker_processes  1; #ȷ��ֻ��һ��worker.
	use_async on;		 #�����첽
	#����nginx
	$TCP_SERVER/sbin/nginx -s stop
	$TCP_SERVER/sbin/nginx
	./testcli -h 127.0.0.1 -t 4 -r 10 -f 4 
	#����������������£�
	requests,error,threads,totaltimes,  QPS
          10,    0,      4,     3.006,  3.33
	************** All Test Is OK **************
	#���������ǵ����̣�����ÿ����������1�롣���������첽�ģ�
	# ��һ�����󱻺������ʱ�����ܴ�����������
	# (���ڲ��Կͻ�����ʹ�õ�ͬ����ʽ������QPS�����ܻ�����߳�����
	# ����ͻ���Ҳ�����첽��ʽ�����ܻ����)
	```
	

����(demo)�ṹ˵����
-----------------------------------
###	nginx-tcp 
	nginx��1.9.3Դ���룬�޸������²��֣�����Ӧtcp_svrҪ��
	1. auto/make�ļ��޸ģ���֧��C++��tcp_svrģ����Ҫ����C++��д��
	
### nginx-tcp/src/tcp_svr
    tcp�������ĺ���ģ�飬ʵ����Э�̼��첽IO�ķ�װ��
* ngx_log_mod.* ��־��صĴ���
* ngx_tcp_async_proc.* �첽��صĴ���
* ngx_tcp_def_protocol.* Ĭ�ϵ�Э�鴦��ʵ�֡�(���Բο���ʵ���Լ���Э�鴦��)
* ngx_tcp_server.h ģ��ͷ�ļ�����ص����ݽṹ�����������������
* ngx_tcp_server_module.cpp ģ��ʵ�����ļ���tcp_svr�ĺ����߼��������

### nginx-tcp/src/tcp_svr/demo
	һ����׼�Ļ���nginx tcp server�ķ�����ʵ�֣������Կͻ��ˡ�
* test_protocol.h 

```
TCP��������һЩָ��ͷ(ʹ�ÿ��Ĭ�϶����ָ��ͷ)��ָ���壬�����ֵȡ�
��������ʵ�ֲ���Ӧ��ʽ���������ɿͻ��˷�������Ȼ�������һ������һ����Ӧ��
�����Ϊ������ͷ+�����壬��ӦΪ�֣���Ӧͷ+��Ӧ�塣
����ͷ����Ӧͷ ������ù̶����ȣ�һ����������ṹ�壬���б������һ�������ֶΣ���һ���������ֶΡ�
���������Ӧ�壬�����Ƕ����Ƹ�ʽ��Ҳ�������ı���ʽ������ָ����Ľ��������߼�ʵ�ִ����н����ġ�
```

* test_impl.*

    * test_config_t ������������塣
    * TestContext �����������ģ���Ҫ�������ý��������ݿ����ӳس�ʼ�ȷ�������ʼ�����̡�
    * TestProtocol ������Э�鴦���࣬�̳���CDefProtocol��Ҳ�����Լ�ʵ��һ��Э�鴦���ࡣ
    * g_context_creater ��Ҫ�����������ģ��ú����ᱻtcp_svr��ܵ��á�
    * g_tcp_set_callbacks ��Ҫ�����ûص��ķ�������Ҫ��DemoЭ�鴦��ʵ����ҵ�����������ϣ�����ܵ��á�
* test_service.cpp

```
TCP���������߼�ʵ�֡�
��ҪҪʵ�����渳ֵ��tcp_data->tcp_proc���������ҵ���߼�������
extern int test_proc(ngx_tcp_req_t* req, ngx_tcp_rsp_t* rsp)
��ʵ��һ����ȡ������ͷ��Ȼ���������ͷ�е�ָ��Ž��зֱ���
```
  
���Գ���nginx-tcp/src/tcp_svr/demo/client
--------------------------------------
	�������ڲ���tcp_svr/demo����Ŀͻ��˹��ߡ�

����nginx-tcp-server��Hello World���򿪷�
--------------------------------------
[Hello Worldʾ������](./HelloWorld.md "Hello Worldʾ������")

Authors
=======

* liuxiaojie (��С��)  <jie123108@163.com>


Copyright & License
===================

This module is licenced under the BSD license.

Copyright (C) 2015, by liuxiaojie (��С��)  <jie123108@163.com>

All rights reserved.
