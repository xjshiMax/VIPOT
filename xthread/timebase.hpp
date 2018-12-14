//2018-11-29   ����һ���̰߳�ȫ�ļ�ʱ����
#pragma once
#include <stdio.h>
#include <time.h>
#ifdef WIN32
#include <Windows.h>
#endif
#define objMS 10000
#define objS  10000000
class timeobj
{
public:
	timeobj(char *pmsg="life:",int timeval=objMS):m_val(timeval),m_pmsg(pmsg)
	{
#ifdef WIN32
		GetSystemTimeAsFileTime((LPFILETIME)&m_starttime);
#endif
	}
	virtual ~timeobj()
	{
		GetSystemTimeAsFileTime((LPFILETIME)&m_endtime);
		m_lifeTime=m_endtime-m_starttime;
		if(m_pmsg!=NULL)
			printf("%s %f(ms)\n",m_pmsg,m_lifeTime*1.0/m_val);
		else
			printf("the life cost(100ns):%f\n",m_lifeTime*1.0/m_val);
	}
	LONGLONG startcount()
	{
#ifdef WIN32
		GetSystemTimeAsFileTime((LPFILETIME)&m_starttime);
		return m_starttime; 
	}
	LONGLONG utilstartcount()  //�ӵ���start��ʼ������utilstartcount �߹���ns��
	{
		GetSystemTimeAsFileTime((LPFILETIME)&m_endtime);
		return m_endtime-m_starttime;
	}
private:
	LONGLONG m_starttime;
	LONGLONG m_endtime;
	LONGLONG m_lifeTime;
	int m_val;
	char* m_pmsg;
};