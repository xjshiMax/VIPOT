#pragma once
#include <map>
#include <vector>
#include "xtimeheap.hpp"
//event_t �¼�����, ֧����������
typedef unsigned int event_t;
//handle_t ����������socket
typedef int handle_t;
	enum{
		xReadEvent    =0x01,
		xWriteEvent   =0x02,
		xErrorEvent   =0x04,
		xEventMask    =0xff
};

//�¼������������
class xEventHandler
{
public:
	// ��ȡ��Ҫע����׽��ֻ��������ļ�������
	virtual handle_t GetHandler()const = 0;
	virtual void HandleRead(){}
	virtual void HandlerWrite(){}
	virtual void HandlerError(){}
};

// �ַ���ʵ�֣�IO���÷���ʱ��Ļ��ƣ�

class xEventDemultiplexer
{
public:
	virtual ~xEventDemultiplexer(){}
	//�������ȴ��¼�����
	virtual int WaitEvents(std::map<handle_t,xEventHandler*>*handlers,
		int timeout=0,xtime_heap* event_timer=NULL )=0;
	virtual int RequestEvent(handle_t handle,event_t evt)=0;


	virtual int UnrequestEvent(handle_t handle)=0;
};