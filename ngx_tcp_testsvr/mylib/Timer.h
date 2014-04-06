#ifndef __BASE_LIB_TIMER_H__
#define  __BASE_LIB_TIMER_H__
#include <signal.h> 
#include <time.h>
#include <math.h>
/*==============================================================
 * FileName:      Timer.h
 * Version:        1.0
 * Created by:    liuxj
 * Copyright (c) 2011 qvod Corporation.  All Rights Reserved. 
 *--------------------------------------------------------------
 * Description:   
 *      ��ʱ���ࡣ
 *=============================================================*/

#define Floor(double_val) ((long)floor(double_val))
#define Frac(double_val) (double_val-Floor(double_val))
#define ToNanoseconds(seconds) (long int)(seconds * 1000000000)

typedef void (*TimerProc)(void* args);
typedef struct {
	TimerProc proc;
	void* args;
}TimerArg;

class CTimer{
private:
	char m_name[32]; //�������������Ϣ��
	TimerArg m_timerArg;
	timer_t m_timer;
	double m_first; 
	double m_interval;
public:
	CTimer();
	CTimer(const char* name);
	~CTimer(){
		StopTimer();
		DestroyTimer();
	}

	void SetTimerName(const char* name);
	
	/*--------------------------------------------------------------
	* Function:     	InitThreadTimer
	* Description:  	��ʼ��һ����ʱ��(����Ϊ�߳�) �߳����͵Ķ�ʱ
	*				��ʹ���ڵ���Ƶ�ʲ�̫�ߵĳ��ϡ�
	* Input:			proc, ��ʱ����ʱ���Ҫִ�е��̴߳�������
	*				proc_arg, �̴߳������Ĳ�����
	*				threadStackSize, �߳�ʹ�õĶ�ջ��С,��λKB��
	* Return:       	����״̬��0��ʾ�ɹ��� 1��ʾʧ�ܡ�
	*-------------------------------------------------------------*/
	int InitThreadTimer(TimerProc proc, void* proc_arg, size_t threadStackSize =10);

	int InitSignalTimer(TimerProc proc, void* proc_arg);
	
	/*--------------------------------------------------------------
	* Function:     	StartTimer
	* Description:  	����һ����ʱ��
	* Input:			first, ÿһ�����е�ʱ��(��λΪ��,С������ת��nanoseconds)
	*				interval, �������еļ��ʱ��(��λΪ��,С������ת��nanoseconds)
	* Return:       	����״̬��0��ʾ�ɹ��� 1��ʾʧ�ܡ�
	*-------------------------------------------------------------*/
	int StartTimer(double first, double interval);

	/*--------------------------------------------------------------
	* Description:  	����һ����ʱ��(��������ʱ����)
	* Input:			first, ÿһ�����е�ʱ��(��λΪ��,С������ת��nanoseconds)
	*				interval, �������еļ��ʱ��(��λΪ��,С������ת��nanoseconds)
	* Return:       	����״̬��0��ʾ�ɹ��� 1��ʾʧ�ܡ�
	*-------------------------------------------------------------*/
	int RestartTimer(double first, double interval);
	
	int StopTimer();

	int DestroyTimer();
};

#if 0
union sigval { 
 int sival_int; 
 void *sival_ptr; 
 }; 

 struct sigevent { 
 int sigev_notify; /* Notification method */ 
 int sigev_signo; /* Timer expiration signal */ 
 union sigval sigev_value; /* Value accompanying signal or 
 passed to thread function */ 
 void (*sigev_notify_function) (union sigval); 
 /* Function used for thread 
 notifications (SIGEV_THREAD) */ 
 void *sigev_notify_attributes; 
 /* Attributes for notification thread 
 (SIGEV_THREAD) */ 
 pid_t sigev_notify_thread_id; 
 /* ID of thread to signal (SIGEV_THREAD_ID) */ 
 };

 ���У�sigev_notify ָ����֪ͨ�ķ�ʽ :

SIGEV_NONE

����ʱ������ʱ���������첽֪ͨ�����ö�ʱ�������н��ȿ���ʹ�� timer_gettime(2) ��⡣

SIGEV_SIGNAL

����ʱ������ʱ������ sigev_signo ָ�����źš�

SIGEV_THREAD

����ʱ������ʱ���� sigev_notify_function ��ʼһ���µ��̡߳��ú���ʹ�� sigev_value ��Ϊ��������� sigev_notify_attributes �ǿգ����ƶ����̵߳����ԡ�ע�⣬���� Linux ���̵߳������ԣ��������ʵ�������� glibc ���ں�һ��ʵ�ֵġ�

SIGEV_THREAD_ID (Linux-specific)

���Ƽ���ʵ���߳̿�ʱ��ʹ�á�

��� evp Ϊ�յĻ�����ú�������Ϊ��Ч�ڣ�sigev_notify = SIGEV_SIGNAL��sigev_signo = SIGVTALRM��sigev_value.sival_int = timer ID ��

  struct   timespec   {   
                  long                 tv_sec;                  /*   seconds   */   
                  long                 tv_nsec;                 /*   nanoseconds  (1/1000000000 seconds) */   
  };   

  struct     itimerspec   {   
                  struct     timespec   it_interval;         /*   timer   period   */   
                  struct     timespec   it_value;            /*   timer   expiration   */   
  };  

#endif

#endif



