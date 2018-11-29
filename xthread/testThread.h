#pragma once
#include "Threadbase.h"
#include "stdio.h"
class testThread:public Threadbase
{
public:
	testThread(void);
	~testThread(void);
	void run();
	//在run里调用需要测试的接口
};

