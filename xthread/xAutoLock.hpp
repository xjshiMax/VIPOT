//2018-11-29
//×Ô¶¯Ëø
//pthread_mutex ºÍMutex
#ifdef WIN32
#include "E:\workstation\Common\pthread\Pre-built.2\include\pthread.h"
#pragma comment(lib,"E:\\workstation\\Common\\pthread\\Pre-built.2\\lib\\pthreadVC2.lib")
#pragma comment(lib,"E:\\workstation\\Common\\pthread\\Pre-built.2\\lib\\pthreadVCE2.lib")
#pragma comment(lib,"E:\\workstation\\Common\\pthread\\Pre-built.2\\lib\\pthreadVSE2.lib")
#else
#include <pthread.h>
#endif
#define Mutex pthread_mutex_t
class xAutoLock
{
public:
	xAutoLock(Mutex* mutex):m_mutex(*mutex)
	{
		pthread_mutex_lock(&m_mutex);
		//m_mutex.lock();
		printf("lock\n");
	}
	xAutoLock(Mutex&mutex):m_mutex(mutex)
	{
		pthread_mutex_lock(&m_mutex);
		//m_mutex.lock();
		printf("lock\n");
	}
	~xAutoLock()
	{
		pthread_mutex_unlock(&m_mutex);
		//m_mutex.unlock();
		printf("unlock\n");
	}
private:
	Mutex m_mutex;
};