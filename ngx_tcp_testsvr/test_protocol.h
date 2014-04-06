#ifndef __MSGONLINE_PROTOCOL_H__
#define  __MSGONLINE_PROTOCOL_H__
#include <stdint.h>
#include <arpa/inet.h>

#pragma pack(push) //�������״̬
#pragma pack(1) //����1�ֽڶ���

//Э��ͷ��
typedef struct header_t{
	uint16_t magic; //Э��ʶ���룬Ϊһ���̶�ֵ��������Ӧ����ͬ��
	uint32_t len; 	//ָ���峤�ȣ�0��ʾû��ָ���塣
	uint16_t cmd;	//ָ��š�
	uint16_t seq;   //ָ����š�
	uint16_t code;  //����ֵ��(����ʱΪ0)
}__attribute__ ((packed)) header_t;

typedef struct header_t req_header_t;
typedef struct header_t rsp_header_t;

#define LOG_HEADER(str, header);  LOG_DEBUG("%s magic:0x%04x, len:%d,cmd:%d,seq:%d, code:%d",\
			str, (int)header->magic,header->len,header->cmd,header->seq, header->code);

#pragma pack(pop) //�ָ�����״̬��

typedef struct {
	int n;
}test_add_dt;

typedef struct {
	int n;
}test_sub_dt;

typedef struct {
	int value;
}test_result_dt;

typedef struct {
	int second;
}test_sleep_dt;

//���ش�����

#define ERRNO_OK	0					//�ɹ�	
#define ERRNO_SYSTEM	1				//ϵͳ����	���нӿ�
#define ERRNO_REQ_INVALID	2			//�����������	���нӿ�

#define TEST_MAGIC 0xabcd

#define CMD_TEST_INIT 0
#define CMD_TEST_ADD 1
#define CMD_TEST_SUB 2
#define CMD_TEST_QUERY 3
#define CMD_TEST_SLEEP 4

#endif

