#pragma once
#include <map>
#include <vector>
#include "xtimeheap.hpp"
//event_t 事件类型, 支持以下四种
typedef unsigned int event_t;
//handle_t 描述符，如socket
typedef int handle_t;
	enum{
		xReadEvent    =0x01,
		xWriteEvent   =0x02,
		xErrorEvent   =0x04,
		xEventMask    =0xff
};

//事件处理基类句柄，
class xEventHandler
{
public:
	// 获取需要注册的套截字或者其他文件描述符
	virtual handle_t GetHandler()const = 0;
	virtual void HandleRead(){}
	virtual void HandlerWrite(){}
	virtual void HandlerError(){}
};

// 分发器实现（IO复用分离时间的机制）

class xEventDemultiplexer
{
public:
	virtual ~xEventDemultiplexer(){}
	//分离器等待事件到来
	virtual int WaitEvents(std::map<handle_t,xEventHandler*>*handlers,
		int timeout=0,xtime_heap* event_timer=NULL )=0;
	virtual int RequestEvent(handle_t handle,event_t evt)=0;


	virtual int UnrequestEvent(handle_t handle)=0;
};