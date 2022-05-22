//n^2
//tristan helmink
//CEvent class class

#ifndef _CEVENT_
#define _CEVENT_

#include <windows.h>

class CEvent
{
private:
	char name[16];
	CEvent();
public:
	CEvent(char* name) { strcpy_s(this->name, 16, name); }

	const char* GetEventName()
	{
		return name;
	}
};

struct tDispatch
{
	float m_processingPriority;
	CEvent* m_pEvent;
	char* m_buffer;
};

#endif