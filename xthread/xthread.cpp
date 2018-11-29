// xthread.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "testThread.h"
#include "stdlib.h"

#define Testtimes 1000
void Testsdk(int times)
{
	testThread threadgroup[Testtimes];
	for(int i=0;i<Testtimes;i++)
	{
		threadgroup[Testtimes].start();
	}

}

int _tmain(int argc, _TCHAR* argv[])
{
	testThread p1;
	//p1.start();
	//p1.join();

	system("pause");
	return 0;
}

