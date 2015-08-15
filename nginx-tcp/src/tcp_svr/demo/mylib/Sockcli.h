#ifndef __SOCKET_CLIENT_H__
#define __SOCKET_CLIENT_H__

#define SND_BUF_SZ (1024*4)

typedef struct client_ctx_t{
	int socket;
	char ip[32];
	int port:16;
	int timeout_recv:16;
	int timeout_send:16;
	int err:16;
	/**
	 * �Ƿ�Ҫ�ر����ӣ������������ճ���ʱ��
	 * ��Ҫ���ø�λ.��sock_pool_putʱ��رո����ӡ�
	 */
	int needclose:1; 
	int ext1;
}client_ctx_t;

int sock_recv_all(int sockfd, void* pMsg, int iMsgLen);
int sock_send_all(int sockfd, const void* pMsg, int iMsgLen);
/**
 * optname: SO_SNDTIMEO or SO_RCVTIMEO
 */
int sock_set_timeout(int socket, int optname, int sec, int usec);
int sock_set_nodelay(int socket);
int sock_set_nonblock(int socket);
int sock_set_block(int socket);
int sock_set_cork(int socket, int on);
int sock_set_linger(int socket, int second);
int sock_set_reuseaddr(int socket);
int sock_set_keepalive(int socket, int iInterval, int iCount);

/**
 * ����һ��session������
 * ����ֵ: �����´�����������.
 */
client_ctx_t* client_new();
/**
 * ������ͷ�session������
 */
void client_free(client_ctx_t* ctx);

/**
 * ��ʼ��session������,������������
 * ip: ��������ַ
 * port: ������Ʒ
 * timeout:��������յĳ�ʱʱ��(��)
 * ����ֵ: 0��ʾ������ -1��ʾ����
 */
int client_init(client_ctx_t* ctx, const char* ip, int port, int timeout_send=5,int timeout_recv=10);

/**
 * ��������
 */
int client_reinit(client_ctx_t* ctx);

int client_close(client_ctx_t* ctx);
int client_test_conn(client_ctx_t* ctx);

/**
 * ֱ��ʹ���Ѿ������õ���������ʼ��
 * ����ֵ: 0��ʾ������ -1��ʾ����
 */
int client_init2(client_ctx_t* ctx, int socket);

#endif

