//n^2
//tristan helmink
//Ilistener class

#ifndef _ILISTENER_
#define _ILISTENER_

#include "CEvent.h"

class IListener
{
private:
	
	/*IListener()
	{
		Register();
	}

	~IListener()
	{
		Deregister();
	}*/
public:
	virtual void Register()=0;
	virtual void Deregister()=0;
	virtual void React(const tDispatch& toReactTo)=0;
};

#endif