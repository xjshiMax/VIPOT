//ͨ��˽�л���������͸�ֵ�������ﵽ�����ɱ�������Ŀ��
//�����Ҫ��Ķ����ܱ�������ֱ�Ӽ̳�xNoncopyable����
class xNoncopyable
{
protected:
	xNoncopyable();
	~xNoncopyable();
private:
	xNoncopyable(const xNoncopyable&);
	xNoncopyable& operator = (const xNoncopyable);
};