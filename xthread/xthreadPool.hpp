//2018-12-5
//创建基于对象的线程池
//      threadpool --------taskbase.run()
//
#include <stdio.h>
//任务对象的即基类。每个job都是xtaskbase派生类的对象
class xtaskbase
{
public:
	xtaskbase(void *arg=NULL,const std::string taskName = "")
		:arg_(arg),taskName_(taskname)
	{

	}
	virtual ~Task(){}
	void setArg(void arg){arg_=arg};
	virtual int run() = 0;
protected:
	void* arg_;
	std::string taskName;

};

//需要设置成不可复制的类
class xthreadPool
{
public:
	xthreadPool(const char* poolname);
	virtual ~xthreadPool(){stopPool();};

	void initPool();
	void startPool(bool defaultpools=false );
	void stopPool(bool defaultpools=true);  //默认defaultpools为true，清除掉所有线程
	bool pushObj();
	bool trypushObj();
	void waitforAllTaskDone(void); // 等待所有任务结束

	bool isIdle(void) const {}    //线程池队列是否为空
	bool isBusy(void) const {}		//线程池中工作线程大于空闲线程
	bool isFull(void) const {}		//线程池是否已满
	void dump();					//获取线程池信息，超过一行使用分隔符
	virtual void onThreadBuild();
	virtual void onThreadEnd();
	static void proxythread(void*);

protected:
	const string	m_strPoolName;
	volatile bool	m_boStartPool;
	size_t			m_nThreadLowThreshold;
	size_t			m_nThreadHighThreshold;
	volatile size_t	m_nFreeThreadCount;
	size_t			m_nMaxThreadUsedCount;
	size_t			m_nIdleTimeout;
	size_t			m_nThreadStackSize;
	size_t			m_nJobNotFinish;
	TLock			m_lockForThread,m_lockForFinishJob;
	TCondition		m_CondForThreadStop,m_CondForBuildThread,m_CondForFinishJob;
};

void xthreadPool::initPool()
{

}
