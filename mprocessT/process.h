#include "../include/inclu.h"
class process
{
public:
	process():m_pid(-1){}
public:
	pid_t m_pid;
	int m_pipefd[2];
};

template<typename T>
class processpool
{
private:
	processpool(int listenfd,int process_number=8);
public:
	static processpool<T> *m_instance;

	static processpool<T> *create(int listenfd,int process_number=8);

	~processpool()
	{
		delete []m_sub_process;
	}
	void run();


private:
	void setup_sig_pipe();
	void run_parent();
	void run_child();

private:
	static const int MAX_PROCESS_NUMBER = 16;
	static const int USER_PER_PROCESS = 65536;
	static const int MAX_EVENT_NUMBER = 10000;
	int m_process_number;
	int m_idx;
	int m_epollfd;
	int m_listenfd;
	int m_stop;
	process * m_sub_process;
	

};
static void removefd(int epollfd,int fd);
static void addfd(int epollfd,int fd);
static int setnonblocking(int fd);
static void sig_handler(int sig);
static void addsig(int sig,void(handler)(int),bool restart=true);
static int sig_pipefd[2];
static int setnonblocking(int fd)
{
	int old_option = fcntl(fd,F_GETFL);
	int new_option = old_option | O_NONBLOCK;
	fcntl(fd,F_SETFL,new_option);
	return old_option;
}
static void addfd(int epollfd,int fd)
{
	epoll_event event;
	event.data.fd=fd;
	event.events = EPOLLIN|EPOLLET;
	epoll_ctl(epollfd,EPOLL_CTL_ADD,fd,&event);
	setnonblocking(fd);
}
static void removefd(int epollfd,int fd)
{
	epoll_ctl(epollfd,EPOLL_CTL_DEL,fd,0);
	close(fd);
}
static void sig_handler(int sig)
{
	int save_errno=errno;
	int msg = sig;
	send(sig_pipefd[1],(char*)&msg,1,0);
	errno=save_errno;
}
static void addsig(int sig,void(handler)(int),bool restart)
{
	struct sigaction sa;
	memset(&sa,'\0',sizeof(sa));
	sa.sa_handler = handler;
	if(restart)
	{
		sa.sa_flags |= SA_RESTART;
		sigfillset(&sa.sa_mask);
		assert(sigaction(sig,&sa,NULL)!=-1);
	}
}

template<typename T>
processpool<T> *processpool<T>::m_instance=NULL;

template<typename T>
void processpool<T>::run()
{
	if(m_idx!=-1)
	{
		run_child();
		return;
	}
	run_parent();
}
template<typename T>
processpool<T> *processpool<T>::create(int listenfd,int process_number)
{
	if(!m_instance)
	{
		m_instance = new processpool<T>(listenfd,process_number);
	}
	return m_instance;
}
template<typename T>
processpool<T>::processpool(int listenfd,int process_number)
	:m_listenfd(listenfd),m_process_number(process_number),m_idx(-1),m_stop(false)
{
	assert((process_number > 0)&&(process_number<=MAX_PROCESS_NUMBER));
	m_sub_process = new process[process_number];
	assert(m_sub_process);
	//创建process_number个子进程，并建立他们和父进程之间的管道。
	for(int i=0 ;i<process_number;++i)
	{
		int ret = socketpair(PF_UNIX,SOCK_STREAM,0,m_sub_process[i].m_pipefd);
		assert(ret==0);
		m_sub_process[i].m_pid = fork();
		if(m_sub_process[i].m_pid>0) //父进程
		{
			close(m_sub_process[i].m_pipefd[1]);
			continue;
		}
		else 
		{
			close(m_sub_process[i].m_pipefd[0]);
			m_idx = i;
			break;
		}
	}
}

template<typename T>
void processpool<T>::setup_sig_pipe()
{
	m_epollfd = epoll_create(5);
	int ret = socketpair(PF_UNIX,SOCK_STREAM,0,sig_pipefd);
	setnonblocking(sig_pipefd[1]);
	addfd(m_epollfd,sig_pipefd[0]);
	addsig(SIGCHLD,sig_handler);
	addsig(SIGTERM,sig_handler);
	addsig(SIGINT,sig_handler);
	addsig(SIGPIPE,SIG_IGN);

}
// template<typename T>
// void processpool<T>::run()
// {
// 	if(m_idx!=-1)
// 	{
// 		run_child();
// 		return;
// 	}
// 	run_parent();
// }
template <typename T>
void processpool<T>::run_child()
{
	setup_sig_pipe();
	int pipefd=m_sub_process[m_idx].m_pipefd[1];
	addfd(m_epollfd,pipefd);

	epoll_event events[MAX_EVENT_NUMBER];
	T *users = new T[USER_PER_PROCESS];
	int number = 0;
	int ret=-1;
	while(!m_stop)
	{
		number = epoll_wait(m_epollfd,events,MAX_EVENT_NUMBER,-1);
		if((number<0)&&(errno!=EINTR))
		{
			printf("epoll failure!\n");
			break;
		}
		for(int i=0;i<number;i++)
		{
			int sockfd=events[i].data.fd;
			if((sockfd==pipefd)&&(events[i].events&EPOLLIN))
			{
				int client=0;
				ret=recv(sockfd,(char*)&client,sizeof(client),0);
				if(((ret<0)&&(errno!=EAGAIN))||ret==0)
				{
					continue;
				}
				else
				{
					struct sockaddr_in client_address;
					socklen_t client_addrlength = sizeof(client_address);
					int connfd = accept(m_listenfd,(struct sockaddr*)&client_address,&client_addrlength);
					if(connfd<0)
					{
						printf("errno is :%d\n",errno);
						continue;
					}
					addfd(m_epollfd,connfd);
					users[connfd].init(m_epollfd,connfd,client_address);
				}
			}	
			//下面处理子进程接收到的信号
			else if((sockfd==sig_pipefd[0])&&(events[i].events&EPOLLIN))
			{
				int sig;
				char signals[1024];
				ret=recv(sig_pipefd[0],signals,sizeof(signals),0);
				if(ret<=0)
				{
					continue;
				}
				else
				{
					for(int i=0;i<ret;++i)
					{
						switch(signals[i])
						{
						case SIGCHLD:
							{
								pid_t pid;
								int stat;
								while((pid=waitpid(-1,&stat,WNOHANG)))
								{
									continue;
								}
								break;
							}
						case SIGTERM:
						case SIGINT:
							{
								m_stop=true;
								break;
							}
						default:
							{
								break;
							}

						}
					}
				}
			}
			else if(events[i].events&EPOLLIN)
			{
				users[sockfd].process();
			}
			else
			{
				continue;
			}


		}
	}
	delete [] users;
	users=NULL;
	close(pipefd);
	close(m_epollfd);
}

template <typename T>
void processpool<T>::run_parent()
{
	setup_sig_pipe();
	addfd(m_epollfd,m_listenfd);
	epoll_event events[MAX_EVENT_NUMBER];
	int sub_process_counter = 0;
	int new_conn=1;
	int number=0;
	int ret=-1;
	while(!m_stop)
	{
		number=epoll_wait(m_epollfd,events,MAX_EVENT_NUMBER,-1);
		if((number<0)&&(errno!=EINTR))
		{
			printf("epoll failure\n");
			break;
		}
		for(int i=0;i<number;i++)
		{
			int sockfd=events[i].data.fd;
			if(sockfd==m_listenfd)
			{
				int i=sub_process_counter;
				do
				{
					if(m_sub_process[i].m_pid!=-1)
					{
						break;
					}
				}
				while (i!=sub_process_counter);

				if(m_sub_process[i].m_pid==-1)
				{
					m_stop=true;
					break;
				}
				sub_process_counter=(i+1)%m_process_number;
				send(m_sub_process[i].m_pipefd[0],(char*)&new_conn,sizeof(new_conn),0);
				printf("send request to child %d\n",i);

			}
			else if((sockfd==sig_pipefd[0])&&(events[i].events&EPOLLIN))
			{
				int sig;
				char signals[1024];
				ret=recv(sig_pipefd[0],signals,sizeof(signals),0);
				if(ret<=0)
				{
					continue;
				}
				else
				{
					for(int i=0;i<ret;i++)
					{
						//如果进程池中第i个子进程退出了。
						//则主进程关闭通信管道，并设置相应的m_pid为-1，以标记该子进程已经退出
						switch(signals[i])
						{
						case SIGCHLD:  //子进程结束信号
							{
								pid_t pid;
								int stat;
								while((pid=waitpid(-1,&stat,WNOHANG))>0)
								{
									for(int i=0;i<m_process_number;++i)
									{


										if(m_sub_process[i].m_pid==pid)
										{
											printf("child %d join \n",i);
											close(m_sub_process[i].m_pipefd[0]);
											m_sub_process[i].m_pid=-1;
										}
									}
								}
								//如果所有子进程都已经退出了，则父进程也退出
								m_stop=true;
								for(int i=0;i<m_process_number;++i)
								{
									if(m_sub_process[i].m_pid!=-1)
									{
										m_stop=false;
									}
								}
							}
							break;
						case SIGTERM:  //终止信号
						case SIGINT:   //键盘终端（break建被按下）
							{
								//如果父进程接收到终止信号，那么就杀死所有子进程，并等待它通知子进程结束更好的
								//方法是向父/子进程之间的通信管道发送特殊数据
								printf("kill all the child now\n");
								for(int i=0;i<m_process_number;++i)
								{
									int pid=m_sub_process[i].m_pid;
									if(pid!=-1)
									{
										kill(pid,SIGTERM);
									}
								}
								break;
							}
						default:
							{
								break;
							}

						}//switch
					}//for
				}//else
			}//else if
			else
			{
				continue;
			}

		}//for
	}//while
	close(m_epollfd);
}