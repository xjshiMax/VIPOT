//2018/12/10
//�򵥷�װreactor
//namespace CRWORK
//{
#pragma  once 

#include <map>
#include "xbaseclass.hpp"
#include "xtimeheap.hpp"
#include "xEventDemultiplexer.hpp"

class xReactorImplentation;
class xReactor
{
public:
	xReactor();
// 	{
// 		m_reactorimp = new xReactorImplentation();
// 	}
	~xReactor();
	int start() //�����¼�ѭ��
	{
		HandlerEvents();
	}
	//ע���¼����¼���Ӧ������¼����ͣ���д
	int RegisterHandler(xEventHandler*handler,event_t event_);

	int RemoveHandler(xEventHandler* handler);
	void HandlerEvents();
	int RegisterTimeTask(xheaptimer* timerevent);
	private:
		xReactor(const xReactor&);
		xReactor & operator=(const xReactor&);
private:

	xReactorImplentation* m_reactorimp;

};
#define INITSIZE 100
class xReactorImplentation
{
public:
	//���������ʱ����С�ѵ�����СΪ INITSIZE=100.����������Զ�resize()
	xReactorImplentation()
	{
		m_demultiplexer = static_cast<xEventDemultiplexer*>(new xEpollDemultiplexer());
		m_eventtimer = new xtime_heap(INITSIZE);
	}
	~xReactorImplentation()
	{
		delete m_demultiplexer;
	}

	int RegisterHandler(xEventHandler*handler,event_t event_);
	int RemoveHandler(xEventHandler* handler);
	void HandlerEvents();
	int RegisterTimeTask(xheaptimer* timerevent);
private:
	xEventDemultiplexer *		m_demultiplexer;
	std::map<handle_t,xEventHandler*> m_handlers;
	xtime_heap* m_eventtimer;
};

xReactor::xReactor()
{
	m_reactorimp = new xReactorImplentation();
}
xReactor::~xReactor()
{
	if(m_reactorimp!=NULL)
		delete m_reactorimp;
}
int xReactor::RegisterHandler(xEventHandler*handler,event_t event_)
{
	return m_reactorimp->RegisterHandler(handler,event_);
}

int xReactor::RemoveHandler(xEventHandler* handler)
{
	return m_reactorimp->RemoveHandler(handler);
}
void xReactor::HandlerEvents()
{
	m_reactorimp->HandlerEvents();
}
int xReactor::RegisterTimeTask(xheaptimer* timerevent)
{
	m_reactorimp->RegisterTimeTask(timerevent);
}

int xReactorImplentation::RegisterHandler(xEventHandler*handler,event_t event_)
{
	handle_t handle = handler->GetHandler();
	std::map<handle_t,xEventHandler*>::iterator it = m_handlers.find(handle);
	if(it == m_handlers.end())
	{
		m_handlers[handle] = handler;
	}
	return m_demultiplexer->RequestEvent(handle,event_);
}
int xReactorImplentation::RemoveHandler(xEventHandler* handler)
{
	handle_t handle =handler->GetHandler();
	m_handlers.erase(handle);
	return m_demultiplexer->UnrequestEvent(handle);
}
//��������¼�ѭ����
//���start
void xReactorImplentation::HandlerEvents()
{
	while(1)
	{
		int timeout = 0;
		if(m_eventtimer->top() ==NULL)
		{
			timeout = 0;
		}
		else
		{
			timeout = (m_eventtimer->top()->expire-time(NULL))*1000;
		}
		m_demultiplexer->WaitEvents(&m_handlers,timeout,m_eventtimer);

	}
}
int xReactorImplentation::RegisterTimeTask(xheaptimer* timerevent)
{
	if(timerevent == NULL)
		return -1;
	m_eventtimer->add_timer(timerevent);
	return 0;
}