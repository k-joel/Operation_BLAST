//---------------------------------------------------------------
//
// Name: CObjectFactory
//
// Author: TJ "Meh" Farnie
//
// Purpose: Object factory will create new objects for the game.
//			It is a singleton.
//---------------------------------------------------------------
#pragma once

#include "CBase.h"
#include "CPlayer.h"
#include "Bullet/CMachineGunBullet.h"
#include "Enemy/CStriker.h"
#include "CSceneObject.h"

//Event system includes
#include "../Event System/CEvent.h"
#include "../Event System/CEventSystem.h"
#include "../Event System/IListener.h"

#include "../Mesh/CModelManager.h"
#include "../Rendering/CRenderContextManager.h"
#include "../States/CLoadState.h"
#include "../LuaEngine/CLuaEngine.h"
#include "../fx/CFXManager.h"

#include <vector>
#include <list>
using namespace std;
//Definitions for the warehouse arrays
#define MAX_PLAYERS 2
#define MAX_BULLETS 100
#define MAX_STRIKERS 100
#define MAX_SCENE 200

class CObjectFactory
{
private:
	map<int, vector<CBase*>> m_mObjects;
	map<int, int> m_mObjectCount;

	//Singleton functions
	CObjectFactory();
	~CObjectFactory();
	CObjectFactory(const CObjectFactory& obj);
	CObjectFactory& operator=(const CObjectFactory& obj);

public:
	static CObjectFactory* GetInstance(void);

	///////////////////////////////////////////////////////////////////////
	// Function Name: GetObject
	//
	// Author: TJ "OBJECTION!" Farnie
	//
	// In: type - Enum of the object type.  Use CObjectFactory:: to bring
	//			  up enum list
	//
	// Out: None
	//
	// Return: CBase* - Pointer of the object to be uesd
	//
	// Description: Returns a pointer to an object from the factory
	///////////////////////////////////////////////////////////////////////
	template<typename T>
	T* GetObject(int type);

	///////////////////////////////////////////////////////////////////////
	// Function Name: ReturnObject
	//
	// Author: TJ "Return to Sender" Farnie
	//
	// In: item - Pointer of the item returning to the factory
	//			  type - Enum of the object type.  Use CObjectFactory:: to bring
	//				     up enum list
	//
	// Out: None
	//
	// Return: None
	//
	// Description: Returns an item back to the factory, so that it may
	//				be used later
	///////////////////////////////////////////////////////////////////////
	template<typename T>
	void ReturnObject(T* pObj, int type);

	int GetObjectCount(int nType) { return m_mObjectCount[nType]; }
	void ShutdownObjectFactory();
};

#include "CObjectFactory.hpp"