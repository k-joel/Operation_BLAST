//n^2
//tristan helmink
//Event system class

#ifndef _CEventSystem_
#define _CEventSystem_

#include <map>
#include <list>
using namespace std;

#include "CEvent.h"
#include "IListener.h"

class CEventSystem
{
private:

	//map for event storage& registration
	map<CEvent*, list<pair<IListener*,float>>> m_listners;

	//queue for event proccessing & data buffer
	list<tDispatch> m_toProc;

	CEventSystem();
	~CEventSystem();
	CEventSystem(CEventSystem& rhs);
	CEventSystem& operator=(const CEventSystem&);
public:
	static CEventSystem* GetInstance()
	{
		static CEventSystem toReturn;
		return &toReturn;
	}

	void Initialize();
	void Shutdown();

	CEvent* FindEvent(char* eventName);
	void RemoveEvent(char* eventName);

	void RegisterForEvent(IListener& toRegister, CEvent& toRespondTo, float processingPriority = -1.0f);
	void DeregisterForEvent(IListener& toUnregister, CEvent& toIgnore);

	void DispatchEvent(CEvent& toDispatch, char* dataBuffer, float processingPriority = -1.0f);
	void ProcessEvents();
};

#endif