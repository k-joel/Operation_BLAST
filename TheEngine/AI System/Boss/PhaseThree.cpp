#ifndef PhaseThree_h__
#define PhaseThree_h__
#include "../CState.h"
#include "../../Game Objects/Bullet/CRocketBullet.h"
#include "../../Game Objects/CObjectFactory.h"
#include "../../Game Objects/CObjectManager.h"
#include "../../fx/Helpers.h"
using namespace Helpers;
#include "../../Camera/Camera.h"

#include "../../Collision/CollisionObject.h"
#include "../../Collision/CCollisionTests.h"
#include "../../Spacial Tree/CBVH.h"
#include "../../Game Objects/Scene/CFlag.h"
#include "../../Debug System/DebugSystem.h"
#include "../../fx/CParticleManager.h"
#include "../../Game Objects/CSpawner.h"
#include "../../fx/CFXManager.h"

// Enter: TUrns on the ability to start shooting rockets
void	CRocketState::Enter()
{
	m_bFirstTime = true;
	m_bShootingBlanks = true;
	D3DXMatrixIdentity(&CannonLaserMat);
	CannonLaserMat	= *(((CUltimus*)m_pNPC)->GetPart(CANNON)->GetShape(0).GetAnimCtrl().getMatrixByID(1)) * m_pNPC->GetFrame().GetWorldMatrix();

	CannonLaserPos.x = CannonLaserMat._41;
	CannonLaserPos.y = CannonLaserMat._42;
	CannonLaserPos.z = CannonLaserMat._43;

}


// Update: Updates a timer if the timer hits a certain time it shoots multiple rockets out of the cannon
int	CRocketState::Update(float fElapsedTime)
{
	static float fTimer = 0.0f;
	static float fBlanksTimer = 0.0f;
	static float fRealsTimer = 0.0f;
	static float fSpawnTimer = 0.0f;
	fTimer += fElapsedTime;
	fBlanksTimer += fElapsedTime;
	fRealsTimer += fElapsedTime;
	fSpawnTimer += fElapsedTime;

	CannonLaserMat	= *(((CUltimus*)m_pNPC)->GetPart(CANNON)->GetShape(0).GetAnimCtrl().getMatrixByID(1)) * m_pNPC->GetFrame().GetWorldMatrix();

	if(m_pNPC->GetIsDead())
		return E_GOTODEATH;


	if(fTimer < 3.0f && m_bShootingBlanks && fBlanksTimer > 0.35f)
	{
		fBlanksTimer = 0.0f;
		((CUltimus*)m_pNPC)->SetPlayBossCannon(true);
		CRocketAlternate* bullet = (CRocketAlternate*)(CObjectFactory::GetInstance()->GetObject<CRocketAlternate>(OBJ_ROCKET_ALTERNATE));
		D3DXVECTOR3 tempPos = D3DXVECTOR3(CannonLaserMat._41,CannonLaserMat._42,CannonLaserMat._43);
		tempPos.x -= 25.0f;
		tempPos.y += 55.0f;
		bullet->SetVelocity(-50,250,0);
		bullet->SetPosition(tempPos);
		bullet->SetIsActive(true);	
		bullet->SetOwner(m_pNPC);
		bullet->SetType(OBJ_ROCKET_ALTERNATE);
		CObjectManager::GetInstance()->AddObject(bullet);
		((CUltimus*)m_pNPC)->GetPart(CANNON)->GetShape(0).GetAnimCtrl().BlendAnimation("largeCannonShoot", 0.1f);
		m_bShootingReals = true;
		if(fSpawnTimer > 0.45f)
		{
			fSpawnTimer = 0.0f;
			CSpawner::GetInstance()->RunScript("BossScript", false, NORMAL_WAVE);
		}
	}
	if(fTimer > 3.0f && m_bShootingReals && fRealsTimer > 0.35f)
	{
		m_bShootingBlanks = false;
		fRealsTimer = 0.0f;
		CRocketAlternate* realbullet = (CRocketAlternate*)(CObjectFactory::GetInstance()->GetObject<CRocketAlternate>(OBJ_ROCKET_ALTERNATE));
		D3DXVECTOR3 realPos = D3DXVECTOR3(GetRandomFloat(CCamera::GetInstance()->GetViewPosition().x - 150,CCamera::GetInstance()->GetViewPosition().x + 100),400,0);
		realbullet->SetVelocity(0,-350,0);
		realbullet->SetPosition(realPos);
		realbullet->SetIsActive(true);	
		realbullet->SetOwner(m_pNPC);
		realbullet->SetType(OBJ_ROCKET_ALTERNATE);

		D3DXMATRIX trans;
		D3DXMatrixTranslation(&trans, realbullet->GetPosition().x, realbullet->GetPosition().y, realbullet->GetPosition().z);
		CFXManager::GetInstance()->LoadEffect(FX_DROP_TARGET, trans, 1.25f, false);

		CObjectManager::GetInstance()->AddObject(realbullet);
		((CUltimus*)m_pNPC)->GetPart(CANNON)->GetShape(0).GetAnimCtrl().SetAnimation("largeCannonIdle");
	}


	if(fTimer > 6.0f)
	{
		fTimer = 0.0f;
		return E_GOTOSPAWN;
	}

	return E_STAYINSTATE;
}

// Exit: Switches the animations for the cannon to idle
void	CRocketState::Exit()
{
	m_bFirstTime = false;
	((CUltimus*)m_pNPC)->GetPart(CANNON)->GetShape(0).GetAnimCtrl().BlendAnimation("largeCannonIdle", 0.1f);

}

#endif // PhaseThree_h__

