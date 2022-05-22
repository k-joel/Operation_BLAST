#pragma once
#include <vector>
#include <map>
using namespace std;

#include "../Mesh/CModel.h"
#include "../Mesh/animation.h"

class CLayoutImporter;

class CBase;
class CCharacter;

class CObjectManager
{
	friend class CLayoutImporter;
	friend class CLoadState;
	friend class CBVH;
private:
	//map<int, list<CBase*>> m_mObjectList;
	list<CBase*> m_lObjectList;
	list<CBase*> m_lEnemyList;
	list<CBase*> m_lPlayerList;
	list<CBase*> m_lBulletList;

	vector<CBase*> m_vTriggers;

	vector<CBase*> m_vSceneRenderableList;
	vector<CBase*> m_vSceneCollisionList;

	static CObjectManager* sm_Instance;

	CObjectManager(void);
	CObjectManager(const CObjectManager&);
	CObjectManager& operator=(const CObjectManager&);
	~CObjectManager(void);

public:
	//list<CBase*> & GetObjectByType(unsigned int nObjectType) {return m_mObjectList[nObjectType]; }
	//	GetInstance
	static CObjectManager* GetInstance(void);

	void UpdateObjects(float fElapsedTime);
	void RenderObjects(void);

	void AddObject(CBase* pObject);
	void AddTrigger(CBase* pObject);
	void AddRenderable(CBase* pRenderObj);
	void AddCollidable(CBase* pColObj);

	void RenderAnObject(CBase* pObj);

	void RemoveObject(CBase* pObject);

	void RemoveAllObjects(void);
	void RemoveEverything(void);

	void ResetTriggers(void);

	static void DeleteInstance(void);

	void CheckCollisions(void);

	//
	list<CBase*>& GetEnemyList() {return m_lEnemyList; }
	list<CBase*>& GetPlayerList() {return m_lPlayerList; }
	list<CBase*>& GetBulletList() {return m_lBulletList; }
	CBase* GetGoalObject(unsigned int goalID, unsigned int targetID);
	//
	//Function to kill all the crap on after a merge.
	void DestroyAllGameObjects(void);
};