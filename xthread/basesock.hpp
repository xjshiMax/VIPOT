//2018-12-13
/*
��װ�����׽��ֽӿڣ��������ļ�ʹ�� linux����windows
1) �����ӿ� socket bind listen accept connect send recv close shutdown peek
2��udp��tcp
3) �����㲥�Ͷಥ
*/
#ifdef WIN32
#include <WinSock2.h>
#include <WS2tcpip.h>
#pragma comment(lib,"ws2_32.lib")

#else
#include <sys/socket.h>
#include <netdb.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>
#endif

inline static int InitSocket()
{

}

//socket() �����׽��֡� 
/*
type: SOCK_STREAM ���� SOCK_DGRAM
*/
inline static int CreateSocket(int type,int af=AF_INET ,int protocal=0)
{
	int socket_=socket(af,type,protocal);
#ifdef WIN32
	int nBuf = 0;
	setsockopt(socket_, SOL_SOCKET, SO_SNDBUF,(char *) &nBuf, sizeof(nBuf));
#endif
}
//bind �׽��ֺ�ָ����ַ�󶨡�
/*
socket:��ʶһ���׽ӿڵ������֡�
addr: ָ����ַ��
struct socketaddr_in{
	sa_family_t sin_family;
	in_port_t sin_port;
	struct in_addr sin_addr;
}
struct in_addr{
	uint32_t s_addr;
}
addrlen: ��ַ�ṹ�峤�ȡ�
�ɹ�����0��ʧ�ܷ���-1��
*/
inline static int BindSocket(int socket,struct sockaddr*addr,socklen_t addrlen)
{
	//struct sockaddr 
	return bind(socket,addr,addrlen);
}
// setsockopt: �����������ͣ�����ӿڵ�����ѡ��ֵ�������ٲ�ͬЭ����ϴ���ѡ�������������������ߵ�
//				���׽ӿڡ�����ϵ�ѡ�ѡ��Ӱ���׽ӿڵĲ��������Ƿ�����ͨ�������н��գ��㲥�����Ƿ����
//				���׽ӿڽ���
/*
socket:��ʶһ���׽ӿڵ������֡� 
level: ������Ĳ�Σ�Ŀǰֻ֧��SOL_SOCKET �� IPPOTO_TCP ��
option_name: ��Ҫ���õ�ѡ�
option_value�� ָ�룬ָ����ѡ��ֵ�Ļ�������
option_len�� optval�������ĳ��ȡ�
�ɹ�����0.ʧ�ܷ���-1.
�÷���
1. option_name=SO_DEBUG // �򿪻��߹رյ�����Ϣ��
	��option_value������0ʱ���򿪵�����Ϣ������رյ�����Ϣ��
2. option_name= SO_REUSEADDR //�򿪻�رյ�ַ���ù��ܡ�
	��option_value������0ʱ���򿪣����򣬹رա���ʵ�������Ĺ���ʱ��sock->sk->sk_reuseΪ1����0��
ԭ��closesocket �ӿ��ڹر��׽���ʱһ�㲻�������رն���Ҫ����TIME_WAIT �Ĺ��̡�����������ø�socket��������SO_REUASEADDR��
������bool bReuseaddr=TRUE;
	 setsockopt(socket,SOL_SOCKET,SO_REUSEADDR,(constchar*)&bReuseaddr,sizeof(bool))
3. option_name=SO_BROADCAST //������ֹ���͹㲥���ݡ�
	��option_value ������0ʱ���������򣬽�ֹ��ʵ�������Ĺ�������sock->sk->sk_flag���û���SOCK_BROADCASTλ��
4. option_name= SO_SNDBUF // ���÷��ͻ�������С��
	���ͻ������Ĵ�С���д�С���Ƶģ������� 256*��sizeof(struct sk_buff)+256��,����Ϊ2048�ֽڡ�
	�ò�����sock->sk->sk_sndbuf����Ϊval * 2��֮����Ҫ����2���Ƿ�ֹ�������ݵķ��ͣ�ͻȻ���»����������
ԭ�� ��send������ʱ�򣬷��ص���ʵ�ʷ��ͳ�ȥ���ֽڣ�ͬ�������͵�socket���������ֽڣ��첽����ϵͳĬ�ϵ�״̬���ͺͽ���һ�ε�8688
	  �ֽڣ�Լ8.5k���� ��ʵ�ʵĹ����з����������ͽ����������Ƚϴ󣬿�������socket����������������send(),recv()���ϵ�ѭ���շ���
������ int nSendBuf=32*1024 //32k
	  setsockopt(socket,SOL_SOCKET,SO_SNDBUF.(const char*)nSendBuf,sizeof(nSendBuf));
5. option_name=SO_RCVBUF //���ý��ջ������Ĵ�С
	���ջ�������С�������߷ֱ���256*��sizeof(struct sk_buff)+256����256�ֽڡ�
6. option_name=SO_RCVTIMEO // ���ý��ճ�ʱʱ�� ��ʱδ���գ���᷵��0.
	��ѡ����ͳ�ʱʱ�丳��sock->sk->sk_sndtimeo��
ԭ����send()����recv() ��������ʱ��������״����ԭ���շ�����Ԥ�ڽ��У��������շ���ʱ/
������int nNetTimeout=1000; //��
	 setsockopt(socket,SOL_SOCKET,SO_SNDTIMEO/SO_RCVTIMEO.(char*)&nNetTimeout,sizeof(int));
7. option_name=SO_ASNDTIMEO //���÷��ͳ�ʱʱ��
	��ѡ�����ս����ͳ�ʱʱ�丳��sock->sk->sk_sndtimeo��
8. option_name=SO_LINGER // ���ѡ����close��shutdown���ȵ������׽������Ŷӵ���Ϣ�ɹ����ͻ򵽴���ʱʱ��
	�� �Ż᷵�أ����򣬽��������ء�
	struct linger{
		int l_onoff;  // l_onoff δ0�رգ�Ϊ1��������
		int l_linger;
	}
ԭ�� ����ڷ������ݵĹ�����ȥ�رգ�send()��û��ɣ�����������closesocket()����ǰ����һ���ȡ
	  shutdown���ǣ�SO_BOTH��,�������ݻᶪʧ������SO_LINGER ����Ȼ�����ݷ������Ժ��ٹرա�
����:  linger m_sling;
	  m_sling.l_onoff=1; (�������closesocket���������û������ɣ�Ҳ���Զ��ݶ�����)
	  m_sling.l_linger=5 // ������5��
	  setsockopt(socket,SOL_SOCKET,SO_LINGER,(const char*)&m_sling,sizeof(linger));
*/
inline static int SetSockOpt( int socket, int level, int option_name,
	const void *option_value, size_t option_len)
{
	return setsockopt(socket,level,option_name,option_value,option_len);
}
inline static int AccpetSocket(int socket,struct sockaddr* addr,socklen_t*addrlen)
{
	return accept(socket,addr,addrlen);
}
//listen
/*
socket: ��listen���׽��֡� listen�Ὣ���׽�����Ϊ�����׽���
backlog: ָ������socket�����pending�����Ӷ��пɴﵽ������ȡ����Ϊ128.
		һ��С��30

�ɹ�����0��ʧ�ܷ���-1
*/
inline static int ListenSocket(int socket,int backlog)
{
	return listen(socket,backlog);
}

//connect �������ӷ����
/*
socket: û�а󶨵��׽��֡����ؿͻ����׽��֡�
server_addr: ����˵�ַ
addrlen �� ����˵�ַ����
�ɹ�����0 ��ʧ�ܷ���-1.
*/
inline static int ConnectSocket(int socket,const struct sockaddr* server_addr,socklen_t addrlen)
{
	return connect(socket,server_addr,addrlen);
}
//send ��������
/*
socket: �Է�socket
buf :   ���ݴ�
len :   ���ݳ���
flags:  һ������Ϊ0
�ɹ�����0 ��ʧ�ܷ���-1
*/
inline static int SendSocket(int socket,char*buf,int len,int flags=0)
{
#ifdef WIN32
	return send(socket,buf,len,flags);
#else
	return write(socket,buf,len);
#endif
}
//recv  ��������
/*
socket:ָ�����ն��׽���������
buf ������������Ž��յ�������
len ����������
flags һ������Ϊ0
�ɹ�����0��ʧ�ܷ���-1 ��windows��ʧ�ܷ��ش����룩
*/
inline static int ReadSocket(int socket,char* buf,int len ,int flags=0)
{
#ifdef WIN32
		int res= recv(socket,buf,len,flags);
		if(res==0)
			return 0;
		else if(res==SOCKET_ERROR)
		{
			int err=WSAGetLastError();
			return err;
		}
#else
	return recv(socket,buf,len,flags);
#endif
}
inline static CloseSocket(int socket)
{
#ifdef WIN32
	return closesocket(socket);
#else
	return close();
#endif
}
//shutdown close��Ѷ�дͨ��ȫ���رգ���ʱ����ֻϣ���ر�һ���������ʱ������
//		����ʹ��shutdown.
/*
socket: ��Ҫ�رյ��׽���
howto: 0 ���ʱ��ϵͳ��رն�ͨ�������ǿ��Լ������׽�����д��
	   1 ���ʱ��ر�дͨ�������ǿ��Զ���
	   2 �رն�дͨ������closeһ���ˡ��ڶ��̳߳��������м����ӽ��̹���һ���׽���ʱ��
	     �������ʹ��shutdown����ô���е��ӽ��̶����ܲ����ˣ����ʱ������ֻ�ܹ�ʹ��close���ر�
		 �ӽ��̵��׽��֡�
�ɹ�����0��ʧ�ܷ���-1.
*/
inline static int ShutDownSocket(int socket,int howto)
{
	return shutdown(socket,howto);
}


//��socket ��ص�һЩ������װ��

namespace Network_function
{
	//��ȡ����˿ں�
	/*
		lpServcie :
	*/
	unsigned int getPortNumber(const char* lpService,const char*lptransport)throw();
	//ͨ���������ƻ�ȡ��������
	bool getHostByName(IN const char *host,OUT struct in_addr&addr)throw(){return true;}
	//ͨ�������ַ��ȡ������Ϣ
	std::string getHostByAddr(IN const struct in_addr& addr)throw(){return "";}
	//��ȡ������IO��socket �����
	int getSocktAsyncError(IN int socket){return 0;}
	//����socket�Ƿ�������
	int setSocketBlock(IN int socket,IN bool boBlocking){return 0;}

	//����tcp ��ʱ
	int setTCPDelay(IN int socket,bool boDelay=false){return 0;}
	//
	bool getPeerInfo(IN int socket,OUT struct sockaddr_in &addr);
	bool getPeerInfo(IN int socket,OUT char*ip,OUT int &port);
	bool getLocalInfo(IN int socket ,OUT struct sockaddr_in &addr );
	bool getLocalInfo(IN int socket,OUT char*ip,OUT int &port);


}
//
unsigned int Network_function::getPortNumber(const char* lpService,const char*lptransport)throw()
{
	return 0;
}
bool Network_function::getPeerInfo(IN int socket,OUT struct sockaddr_in &addr)
{
	if(socket!= INVALID_SOCKET)
	{
		socklen_t namelen=sizeof(addr);
		if(getpeername(socket,(struct sockaddr*)&addr,(socklen_t*)&namelen)==0)
		{
			return true;
		}
	}
	return false;
}
bool Network_function::getLocalInfo(IN int socket ,OUT struct sockaddr_in &addr )
{
	if(socket!=INVALID_SOCKET)
	{
		socklen_t namelen=sizeof(addr);
		if(getpeername(socket,(struct sockaddr*)&addr,(socklen_t*)&namelen)==0)
		{
			return true;
		}
	}
	return false;
}
bool Network_function::getPeerInfo(IN int socket,OUT char*ip,OUT int &port)
{
	struct sockaddr_in name;
	if(getPeerInfo(socket,name))
	{
		ip=(char*)name.sin_addr;
		port=ntohs(name.sin_port);
		return true;
	}
	return false;
}
bool Network_function::getLocalInfo(IN int socket,OUT char*ip,OUT int &port)
{
	struct sockaddr_in name;
	if ( getLocalInfo(socket, name) )
	{
		ip = (char*)name.sin_addr;
		port = ntohs(name.sin_port);
		return true;
	} 
	return false;
}