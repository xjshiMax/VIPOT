// xthread.cpp : 定义控制台应用程序的入口点。
//

#pragma once
#include "stdafx.h"
#include "testThread.h"
#include "stdlib.h"
#include "stdlib.h"
#include "Threadbase.h"
#include "xAutoLock.hpp"
#include "timebase.hpp"
#define Testtimes 300
unsigned int __stdcall Testsdk(void *m)
{
	testThread *threadgroup=new testThread[Testtimes];
	for(int i=0;i<Testtimes;i++)
	{
		threadgroup[i].start();
	}
	return 0;
}
xMutex mu;

unsigned int __stdcall Testfunc(void *m)
{
 	xAutoLock lock(mu);
// 	timeobj Timecost("Testfunc");
	int i=1;
	printf("%d\n",i);
	return 0;
}
void test()
{
	xMutex mutex;
	xThread mythread[Testtimes];
	char*msg[10]={"thread 0","thread 1","thread 2","thread 3","thread 4","thread 5","thread 6","thread 7","thread 8","thread 9"};
	for(int i=0;i<Testtimes;i++)
	{
		//xAutoLock l(mutex);
		//xThread myt;
		mythread[i]mythread[Testtimes].start(Testfunc,"");
		//printf("%d\n",i);
	}
}
int _tmain(int argc, _TCHAR* argv[])
{
	testThread p1;
	//p1.start();
	//p1.join();
	//Testsdk(100);
// 	xThread myt;
// 	myt.start(Testfunc,"fun");
//多线程并发测试：
//同时开启 Testtimes个线程，执行 Testfunc里面的内容，
//如果被调测试的接口函数正常且结果正确，则说明接口是线程安全的
//调整 Testtimes大小，可以测试程序粒度以及承受能力。
	test();
	system("pause");
	return 0;
}

