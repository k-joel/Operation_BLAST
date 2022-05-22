//#include "stdafx.h"
#include "CEventSystem.h"

//Function to sort the container by - TJ
bool isGreaterThan(tDispatch& first, tDispatch& second) { return first.m_processingPriority > second.m_processingPriority; }

CEventSystem::CEventSystem()
{}

CEventSystem::~CEventSystem()
{
Shutdown();
}

//singleton--------------------------------------------------------------------------

void CEventSystem::Initialize()
{}

void CEventSystem::Shutdown()
{

	map<CEvent*, list<pair<IListener*, float>>>::iterator cleanIter = m_listners.begin();

	for(; cleanIter != m_listners.end(); ++cleanIter)
	{
		delete cleanIter->first;
	}
}

//coupling/decoupling--------------------------------------------------------------------------

CEvent* CEventSystem::FindEvent(char* eventName)
{
	//look through container for name
	map<CEvent*, list<pair<IListener*,float>>>::iterator eventIter = m_listners.begin();
	for(;eventIter != m_listners.end(); eventIter++)
	{
		//if it exsists
			//return it
		if(strcmp((*eventIter).first->GetEventName(), eventName) == 0)
		{
			return (*eventIter).first;
		}
	}

	//else
		//create a new one with the name provided, add it to the container, and then return it
	CEvent* toReturn = new CEvent(eventName);
	list<pair<IListener*, float>> newlist;
	m_listners.insert(make_pair(toReturn, newlist));

	return toReturn;
}

void CEventSystem::RemoveEvent(char* eventName)
{
	//look through container for name
	map<CEvent*, list<pair<IListener*,float>>>::iterator eventIter = m_listners.begin();
	for(;eventIter != m_listners.end(); eventIter++)
	{
		//if it exsists
		//remove it
		if(strcmp((*eventIter).first->GetEventName(), eventName) == 0)
		{
			m_listners.erase(eventIter);
			return;
		}
	}

}

void CEventSystem::RegisterForEvent(IListener& toRegister, CEvent& toRespondTo, float processingPriority)
{

	float lowestPriority = -1.0f;

	//add the listener to the events listener group
	list<pair<IListener*,float>>::iterator eventListenerIter;

	if(m_listners.find(&toRespondTo) != m_listners.end())
	{
		list<pair<IListener*,float>>& eventList = (*m_listners.find(&toRespondTo)).second;

		eventListenerIter = eventList.begin();

		for(;eventListenerIter != eventList.end(); eventListenerIter++)
		{
			if((*eventListenerIter).first == &toRegister)
			{
				return;
			}

			if(lowestPriority > (*eventListenerIter).second)
			{
				lowestPriority = (*eventListenerIter).second;
			}

			if((*eventListenerIter).second < processingPriority)
			{
				break;
			}
		}

		//if no priority is given
			//priority = min(last/2.0f, 1.0f/2.0f)
		if(processingPriority < 0.0f)
		{
			processingPriority = min(0.5f, lowestPriority/2.0f);
		}

		//insert in to list or add to end
		if(eventListenerIter != eventList.end())
		{
			eventList.insert(eventListenerIter, make_pair(&toRegister,processingPriority));
		}
		else
		{
			eventList.push_back(make_pair(&toRegister, processingPriority));
		}
	}
}

void CEventSystem::DeregisterForEvent(IListener& toUnregister, CEvent& toIgnore)
{
	//remove listener from events listener list
	list<pair<IListener*, float>>::iterator listIter = m_listners[&toIgnore].begin();
	for(; listIter != m_listners[&toIgnore].end(); ++listIter)
	{
		if((*listIter).first == &toUnregister)
		{
			m_listners[&toIgnore].erase(listIter);
			return;
		}
	}
}

//action/reaction--------------------------------------------------------------------------

void CEventSystem::DispatchEvent(CEvent& toDispatch, char* dataBuffer, float processingPriority)
{
	tDispatch newDispatch;

	if(processingPriority < 0.0f)
	{
		newDispatch.m_processingPriority = min( m_toProc.rbegin()->m_processingPriority / 2.0f, 1.0f / 2.0f);
	}
	else
	{
		newDispatch.m_processingPriority = processingPriority;
	}

	newDispatch.m_pEvent = &toDispatch;

	//HACK not the best way to do this
	newDispatch.m_buffer = dataBuffer;

	m_toProc.push_back(newDispatch);
	
	m_toProc.sort(isGreaterThan);

}

void CEventSystem::ProcessEvents()
{
	//iterate through proc event list, highest priority to lowest priority
		//for each event, iterate through event listeners, higest priority to lowest priority
			//call listeners react funtion

	list<tDispatch>::iterator listIter = m_toProc.begin();
	//m_toProc
	for(; listIter != m_toProc.end(); ++listIter)
	{
		list<pair<IListener*, float>>::iterator lstnrIter = m_listners[(*listIter).m_pEvent].begin();
		for(; lstnrIter != m_listners[(*listIter).m_pEvent].end(); ++lstnrIter)
		{
			(*lstnrIter).first->React((*listIter));
		}

		delete[] (*listIter).m_buffer;
		//m_toProc.erase(listIter);
	}

	m_toProc.clear();
}