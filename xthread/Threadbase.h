class Threadbase
{
public:
	Threadbase(bool bDetach=true);
	virtual ~Threadbase(){};
	virtual void run()=0;		//ҵ��ӿ�
	int start();			//�����߳�
	int join();				//�ȴ��߳̽���
	void destory();			//�����߳����������Դ

	int get_thread_id(){return thr_id;}
	int set_thread_id(unsigned long thrID){thr_id=thrID;}

protected:
	static unsigned int __stdcall thread_proxy(void* arg);

private:
	size_t thr_id;			//�߳�id
	bool bExit_;			//�߳��Ƿ�Ҫ�˳���־

};