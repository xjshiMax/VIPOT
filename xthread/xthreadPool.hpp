//2018-12-5
//�������ڶ�����̳߳�
//      threadpool --------taskbase.run()
//
#include <stdio.h>
//�������ļ����ࡣÿ��job����xtaskbase������Ķ���
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

//��Ҫ���óɲ��ɸ��Ƶ���
class xthreadPool
{
public:
	xthreadPool(const char* poolname);
	virtual ~xthreadPool(){stopPool();};

	void initPool();
	void startPool(bool defaultpools=false );
	void stopPool(bool defaultpools=true);  //Ĭ��defaultpoolsΪtrue������������߳�
	bool pushObj();
	bool trypushObj();
	void waitforAllTaskDone(void); // �ȴ������������

	bool isIdle(void) const {}    //�̳߳ض����Ƿ�Ϊ��
	bool isBusy(void) const {}		//�̳߳��й����̴߳��ڿ����߳�
	bool isFull(void) const {}		//�̳߳��Ƿ�����
	void dump();					//��ȡ�̳߳���Ϣ������һ��ʹ�÷ָ���
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
