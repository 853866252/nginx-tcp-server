
#ifndef __NGX_TCP_SESSION_H__
#define __NGX_TCP_SESSION_H__

#include <ngx_core.h>
#include <ngx_config.h>
#include <ngx_event.h>
#include <ngx_tcp.h>
#include <ngx_event_connect.h>
#include <ucontext.h>

typedef void* (*app_cfg_new_pt)(ngx_conf_t* cf);
typedef int (*app_cfg_init_pt)(const char* config, void* appcfg,ngx_tcp_core_srv_conf_t* core_cfg);
typedef void* (*app_ctx_new_cb)(ngx_cycle_t* cycle, void* appcfg);
typedef int (*app_ctx_init_cb)(void* appcfg, void* appctx);
typedef void (*app_ctx_destroy_cb)(ngx_cycle_t* cycle, void* appctx);
typedef void (*app_cfg_destroy_cb)(ngx_cycle_t* cycle, void* appcfg);
typedef void (*app_exit_master_cb)(ngx_cycle_t *cycle);

typedef struct app_ctx_t {
	app_cfg_new_pt app_cfg_new;	//��������ʵ����
	app_cfg_init_pt app_cfg_init;		//�������ó�ʼ��
	app_ctx_new_cb app_ctx_new;	//����������ʵ����
	app_ctx_init_cb app_ctx_init;		//���������ĳ�ʼ��
	app_ctx_destroy_cb app_ctx_destroy;	//��������������
	app_cfg_destroy_cb app_cfg_destroy;	//������������
	app_exit_master_cb app_exit_master;	//master�˳�ʱ���ûص�
}app_ctx_t;

extern app_ctx_t g_app_ctx;

#define NGX_TCP_PUB

double ngx_second(void) ;

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
	uint32_t header_len:16;//received header size.
	uint32_t body_len; //received body size
	ngx_connection_t* c;
	struct ngx_tcp_data_s* tcp_data;
	ngx_flag_t isbigendian;
}ngx_tcp_req_t;

typedef struct ngx_tcp_rsp_s{
	rsp_head_t* rsp_header;
	void* body;
	uint32_t body_len; //body size.
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

typedef req_head_t* (*ngx_new_req_head_pt)(ngx_pool_t* pool);
typedef rsp_head_t* (*ngx_new_rsp_head_pt)(ngx_pool_t* pool, req_head_t* header);
typedef size_t (*ngx_tcp_get_req_body_size_pt)(req_head_t* header);
typedef size_t (*ngx_tcp_get_rsp_body_size_pt)(rsp_head_t* header);

typedef int (*ngx_tcp_preproc_req_header_pt)(ngx_tcp_req_t* req);
typedef int (*ngx_tcp_preproc_req_body_pt)(ngx_tcp_req_t* req);

typedef int (*ngx_tcp_preproc_rsp_header_pt)(ngx_tcp_rsp_t* rsp);
typedef int (*ngx_tcp_preproc_rsp_body_pt)(ngx_tcp_rsp_t* rsp);

typedef void (*ngx_tcp_debug_req_pt)(ngx_tcp_req_t* req);
typedef void (*ngx_tcp_debug_rsp_pt)(ngx_tcp_rsp_t* rsp);
typedef void (*ngx_tcp_free_req_pt)(ngx_pool_t* pool,ngx_tcp_req_t* req);
typedef void (*ngx_tcp_free_rsp_pt)(ngx_pool_t* pool,ngx_tcp_rsp_t* rsp);

typedef void (*ngx_set_rsp_code_pt)(ngx_tcp_rsp_t* rsp, int ret);
//typedef const char* (*ngx_get_service_name_pt)(ngx_tcp_req_t* req, char* servicename);
typedef void (*ngx_tcp_debug_stats_pt)(ngx_tcp_data_t* data);
//����������ʱ������TCP_AGAINʱ���õĻص���
typedef int (*ngx_tcp_req_again_pt)(ngx_tcp_req_t* req); 

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
	ngx_tcp_core_srv_conf_t* conf;
	ngx_tcp_req_t* req_data;
	ngx_tcp_rsp_t* rsp_data;
	void* userdata;
	ngx_stat_t stat;	

	ngx_tcp_protocol_info_t protocbs;
	ngx_tcp_proc_pt tcp_proc;
	ngx_tcp_async_t* async; //�첽����������ݽṹ��
};


#pragma pack(pop) //�ָ�����״̬��

typedef int (*ngx_set_callbacks_pt)(ngx_tcp_data_t* tcp_data);

extern ngx_set_callbacks_pt g_tcp_set_callbacks;

void ngx_tcp_init_connection(ngx_connection_t *c);
void ngx_tcp_close_connection_(ngx_connection_t *c,const char* func, int line);

#define ngx_tcp_close_connection(c) ngx_tcp_close_connection_(c, __FUNCTION__,__LINE__)

void ngx_tcp_server_write_handler(ngx_event_t *wev);

u_char *ngx_tcp_log_error(ngx_log_t *log, u_char *buf, size_t len);

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


#endif
