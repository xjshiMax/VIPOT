#pragma once
#include "Threadbase.h"
#include "stdio.h"
class testThread:public Threadbase
{
public:
	testThread(void);
	~testThread(void);
	void run();
	//��run�������Ҫ���ԵĽӿ�
};

