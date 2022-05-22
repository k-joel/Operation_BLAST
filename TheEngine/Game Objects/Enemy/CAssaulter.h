#ifndef CAssaulter_h__
#define CAssaulter_h__


#include "CEnemy.h"
#include "../../AI System/CAIStateMachine.h"
#include "../../Mesh/CModel.h"
#include "../../fx/CParticleEffect.h"
#include "../../fx/CParticleManager.h"

class CAssaulter	: public CEnemy
{
private:

	CAIStateMachine m_mStateMachine;
	bool m_bCounted; //This bool makes sure that this enemy is not updated for score multiple times.
	CParticleEffect* pChargeParticle;
	D3DXMATRIX Blaster;
public:

	CAssaulter();
	~CAssaulter();

	void Init(D3DXVECTOR3 vPosition,  bool bDummyMode = false, int nHealth = 5);
	void Input();
	void Update(float fElapsedTime);
	void Render();

	
	void StartCharging(bool bCharge) 
	{
		if(bCharge)
		{

			if(!pChargeParticle)
			{
				pChargeParticle = CParticleManager::GetInstance()->SpawnParticle("Laser_charge",(*GetShape(0).GetAnimCtrl().getMatrixByID(1) * GetFrame().GetWorldMatrix()));
				//pChargeParticle->SetWorld(Blaster);
			}
			//CParticleManager::GetInstance()->SpawnParticle("laser_charge",(*GetShape(0).GetAnimCtrl().getMatrixByID(1) * GetFrame().GetWorldMatrix()));
		}
		else
		{
			if(pChargeParticle)
				pChargeParticle->SetIsActive(false);
		}

	}
	bool CheckCollision(CBase *pBase);
	void CollisionReact(CBase *pBase);
	bool CheckAngles();

	void Shutdown(void);
};
#endif // CAssaulter_h__
