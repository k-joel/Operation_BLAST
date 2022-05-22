#ifndef PhaseTwo_h__
#define PhaseTwo_h__
#include "../CState.h"
#include "../../Game Objects/Bullet/CChargedLaser.h"
#include "../../Game Objects/Bullet/CFizzle.h"
#include "../../Game Objects/CObjectFactory.h"
#include "../../Game Objects/CObjectManager.h"
#include "../../Game Objects/CBase.h"
#include "../../fx/CParticleManager.h"
#include "../../Game Objects/Enemy/CUltimus.h"
#include "../../Game Objects/Enemy/CEnemy.h"
#include "../../Game Objects/CSpawner.h"

// Enter: Turns on the charge particle and sets up a matrix for the bottom laser
void	CLargeLaserState::Enter()
{
	LargeLaserMat = *((CUltimus*)m_pNPC)->GetPart(BOTTOM)->GetShape(0).GetAnimCtrl().getMatrixByID(2) * ((CUltimus*)m_pNPC)->GetPart(BOTTOM)->GetFrame().GetWorldMatrix();


	m_bFirstTime = true;
	m_bChargeOne = true;
	((CUltimus*)m_pNPC)->SetParticles(false,true,true);
	((CUltimus*)m_pNPC)->GetPart(BOTTOM)->GetShape(0).GetAnimCtrl().BlendAnimation("bottomCannonCharge", 0.1f);
	((CEnemy*)m_pNPC)->SetPlayBossCharge1(true);
}

// Update: Updates a timer if the timer hits a certain time it shoots a giant laser and after that it moves to a different state.
int	CLargeLaserState::Update(float fElapsedTime)
{
	static float fTimer = 0.0f;
	fTimer += fElapsedTime;
	static float fSpawnTimer = 0.0f;
	fSpawnTimer += fElapsedTime;

	LargeLaserMat = *((CUltimus*)m_pNPC)->GetPart(BOTTOM)->GetShape(0).GetAnimCtrl().getMatrixByID(2) * ((CUltimus*)m_pNPC)->GetPart(BOTTOM)->GetFrame().GetWorldMatrix();
	if(m_pNPC->GetIsDead())
	{
		((CUltimus*)m_pNPC)->SetParticles(false,true,false);
		((CUltimus*)m_pNPC)->SetParticles(false,false,false);
		return E_GOTODEATH;
	}

	if(fTimer < 3.0f && m_bChargeOne && !((CUltimus*)m_pNPC)->GetFirst())
	{
		if(fSpawnTimer > 0.5f)
		{
			fSpawnTimer = 0.0f;
			CSpawner::GetInstance()->RunScript("BossScript", false, NORMAL_WAVE);
		}
	}

	if(fTimer > 3.0f && m_bChargeOne)
	{
		fTimer = 0.0f;
		m_bChargeOne = false;
		CChargedLaser* pLaserBlast = (CChargedLaser*)(CObjectFactory::GetInstance()->GetObject<CChargedLaser>(OBJ_CLASER));
		CFizzle* pFizzle		   = (CFizzle*)(CObjectFactory::GetInstance()->GetObject<CFizzle>(OBJ_FIZZLE));

		pLaserBlast->SetType(OBJ_CLASER);
		pLaserBlast->SetIsAffectedByGrav(false);
		pLaserBlast->SetVelocity(-500,0,0);
		pLaserBlast->SetPosition(LargeLaserMat._41+100,LargeLaserMat._42,LargeLaserMat._43);
		CObjectManager::GetInstance()->AddObject(pLaserBlast);


		pFizzle->SetType(OBJ_FIZZLE);
		pFizzle->SetIsAffectedByGrav(false);
		pFizzle->SetVelocity(-500,0,0);
		pFizzle->SetPosition(LargeLaserMat._41+100,LargeLaserMat._42,LargeLaserMat._43);
		CObjectManager::GetInstance()->AddObject(pFizzle);


		((CUltimus*)m_pNPC)->GetPart(BOTTOM)->GetShape(0).GetAnimCtrl().BlendAnimation("bottomCannonShoot", 0.1f);
		D3DXMATRIX tempMat = LargeLaserMat;
		tempMat._43 += -10;
		((CUltimus*)m_pNPC)->SetParticles(false,true,false);
		((CUltimus*)m_pNPC)->SetParticles(false,false,true);

		m_bChargeBlast = true;
		((CEnemy*)m_pNPC)->SetPlayBossLaser1(true);
		CCamera::GetInstance()->ShakeCamera(3.0f);
	}
	if(fTimer > 3.0f && m_bChargeBlast)
	{
		fTimer = 0.0f;
		return E_GOTOSLASERATTACK;
	}

	return E_STAYINSTATE;
}

// Exit: Turns off all particles for the bottom laser
void	CLargeLaserState::Exit()
{
	m_bFirstTime = false;
	m_bChargeOne = false;
	m_bChargeBlast = false;
	((CUltimus*)m_pNPC)->GetPart(BOTTOM)->GetShape(0).GetAnimCtrl().BlendAnimation("bottomCannonIdle", 0.1f);
	((CUltimus*)m_pNPC)->SetFirst(false);
	((CUltimus*)m_pNPC)->SetParticles(false,false,false);

}

//////////////////////////////////////////////////////////////////////////

// Enter: Turns on the charge particle and sets up a matrix for the top laser
void	CSmallLaserState::Enter()
{
	m_bFirstTime = true;
	m_bChargeOne = true;
	SmallLaserMat = *((CUltimus*)m_pNPC)->GetPart(TOP)->GetShape(0).GetAnimCtrl().getMatrixByID(4) * ((CUltimus*)m_pNPC)->GetPart(TOP)->GetFrame().GetWorldMatrix();
	((CUltimus*)m_pNPC)->SetParticles(true,true,true);

	((CUltimus*)m_pNPC)->GetPart(TOP)->GetShape(0).GetAnimCtrl().BlendAnimation("topCannonCharge", 0.1f);

	((CEnemy*)m_pNPC)->SetPlayBossCharge2(true);

}

// Update: Updates a timer if the timer hits a certain time it shoots a giant laser and after that it moves to a different state.
int	CSmallLaserState::Update(float fElapsedTime)
{
	static float fTimer = 0.0f;
	fTimer += fElapsedTime;
	SmallLaserMat = *((CUltimus*)m_pNPC)->GetPart(TOP)->GetShape(0).GetAnimCtrl().getMatrixByID(4) * ((CUltimus*)m_pNPC)->GetPart(TOP)->GetFrame().GetWorldMatrix();
	if(m_pNPC->GetIsDead())
	{
		((CUltimus*)m_pNPC)->SetParticles(true,false,false);
		((CUltimus*)m_pNPC)->SetParticles(true,true,false);
		return E_GOTODEATH;
	}

	if(fTimer > 3.0f && m_bChargeOne)
	{
		fTimer = 0.0f;
		m_bChargeOne = false;
		CChargedLaser* test2 = (CChargedLaser*)(CObjectFactory::GetInstance()->GetObject<CChargedLaser>(OBJ_CLASER));

		test2->SetType(OBJ_CLASER);
		test2->SetIsAffectedByGrav(false);
		test2->SetVelocity(-500,-100,0);


		test2->SetPosition(SmallLaserMat._41+100,SmallLaserMat._42+25,SmallLaserMat._43);
		CObjectManager::GetInstance()->AddObject(test2);

		CFizzle* pFizzle		   = (CFizzle*)(CObjectFactory::GetInstance()->GetObject<CFizzle>(OBJ_FIZZLE));
		pFizzle->SetType(OBJ_FIZZLE);
		pFizzle->SetIsAffectedByGrav(false);
		pFizzle->SetVelocity(-500,-100,0);
		pFizzle->SetPosition(SmallLaserMat._41+100,SmallLaserMat._42+25,SmallLaserMat._43);
		CObjectManager::GetInstance()->AddObject(pFizzle);


		D3DXMATRIX tempMat = SmallLaserMat;
		tempMat._43 += -10;
		((CUltimus*)m_pNPC)->SetParticles(true,true,false);
		((CUltimus*)m_pNPC)->SetParticles(true,false,true);

		((CUltimus*)m_pNPC)->GetPart(TOP)->GetShape(0).GetAnimCtrl().BlendAnimation("topCannonShoot", 0.1f);
		m_bChargeBlast = true;
		((CEnemy*)m_pNPC)->SetPlayBossLaser2(true);
		CCamera::GetInstance()->ShakeCamera(3.0f);
	}
	if(fTimer > 3.0f && m_bChargeBlast)
	{
		fTimer = 0.0f;
		return E_GOTOROCKETATTACK;
	}

	return E_STAYINSTATE;
}

// Exit: Turns off all particles for the top laser
void	CSmallLaserState::Exit()
{
	m_bFirstTime = false;
	m_bChargeOne = false;
	m_bChargeBlast = false;
	((CUltimus*)m_pNPC)->GetPart(TOP)->GetShape(0).GetAnimCtrl().BlendAnimation("topCannonIdle", 0.1f);
	((CUltimus*)m_pNPC)->SetParticles(true,false,false);

}



#endif // PhaseTwo_h__

