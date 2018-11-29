class Threadbase
{
public:
	Threadbase(bool bDetach=true);
	virtual ~Threadbase(){};
	virtual void run()=0;		//业务接口
	int start();			//启动线程
	int join();				//等待线程结束
	void destory();			//销毁线程所申请的资源

	int get_thread_id(){return thr_id;}
	int set_thread_id(unsigned long thrID){thr_id=thrID;}

protected:
	static unsigned int __stdcall thread_proxy(void* arg);

private:
	size_t thr_id;			//线程id
	bool bExit_;			//线程是否要退出标志

};