//2018-12-6
//没有结构体的队列，也不依赖stl
//boost用在封装reactor里

template <typename Operation>
class xtaskqueue;			//队列类

//辅助类
class xqueueHelp			
{
public:
	template <typename Operation>
//获取 o元素的下一个元素
	static Operation* next(Operation*o)
	{
		return static_cast<Operation*> (o->nextOperation_);
	}
//
	template <typename Operation1,typename Operation2>
	static void next(Operation1 *&o1,Operation2 * 02)
	{
		o1->nextOperation_ = o2;
	}

	template <typename Operation>
	static void destroy(Operation* o)
	{
		o->destroy();
	}
	template <typename Operation> 
	static	Operation *& front(xtaskqueue<Operation> &q)
	{
		return q.frontOperation_;
	}
	template <typename Operation>
	static Operation *& back(xtaskqueue<Operation> &q)
	{
		return q.backOperation_;
	}
};

template<typename Operation>
class xtaskqueue
{
public:
	xtaskqueue():frontOperation_(0),backOperation(0){}
	~xtaskqueue()
	{
		while(Operation*op = frontOperation_)
		{
			//如果队列的首元素不为空，pop()之后销毁队列
			pop();
			xqueueHelp::destroy(op);

		}
	}
private:

	friend class xqueueHelp;
	Operation* frontOperation_;
	Operation* backOperation_;
};