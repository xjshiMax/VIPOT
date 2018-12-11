// VIPOT.cpp : 定义控制台应用程序的入口点。
//
#pragma once
#include "stdafx.h"

#include "../xthread/xTaskqueue.h"
#include "../xthread/xAutoLock.hpp"

typedef struct objtest
{
	int Messagetype;
	char*message;
}objtest_t;
xMutex mylock;
xCondition myCond;
class testclass
{
public:
	testclass(objtest_t&t):m_t(t){}
private:
	objtest_t m_t;
};
int _tmain(int argc, _TCHAR* argv[])
{
 	xTaskqueue<objtest_t> myqueue;
 	myqueue.setActive();
	objtest_t Task1={1,"hello"};
 	//myqueue.pushTaskWithTimeOut(Task1);
	//myqueue.pushTask(Task1);
	//myqueue.waitForTask(Task1);
	{
		testclass temp1(Task1);
	}
	testclass temp2(Task1);

	return 0;
}

