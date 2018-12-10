//2018/12/10
//�򵥷�װreactor
//namespace CRWORK
//{
#pragma  once 
typedef unsigned int event_t;
typedef int handle_t
enum{
	xReadEvent    =0x01,
	xWriteEvvent  =0x02,
	xErrorEvent   =0x04,
	xEventMask    =0xff
};

//�¼������������
class xEventHandler
{
public:
	virtual handler_t GetHandler()const = 0;
	virtual void HandleRead(){}
	virtual void HandlerWrite(){}
	virtual void Handler Error(){}
};

class xReactorImplenrtion;
class xReactor
{
public:
	xReactor();
	~xReactor();
	//ע���¼����¼���Ӧ������¼�������
	int RegisterHandler(xEventHandler*handler,event_t event_);

	int RemoveHandler(xEventHandler* handler);
	void HandlerEvents();
	int RegisterTimeTask(heap_timer* timerevent);
	private:
		Reactor(const Reactor&);
		Reactor & operator=(const Reactor&);
private:

	xReactorImplenrtion* m_reactorimp;

};
