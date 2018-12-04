//2018-12-4
//��װ����ָ��
//1��
//2��
//3��

#define AtomicCountType unsigned int 
template<class Type>
class xsmartPtr
{
public:
	xsmartPtr(Type* pObj):m_Count(1),m_Ptr(pObj){}
	~xsmartPtr(){
		delete m_Ptr;
		m_Ptr=NULL;
	}
	template<typename pType>
	static inline void decRef(pType*ptr)
	{
		if(ptr!=NULL&&ptr->m_Count.dec()==0)
			delete ptr;
	}
	template<typename pType>
	static inline pType* addRef(pType *ptr)
	{
		if(ptr!=NULL&&ptr->m_Count.inc())
			return ptr;
	}
	//��ȡԭʼָ��
	inline Type *getImplPtr(void){return m_Ptr;}
	//��ȡԭʼ����
	inline Type &getInplRef(void) {return *m_Ptr;}
	inline const Type * getConstImplPtr(void) const {return m_Ptr;}
	inline const Type * getConstImplRef(void) const {return *m_Ptr;}
	AtomicCountType getCount() {return m_Count.get();}
private:
	xAtomicInt32 m_Count;	//���ü���
	Type       m_Ptr;		//��ʵָ�룬�����ü�����Ϊ0ʱָ�����ĵ�ַ�������ͷţ�ָ��NULL.
};

//������ָ���ٴν��з�װ�������ز������������
template<class ptrType>
class xsmartRef
{
public:
	xsmartRef(void):m_pImp(NULL){}
	xsmartRef(const xsmartRef &ref):m_pImp(NULL) {reset(ref);}
	//explicit ���εĺ�����ʵ��ת��Ϊ�ββ�����ʽת����һ�����ε��������Ĺ��졣��������Զ�ȥ������ʽת��
	explicit xsmartRef(Type* ptr):m_pImp(ptr == NULL? NULL:new xsmartPtr(ptr)){}
	template <class Type2>
	explicit xsmartRef(Type* ptr):m_pImp(ptr==NULL ? NULL:new xsmartPtr(ptr)){}
	~xsmartRef(){release();}

	//����ָ������
	//���ٵ�ǰ����ָ������ü�����������ü���Ϊ0����ɾ������Ȼ���ұߵ�
	//���󸳸��������ü�����һ��
	inline const xsmartRef& reset(const xsmartRef&rh){
		if(this!=&rh)
		{
			xsmartPtr::decRef(m_pImp);
			m_pImp = xsmartPtr::addRef(rh.m_pImp);
		}
		return *this;
	}

	//���õ�ǰ�Ķ���ָ��
	inline const xsmartRef &reset(ptrType * ptr)
	{
		release();
		if(ptr!=NULL)	m_pImp = new xsmartPtr(ptr);
		return *this;
	}

	//�������ʹ������Ի��������ָ���������
	inline const xsmartRef & reset(Type2 *ptr)
	{
		release();
		if(ptr!=NULL)	m_pImp = new xsmartPtr(ptr);
		return *this;
	}

	inline const xsmartRef& operator = (Type*ptr) {return reset(ptr);}
	inline const xsmartRef &operator = (xsmartRef& rh) {return reset(rh);}
	template<typename Type2>

	// if(Inst)...  �ж϶����Ƿ�Ϊ�ա� ���������ָ��Ϊ�գ��򷵻ؿգ����򷵻�this.
	inline const xsmartRef& operator = (Type2 * ptr) {return reset(ptr);}
	//if(!Inst)...  �ж��Ƿ���ڣ������Ƿ�Ϊ��
	bool operator !() const
	{
		return (m_pImp == NULL|| m_pImp->getConstImplPtr()==NULL)
	}
	operator void *() const
	{
		if(m_pImp!=NULL&&m_pImp->getConstImplPtr()!=NULL)
			return (void*)this;
		return NULL;
	}
	inline const ptrType* operator ->() const {return getConstPtr();}
	inline const ptrType & operator *() const throw(std::rntime_error) {return getConstRef();}
	inline ptrType * operator ->() {return getPtr();}
	inline ptrType * operator ->() {return getRef();}

	inline void release()
	{
		xsmartPtr::decRef(m_pImp);
		m_pImp=NULL;
	}
	inline ptrType * getPtr(void){return (m_pImp==NULL)?NULL:m_pImp->getImplPtr();}
	inline const ptrType*  getConstPtr(void) const {return (m_pImp==NULL)?NULL:m_pImp->getConstImplPtr();}
	inline ptrType & getRef(void) 
	{
		if(m_pImp==NULL)
			throw std::runtime_error("Not Init Counting Pointer");
		return (m_pImp == NULL)?NULL:m_pImp->getImplRef();
	}
	inline const Type& getConstRef(void) const throw(std::runtime_error)
	{
		if(m_pImp==NULL)
			throw std::runtime_error("Not Init Counting Pointer");
		return m_pImp->getConstImplRef();
	}
	inline void swap(xsmartRef& rhs)
	{
		std::swap(m_pImp,rhs->m_pImp);
	}
	inline AtomicCountType getRefCount const {return m_pImp==NULL?0:m_pImp->getCount();}
	inline void incRef(void) {xsmartPtr::addRef(m_pImp);}
	inline void decRef(void) {xsmartPtr::decRef(m_pImp);}
protected:
	xsmartPtr * m_pImp;
};