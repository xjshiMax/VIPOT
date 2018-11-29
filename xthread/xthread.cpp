// xthread.cpp : 定义控制台应用程序的入口点。
//

#pragma once
#include "stdafx.h"
#include "testThread.h"
#include "stdlib.h"
#define Testtimes 10
void Testsdk(int times)
{
	testThread *threadgroup=new testThread[Testtimes];
	for(int i=0;i<Testtimes;i++)
	{
		threadgroup[i].start();
	}

}

int _tmain(int argc, _TCHAR* argv[])
{
	testThread p1;
	//p1.start();
	//p1.join();
	Testsdk(100);
	system("pause");
	return 0;
}

