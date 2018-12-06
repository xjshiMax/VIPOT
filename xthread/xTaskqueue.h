//2018-12-6
//û�нṹ��Ķ��У�Ҳ������stl
//boost���ڷ�װreactor��

template <typename Operation>
class xtaskqueue;			//������

//������
class xqueueHelp			
{
public:
	template <typename Operation>
//��ȡ oԪ�ص���һ��Ԫ��
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
			//������е���Ԫ�ز�Ϊ�գ�pop()֮�����ٶ���
			pop();
			xqueueHelp::destroy(op);

		}
	}
private:

	friend class xqueueHelp;
	Operation* frontOperation_;
	Operation* backOperation_;
};