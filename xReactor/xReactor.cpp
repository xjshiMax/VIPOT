// xReactor.cpp : 定义控制台应用程序的入口点。
//

#include <stdio.h>
#include "xReactor.hpp"
#include "xtimeheap.hpp"
#include <string>
// class Timer_
// {
// 
// };
void mytimeout(void*arg)
{
	printf("on mytimeout\n");
	//std::string smsg=(char*) arg;
	printf("get the time outdata:%s",(char*)arg);
}

int main(int argc, char* argv[])
{
	xReactor m_reactor;
	xheaptimer*time_once=new xheaptimer(5);
	time_once->cb_func=mytimeout;
	const char* msg="this is a test!";
	time_once->user_data=(void*)msg;
	m_reactor.RegisterTimeTask(time_once);
	m_reactor.start();

	return 0;
}

