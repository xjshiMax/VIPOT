//通过私有化拷贝构造和赋值函数而达到独享不可被拷贝的目的
//如果需要类的对象不能被拷贝，直接继承xNoncopyable即可
class xNoncopyable
{
protected:
	xNoncopyable();
	~xNoncopyable();
private:
	xNoncopyable(const xNoncopyable&);
	xNoncopyable& operator = (const xNoncopyable);
};