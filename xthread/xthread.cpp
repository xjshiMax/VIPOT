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
#include "xAtomicInt32.hpp"
#include "baseclass.hpp"
#define Testtimes 100
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
xAtomicInt32 i(0);
//int i=0;
unsigned int __stdcall Testfunc(void *m)
{
 	//xAutoLock lock(mu);
// 	timeobj Timecost("Testfunc");
	//i=i+1;
	for(int temp=0;temp<10000;temp++)
		i.inc();
		//i=i+1;
	//xAutoLock lock(mu);
	//printf("%d\n",i.get());
	return 0;
}
void testatomic()
{
	timeobj Timecost("automic ++ 100");
	for(int i=0;i<10000;i++)
	{

	}
}
void test()
{
	xMutex mutex;
	xThread mythread[Testtimes];
	char*msg[10]={"thread 0","thread 1","thread 2","thread 3","thread 4","thread 5","thread 6","thread 7","thread 8","thread 9"};
	for(int i=0;i<Testtimes;i++)
	{
		//xAutoLock l(mutex);
		mythread[i].start(Testfunc,"");
	}
	for(int i=0;i<Testtimes;i++)
	{
		mythread[i].join();
	}
}
 xMutex mymutex;
 xCondition myCondition;
 static unsigned int __stdcall waitfuc(void *)
 {
 	xAutoLock L(mymutex);
 	printf("i am waittinf for the signal\n");
 	myCondition.wait(mymutex);
 	printf("---it coming!\n ");
 	return 0;
 }
 static unsigned int __stdcall siginalfuc(void *)
 {
 	xAutoLock L(mymutex);
 	printf("give the sigianl;\n");
 	myCondition.signal();
 	return 0;
 }
 void testcondition()
 {
 	xThread mythread[2];
 	mythread[0].start(waitfuc,"");
 	//xMutex mutex;
 	Sleep(2000);
 	mythread[1].start(waitfuc,"");
 	mythread[0].join();
 	mythread[1].join();
 
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
// 	{
// 		timeobj t("thread");
// 		test();
// 		printf("i=%d\n",i);
// 	}
// 	//test();
	testcondition();
	system("pause");
	return 0;
}

