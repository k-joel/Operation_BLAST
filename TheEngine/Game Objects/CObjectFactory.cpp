#include "CObjectFactory.h"
#include "CPlayer.h"
#include "CObjectManager.h"
#include "../LuaEngine/CLuaEngine.h"
#include "../Rendering/CRenderContextManager.h"
#include "../Rendering/CRenderShape.h"

//Defining the singleton methods
CObjectFactory::CObjectFactory()
{
}

CObjectFactory::~CObjectFactory()
{
}

CObjectFactory::CObjectFactory(const CObjectFactory& obj)
{
}

CObjectFactory& CObjectFactory::operator=(const CObjectFactory& obj)
{
	return *this;
}

CObjectFactory* CObjectFactory::GetInstance(void)
{
	static CObjectFactory instance;
	return &instance;
}

void CObjectFactory::ShutdownObjectFactory()
{
	for(map<int, vector<CBase*>>::iterator mapIter = m_mObjects.begin(); mapIter != m_mObjects.end(); mapIter++)
	{
		for(vector<CBase*>::iterator vecIter = (*mapIter).second.begin(); vecIter != (*mapIter).second.end(); vecIter++)
		{
			delete (*vecIter);
		}
		(*mapIter).second.clear();
	}

	m_mObjects.clear();
}