#include "testThread.h"
#include "xAutoLock.hpp"
int testThread::testnum=0;
testThread::testThread(void)
{
}


testThread::~testThread(void)
{
}
void testThread::run()
{
	pthread_mutex_t zx;
	pthread_mutex_init(&zx,NULL);
	xAutoLock antolock(zx);
	printf("i am the child thread ,id:%d,num=%d\n",get_thread_id(),testnum++);
}