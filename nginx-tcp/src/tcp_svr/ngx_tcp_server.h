
#ifndef _NGX_TCP_SERVER_H_INCLUDED_
#define _NGX_TCP_SERVER_H_INCLUDED_
extern "C"{
#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_event.h>
#include <ngx_event_connect.h>
#include <ucontext.h>
}

inline double ngx_second(void) {
    struct timeval tv;
    ngx_gettimeofday(&tv);
    return (double)tv.tv_sec+tv.tv_usec/1000000.0;
}

class IContext;

typedef struct ngx_tcp_server_srv_conf_s{
    ngx_msec_t  timeout;
	
	ngx_msec_t timeout_recv;
	ngx_msec_t timeout_send;
	ngx_msec_t timeout_proc;
	ngx_msec_t backend_timeout_send;
	ngx_msec_t backend_timeout_recv;
	ngx_uint_t stack_size;
	ngx_flag_t so_keepalive;
	ngx_flag_t tcp_nodelay;
	ngx_flag_t use_async;
	
	ngx_str_t appcfgfile;
	//void* appcfg;
	IContext* appctx;
}ngx_tcp_server_srv_conf_t;

/**
 * �����Ľӿ�
 */
class IContext {
public:
	virtual ~IContext(){};
	// �����ļ���ʼ����
	virtual int cfg_init(const char* config, ngx_tcp_server_srv_conf_t* tcp_svr_cfg)=0;
	// �����ĳ�ʼ����һ���ǽ������ݿ⣬����������صĳ�ʼ����
	virtual int ctx_init(ngx_cycle_t* cycle)=0;
	// ��Դ���١�
	virtual void destroy(ngx_cycle_t* cycle)=0;
};

extern IContext* g_context_creater(ngx_conf_t *cf);

typedef struct ngx_tcp_event_s{
	ngx_event_t ev;
	ngx_uint_t sec;
	ngx_event_handler_pt handler;
	void* args;
}ngx_tcp_event_t;

inline void ngx_tcp_event_handler_wrapper(ngx_event_t *ev){
	ngx_tcp_event_t* tcp_ev = (ngx_tcp_event_t*)ev->data;
	tcp_ev->handler(ev);
	ngx_add_timer(ev, tcp_ev->sec*1000);
}

inline void ngx_tcp_event_init(ngx_log_t *log, ngx_tcp_event_t* tcp_ev
		, ngx_uint_t sec, ngx_event_handler_pt handler, void* args){
	ngx_memzero(tcp_ev, sizeof(ngx_tcp_event_t)); 
	tcp_ev->handler = handler;
	tcp_ev->sec = sec;
	tcp_ev->args = args;
	tcp_ev->ev.handler = ngx_tcp_event_handler_wrapper;  
	tcp_ev->ev.log = log;  
	tcp_ev->ev.data = tcp_ev;
  	tcp_ev->ev.timer_set = 0;
	ngx_add_timer(&tcp_ev->ev, tcp_ev->sec*1000);
}

inline void ngx_tcp_event_destroy(ngx_tcp_event_t* tcp_ev){
	if(tcp_ev->ev.timer_set)ngx_del_timer(&tcp_ev->ev);
}


#define NGX_PFREE(pool, ptr)\
	if(ptr != NULL){\
		ngx_tcp_pfree(pool, ptr);\
		ptr = NULL;\
	}

typedef enum NGX_RECV_STAT_S{
	RS_AGAIN=-2,
	RS_ERROR=-1,
	RS_CLOSE=0,
	RS_OK=1,
	RS_ERROR_REQ_INVALID,
	RS_TIMEDOUT
}NGX_RECV_STAT_T;

#pragma pack(push) //�������״̬
#pragma pack(1) //����1�ֽڶ���

typedef void req_head_t;
typedef void rsp_head_t;

struct ngx_tcp_data_s;

typedef struct ngx_tcp_req_s{
	req_head_t* req_header;
	void* body;
	uint16_t req_header_size;// sizeof real req_head_t
	size_t header_len:16;//received header size.
	size_t body_len; //received body size
	ngx_connection_t* c;
	struct ngx_tcp_data_s* tcp_data;
	ngx_flag_t isbigendian;
}ngx_tcp_req_t;

typedef struct ngx_tcp_rsp_s{
	rsp_head_t* rsp_header;
	uint16_t rsp_header_size;// sizeof real rsp_head_t
	void* body;
	size_t body_len; //body size.
	ngx_connection_t* c;
	struct ngx_tcp_data_s* tcp_data;
	ngx_flag_t isbigendian;
	ngx_chain_t* rsp_chain;//���͵�chain��������
	ngx_chain_t* rsp_rest; //���ͺ�ʣ��ġ�
	ngx_uint_t rsp_send_times; //���ʹ�����
}ngx_tcp_rsp_t;

typedef enum ngx_tcp_status_s{
	NS_ACCEPT=0,
	NS_RECV,
	NS_PROC,
	NS_SEND,
	NS_DONE,
	NS_CLOSE
}ngx_tcp_status_t;


typedef struct ngx_stat_t{
	double all_begin;
	double all_end;
	double all;
	double recv_req;
	double proc;
	double send_rsp;
	void* ext;
	int code; //״̬��
}ngx_stat_t;

typedef struct ngx_tcp_data_s ngx_tcp_data_t;

typedef int (*ngx_tcp_proc_pt)(ngx_tcp_req_t* req, ngx_tcp_rsp_t* rsp);

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

typedef struct ngx_tcp_async_s{
	ucontext_t work_ctx; //��ǰ�����ġ�
	ucontext_t main_ctx; //�������ġ�
	uint32_t timedout:1;
	uint32_t free_stask:1;

	//�޸�ʱ���뱣��stack�����ڽṹ������λ�á�
	char stack[0]; //stack��ʼλ�á�
}ngx_tcp_async_t;

struct ngx_tcp_data_s{
	time_t lastkeepalivetm;
	ngx_event_t first_to_ev; ////���Ӻ�δ�������ݵ�ʱ��ʹ�ô��¼�����ʱ��
	ngx_event_t to_ev;
	ngx_uint_t listen_port;
	ngx_tcp_status_t status;
	ngx_tcp_server_srv_conf_t* conf;
	ngx_tcp_req_t* req_data;
	ngx_tcp_rsp_t* rsp_data;
	void* userdata;
	ngx_stat_t stat;	
	IProtocol* cppcbs;
	ngx_tcp_proc_pt tcp_proc;
	ngx_tcp_async_t* async; //�첽����������ݽṹ��
};

#define prot_req_head_size cppcbs->req_head_size
#define prot_rsp_head_size cppcbs->rsp_head_size
#define prot_get_req_body_size cppcbs->get_req_body_size
#define prot_get_rsp_body_size cppcbs->get_rsp_body_size
#define prot_new_req_head cppcbs->new_req_head
#define prot_new_rsp_head cppcbs->new_rsp_head
#define prot_preproc_req_header cppcbs->preproc_req_header
#define prot_preproc_req_body cppcbs->preproc_req_body
#define prot_preproc_rsp_header cppcbs->preproc_rsp_header
#define prot_preproc_rsp_body cppcbs->preproc_rsp_body
#define prot_free_req cppcbs->free_req
#define prot_free_rsp cppcbs->free_rsp
#define prot_debug_req cppcbs->debug_req
#define prot_debug_rsp cppcbs->debug_rsp
#define prot_set_rsp_code cppcbs->set_rsp_code
#define prot_debug_stats cppcbs->debug_stats
#define prot_req_again cppcbs->req_again
#define prot_tcp_proc cppcbs->tcp_proc

#pragma pack(pop) //�ָ�����״̬��

void ngx_tcp_server_finalize(ngx_connection_t *c);
void ngx_tcp_server_write_handler(ngx_event_t *wev);

typedef int (*ngx_set_callbacks_pt)(ngx_tcp_data_t* tcp_data);

extern ngx_set_callbacks_pt g_tcp_set_callbacks;

/*
void *ngx_tcp_palloc(ngx_pool_t *pool, size_t size);
ngx_int_t ngx_tcp_pfree(ngx_pool_t *pool, void *p);
*/
void *ngx_tcp_palloc(ngx_pool_t *pool, size_t size,const char* file, int line);
void *ngx_tcp_realloc(ngx_pool_t *pool, void* p, size_t size,const char* file, int line);
ngx_int_t ngx_tcp_pfree(ngx_pool_t *pool, void *p,const char* file, int line);

#define NGX_TCP_PALLOC(pool,size) ngx_tcp_palloc(pool,size,__FILE__,__LINE__)
#define NGX_TCP_REALLOC(pool, p, size) ngx_tcp_realloc(pool, p, size, __FILE__,__LINE__)
#define NGX_TCP_PFREE(pool,p)  if(p){ngx_tcp_pfree(pool,p,__FILE__,__LINE__),p=NULL;}

inline const char* bin2hex(unsigned char *data, int data_len, char* buf)
{
	int	i;	

	int len = 0;
	for (i=0; i<data_len; i++)
	{
		if ((i % 8 == 0) && i)
			len += sprintf(buf+len," ");

		if ((i % 16 == 0) && i)
			len += sprintf(buf+len,"\n");

		len += sprintf(buf+len,"%02x ", data[i]);
	}
	return buf;
}

#endif

