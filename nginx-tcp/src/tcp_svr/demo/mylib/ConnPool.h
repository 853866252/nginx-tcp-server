#ifndef __JSONLIB_CONN_POOL_H__
#define __JSONLIB_CONN_POOL_H__
#include <pthread.h>
#include <stdlib.h>
#include <stdint.h>

uint64_t uint64_inc(uint64_t* pcount);

typedef void* (*conn_new_cb)(void* args);
typedef int (*conn_connect_cb)(void* conn,void* args);
typedef int (*conn_close_cb)(void* conn);
typedef int (*conn_test_close_cb)(void* conn); //�����Ƿ���Ҫ�ر�����(�����ӳ���������) 0:����Ҫ�رգ�1:��Ҫ�ر�
typedef void (*conn_free_cb)(void* conn);

typedef struct conn_cb_t{
	conn_new_cb conn_new;
	conn_free_cb conn_free;
	conn_connect_cb connect;
	conn_close_cb close;
	conn_test_close_cb test_close;
}conn_cb_t;

typedef struct conn_statis_t{
	uint64_t connect; 	//��������(�������ڼ�����)
	uint64_t close; 	//�ر���������
	uint64_t get;//��ȡ��������
	uint64_t get_real; //�ӳ������ӵ�����
	uint64_t release;//�����ͷ�������
	uint64_t release_real;//�����ͷ���������
}conn_statis_t;

typedef struct conn_pool_t{
	volatile int curconns; 
	void* args; //���Ӳ���
	void** conns; //����
	//char* status; //�Ƿ������ϡ�'y':�Ѿ������ϣ�'n':δ�����ϡ�'0':��ʾ�����ӡ�
	conn_cb_t* cbs; //���ӳ���ػص���
	int size;
	int start; //��start==endʱ���п��������ˣ�Ҳ�����ǿ��ˡ���curconns�жϡ�curconns > 0 ���ˣ�<=0 ���ˡ�
	int end;
	time_t pre_err_time; //�ϴ����ӳ����ʱ�䡣
	int reconn_interval; 	  //����������Сʱ����(��)�� ����������ӳ�����ڸõ�λʱ�䲻�ٽ��������ӡ�
	pthread_spinlock_t spin;
	conn_statis_t statis;
}conn_pool_t;

/**
 * size ���ӳش��С
 * lazy_init ��ʾ��newʱ������ʵ�ʵ�����
 */
conn_pool_t* conn_pool_new(int size,int lazy_init,conn_cb_t* cbs, void* args);
void conn_pool_free(conn_pool_t* pool);
void* conn_pool_get(conn_pool_t* pool);
int conn_pool_put(conn_pool_t* pool, void* conn, int real_close=0);


//}
#endif

