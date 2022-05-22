#include "CLightningBullet.h"
#include "../Enemy/CEnemy.h"
#include "../../Rendering/CRenderContext.h"
#include "../CObjectManager.h"
#include "../CObjectFactory.h"
#include "../../Camera/Camera.h"
#include "../../fx/CParticleManager.h"

class CEnemy;

const AkGameObjectID Ak_LIGHTNING_B = 120;

//--------------------------------------------------------------------
//
//Normal
//
//--------------------------------------------------------------------

CLightningBullet::CLightningBullet()
{
}

void CLightningBullet::Initialize()
{
	SetIsAffectedByGrav(false);
	SetIsOnGround(false);
	SetType(OBJ_LIGHTNING);
	m_fDespawnTimer = 0.0f;
	m_fShockTimer = 0.0f;
	m_pPreviousEnemy = NULL;
	m_pCurrentEnemy = NULL;
	m_pNextEnemy = NULL;
	m_nMaxJumps = 4;
	m_nJumpCount = 0;
	m_bHit = false;
	m_fArcDistance = 4 * 32;
	CWwiseManager::GetInstance()->RegisterObject(Ak_LIGHTNING_B, "Lightning Bullet");
}

CLightningBullet::~CLightningBullet()
{
}

void CLightningBullet::Update(float fDt)
{
	if(!m_bHit)
	{
		m_fDespawnTimer += fDt;
		SetRenderType(RT_OPAQUE);
		CBase::Update(fDt);

		//correct direction and forward
		D3DXVECTOR3 nDir, curX, newY;
		D3DXMATRIX newOrien = GetFrame().GetLocalMatrix();
		curX = D3DXVECTOR3(newOrien._11, newOrien._12, newOrien._13);
		nDir = GetVelocity();
		D3DXVec3Normalize(&nDir, &nDir);

		D3DXVec3Cross(&newY, &nDir, &curX);
		D3DXVec3Normalize(&newY, &newY);

		newOrien._11 = nDir.x;
		newOrien._12 = nDir.y;
		newOrien._13 = nDir.z;

		newOrien._21 = newY.x;
		newOrien._22 = newY.y;
		newOrien._23 = newY.z;

		newOrien._31 = curX.x;
		newOrien._32 = curX.y;
		newOrien._33 = curX.z;

		GetFrame().GetLocalMatrix() = newOrien;
	}
	else
	{
		m_fShockTimer += fDt;
		SetPosition(m_pCurrentEnemy->GetBVAABB().GetAABB().center);
		SetRenderType(RT_DONOTRENDER);
		if(m_fShockTimer > 0.05f)
		{
			//First, getting the next target
			std::list<CBase*> enemies = CObjectManager::GetInstance()->GetEnemyList();
			std::list<CBase*>::iterator enIter = enemies.begin();
			for(; ;)
			{
				if(enIter == enemies.end())
				{
					if(GetOwner()->GetType() == OBJ_BARRON)
					{
						//CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::STOP_FX_LIGHTNINGGUN, 120);
					}
					else if(GetOwner()->GetType() == OBJ_CLAIRE)
					{
						//CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::STOP_FX_LIGHTNINGGUN, 120);
					}
					SetIsActive(false);
					((CEnemy*)(m_pCurrentEnemy))->SetIsThunderStruck(false);
					if(m_pPreviousEnemy && !((CEnemy*)(m_pPreviousEnemy))->GetIsDead())
						((CEnemy*)(m_pPreviousEnemy))->SetIsThunderStruck(false);

					return;
				}

				//Making sure I'm not checking against myself
				if((*enIter) != m_pCurrentEnemy)
				{
					D3DXVECTOR3 distanceVec = (*enIter)->GetBVAABB().GetAABB().center - GetPosition();
					float distance = D3DXVec3Length(&distanceVec);

					if(distance == 0.0f)
					{
						++enIter;
						continue;
					}

					if(distance > m_fArcDistance)
					{
						++enIter;
						continue;
					}
					if(((CEnemy*)(*enIter))->GetIsThunderStruck())
					{
						++enIter;
						continue;
					}

					m_pNextEnemy = (*enIter);
					break;
				}

				++enIter;
			}

			//Now that I have a new target, head that way!
			float speed = D3DXVec3Length(&GetVelocity());
			D3DXVECTOR3 directionVec = m_pNextEnemy->GetBVAABB().GetAABB().center - GetPosition();
			D3DXVec3Normalize(&directionVec, &directionVec);
			SetVelocity(directionVec * speed);
			//Handle rotation here

			if(m_pPreviousEnemy)
				((CEnemy*)m_pPreviousEnemy)->SetIsThunderStruck(false);

			SetRenderType(RT_OPAQUE);
			++m_nJumpCount;
			m_bHit = false;
			m_fShockTimer = 0.0f;
		}
	}

	if(m_nJumpCount >= m_nMaxJumps)
	{
		SetIsActive(false);
		if(GetOwner()->GetType() == OBJ_BARRON)
		{
			//CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::STOP_FX_LIGHTNINGGUN, 120);
		}
		else if(GetOwner()->GetType() == OBJ_CLAIRE)
		{
			//CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::STOP_FX_LIGHTNINGGUN, 120);
		}
		m_fDespawnTimer = 0;
		m_nJumpCount = 0;
	}

	if(m_fDespawnTimer > m_fMaxLife)
	{
		SetIsActive(false);
		if(GetOwner()->GetType() == OBJ_BARRON)
		{
			//CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::STOP_FX_LIGHTNINGGUN, 120);
		}
		else if(GetOwner()->GetType() == OBJ_CLAIRE)
		{
			//CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::STOP_FX_LIGHTNINGGUN, 120);
		}
		if(m_pCurrentEnemy)
			((CEnemy*)(m_pCurrentEnemy))->SetIsThunderStruck(false);

		if(m_pPreviousEnemy)
			((CEnemy*)(m_pPreviousEnemy))->SetIsThunderStruck(false);
	}

	if(GetIsOnGround())
	{
		SetIsActive(false);
		if(GetOwner()->GetType() == OBJ_BARRON)
		{
			//CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::STOP_FX_LIGHTNINGGUN, 120);
		}
		else if(GetOwner()->GetType() == OBJ_CLAIRE)
		{
			//CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::STOP_FX_LIGHTNINGGUN, 120);
		}
		m_fDespawnTimer = 0.0f;

		if(m_pCurrentEnemy)
			((CEnemy*)(m_pCurrentEnemy))->SetIsThunderStruck(false);

		if(m_pPreviousEnemy)
			((CEnemy*)(m_pPreviousEnemy))->SetIsThunderStruck(false);
	}
}

void CLightningBullet::Render()
{
	CBase::Render();
}

bool CLightningBullet::CheckCollision(CBase* pBase)
{
	//return CBase::CheckCollision(pBase);
	return false;
}

void CLightningBullet::CollisionReact(CBase* pBase)
{
}

//--------------------------------------------------------------------
//
//Alternate
//
//--------------------------------------------------------------------

CLightningAlternate::CLightningAlternate()
{
	SetType(OBJ_LIGHTNING_ALTERNATE);
}

CLightningAlternate::~CLightningAlternate()
{
}

void CLightningAlternate::Initialize()
{
	SetType(OBJ_LIGHTNING_ALTERNATE);
	SetDamage(0.0f);
	SetIsAffectedByGrav(false);
	m_fDespawnTimer = 0.0f;
	m_fMaxLife = 0.0f;
	SetIsOnGround(false);
	m_pPreviousEnemy = NULL;
	m_pCurrentEnemy = NULL;
}

void CLightningAlternate::Update(float fDt)
{
	if(!m_bSplit)
	{
		CBase::Update(fDt);
		m_fDespawnTimer += fDt;

		//correct direction and forward
		D3DXVECTOR3 nDir, curX, newY;
		D3DXMATRIX newOrien = GetFrame().GetLocalMatrix();
		curX = D3DXVECTOR3(newOrien._11, newOrien._12, newOrien._13);
		nDir = GetVelocity();
		D3DXVec3Normalize(&nDir, &nDir);

		D3DXVec3Cross(&newY, &nDir, &curX);
		D3DXVec3Normalize(&newY, &newY);

		newOrien._11 = nDir.x;
		newOrien._12 = nDir.y;
		newOrien._13 = nDir.z;

		newOrien._21 = newY.x;
		newOrien._22 = newY.y;
		newOrien._23 = newY.z;

		newOrien._31 = curX.x;
		newOrien._32 = curX.y;
		newOrien._33 = curX.z;

		GetFrame().GetLocalMatrix() = newOrien;
	}
	else
	{
		switch(m_nSplitState)
		{
		case BASE_BOLT:
			{
				//Creating new mini-bolts in the diagonal directions
				int x = 0;

				CLightningAlternate* bullets[4];

				for( ; x < 4; ++x)
				{
					bullets[x] = (CLightningAlternate*)CObjectFactory::GetInstance()->GetObject<CLightningAlternate>(OBJ_LIGHTNING_ALTERNATE);
					bullets[x]->SetCurrentEnemy(m_pCurrentEnemy);
					bullets[x]->SetDamage(m_fDamage * 0.5f);
					bullets[x]->SetSplitState(FIRST_SPLIT_BOLT);
					bullets[x]->SetOwner(m_pOwner);
					bullets[x]->SetMaxLife(m_fMaxLife);
					bullets[x]->SetIsSplitting(false);
					bullets[x]->SetIsActive(true);
					bullets[x]->SetIsAffectedByGrav(false);
					bullets[x]->SetPreviousEnemy(NULL);
					bullets[x]->SetPosition(m_pCurrentEnemy->GetBVAABB().GetAABB().center);
				}

				float speed = D3DXVec3Length(&GetVelocity());

				//Up right
				bullets[0]->SetRotationY(D3DXToRadian(270.0f));
				bullets[0]->SetRotationZ(D3DXToRadian(45.0f));
				bullets[0]->SetVelocity(cos(bullets[0]->GetRotationZ()) * speed, sin(bullets[0]->GetRotationZ()) * speed, 0.0f);

				//Up left
				bullets[1]->SetRotationY(D3DXToRadian(90.0f));
				bullets[1]->SetRotationZ(D3DXToRadian(135.0f));
				bullets[1]->SetVelocity(cos(bullets[1]->GetRotationZ()) * speed, sin(bullets[1]->GetRotationZ()) * speed, 0.0f);

				//Down right
				bullets[2]->SetRotationY(D3DXToRadian(270.0f));
				bullets[2]->SetRotationZ(D3DXToRadian(315.0f));
				bullets[2]->SetVelocity(cos(bullets[2]->GetRotationZ()) * speed, sin(bullets[2]->GetRotationZ()) * speed, 0.0f);

				//Down left
				bullets[3]->SetRotationY(D3DXToRadian(90.0f));
				bullets[3]->SetRotationZ(D3DXToRadian(225.0f));
				bullets[3]->SetVelocity(cos(bullets[3]->GetRotationZ()) * speed, sin(bullets[3]->GetRotationZ()) * speed, 0.0f);

				for(x = 0; x < 4; ++x)
				{
					CObjectManager::GetInstance()->AddObject(bullets[x]);
				}

				break;
			}
		case FIRST_SPLIT_BOLT:
			{
				//Creating new mini-bolts in the non-diagonal directions
				int x = 0;

				CLightningAlternate* bullets[4];

				for( ; x < 4; ++x)
				{
					bullets[x] = (CLightningAlternate*)CObjectFactory::GetInstance()->GetObject<CLightningAlternate>(OBJ_LIGHTNING_ALTERNATE);
					bullets[x]->SetCurrentEnemy(m_pCurrentEnemy);
					bullets[x]->SetDamage(m_fDamage * 0.5f);
					bullets[x]->SetSplitState(FINAL_SPLIT_BOLT);
					bullets[x]->SetOwner(m_pOwner);
					bullets[x]->SetMaxLife(m_fMaxLife);
					bullets[x]->SetIsSplitting(false);
					bullets[x]->SetIsActive(true);
					bullets[x]->SetIsAffectedByGrav(false);
					bullets[x]->SetPreviousEnemy(NULL);
				}

				float speed = D3DXVec3Length(&GetVelocity());

				//Up
				bullets[0]->SetRotationY(D3DXToRadian(270.0f));
				bullets[0]->SetRotationZ(D3DXToRadian(90.0f));
				bullets[0]->SetVelocity(cos(bullets[0]->GetRotationZ()) * speed, sin(bullets[0]->GetRotationZ()) * speed, 0.0f);

				//Down
				bullets[1]->SetRotationY(D3DXToRadian(90.0f));
				bullets[1]->SetRotationZ(D3DXToRadian(270.0f));
				bullets[1]->SetVelocity(cos(bullets[1]->GetRotationZ()) * speed, sin(bullets[1]->GetRotationZ()) * speed, 0.0f);

				//Left
				bullets[2]->SetRotationY(D3DXToRadian(90.0f));
				bullets[2]->SetRotationZ(D3DXToRadian(180.0f));
				bullets[2]->SetVelocity(cos(bullets[2]->GetRotationZ()) * speed, sin(bullets[2]->GetRotationZ()) * speed, 0.0f);

				//Right
				bullets[3]->SetRotationY(D3DXToRadian(270.0f));
				bullets[3]->SetRotationZ(D3DXToRadian(0.0f));
				bullets[3]->SetVelocity(cos(bullets[3]->GetRotationZ()) * speed, sin(bullets[3]->GetRotationZ()) * speed, 0.0f);

				for(x = 0; x < 4; ++x)
				{
					CObjectManager::GetInstance()->AddObject(bullets[x]);
				}

				break;
			}
		case FINAL_SPLIT_BOLT:
			{
				SetIsActive(false);
				return;
			}
		}

		SetIsActive(false);
	}

	if(m_fDespawnTimer > m_fMaxLife)
	{
		SetIsActive(false);
		m_fDespawnTimer = 0.0f;
	}

	if(GetIsOnGround())
	{
		SetIsActive(false);
		m_fDespawnTimer = 0.0f;
	}
}

void CLightningAlternate::Render()
{
	CBase::Render();
}

bool CLightningAlternate::CheckCollision(CBase* pBase)
{
	return CBase::CheckCollision(pBase);
}

void CLightningAlternate::CollisionReact(CBase* pBase)
{
}

//--------------------------------------------------------------------
//
//Ultimate
//
//--------------------------------------------------------------------

CLightningUltimate::CLightningUltimate()
{
	SetType(OBJ_FLAME_ALTERNATE);
	SetDamage(10.0f);
	SetIsAffectedByGrav(false);
}

CLightningUltimate::~CLightningUltimate()
{
}

void CLightningUltimate::Update(float fDt)
{
	CBase::Update(fDt);
	//correct direction and forward
	D3DXVECTOR3 nDir, curX, newY;
	D3DXMATRIX newOrien = GetFrame().GetLocalMatrix();
	curX = D3DXVECTOR3(newOrien._11, newOrien._12, newOrien._13);
	nDir = GetVelocity();
	D3DXVec3Normalize(&nDir, &nDir);

	D3DXVec3Cross(&newY, &nDir, &curX);
	D3DXVec3Normalize(&newY, &newY);

	newOrien._11 = nDir.x;
	newOrien._12 = nDir.y;
	newOrien._13 = nDir.z;

	newOrien._21 = newY.x;
	newOrien._22 = newY.y;
	newOrien._23 = newY.z;

	newOrien._31 = curX.x;
	newOrien._32 = curX.y;
	newOrien._33 = curX.z;

	GetFrame().GetLocalMatrix() = newOrien;

	if(CCollisionTests::TestSpherePlane(m_Sphere.GetSphere(), CCamera::GetInstance()->GetTopFrustumPlane()) == CT_INTERSECT)
	{
		CParticleManager::GetInstance()->SpawnParticle("Robot_spark2", GetFrame().GetWorldMatrix());
	}

	if(GetIsOnGround())
	{
		SetIsActive(false);
		m_fDespawnTimer = 0.0f;
	}
}

void CLightningUltimate::Render()
{
	CBase::Render();
}

bool CLightningUltimate::CheckCollision(CBase* pBase)
{
	return CBase::CheckCollision(pBase);
}

void CLightningUltimate::CollisionReact(CBase* pBase)
{
}