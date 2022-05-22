#include "CTrigger.h"
#include "../Event System/CEventSystem.h"

CTriggerObject::CTriggerObject()
{
	SetType(OBJ_TRIGGER);
	SetRenderType(RT_DONOTRENDER);
	SetIsAffectedByGrav(false);
	m_spawnType = 0;
	m_numSpawn = 0;
}

CTriggerObject::~CTriggerObject()
{
}

void CTriggerObject::Update(float fElapsedTime)
{
}

void CTriggerObject::Render()
{
}

void CTriggerObject::Initialize()
{
	CBase::Initialize();
}

bool CTriggerObject::CheckCollision(CBase* pBase)
{
	return CBase::CheckCollision(pBase);
}

void CTriggerObject::CollisionReact(CBase* pBase)
{
	if(pBase->GetType() == OBJ_BARRON || pBase->GetType() == OBJ_CLAIRE || pBase->GetType() == OBJ_PLAYER)
	{
		char* buffer = "Fact: Nayn-Cat is Awesome";
		CEvent* toDispatch = 0;
		switch(m_spawnType)
		{
		case SPAWN_WAVE:
			{
				switch(m_numSpawn)
				{
				case LIGHT_WAVE:
					toDispatch = CEventSystem::GetInstance()->FindEvent("SpawnLightWave");
					CEventSystem::GetInstance()->DispatchEvent(*toDispatch, NULL, 1.0f);
					break;
				case LIGHT_LOCK:
					toDispatch = CEventSystem::GetInstance()->FindEvent("LightLock");
					CEventSystem::GetInstance()->DispatchEvent(*toDispatch, NULL, 1.0f);
					break;
				case MEDIUM_WAVE:
					toDispatch = CEventSystem::GetInstance()->FindEvent("SpawnMediumWave");
					CEventSystem::GetInstance()->DispatchEvent(*toDispatch, NULL, 1.0f);
					break;
				case MEDIUM_LOCK:
					toDispatch = CEventSystem::GetInstance()->FindEvent("MediumLock");
					CEventSystem::GetInstance()->DispatchEvent(*toDispatch, NULL, 1.0f);
					break;
				case HEAVY_WAVE:
					toDispatch = CEventSystem::GetInstance()->FindEvent("SpawnHeavyWave");
					CEventSystem::GetInstance()->DispatchEvent(*toDispatch, NULL, 1.0f);
					break;
				case HEAVY_LOCK:
					toDispatch = CEventSystem::GetInstance()->FindEvent("HeavyLock");
					CEventSystem::GetInstance()->DispatchEvent(*toDispatch, NULL, 1.0f);
					break;
				}
			}
			break;
		case SPAWN_BOSS:
			toDispatch = CEventSystem::GetInstance()->FindEvent("SpawnBoss");
			CEventSystem::GetInstance()->DispatchEvent(*toDispatch, NULL, 1.0f);
			break;
		default:
			break;
		}

		SetIsActive(false);
	}
}
//*************************************
//Jungle spawner stuff
//*************************************
void CJungleSpawn::Update(float fElapsedTime)
{
	if(m_bActive)
	{
		m_fTimer += fElapsedTime;
	}
}
void CJungleSpawn::Render()
{
}
bool CJungleSpawn::CheckCollision(CBase* pBase)
{
	return false;
}

void CJungleSpawn::CollisionReact(CBase* pBase)
{
}
void CJungleSpawn::Initialize()
{
}

//*************************************
//Beach spawner stuff
//*************************************
void CBeachSpawn::Update(float fElapsedTime)
{
	if(m_bActive)
	{
		m_fTimer += fElapsedTime;
	}
}
void CBeachSpawn::Render()
{
}
bool CBeachSpawn::CheckCollision(CBase* pBase)
{
	return false;
}

void CBeachSpawn::CollisionReact(CBase* pBase)
{
}
void CBeachSpawn::Initialize()
{
}