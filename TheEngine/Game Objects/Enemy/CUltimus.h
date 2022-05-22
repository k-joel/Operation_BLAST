#ifndef CUltimus_h__
#define CUltimus_h__


#include "CEnemy.h"
#include "../../AI System/CAIStateMachine.h"
#include "../../Mesh/CModel.h"
#include "../../Event System/IListener.h"
#include "../../fx/CParticleEffect.h"
#include "../../fx/CParticleManager.h"

enum Parts {TOP = 0, BOTTOM = 1, CANNON = 2, CONVEYOR = 3, BODY = 4 };
class CUltimus	: public CEnemy, IListener
{
private:
	float m_fDebrisTimer; //Timer for when to start tossing debris.  Set in the constructor.
	float m_fCrateTimer; //Timer to spawn crates
	float m_fAudioSmashTimer; //Timer for when to play the smash sound
	float m_fAudioPhaseTimer; //Timer for when to play the end phase sound

	CAIStateMachine m_mStateMachine;
	CEvent* m_eRainDebris; //Event to make it start raining debris
	CEvent* m_eStopDebris; //Event to stop raining debris
	bool m_bPhaseOne;
	bool m_bPhaseTwo;
	bool m_bPhaseSan;
	bool m_bBossFightStart;

	bool m_bAtk1;
	bool m_bAtk2;
	bool m_bAtk3;
	bool m_bAtk1Fire;
	bool m_bAtk2Fire;
	bool m_bAtk3Fire;
	bool m_bSpawnEnemies;

	CBase* m_pTopLaser;
	CBase* m_pBottomLaser;
	CBase* m_pCannon;
	CBase* m_pConveyor;
	CBase* m_pBody;

	float m_fHitCounter;

	CParticleEffect* m_pTopCharge;
	CParticleEffect* m_pBottomCharge;
	CParticleEffect* m_pTopBlast;
	CParticleEffect* m_pBottomBlast;

	CParticleEffect* m_pSmoke0;
	CParticleEffect* m_pSmoke1;
	CParticleEffect* m_pSmoke2;
	CParticleEffect* m_pSmoke3;

	CParticleEffect* m_pGiantFUCKINGEXPLOSIONS;

	bool m_bFirst;


public:

	CUltimus();
	~CUltimus();

	bool GetPhaseOne()		const	{return		m_bPhaseOne;	}
	bool GetPhaseTwo()		const	{return		m_bPhaseTwo;	}
	bool GetPhaseSan()		const	{return		m_bPhaseSan;	}
	CParticleEffect* GetParticle()		const	{return		m_pGiantFUCKINGEXPLOSIONS; };
	bool GetFirst()			const	{return		m_bFirst;	}
	void SetFirst(bool bFirst)			{m_bFirst = bFirst;	}
	bool GetBossFighting()	const	{return		m_bBossFightStart;	}
	void SetBossFighting(bool bFighting)		{m_bBossFightStart = bFighting;	}

	CBase* GetPart(int nType) 
	{
		switch(nType)
		{
		case TOP:
			return m_pTopLaser;
			break;
		case BOTTOM:
			return m_pBottomLaser;
			break;
		case CANNON:
			return m_pCannon;
			break;
		case CONVEYOR:
			return m_pConveyor;
			break;
		case BODY:
			return m_pBody;
			break;
		default:
			return NULL;
			break;
		}
	}

	void SetParticles(bool bTop,bool bCharging, bool bOn) 
	{
		if(bTop)
		{
			if(bCharging)
			{
				if(bOn)
				{
					if(!m_pTopCharge)
						m_pTopCharge = CParticleManager::GetInstance()->SpawnParticle("Laser_charge2",(*GetPart(TOP)->GetShape(0).GetAnimCtrl().getMatrixByID(4) * GetPart(TOP)->GetFrame().GetWorldMatrix()));
				}
				else
				{
					if(m_pTopCharge)
					{
						m_pTopCharge->SetIsActive(false);
						m_pTopCharge = NULL;
					}
				}
			}
			else
			{
				if(bOn)
				{
					D3DXMATRIX tempMat = (*GetPart(TOP)->GetShape(0).GetAnimCtrl().getMatrixByID(4) * GetPart(TOP)->GetFrame().GetWorldMatrix());
					tempMat._43 += -10;

					if(!m_pTopBlast)
						m_pTopBlast = CParticleManager::GetInstance()->SpawnParticle("Laser_chargeburst",tempMat);
				}
				else
				{
					if(m_pTopBlast)
					{
						m_pTopBlast->SetIsActive(false);
						m_pTopBlast = NULL;
					}
				}
			}
		}
		else
		{
			if(bCharging)
			{
				if(bOn)
				{
					if(!m_pBottomCharge)
						m_pBottomCharge = CParticleManager::GetInstance()->SpawnParticle("Laser_charge2",(*GetPart(BOTTOM)->GetShape(0).GetAnimCtrl().getMatrixByID(2) * GetPart(BOTTOM)->GetFrame().GetWorldMatrix()));
				}
				else
				{
					if(m_pBottomCharge)
					{
						m_pBottomCharge->SetIsActive(false);
						m_pBottomCharge = NULL;
					}
				}
			}
			else
			{
				if(bOn)
				{
					D3DXMATRIX tempMat = (*GetPart(BOTTOM)->GetShape(0).GetAnimCtrl().getMatrixByID(2) * GetPart(BOTTOM)->GetFrame().GetWorldMatrix());
					tempMat._43 += -10;

					if(!m_pBottomBlast)
						m_pBottomBlast = CParticleManager::GetInstance()->SpawnParticle("Laser_chargeburst",tempMat);
				}
				else
				{
					if(m_pBottomBlast)
					{
						m_pBottomBlast->SetIsActive(false);
						m_pBottomBlast = NULL;
					}
				}
			}
		}

	}
/*
		void SetParticleTopBlast(bool bOn) 
		{
			if(bOn)
			{
				if(!m_pTopBlast)
					m_pTopBlast = CParticleManager::GetInstance()->SpawnParticle("Laser_chargeburst",(*GetPart(TOP)->GetShape(0).GetAnimCtrl().getMatrixByID(4) * GetFrame().GetWorldMatrix()));
			}
			else
			{
				if(m_pTopBlast)
				{
					m_pTopBlast->SetIsActive(false);
					m_pTopBlast = NULL;
				}
			}
	
		}
	
		void SetParticleBottomCharge(bool bOn) 
		{
			if(bOn)
			{
				if(!m_pBottomCharge)
					m_pBottomCharge = CParticleManager::GetInstance()->SpawnParticle("Laser_charge2",(*GetPart(BOTTOM)->GetShape(0).GetAnimCtrl().getMatrixByID(2) * GetFrame().GetWorldMatrix()));
			}
			else
			{
				if(m_pBottomCharge)
				{
					m_pBottomCharge->SetIsActive(false);
					m_pBottomCharge = NULL;
				}
			}
	
		}
		void SetParticleBottomBlast(bool bOn) 
		{
			if(bOn)
			{
				if(!m_pBottomBlast)
					m_pBottomBlast = CParticleManager::GetInstance()->SpawnParticle("Laser_chargeburst",(*GetPart(BOTTOM)->GetShape(0).GetAnimCtrl().getMatrixByID(2) * GetFrame().GetWorldMatrix()));
			}
			else
			{
				if(m_pBottomBlast)
				{
					m_pBottomBlast->SetIsActive(false);
					m_pBottomBlast = NULL;
				}
			}
	
		}*/
	

	float GetAudioSmashTimer()	{return m_fAudioSmashTimer; }
	float GetAudioPhaseTimer()	{return m_fAudioPhaseTimer; }
	void SetAudioSmashTimer(float fTime)	{ m_fAudioSmashTimer = fTime; }
	void SetAudioPhaseTimer(float fTime)	{ m_fAudioPhaseTimer = fTime; }

	void SetPhaseOne(bool bPhaseOne)	{m_bPhaseOne = bPhaseOne;	}
	void SetPhaseTwo(bool bPhaseTwo)	{m_bPhaseTwo = bPhaseTwo;	}
	void SetPhaseSan(bool bPhaseSan)	{m_bPhaseSan = bPhaseSan;	}

	int HealthCheck()
	{
		if(GetPhaseOne())
			return int(GetMaxHealth() * (2/3.0f));
		if(GetPhaseTwo())
			return int(GetMaxHealth() * (1/3.0f));
		//if(GetPhaseSan())
		return 0;
	}

	void Init(D3DXVECTOR3 vPosition,  bool bDummyMode = false, int nHealth = 5);
	void Input();
	void Update(float fElapsedTime);
	void Render();

	bool CheckCollision(CBase *pBase);
	void CollisionReact(CBase *pBase);
	bool CheckSceneCollision(CBase* pBase);

	//Event system stuff
	void Register();
	void React(const tDispatch &toReactTo);
	void Deregister();
	void Shutdown();

};
#endif // CUltimus_h__
