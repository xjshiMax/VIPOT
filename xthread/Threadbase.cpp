#pragma once
#include "Threadbase.h"
#ifdef WIN32
#include <process.h>
#include <Windows.h>
#endif
#include "stdio.h"
#include "timebase.hpp"
#include "xAutoLock.hpp"
//pthread_mutex_t zx;
xMutex zx;
unsigned int __stdcall Threadbase::thread_proxy(void* arg)
{

	xAutoLock antolock(zx);
	timeobj proxylife;
	Threadbase* pbase=reinterpret_cast<Threadbase*> (arg);
	pbase->run();
	//Sleep(3000);
	return 0;
}

Threadbase::Threadbase(bool bDetach)
{
	thr_id=0;
}
int Threadbase::start()
{

#ifdef WIN32
	unsigned int nval=_beginthreadex(0,0,thread_proxy,this,0,&thr_id);
	thr_id=nval;
#endif
	return 0;
}
int Threadbase::join()
{
#ifdef WIN32
	//WAIT_OBJECT 表示执行结束
	if(WaitForSingleObject(reinterpret_cast<HANDLE>(thr_id),INFINITE)==WAIT_OBJECT_0)
	{
		printf("\n join thread %d finish\n",thr_id);
	}
#endif
	return 0;
}
void Threadbase::destory()
{
	CloseHandle(reinterpret_cast<HANDLE>(thr_id));
}
//////////////////////////////////////////////////////////////////////////
//xThread 实现


int xThread::start(pfunc func,void *arg)
{
#ifdef WIN32
	unsigned int nval=_beginthreadex(0,0,func,arg,0,&thr_id);
	thr_id=nval;
#endif
	return 0;
}
int xThread::join()
{
#ifdef WIN32
	//WAIT_OBJECT 表示执行结束
	if(WaitForSingleObject(reinterpret_cast<HANDLE>(thr_id),INFINITE)==WAIT_OBJECT_0)
	{
		printf("\n join thread %d finish\n",thr_id);
	}
#endif
	return 0;
}
void xThread::destory()
{
	CloseHandle(reinterpret_cast<HANDLE>(thr_id));
}
