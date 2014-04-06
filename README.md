nginx tcp server
===================================  
	����nginx�ĸ����ܵ�TCP������ģ�顣����Э��(makecontext)ʵ�ִ��첽��
	��ģ�����nginx_tcp_proxy_module�޸Ķ�������Ŀ��ҳΪ��https://github.com/yaoweibin/nginx_tcp_proxy_module
	nginx_tcp_proxy_module��һ������nginx��TCP����ģ�顣

����Э�̵��첽ʵ�֣�
-----------------------------------
	����Э�̵��첽ʵ�ֻ���ԭ���������ģ�
	ͨ����дϵͳ��connect,recv,send,read,write���������IO������
	ÿһ���������ʱ������һ���µ�Э�̣������Э���У���������������ʱ(connect)����д��connect�����Զ���socket����Ϊ��������
	����Э���н��ж�дʱ�����socketδ����(���ɶ��򲻿�д)����д��send,recv�����Զ������ǰЭ�̣����ص������������������ڹ���ǰЭ�̵�ͬʱ�������socket��Ӷ���д���¼���������������������ʱ(�ɶ����д)���ٻ���֮ǰ�������Э�̡�

ͨ������˵����
-----------------------------------


���Դ�����룺
-----------------------------------
	cd path/to/nginx-tcp/
	./configure --prefix=/usr/local/nginx-tcp --conf-path=conf/nginx.conf --add-module=../ngx_tcp_testsvr --with-debug --without-pcre --without-http
	make && make install
	#�����������á�
	cp ngx_tcp_testsvr/conf/nginx.conf /usr/local/nginx-tcp/conf/
	cp ngx_tcp_testsvr/conf/testcfg.ini /usr/local/nginx-tcp/conf/
	#��������
	/usr/local/nginx-tcp/sbin/nginx
	#������Կͻ���
	cd path/to/ngx_tcp_testsvr/client/
	make
	#�鿴���԰�����
	./testcli -?
	./testcli -h [host] -t [threads] -r [request count] -f [function]  -?
	-f function: ######## functions ##########
          1:ADD 2:SUB 3:QUERY 4:SLEEP

	#����֮ǰ���Զ���װ����mysql,��ȷ��������ȷ��
	#�����ۼ�
	./testcli -h 127.0.0.1 -t 4 -r 1000 -f 1
	#�����ۼ�
	./testcli -h 127.0.0.1 -t 4 -r 1000 -f 2
	
	############# ����ͬ�����첽�Ĳ�� #############
	#����ͬ���ķ�����
	```
	�޸�/usr/local/nginx-tcp/conf/nginx.conf���ã�
	worker_processes  1; #ȷ��ֻ��һ��worker.
	use_async off;		 #�ر��첽
	#����nginx
	/usr/local/nginx-tcp/sbin/nginx -s stop
	/usr/local/nginx-tcp/sbin/nginx
	./testcli -h 127.0.0.1 -t 4 -r 10 -f 4
	#����������������£�
	requests,error,threads,totaltimes,  QPS
    	  10,    0,      4,    10.007,1.00
	************** All Test Is OK **************
	#���ڷ������ǵ����̣����̲߳���ÿ����������1�롣����10��������Ҫ10����ɣ���ʹ�ͻ�����4���߳�Ҳ����ˣ�����˵����ͬ��������£����̱��������ʱ��û�а취������������

	#�����첽�ķ�����
	�޸�/usr/local/nginx-tcp/conf/nginx.conf���ã�
	worker_processes  1; #ȷ��ֻ��һ��worker.
	use_async on;		 #�����첽
	#����nginx
	/usr/local/nginx-tcp/sbin/nginx -s stop
	/usr/local/nginx-tcp/sbin/nginx
	./testcli -h 127.0.0.1 -t 4 -r 10 -f 4
	#����������������£�
	requests,error,threads,totaltimes,  QPS
    	  10,    0,      4,     3.005,3.33
	************** All Test Is OK **************
	#���������ǵ����̣����̲߳���ÿ����������1�롣���������첽�ģ���һ�����󱻺������ʱ�����ܴ�����������
	
	```
	

����ṹ˵����
-----------------------------------
###	nginx-tcp 
	nginx��1.2.1Դ���룬�����src/tcp_asyncĿ¼�����������tcp�������ĺ���ģ�飬����ʵ�����첽��
	����nginxĬ��ʵ���У��ͻ������Ӳ�����ȫ���ؾ���(http�Ƕ����ӱȽ϶࣬������ⲻ�������)�����ִ���������޸ģ�����ӦTCP�����ӵ��龰��

### ngx_tcp_testsvr ʾ��TCP��������
	һ����׼�Ļ���nginx tcp server�ķ�����ʵ�֡�

### ngx_tcp_testsvr/test_protocol.h  	TCP��������һЩָ��ͷ��ָ���壬�����ֵȡ�
	��������ʵ�ֲ���Ӧ��ʽ���������ɿͻ��˷�������Ȼ�������һ������һ����Ӧ�������Ϊ������ͷ+�����壬��ӦΪ�֣���Ӧͷ+��Ӧ�塣
	����ͷ����Ӧͷ ������ù̶����ȣ�һ����������ṹ�壬���б������һ�������ֶΣ���һ���������ֶΡ�
	���������Ӧ�壬�����Ƕ����Ƹ�ʽ��Ҳ�������ı���ʽ������ָ����Ľ��������߼�ʵ�ִ����н����ġ�
###	ngx_tcp_testsvr/test_impl.h		 	TCP������������������Ķ��塣��������Ӧָ��Ľ�����
	���������涨���˳��õ����ã���������һ�㶨��һЩ��Ҫ�־ñ������ӣ�����
	
### ngx_tcp_testsvr/test_impl.cpp 		������Ľ����������ĳ�ʼ����Э��ָ��Ľ���������
	xxx_impl.cpp��һ��Ҫ��������ص�������
	1. ����������������ĳ�ʼ����غ����ص���
	```
	typedef struct app_ctx_t {
		app_cfg_new_pt app_cfg_new;			//��������ʵ����
		app_cfg_init_pt app_cfg_init;		//�������ó�ʼ��
		app_ctx_new_cb app_ctx_new;			//����������ʵ����
		app_ctx_init_cb app_ctx_init;		//���������ĳ�ʼ��
		app_ctx_destroy_cb app_ctx_destroy;	//��������������
		app_cfg_destroy_cb app_cfg_destroy;	//������������
		app_exit_master_cb app_exit_master;	//master�˳�ʱ���ûص�
	}app_ctx_t;
	```
	���÷���Ϊֱ�Ӷ���һ����Ϊ��g_app_ctx�ı�����
	```
	app_ctx_t g_app_ctx ={&test_cfg_new,&test_cfg_init,&test_ctx_new, &test_ctx_init,&test_ctx_destroy, &test_cfg_destroy,NULL};
	```

	2. Э��ָ�����������صĺ����ص���
	```
	typedef struct ngx_tcp_protocol_info_s{
		uint32_t req_head_size;			//����ͷ��С(���ڿ�ܷ����ڴ棬��������ͷ)
		uint32_t rsp_head_size;			//��Ӧͷ��С(���ڿ�ܷ����ڴ棬������Ӧͷ)
		ngx_tcp_get_req_body_size_pt get_req_body_size;	//��ȡ�������С(������ͷ�л�ȡ)
		ngx_tcp_get_rsp_body_size_pt get_rsp_body_size;		//��ȡ��Ӧ���С(����Ӧͷ�л�ȡ)
		ngx_new_req_head_pt new_req_head;				//��������ͷ�Ŀռ�
		ngx_new_rsp_head_pt new_rsp_head;				//������Ӧͷ�Ŀռ�
		ngx_tcp_preproc_req_header_pt preproc_req_header;	//����������ͷ֮�������ͷ���д���(�����ֽ���ת�������ܵ�)
		ngx_tcp_preproc_req_body_pt preproc_req_body;		//������������֮�����������д���(�����ֽ���ת�������ܵ�)
		ngx_tcp_preproc_rsp_header_pt preproc_rsp_header;	//������Ӧͷ֮ǰ����Ӧͷ���д���(�����ֽ���ת�������ܵ�)
		ngx_tcp_preproc_rsp_body_pt preproc_rsp_body;		//������Ӧ��֮ǰ����Ӧ����д���(�����ֽ���ת�������ܵ�)
		ngx_tcp_free_req_pt free_req;		//�ͷ���������ڴ�
		ngx_tcp_free_rsp_pt free_rsp;		//�ͷ���Ӧ����ڴ�
		ngx_tcp_debug_req_pt debug_req; 	//����ͷ�������������ɺ󣬽��������ӡ�ķ�����
		ngx_tcp_debug_rsp_pt debug_rsp;	//��Ӧͷ����Ӧ�巢��֮ǰ�����������ӡ�ķ�����
		ngx_set_rsp_code_pt set_rsp_code;		//������������ɺ󣬽�����ֵ���õ���Ӧͷ�ķ�����
		ngx_tcp_debug_stats_pt debug_stats;	//������ɺ��������ͳ����Ϣ�ķ�����
		ngx_tcp_req_again_pt req_again;		//�����������յ�E_AGAIN�źź󣬵��õķ�����
	}ngx_tcp_protocol_info_t;
	```
	3. ���������ص���
	```
	typedef int (*ngx_tcp_proc_pt)(ngx_tcp_req_t* req, ngx_tcp_rsp_t* rsp);
	```
	2��3�����÷���Ϊʵ��һ�����ú���������ֵ����g_tcp_set_callbacks��
	```
	int test_set_callbacks(ngx_tcp_data_t* tcp_data){
		memcpy(&tcp_data->protocbs, &g_tcp_cbs_test, sizeof(tcp_data->protocbs));
		tcp_data->tcp_proc = &test_proc;
		return 0;
	}	
	ngx_set_callbacks_pt g_tcp_set_callbacks = &test_set_callbacks;
	```

### ngx_tcp_testsvr/test_service.cpp	TCP���������߼�ʵ�֡�
	��ҪҪʵ�����渳ֵ��tcp_data->tcp_proc���������ҵ���߼�������
	int test_proc(ngx_tcp_req_t* req, ngx_tcp_rsp_t* rsp);
	��ʵ��һ����ȡ������ͷ��Ȼ���������ͷ�е�ָ��Ž��зֱ���

  
���Գ���ngx_tcp_testsvr/client
--------------------------------------
	�������ڲ���ngx_tcp_testsvr����Ŀͻ��˹��ߡ�

