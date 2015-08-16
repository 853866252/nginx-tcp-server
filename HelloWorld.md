����˵��
--------------------------------
HelloWorld���ܷǳ��򵥣��ͻ��˿��Է�������ָ�
* ��¼ָ��0x1: �����ַ�����${username}������������һ��"Hello ${username},${welcome}!"�ַ������е�${welcome}�Ǵ������ж�ȡ�����ݡ�
* �˳�ָ��0x2: �����ַ�����${username}������������һ��"Bye ${username}!"�ַ���
* <b>���ڱ�ʾ����û��ʹ��Ĭ�ϵ�CDefProtocolЭ��ʵ�֣��Լ����¶���������ͷ����Ӧͷ����Э�鴦���ϸ�ڣ���ʹ���뿴������Щ���������ֱ�Ӽ̳�CDefProtocol��򻯺ܶࡣ</b>

ʵ�ֲ���
--------------------------------
##### ʵ��������HelloContext, �����ı���ʵ��IContext�ӿڣ��������º�����
* cfg_init �����ļ���ʼ��
* ctx_init �����ĳ�ʼ����һ���ǽ������ݿ⣬����������صĳ�ʼ��
* destroy ��Դ����

ʵ����μ�Դ�����е�HelloContext�ࡣ

##### ����һ��MAGICֵ
    #define MAGIC 0xa0b1
    #define MAGIC_BIG 0xb1a0

##### �������ָ����ֲ�ͬ�ķ���
    #define CMD_LOGIN 0x1
    #define CMD_EXIT 0x2

##### ���������
    #define ERRNO_OK	0					//�ɹ�	
    #define ERRNO_SYSTEM	1				//ϵͳ����	���нӿ�
    #define ERRNO_REQ_INVALID	2			//�����������	���нӿ�

##### ���������弰��Ӧ��
    #ʵ��ʹ���У����ܶ��ڲ�ͬ��ָ���ʹ�ò�ͬ��������Ӧ�塣
    typedef struct {
    	char data[4]; //����ʵ�ʳ����Ǳ䳤�ģ����Կ����Ѿ���ȡ��\0Ϊֹ��
    }hello_req_dt;
    
    typedef struct {
    	char data[4]; //����ʵ�ʳ����Ǳ䳤�ģ����Կ����Ѿ���ȡ��\0Ϊֹ��
    }hello_rsp_dt;

##### ��������ͷ����Ӧͷ
* ����ͷ������magic,len,cmd������len�Ǳ���ģ�magic��Ϊ���ܸ������ʶ��һ������������㴦���õġ�cmd��ʶ��ͬ�������, cmd�ֶ�Ҳ���Էŵ���Ϣ�嵱�С��������£�
    
```
typedef struct {
	uint16_t magic; //Э��ʶ���룬Ϊһ���̶�ֵ��������Ӧ����ͬ��
	uint32_t len; 	//ָ���峤�ȣ�0��ʾû��ָ���塣
	uint16_t cmd;	//ָ��š�
}__attribute__ ((packed)) hello_req_header_t;
```
* ��Ӧͷ������magic,len,cmd,code��codeΪ������Ӧ�롣�������£�

```
typedef struct {
	uint16_t magic; //Э��ʶ���룬Ϊһ���̶�ֵ��������Ӧ����ͬ��
	uint32_t len; 	//ָ���峤�ȣ�0��ʾû��ָ���塣
	uint16_t cmd;	//ָ��š�
	uint16_t code;  
}__attribute__ ((packed)) hello_rsp_header_t;
```
##### Э�鴦���ඨ��
Э�鴦������Ҫʵ��IProtocol�ӿڣ���ȻҲ����ֱ�Ӽ̳�CDefProtocol��ʹ��Ĭ�ϵ�ʵ�֡����߲ο�CDefProtocolʵ���Լ���Э�鴦���ࡣIProtocol�ӿڶ������£�
```
class IProtocol {
public:
	// ��������ͷ��������һ���̶����ȵ��ڴ�(��ṹ��)
	virtual req_head_t* new_req_head(ngx_pool_t* pool,uint16_t* size)=0;
	// ��ʼ��Ӧͷ��������һ���̶����ȵ��ڴ�(��ṹ��)
	virtual rsp_head_t* new_rsp_head(ngx_pool_t* pool, req_head_t* header,uint16_t* size)=0;
	// ��ȡ�������С(������ͷ�л�ȡ)
	virtual size_t get_req_body_size(req_head_t* header)=0;
	// ��ȡ��Ӧ���С(����Ӧͷ�л�ȡ)
	virtual size_t get_rsp_body_size(rsp_head_t* header)=0;

	// ����������ͷ֮�������ͷ���д���(�����ֽ���ת�������ܵ�)
	virtual int preproc_req_header(ngx_tcp_req_t* req)=0;
	// ������������֮�����������д���(�����ֽ���ת�������ܵ�)
	virtual int preproc_req_body(ngx_tcp_req_t* req)=0;
	// ������Ӧͷ֮ǰ����Ӧͷ���д���(�����ֽ���ת�������ܵ�)
	virtual int preproc_rsp_header(ngx_tcp_rsp_t* rsp)=0;
	// ������Ӧ��֮ǰ����Ӧ����д���(�����ֽ���ת�������ܵ�)
	virtual int preproc_rsp_body(ngx_tcp_rsp_t* rsp)=0;

	// ����ͷ�������������ɺ󣬽��������ӡ�ķ�����
	virtual void debug_req(ngx_tcp_req_t* req)=0;
	// ��Ӧͷ����Ӧ�巢��֮ǰ�����������ӡ�ķ�����
	virtual void debug_rsp(ngx_tcp_rsp_t* rsp)=0;

	// �ͷ���������ڴ�
	virtual void free_req(ngx_pool_t* pool,ngx_tcp_req_t* req)=0;
	// �ͷ���Ӧ����ڴ�
	virtual void free_rsp(ngx_pool_t* pool,ngx_tcp_rsp_t* rsp)=0;

	// ������������ɺ󣬽�����ֵ���õ���Ӧͷ�ķ�����
	virtual void set_rsp_code(ngx_tcp_rsp_t* rsp, int ret)=0;
	// ������ɺ��������ͳ����Ϣ�ķ�����
	virtual void debug_stats(ngx_tcp_data_t* data)=0;
	//����������ʱ������TCP_AGAINʱ���õĻص���
	virtual int req_again(ngx_tcp_req_t* req)=0; 
};
```
Э�鴦����ΪCHelloProtocol����ֱ�Ӳ鿴Դ���룬���ﲻ�г���

##### ������ص����弰ʵ��
������ص���Ҫ��ȡ������ͷ��Ȼ�����ָ�������Ӧ�ķ�������Ҫ�������£�
```
int hello_proc(ngx_tcp_req_t* req, ngx_tcp_rsp_t* rsp)
{
	int ret = 0;
	hello_req_header_t* header = (hello_req_header_t*)req->req_header;

	switch(header->cmd){
	case CMD_LOGIN:
		ret = hello_login(req, rsp);
	break;
	case CMD_EXIT:
		ret = hello_exit(req, rsp);
	break;	
	default:
		ret = ERRNO_REQ_INVALID;
		NLOG_ERROR("unexpected cmd [0x%04x], ip:%V", header->cmd, &req->c->addr_text);
	}
  
	return ret;
}
#�����ҵ���������ﲻ���г�����ֱ�Ӳο�Դ���롣
```

##### ���������ģ�Э�鴦��������ص�
���϶���������ģ�Э�鴦���༰������������Ҫ��nginx-tcp-server����ܹ����õ���������ûص���ʽ��
* ʵ��g_context_creater�������ú����ǹ�nginx-tcp-server��ܵ��õģ����ڴ�������HelloContext��һ��ʵ�����ú���ԭ�����£�

```
extern IContext* g_context_creater(ngx_conf_t *cf);
```

ʵ�����£�
```
extern IContext* g_context_creater(ngx_conf_t *cf)
{
	IContext* context = new HelloContext(cf);
	return context;
}
```
* ʵ��g_tcp_set_callbacks�ص����ûص�����ԭ�����£�

```
typedef int (*ngx_set_callbacks_pt)(ngx_tcp_data_t* tcp_data);
```
ʵ�����£�
```
# g_hello_protocol ΪCHelloProtocol���һ����̬ʵ����
# hello_proc Ϊ������ص���
int hello_set_callbacks(ngx_tcp_data_t* tcp_data){
	tcp_data->cppcbs = (IProtocol*)&g_hello_protocol;
	tcp_data->tcp_proc = &hello_proc;
	return 0;
}
```

##### config�ļ���д
config�ļ����ڰ���ӵ�Դ�ļ��������ĵ���������ӽ�������Ҫʹ�õ����¼���������
* NGX_ADDON_SRCS Դ�ļ���ӵ��ñ����С�
* CFLAGS ���������ӵ��ñ����С�
* CORE_LIBS ���Ӳ�����ӵ��ñ����С�

```
NGX_ADDON_SRCS="$NGX_ADDON_SRCS $ngx_addon_dir/helloworld.cpp $ngx_addon_dir/IniFile.cpp"
#���ڱ�ʾ��δ����������⣬CFLAGS,CORE_LIBS������Ҫ���塣
```

##### ����
```
export TCP_SERVER=/usr/local/nginx-tcp-server_hello
./configure --prefix=$TCP_SERVER \
--with-debug --without-pcre --without-http \
--with-stream  --add-module=src/tcp_svr \
--add-module=/path/to/hello-world
make -j 4
make install
```

##### ����
* ����nginx.conf���ɲο�hello-world/conf/nginx.conf����
* ����Ӧ������hellocfg.ini(��appcfgfileָ��ָ��)��

```
[hello]
welcome=welcome to use nginx-tcp-server 2.0
```

##### ���Կͻ���
����Э����ö����ƣ����Է��������ֱ��ʹ��telnet, nc�ȷ�ʽ����Ҫ��Э��ʵ��һ�����Կͻ��ˡ�������μ�"client.cpp"�ļ���
* ����ʹ�� `make`
* ���ԣ�

```
./client 127.0.0.1 2014 1 lxj
>Hello lxj,welcome to use nginx-tcp-server 2.0
./client 127.0.0.1 2014 2 lxj
>Bye lxj
./client 127.0.0.1 2014 3 lxj
error: tcp server return code:2
```

ע������
--------------------------------
* ngx_tcp_server.h�����ǵ�һ������(#include)���ļ���������ܵ��½ṹ���С��һ�¡�

Authors
=======

* liuxiaojie (��С��)  <jie123108@163.com>


Copyright & License
===================

This module is licenced under the BSD license.

Copyright (C) 2015, by liuxiaojie (��С��)  <jie123108@163.com>

All rights reserved.
