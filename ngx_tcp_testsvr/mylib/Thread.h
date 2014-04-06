#ifndef __THREAD_H_
#define __THREAD_H_
/*==============================================================
 * FileName:      Thread.h
 * Version:        1.0
 * Created by:    liuxj
 * Copyright (c) 2011 qvod Corporation.  All Rights Reserved. 
 *--------------------------------------------------------------
 * Description:   
 *      �̷߳�װ�ࡣ���������ö�ջ��С����CPU�Ȼ�������
 *=============================================================*/
#include <pthread.h>

#define THREAD_STACK_SIZE 1024 //Ĭ���̶߳�ջ��С(��λKB).
typedef void* (* PThreadProc)(void*);

#define CLASS_UNCOPYABLE(classname) \
     private: \
      classname(const classname&); \
      classname& operator=(const classname&);

class Runnable {
public:
     virtual void Run() = 0;
     virtual ~Runnable() {}
};

int thread_set_affinity(int cpu_no);


class CThread : public Runnable {
     CLASS_UNCOPYABLE(CThread)
public:
	
    /*--------------------------------------------------------------
	* Function:     	CThread
	* Description:  	���캯����
	* Input:			stackSize, �߳�ʹ�õĶ�ջ��С��
	*				isJoinable, �߳��ǲ��ǿ�Join�ġ�
	* Return:       	����״̬��0��ʾ�ɹ��� 1��ʾʧ�ܡ�
	*-------------------------------------------------------------*/
	CThread(int stackSize=THREAD_STACK_SIZE, int isJoinable=1);
    	virtual ~CThread();

      /*--------------------------------------------------------------
	* Function:     	Start
	* Description:  	�����̡߳�
	* Input:			�ޡ�
	* Return:       	����״̬��0��ʾ�ɹ��� 1��ʾʧ�ܡ�
	*-------------------------------------------------------------*/
	virtual int Start();

	/*--------------------------------------------------------------
	 * Function:		Stop
	 * Description: 	ֹͣ�̡߳�
	 * Input:			�ޡ�
	 * Return:		�ޡ�
	 *-------------------------------------------------------------*/
	virtual void Stop();

	void SetStop(){ m_isStop = true;}
	/*--------------------------------------------------------------
	 * Function:		Join
	 * Description: 	Join�̡߳�
	 * Input:			�ޡ�
	 * Return:		�ޡ�
	 *-------------------------------------------------------------*/
	int  Join();

	/*--------------------------------------------------------------
	 * Function:		SetAffinity
	 * Description: 	���̰߳󶨵�ĳ��CPU֮�ϡ�
	 * 				Linux�£��鿴ÿ��CPU��ʹ���ʣ�top -d 1
	 *				֮����1. ����ʾ���CPU
	 * Input:			cpu_no CPU��š�(��0��ʼ)
	 * Return:		���ذ�״̬�� 0��ʾ�ɹ��� -1��ʾʧ�ܡ�
	 *-------------------------------------------------------------*/
	int SetAffinity(int cpu_no);

	inline int DetachSelf(){return pthread_detach(pthread_self());}
	
	bool isStoped(){
		return m_isStop;
	}
	virtual void Run()=0; // { LOG_ERROR("Function Run Unimplement....");}

protected:
	bool m_isStop;
	int m_isJoinable;	// 1 joinable 0 unjoinable.
	pthread_t m_thread;
	int m_stackSize;

private:
    static unsigned ThreadProc(void* param);
};

#endif
