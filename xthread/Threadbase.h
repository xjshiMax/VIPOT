class Threadbase
{
public:
	Threadbase(bool bDetach=true);
	virtual ~Threadbase();
	virtual run()=0;		//业务接口
	int start();			//启动线程
	int join();				//等待线程结束
	void destory();			//销毁线程所申请的资源

	int get_thread_id(){return thr_id_;}
	int set_thread_id(unsigned long thrID){thr_id=thrID;}

protected:
	//static void*__stdcall thread_proc()

private:
	size_t thr_id;			//线程id
	bool bExit_;			//线程是否要退出标志

};