#include "Threadbase.h"
Threadbase::Threadbase(bool bDetach=true)
{
	thr_id=0;
}
int Threadbase::start()
{

#ifdef WIN32
	unsigned int nval=_beginthreadex(0,0,run,arg,0,&thr_id);
#endif
}
int Threadbase::join()
{

}
void Threadbase::destory()
{

}
