#pragma once
typedef unsigned int (__stdcall*pfunc)(void*);
//���ڶ����ģʽ
class Threadbase
{
public:
	Threadbase(bool bDetach=true);
	virtual ~Threadbase(){};
	virtual void run()=0;		//ҵ��ӿ�
	int start();			//�����߳�
	int join();				//�ȴ��߳̽���
	void destory();			//�����߳����������Դ

	int get_thread_id(){return thr_id;}
	int set_thread_id(unsigned long thrID){thr_id=thrID;}

protected:
	static unsigned int __stdcall thread_proxy(void* arg);

private:
	size_t thr_id;			//�߳�id
	bool bExit_;			//�߳��Ƿ�Ҫ�˳���־

};
//��������ģʽ
 class xThread
{
public:
	xThread(bool bDetach=true):thr_id(0)
	{

	}
	virtual ~xThread(){};
	int start(pfunc func,void *arg);			//�����߳�
	int join();				//�ȴ��߳̽���
	void destory();			//�����߳����������Դ

	int get_thread_id(){return thr_id;}
	int set_thread_id(unsigned long thrID){thr_id=thrID;}
public:
	size_t thr_id;
	bool bExit_;
};