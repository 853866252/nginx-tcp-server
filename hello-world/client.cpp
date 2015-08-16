#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

int socket_conn(const char* ip, int port)
{
	
	int s = socket(PF_INET, SOCK_STREAM, 0);
	struct sockaddr_in cliaddr;
	bzero(&cliaddr , sizeof(cliaddr));
	cliaddr.sin_family = PF_INET;
	cliaddr.sin_port = htons(port);
	int ret = inet_pton(PF_INET, ip, &cliaddr.sin_addr);
	if(ret == -1)
	{
		close(s);
		return -1;
	}

	ret = connect(s , (struct sockaddr*)&cliaddr , sizeof(cliaddr));
	if(ret != 0){		
		return ret;
	}
	
	return s;
}

/*****************************************
 ����������,��������
 *****************************************/
#define MAGIC 0xa0b1
#define MAGIC_BIG 0xb1a0

#define CMD_LOGIN 0x1
#define CMD_EXIT 0x2

#define ERRNO_OK	0					//�ɹ�	
#define ERRNO_SYSTEM	1				//ϵͳ����	���нӿ�
#define ERRNO_REQ_INVALID	2			//�����������	���нӿ�

/*****************************************
 ����������Ӧ��
 *****************************************/
typedef struct {
	char username[32]; //����ʵ�ʳ����Ǳ䳤�ģ����Կ����Ѿ���ȡ��\0Ϊֹ��
}hello_req_dt;

typedef struct {
	char data[4]; //����ʵ�ʳ����Ǳ䳤�ģ����Կ����Ѿ���ȡ��\0Ϊֹ��
}hello_rsp_dt;


/*****************************************
 Э�鴦����ض�����ʵ��
 *****************************************/
// ����ͷ����
typedef struct {
	uint16_t magic; //Э��ʶ���룬Ϊһ���̶�ֵ��������Ӧ����ͬ��
	uint32_t len; 	//ָ���峤�ȣ�0��ʾû��ָ���塣
	uint16_t cmd;	//ָ��š�
}__attribute__ ((packed)) hello_req_header_t;
// ��Ӧͷ����
typedef struct {
	uint16_t magic; //Э��ʶ���룬Ϊһ���̶�ֵ��������Ӧ����ͬ��
	uint32_t len; 	//ָ���峤�ȣ�0��ʾû��ָ���塣
	uint16_t cmd;	//ָ��š�
	uint16_t code;  
}__attribute__ ((packed)) hello_rsp_header_t;

int request_hello(int sock, int cmd, const char* username)
{
	hello_req_header_t req_header;
	memset(&req_header, 0, sizeof(req_header));
	req_header.magic = MAGIC;
	req_header.cmd = cmd;
	req_header.len = strlen(username);
	int ret;
	ret = send(sock, &req_header, sizeof(req_header), 0);
	if(ret == sizeof(req_header)){
		ret = send(sock, username, req_header.len, 0);
		if(ret != req_header.len){
			printf("send body failed! err: %s\n", strerror(errno));
			return -1;
		}
	}else{
		printf("send req_header failed! err: %s\n", strerror(errno));
		return -1;
	}

	// ������Ӧ��
	hello_rsp_header_t rsp_header;
	memset(&rsp_header, 0, sizeof(rsp_header));
	ret = recv(sock, &rsp_header, sizeof(rsp_header), 0);
	if(ret == sizeof(rsp_header)){
		if(rsp_header.code == ERRNO_OK){
			hello_rsp_dt* rsp_data = (hello_rsp_dt*)alloca(rsp_header.len);
			if(rsp_header.magic != MAGIC || rsp_header.cmd != req_header.cmd+1){
				printf(" response header invalid! magic:0x%04x, cmd: %d\n",
							rsp_header.magic, rsp_header.cmd);
				return -1;
			}
			ret = recv(sock, rsp_data, rsp_header.len, 0);
			if(ret == rsp_header.len){
				printf(">%s\n", rsp_data->data);
			}else{
				printf("recv rsp_body failed! err: %s\n", strerror(errno));
				return -1;
			}
		}else{
			printf("error: tcp server return code:%d\n", rsp_header.code);
		}
	}else{
		printf("recv rsp_header failed! err: %s\n", strerror(errno));
		return -1;
	}

}

int main(int argc, char* argv[])
{
	if(argc != 5){
		printf("Usage: %s host port cmd username\n", argv[0]);
		exit(1);
	}
	const char* host = argv[1];
	int port = atoi(argv[2]);
	int cmd = atoi(argv[3]);
	const char* username = argv[4];
	int sock = socket_conn(host, port);
	if(sock <= 0){
		printf("connect to [%s:%d] failed!", host, port);
		exit(2);
	}

	request_hello(sock, cmd, username);
	
	close(sock);
	return 0;
}

