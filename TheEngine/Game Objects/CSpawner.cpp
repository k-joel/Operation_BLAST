#include "CSpawner.h"
#include "../Camera/Camera.h"
#include "CObjectFactory.h"
#include "CObjectManager.h"

//Enemy type includes
#include "Enemy/CStriker.h"
#include "Enemy/CAssaulter.h"
#include "Enemy/CHelios.h"
#include "CCrate.h"

#include "../Core/CGame.h"
#include "../States/CGameState.h"

#include "../FX/CFXManager.h"

#define TRICKLE_THRESHOLD 5.0f

CSpawner::CSpawner()
{
	luaState = lua_open();
	luaL_openlibs(luaState);

	luaL_dofile(luaState, "Resources/Lua/Spawning/WaveScripts.lua");
	lua_register(luaState, "CreateEnemy", CSpawner::CreateEnemy);

	m_nNumberCreated = 0;
}

CSpawner::~CSpawner()
{
	lua_close(luaState);
}

void CSpawner::Refresh(void)
{
	luaL_dofile(luaState, "Resources/Lua/Spawning/WaveScripts.lua");
}

CSpawner* CSpawner::GetInstance(void)
{
	static CSpawner instance;
	return &instance;
}

void CSpawner::RunScript(char* scriptName, bool backPlane, int waveType)
{
	m_nNumberCreated = 0;
	m_nWaveType = waveType;
	lua_getglobal(luaState, "Script");

	lua_pushstring(luaState, scriptName);
	lua_pushnumber(luaState, CCamera::GetInstance()->GetViewPosition().x);

	if(!backPlane)
		lua_pushnumber(luaState, 0.0f);
	else
		lua_pushnumber(luaState, 60.0f);

	lua_pcall(luaState, 3, 0, -1);
}

int CSpawner::CreateEnemy(lua_State* state)
{
	//When this function is called, the numbers come in in this order:
	//Enemy type
	//X Position
	//Y Position
	//Z Position
	//Speed
	//Spawn Method

	int numVars = lua_gettop(state);

	int spawnMethod = (int)lua_tonumber(state, 6);

	switch((int)lua_tonumber(state, 1))
	{
	case OBJ_STRIKER:
		{
			CStriker* enemy = (CStriker*)CObjectFactory::GetInstance()->GetObject<CStriker>(OBJ_STRIKER);
			enemy->SetSpawnMethod(spawnMethod);
			if(spawnMethod != BOSS_METHOD)
				enemy->Init(D3DXVECTOR3((float)lua_tonumber(state, 2), (float)lua_tonumber(state, 3), (float)lua_tonumber(state, 4)),false,1);
			else
				enemy->Init(D3DXVECTOR3((float)lua_tonumber(state, 2), (float)lua_tonumber(state, 3), -45.0f),false,2);

			if(spawnMethod == DROP_DOWN_METHOD || spawnMethod == JUMP_IN_METHOD || spawnMethod == TREE_METHOD)
			{
				D3DXMATRIX trans;
				D3DXMatrixTranslation(&trans, (float)lua_tonumber(state, 2), (float)lua_tonumber(state, 3), (float)lua_tonumber(state, 4));
				CFXManager::GetInstance()->LoadEffect(FX_DROP_TARGET, trans, 1.25f, false);
			}

			//enemy->SetPosition(D3DXVECTOR3());
			enemy->SetRunVelocity(D3DXVECTOR3((float)lua_tonumber(state, 5), 0.0f, 0.0f));
			enemy->SetType(OBJ_STRIKER);
			enemy->SetIsActive(true);
			enemy->SetIsThunderStruck(false);
			if(CSpawner::GetInstance()->GetWaveType() == LOCK_WAVE)
				enemy->SetIsLockEnemy(true);
			else if(CSpawner::GetInstance()->GetWaveType() == GAL_WAVE)
				enemy->SetIsGalEnemy(true);
			enemy->Update(0.0f);
			CObjectManager::GetInstance()->AddObject(enemy);
			break;
		}
	case OBJ_ASSAULTER:
		{
			CAssaulter* enemy = (CAssaulter*)CObjectFactory::GetInstance()->GetObject<CAssaulter>(OBJ_ASSAULTER);
			enemy->SetSpawnMethod(spawnMethod);
			if(spawnMethod != BOSS_METHOD)
				enemy->Init(D3DXVECTOR3((float)lua_tonumber(state, 2), (float)lua_tonumber(state, 3), (float)lua_tonumber(state, 4)),false,3);
			else
			{
				enemy->Init(D3DXVECTOR3((float)lua_tonumber(state, 2), (float)lua_tonumber(state, 3), -60.0f),false,4);
			}

			
			if(spawnMethod == DROP_DOWN_METHOD || spawnMethod == JUMP_IN_METHOD || spawnMethod == TREE_METHOD || spawnMethod == JETPACK_METHOD)
			{
				D3DXMATRIX trans;
				D3DXMatrixTranslation(&trans, (float)lua_tonumber(state, 2), (float)lua_tonumber(state, 3), (float)lua_tonumber(state, 4));
				CFXManager::GetInstance()->LoadEffect(FX_DROP_TARGET, trans, 1.25f, false);
			}

			enemy->SetRunVelocity(D3DXVECTOR3((float)lua_tonumber(state, 5), 0.0f, 0.0f));
			enemy->SetType(OBJ_ASSAULTER);
			enemy->SetIsActive(true);
			enemy->SetIsThunderStruck(false);
			if(CSpawner::GetInstance()->GetWaveType() == LOCK_WAVE)
				enemy->SetIsLockEnemy(true);
			else if(CSpawner::GetInstance()->GetWaveType() == GAL_WAVE)
				enemy->SetIsGalEnemy(true);
			enemy->Update(0.0f);
			CObjectManager::GetInstance()->AddObject(enemy);
			break;
		}
	case OBJ_HELIOS:
		{
			CHelios* enemy = (CHelios*)CObjectFactory::GetInstance()->GetObject<CHelios>(OBJ_HELIOS);
			enemy->SetSpawnMethod(spawnMethod);
			if(spawnMethod != BOSS_METHOD)
				enemy->Init(D3DXVECTOR3((float)lua_tonumber(state, 2), (float)lua_tonumber(state, 3), (float)lua_tonumber(state, 4)),false,1);
			else
				enemy->Init(D3DXVECTOR3((float)lua_tonumber(state, 2), (float)lua_tonumber(state, 3), -60.0f),false,4);
			enemy->SetRunVelocity(D3DXVECTOR3((float)lua_tonumber(state, 5), 0.0f, 0.0f));
			enemy->SetType(OBJ_HELIOS);
			enemy->SetIsActive(true);
			enemy->SetIsThunderStruck(false);
			if(CSpawner::GetInstance()->GetWaveType() == LOCK_WAVE)
				enemy->SetIsLockEnemy(true);
			else if(CSpawner::GetInstance()->GetWaveType() == GAL_WAVE)
				enemy->SetIsGalEnemy(true);
			enemy->Update(0.0f);
			CObjectManager::GetInstance()->AddObject(enemy);
			break;
		}
	case OBJ_CRATE:
		{
			CCrate* crate = (CCrate*)(CObjectFactory::GetInstance()->GetObject<CCrate>(OBJ_CRATE));

			crate->SetPosition(D3DXVECTOR3((float)lua_tonumber(state, 2), (float)lua_tonumber(state, 3), (float)lua_tonumber(state, 4)));
			crate->SetIsActive(true);
			crate->SetIsAffectedByGrav((bool)lua_toboolean(state, 5));

			crate->Update(0.0f);
			crate->Update(0.0f);
			CObjectManager::GetInstance()->AddObject(crate);
			break;
		}
	default:
		{
			char szBuffer[256] = {0};
			int caseNum = (int)lua_tonumber(state, 1);
			sprintf(szBuffer, "Spawner Undefined Case; #%u", caseNum); 
			MessageBox(0, szBuffer, "Spawner Error", MB_OK);
			break;
		}
	}

	if((int)lua_tonumber(state, 1) != OBJ_CRATE)
	{
		CSpawner::GetInstance()->IncreaseCreated();
	}
	lua_pop(state, numVars);
	numVars = lua_gettop(state);

	
	return 1;
}

void CSpawner::Update(float fDT)
{
	if(m_bTrickle)
	{
		m_fTrickleTimer += fDT;

		if(m_fTrickleTimer >= TRICKLE_THRESHOLD)
		{
			if(!CGame::GetInstance()->GetMutliplayer())
			{
				if(CGameState::GetInstance()->GetPlayState() == SPLIT)
				{
					if(CGameState::GetInstance()->GetPlayerOne()->GetPosition().z < 30.0f)
					{
						RunScript("LightFlow", false, TRICKLE);
					}
					else
					{
						RunScript("LightFlow", true, TRICKLE);
					}
				}
				else
				{
					RunScript("LightFlow", false, TRICKLE);
				}
			}
			else
			{
				if(CGameState::GetInstance()->GetPlayState() == SPLIT)
				{
					RunScript("LightFlow", true, TRICKLE);
					RunScript("LightFlow", false, TRICKLE);
				}
				else
				{
					RunScript("LightFlow", false, TRICKLE);
				}
			}

			m_fTrickleTimer = 0.0f;
		}
	}
	else if(m_bForestDrops)
	{
		m_fTreeTimer += fDT;

		if(m_fTreeTimer >= m_fTreeLimit)
		{
			m_fTreeTimer = 0.0f;
			
			char buffer[16];
			sprintf(buffer, "TreeScript%i", m_nTreeCount + 1); 

			RunScript(buffer, false, LOCK_WAVE);

			++m_nTreeCount;

			if(m_nTreeCount == m_nTreeLimit)
			{
				m_nTreeCount = 0;
				m_fTreeTimer = 0.0f;
				m_bForestDrops = false;
				m_bTrickle = true;
			}
		}
	}
}

void CSpawner::TurnOnForestEnemies(float timeBetweenSpawns, int numOfEnemies)
{
	m_bForestDrops = true;
	m_fTreeLimit = timeBetweenSpawns;
	m_nTreeLimit = numOfEnemies;
	m_fTreeTimer = 0.0f;
	m_bTrickle = false;
}