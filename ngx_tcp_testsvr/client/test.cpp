#include <stdio.h>
#include "../mylib/TcpTestClient.h"
#include "../mylib/testlog.h"
#include <string.h>
#include <arpa/inet.h>
#include "testcli.h"
#include <algorithm>
#include <signal.h>
#include <unistd.h>
using namespace std;

extern char *optarg; //ѡ��Ĳ���ָ��
extern int optind;    //��һ�ε���getopt��ʱ����optind�洢��λ�ô����¿�ʼ���ѡ� 
extern int opterr;   //��opterr=0ʱ��getopt����stderr���������Ϣ��
extern int optopt; //��������ѡ���ַ���������optstring�л���ѡ��ȱ�ٱ�Ҫ�Ĳ���ʱ����ѡ��洢��optopt�У�getopt����'����


char g_host[32];
int g_port = 2014;



int TEST_ADD(client_ctx_t* client, int index){

	int result = 0;
	int ret = cli_test_add(client, index, &result);
	printf("test_add(n=%d,result=%d) ret=%d\n", index, result, ret);
	
	return ret;
}

int TEST_SUB(client_ctx_t* client, int index){
	int result = 0;
	int ret = cli_test_sub(client, index, &result);
	printf("test_sub(n=%d,result=%d) ret=%d\n", index, result, ret);
	
	return ret;
}

int TEST_QUERY(client_ctx_t* client, int index){
	int result = 0;
	int ret = cli_test_query(client,  &result);
	printf("test_query(result=%d) ret=%d\n",  result, ret);
	
	return ret;
}

int TEST_SLEEP(client_ctx_t* client, int second){
	int result = 0;

	LOG_DEBUG("test_sleep(second=%d) begin ", second);
	int ret = cli_test_sleep(client,  second);
	LOG_DEBUG("test_sleep(second=%d) ret=%d",  second, ret);
	
	return ret;
}

int TestRequest(int index,ClientCtx* ctx, void* args)
{
	int ret = 0;
	int* func = (int*)args;
	client_ctx_t* conn = (client_ctx_t*)ctx;
	switch(*func){
	case CMD_TEST_ADD:
		ret = TEST_ADD(conn, index);
	break;
	case CMD_TEST_SUB:
		ret = TEST_SUB(conn, index);
	break;
	case CMD_TEST_QUERY:
		ret = TEST_QUERY(conn, index);
	break;
	case CMD_TEST_SLEEP:
		ret = TEST_SLEEP(conn, 1);
	break;
	default:
		printf("Unknow func [%d]\n", func);
		ret = 990;
	}
	return ret;
}

ClientCtx* TestCtxNew(const char* host, int port)
{
	client_ctx_t* client = client_new();
	int ret = client_init(client, host, port,5, 5);
	if(ret != 0){
		free(client);
		client = NULL;
	}
	cli_test_init(client);
	
	return client;
}

void TestCtxFree(ClientCtx* ctx)
{
	if(ctx != NULL){
		client_ctx_t* client = (client_ctx_t*)ctx;
		client_close(client);
	}
}

void TestUsage(const char* program){
	printf("%s -h [host] -t [threads] -r [request count] -f [function]  -?\n", program);
	printf("-f function: ######## functions ##########\n"
		"\t  1:ADD 2:SUB 3:QUERY 4:SLEEP\n"
 		);
}

void pipe_handler(int sig){
	printf("recv sigpipe!\n");
}

int main(int argc, char* argv[]){
	int ret = 0;
	int threads = 8;
	int conns = 0;
	int requests = 0;
	int func = CMD_TEST_QUERY;
	
	signal(SIGPIPE, pipe_handler);

	sprintf(g_host, "127.0.0.1");
	
	int result;
	while((result = getopt(argc, argv, "h:p:t:c:r:f:?")) != -1){
		switch(result){
		case 'h'://host
		strcpy(g_host, optarg);
		break;
		case 'p'://g_port
			g_port = atoi(optarg);
		break;
		case 't'://threads
			threads = atoi(optarg);
		break;
		case 'c'://connections
			conns = atoi(optarg);
		break;
		case 'r'://request count
			requests = atoi(optarg);
		break;
		case 'f'://function
			func = atoi(optarg);
		break;
		case '?':
			TestUsage(argv[0]);
			exit(255);
		break;
		default:
			printf("Unknow option -%c\n", result);
			//exit(0);
		}
	}


	if(requests == 0){
		printf("request count is 0!\n");
		TestUsage(argv[0]);
		exit(99);
	}


	RunTcpTestClient(g_host, g_port, threads, requests, 
		&TestRequest, &func,&TestCtxNew, &TestCtxFree, 0);


	printf("################## all test is ok #################\n");
	return 0;
}


