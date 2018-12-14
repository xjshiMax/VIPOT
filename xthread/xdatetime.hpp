//2018-12-13
//��װ���õĻ�ȡʱ���ʱ����ĺ���
/*
1.��ȡ��ǰʱ��� s(ms,100ns)
2.��ȡ��ǰʱ���ַ����������ʽ���
3.����ʱ�����ȡʱ���ַ���
4.����ʱ�䴮��ȡʱ���
5.�ֱ��ȡ������
6.ʱ�����ͱȽ� ���� = == + - 
//ϵͳʹ�õ�ʱ�����
*time_t		�����Σ�64λ���������utcʱ���
*struct tm  �ṹ�� 
struct tm {
int tm_sec;  ��Cȡֵ����Ϊ[0,59] 
int tm_min;  �� - ȡֵ����Ϊ[0,59] 
int tm_hour;  ʱ - ȡֵ����Ϊ[0,23] 
int tm_mday;  һ�����е����� - ȡֵ����Ϊ[1,31] 
int tm_mon;  �·ݣ���һ�¿�ʼ��0����һ�£� - ȡֵ����Ϊ[0,11] 
int tm_year; ��ݣ���ֵ��1900��ʼ 
int tm_wday; ���ڨCȡֵ����Ϊ[0,6]������0���������죬1��������һ���Դ����� 
*struct timeval   ��Ӧ����gettimeofday
{
	time_t tv_sec;
	suseconds_t tv_usec;
}
*struct timespec	�����������epoll�ĳ�ʱ
{
	time_t tv_sec;
	long tv_nsec;
}
*/

#if defined(WIN32)
#include <Windows.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
using namespace std;
#else
#include <time.h>
#include <errno.h>
#include <sys/time.h>
#endif
//64bit ���ͱ���
#define LONGLONG long int
#define ULONGLONG unsigned long int
#define s_100ns   10000000
class xTimevar
{
public:
	xTimevar();
	xTimevar(ULONGLONG timestamp=0)
	{
		if(timestamp==0)
		{
			m_timestamp_s = time(NULL);
			
			struct timeval tv;
			gettimeofday(&tv,NULL);
			m_timestamp_ms = tv.tv_sec*1000+tv.tv_usec/1000;
			struct timespec ts;
			clock_gettime(CLOCK_REALTIME,&ts);
			m_timestamp_100ns=ts.tv_sec*s_100ns+ts.ntv_sec*100;
		}
	}
	xTimevar(struct tm &structime);
	ULONGLONG getTimeStamp()
	{
		m_timestamp_s = time(NULL);
		return m_timestamp_s;
	}
	 ULONGLONG getTimeStamp(struct tm &Inputtime)
	{
		return mktime(&Inputtime);
	}
	//ͨ��ʱ�����ȡ�ṹ��ʱ�䣬Ϊ0��ʾ��ȡ��ǰʱ�䡣
	const struct tm &getstructTime(ULONGLONG timestamp=0)
	{
		time_t temp=(time_t)m_timestamp_s;
			if(timestamp!=0)
			{
				temp=timestamp;
			}
		return  *localtime(&temp);
	}
	string GetTimeFormat(const char* lptimeformat)
	{

	}
	bool operator ==(xTimevar);
	//����ʱ���ֵ������
	ULONGLONG operator -(xTimevar& secondTime);
	//��ֵ
	xTimevar operator =(xTimevar&secondTime);
private:
	std::string strtime;
	ULONGLONG m_timestamp_s;
	ULONGLONG m_timestamp_ms;
	ULONGLONG m_timestamp_100ns;
	struct tm m_time_struct;
	
};