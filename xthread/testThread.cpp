#include "testThread.h"


testThread::testThread(void)
{
}


testThread::~testThread(void)
{
}
void testThread::run()
{printf("i am the child thread ,id:%d",get_thread_id());}