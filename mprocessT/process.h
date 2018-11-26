#include "../include/inclu.h"
class process
{
public:
	process():m_pid(-1){}
public:
	pid_t m_pid;
	int m_pipefd[2];
};

template<typename T>
class processpool
{
private:
	processpool(int listenfd,int process_number=8);
public:
	static processpool<T> *create(int listenfd,int process_number=8)
	{
		if(!m_instance)
		{
			m_instance = new processpool<T>(listenfd,process_number);
		}
		return m_instance;
	}
	~processpool()
	{
		delete []m_sub_process;
	}
private:
	static const int MAX_PROCESS_NUMBER = 16;
	static const int USER_PER_PROCESS = 65536;
	static const int MAX_EVENT_NUMBER = 10000;
	int m_process_number;
	int m_idx;
	int m_epollfd;
	int m_listenfd;
	int m_stop;
	process * m_sub_process;
	static processpool<T> *m_instance;

};