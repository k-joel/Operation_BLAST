#include "CObjectManager.h"
#include "CBase.h"
#include "CObjectFactory.h"
#include "../Rendering/CRenderContextManager.h"
#include "../States/CLoadState.h"
#include "../States/CGameState.h"
#include "../Mesh/CModelManager.h"
#include "../Collision/CCollisionTests.h"
#include "../Camera/Camera.h"
#include "../Debug System/DebugSystem.h"
#include "../Spacial Tree/CHGrid.h"
#include "../Spacial Tree/CBVH.h"
#include "../FX/CFXManager.h"
#include "../FX/CParticleManager.h"

#include "CExplosion.h"
#include "CGoal.h"

#include <string>
using namespace std;

CObjectManager *CObjectManager::sm_Instance = NULL;

CObjectManager::CObjectManager(void)
{
	CRenderContextManager::GetInstance()->SetRenderBVH(true);
}

CObjectManager::~CObjectManager(void)
{
}

CObjectManager* CObjectManager::GetInstance(void)
{
	if(sm_Instance == 0)
		sm_Instance = new CObjectManager();

	return sm_Instance;
}

void CObjectManager::UpdateObjects(float fElapsedTime)
{
	for(list<CBase*>::iterator iter = m_lObjectList.begin(); iter != m_lObjectList.end(); iter++)
		{
			if((*iter)->GetIsActive())
			{
				(*iter)->Update(fElapsedTime);
			}
		}

		for(list<CBase*>::iterator iter = m_lObjectList.begin(); iter != m_lObjectList.end(); )
		{
			if(!(*iter)->GetIsActive())
			{
				CHGrid::GetInstance()->RemoveObjectFromHGrid(*iter);
				(*iter)->Release();

				iter = m_lObjectList.erase(iter);
			}
			else
				iter++;
		}

	for(list<CBase*>::iterator  iter = m_lEnemyList.begin(); iter != m_lEnemyList.end(); )
	{
		if((*iter)->GetIsActive())
		{
			//(*iter)->Update(fElapsedTime);
			iter++;
		}
		else // its dead, release and remove from list
		{
			D3DXMATRIX mat;
			D3DXMatrixIdentity(&mat);
			D3DXMatrixTranslation(&mat, (*iter)->GetBVAABB().GetAABB().center.x,
				(*iter)->GetBVAABB().GetAABB().center.y, (*iter)->GetBVAABB().GetAABB().center.z);
			CParticleManager::GetInstance()->SpawnParticle("Explosion2", mat);
			//CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_HEALTH_LOSS, (AkGameObjectID)((CEnemy*)(*iter))->GetAudioID());

			if((*iter)->GetType() == OBJ_ASSAULTER)
			{
				CFXManager::GetInstance()->LoadEffect(FX_ASSAULTER_EXPLODE, (*iter)->GetFrame().GetWorldMatrix(), 0.6f,	false);
			}
			if((*iter)->GetType() == OBJ_HELIOS)
			{
				CFXManager::GetInstance()->LoadEffect(FX_HELIOS_EXPLODE, (*iter)->GetFrame().GetWorldMatrix(), 0.7f,	false);
			}
			if((*iter)->GetType() == OBJ_STRIKER)
			{
				CFXManager::GetInstance()->LoadEffect(FX_STRIKER_EXPLODE, (*iter)->GetFrame().GetWorldMatrix(), 0.6f,	false);
			}

			if(((CEnemy*)(*iter))->GetIsGalEnemy())
			{
				if(((CEnemy*)(*iter))->GetPosition().z < 10.0f && ((CEnemy*)(*iter))->GetPosition().z > -10.0f)
				{
					CGameState::GetInstance()->SetShootingGalleryEnemiesAliveNear(CGameState::GetInstance()->GetShootingGalleryEnemiesAliveNear() - 1);
				}
				else
				{
					CGameState::GetInstance()->SetShootingGalleryEnemiesAliveFar(CGameState::GetInstance()->GetShootingGalleryEnemiesAliveFar() - 1);
				}
			}

			(*iter)->Release();
			iter = m_lEnemyList.erase(iter);
		}
	}

	for(list<CBase*>::iterator  iter = m_lPlayerList.begin(); iter != m_lPlayerList.end(); )
	{
		if((*iter)->GetIsActive())
		{
			//(*iter)->Update(fElapsedTime);
			iter++;
		}
		else // its dead, release and remove from list
		{
			(*iter)->Release();
			iter = m_lPlayerList.erase(iter);
		}
	}
}

void CObjectManager::RenderObjects(void)
{
	CBVH::GetInstance()->TraverseDraw(CBVH::GetInstance()->GetRootRenderNode());

	for(list<CBase*>::iterator iter = m_lObjectList.begin(); iter != m_lObjectList.end(); iter++)
		{
			if(CCollisionTests::TestFrustrumSphere2((*iter)->GetBVSphere().GetSphere(), CCamera::GetInstance()->GetFrustum()))
				RenderAnObject((*iter));
		}
}

void CObjectManager::RenderAnObject(CBase* pObj)
{
	pObj->Render(); // to render sprites n shit

	switch (pObj->GetRenderType())
	{
	case RT_OPAQUE:
		pObj->AddToRenderSet();
		break;
	case RT_TRANSPARENT:
		pObj->SetZDepth(pObj->GetBVAABB().GetAABB().minPt.z);
		pObj->AddToRenderSet();
		break;
	case RT_DONOTRENDER:
		break;
	}
}

void CObjectManager::AddObject(CBase* pObject)
{
	//	Check for valid object pointer
	if (pObject == NULL)
		return;

	//	Add object to object list
	m_lObjectList.push_back(pObject);
	//m_mObjectList[pObject->GetType()].push_back(pObject);

	if(pObject->GetType() == OBJ_STRIKER ||pObject->GetType() == OBJ_ASSAULTER || pObject->GetType() == OBJ_HELIOS || pObject->GetType() == OBJ_ULTIMUS)
	{
		m_lEnemyList.push_back((CCharacter*)pObject);
		pObject->AddRef();
	}
	if(pObject->GetType() == OBJ_BARRON ||pObject->GetType() == OBJ_CLAIRE )
	{
		m_lPlayerList.push_back((CCharacter*)pObject);
		pObject->AddRef();
	}

	CHGrid::GetInstance()->RemoveObjectFromHGrid(pObject);
	CHGrid::GetInstance()->AddObjectToHGrid(pObject);

	//	Add my reference to it.
	pObject->AddRef();
}

void CObjectManager::AddTrigger(CBase* pObject)
{
	if (pObject == NULL)
		return;

	//	Add object to object list
	m_vTriggers.push_back(pObject);

	//	Add my reference to it.
	pObject->AddRef();
}

void CObjectManager::AddRenderable(CBase* pRenderObj)
{
	if (pRenderObj == NULL)
		return;

	m_vSceneRenderableList.push_back(pRenderObj);

	pRenderObj->AddRef();
}

void CObjectManager::AddCollidable(CBase* pColObj)
{
	if (pColObj == NULL)
		return;

	m_vSceneCollisionList.push_back(pColObj);

	pColObj->AddRef();
}

void CObjectManager::ResetTriggers(void)
{
	for(unsigned int i = 0; i < m_vTriggers.size(); i++)
	{
		m_vTriggers[i]->SetIsActive(true);
		AddObject(m_vTriggers[i]);
	}
}

void CObjectManager::RemoveObject(CBase* pObject)
{
	//	Check for valid object pointer
	if (pObject == NULL)
		return;

	for (list<CBase*>::iterator iter = m_lObjectList.begin();
		iter != m_lObjectList.end();
		iter++)
	{
		// if this is the object we are looking for.
		if ((*iter) == pObject)
		{
			CHGrid::GetInstance()->RemoveObjectFromHGrid(*iter);
			//Release object
			(*iter)->Release();
			// Remove the object from the list
			iter = m_lObjectList.erase(iter);
			break;
		}
	}
}

void CObjectManager::RemoveAllObjects(void)
{
	//	Call Release() on all objects.

	for(list<CBase*>::iterator iter = m_lObjectList.begin(); iter != m_lObjectList.end(); iter++)
		{
			CHGrid::GetInstance()->RemoveObjectFromHGrid(*iter);
			(*iter)->Release();
		}
	for(list<CBase*>::iterator  iter = m_lEnemyList.begin(); iter != m_lEnemyList.end(); iter++)
	{
		(*iter)->Release();
	}
	for(list<CBase*>::iterator  iter = m_lPlayerList.begin(); iter != m_lPlayerList.end(); iter++)
	{
		(*iter)->Release();
	}
	//	Collapse the vector
	m_lObjectList.clear();
	m_lEnemyList.clear();
	m_lPlayerList.clear();
}

void CObjectManager::RemoveEverything(void)
{
	//	Call Release() on all objects.

	for(list<CBase*>::iterator iter = m_lObjectList.begin(); iter != m_lObjectList.end(); iter++)
		{
			CHGrid::GetInstance()->RemoveObjectFromHGrid(*iter);
			(*iter)->Release();
		}
	for(list<CBase*>::iterator  iter = m_lEnemyList.begin(); iter != m_lEnemyList.end(); iter++)
	{
		(*iter)->Release();
	}
	for(list<CBase*>::iterator  iter = m_lPlayerList.begin(); iter != m_lPlayerList.end(); iter++)
	{
		(*iter)->Release();
	}
	for(vector<CBase*>::iterator iter = m_vSceneRenderableList.begin(); iter != m_vSceneRenderableList.end(); iter++)
	{
		(*iter)->Release();
	}
	for (unsigned int i=0; i < m_vSceneCollisionList.size(); i++)
	{
		m_vSceneCollisionList[i]->Release();
	}
	for(unsigned int i=0; i < m_vTriggers.size(); i++)
	{
		m_vTriggers[i]->Release();
	}
	//	Collapse the vector
	m_lObjectList.clear();
	m_lEnemyList.clear();
	m_lPlayerList.clear();
	m_vSceneRenderableList.clear();
	m_vSceneCollisionList.clear();
	m_vTriggers.clear();
}

void CObjectManager::DeleteInstance(void)
{
	if(sm_Instance != NULL)
		delete sm_Instance;

	sm_Instance = NULL;
}

void CObjectManager::CheckCollisions(void)
{
	for(list<CBase*>::iterator i = m_lObjectList.begin(); i != m_lObjectList.end(); i++)
		{
			/*for(list<CBase*>::iterator j = m_lObjectList.begin(); j != m_lObjectList.end(); j++)
			{
				if((*i)->GetType() != (*j)->GetType())
				{
					if((*i)->CheckCollision(*j))
					{
						(*i)->CollisionReact(*j);
					}
				}
			}*/

			CHGrid::GetInstance()->CheckObjAgainstGrid(*i);

			vector<CBase*> returnedObjs;
			CBVH::GetInstance()->TraverseCollisions(CBVH::GetInstance()->GetRootCollisionNode(), (*i), &returnedObjs);

			bool bDidHitWall = false, bDidHitGround = false;
			if(returnedObjs.size())
			{
				for(unsigned int x=0; x < returnedObjs.size(); x++)
				{
					if((*i)->CheckSceneCollision(returnedObjs[x]))
					{
						(*i)->SceneCollisionReact(returnedObjs[x]);
						if((*i)->GetHitWall()) bDidHitWall = true;
						if((*i)->GetIsOnGround()) bDidHitGround = true;
					}
				}

				(*i)->SetHitWall(bDidHitWall);
				(*i)->SetIsOnGround(bDidHitGround);
			}
			else
			{
				(*i)->SetHitWall(false);
				(*i)->SetIsOnGround(false);
			}
		}
}

void CObjectManager::DestroyAllGameObjects(void)
{
	unsigned int index = 0;

	for(list<CBase*>::iterator iter = m_lObjectList.begin(); iter != m_lObjectList.end(); iter++)
		{
			if((*iter)->GetType() == OBJ_PICKUP)
				(*iter)->SetIsActive(false);

			if((*iter)->GetType() == OBJ_STRIKER || (*iter)->GetType() == OBJ_ASSAULTER || (*iter)->GetType() == OBJ_HELIOS)
				((CEnemy*)(*iter))->SetIsDead(true);
		}
}

CBase* CObjectManager::GetGoalObject(unsigned int goalID, unsigned int targetID)
{
	for(list<CBase*>::iterator iter = m_lObjectList.begin(); iter != m_lObjectList.end(); iter++)
		{
			if((*iter)->GetType() == OBJ_GOAL && ((CGoalObject*)(*iter))->m_goalID == goalID && ((CGoalObject*)(*iter))->m_targetID == targetID)
			{
				return (*iter);
			}
		}

	return NULL;
}