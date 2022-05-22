#include "CCrate.h"
#include "../Camera/Camera.h"
#include "CPickUps.h"
#include "CObjectFactory.h"
#include "CObjectManager.h"

#include "../Collision/CCollisionTests.h"
#include "../FX/CParticleManager.h"

const AkGameObjectID CRATE = 20;

CCrate::CCrate()
{
	SetType(OBJ_CRATE);
	m_bAudioPlayBreak = false;
	SetIsActive(true);
	SetIsAffectedByGrav(true);
	CWwiseManager::GetInstance()->RegisterObject(CRATE, "Crate");
	//CWwiseManager::GetInstance()->SetAttenuation(CRATE, 75);
}

void CCrate::Initialize()
{
	CBase::Initialize();
	m_bAudioPlayBreak = false;
	SetType(OBJ_CRATE);
	SetIsAffectedByGrav(true);
	SetIsActive(true);
	CWwiseManager::GetInstance()->RegisterObject(CRATE, "Crate");
	//CWwiseManager::GetInstance()->SetAttenuation(CRATE, 75);

	GetShape(0).GetAnimCtrl().reset();
	GetShape(0).GetAnimCtrl().update(0.01f);

	GetShape(1).GetAnimCtrl().SetAnimation("move");
	GetShape(1).GetAnimCtrl().reset();
	GetShape(1).GetAnimCtrl().update(0.01f);

	D3DXMATRIX zero;
	D3DXMatrixIdentity(&zero);
	zero._11 = 0.0f;
	zero._22 = 0.0f;
	zero._33 = 0.0f;
	zero._44 = 0.0f;
	GetShape(1).GetFrame().GetLocalMatrix() = zero;

	m_bExplode = false;
	m_bFloating = false;

	SetVelocity(0.0f, 0.0f, 0.0f);

	m_fLifeSpan = 5.0f;
}

CCrate::~CCrate()
{
}

void CCrate::Update(float fElapsedTime)
{
	pos.Position.X = GetPosition().x;
	pos.Position.Y = GetPosition().y;
	pos.Position.Z = GetPosition().z;
	D3DXVECTOR3 ori = CCamera::GetInstance()->GetViewPosition() - GetPosition();
	pos.Orientation.X = ori.x;
	pos.Orientation.Y = ori.y;
	pos.Orientation.Z = ori.z;
	CWwiseManager::GetInstance()->SetObjectPosition(CRATE, pos);

	m_fLifeSpan -= fElapsedTime;

	if(!GetIsAffectedByGrav())
	{
		if(m_fLifeSpan > 0.0f)
		{
		//move left and right
		int fTest = CCollisionTests::IntersectFrustrumSphere(GetBVSphere().GetSphere(),CCamera::GetInstance()->GetFrustum());
		if( fTest == CT_INTERSECT || fTest == CT_OUT)
		{
			SetVelocity(ori.x*0.7f, ori.y*0.5f, 0.0f);
		}

		//move up and down
		//SetVelocity(GetVelocity() + D3DXVECTOR3(0.0f, ((rand()%100-50)/50.0f * 20.0f ), 0.0f));
		SetVelocity(GetVelocity() + D3DXVECTOR3(ori.x*0.1f, ori.y*0.5f, 0.0f));
		}
		else
		{
			//move away from center of screen
			SetVelocity(GetVelocity() + D3DXVECTOR3(-ori.x*0.1f, 0.1f, 0.0f));

			int fTest = CCollisionTests::IntersectFrustrumSphere(GetBVSphere().GetSphere(),CCamera::GetInstance()->GetFrustum());
			if(fTest == CT_OUT)
			{
				SetIsActive(false);
			}
		}
	}

	D3DXVECTOR3 vel = GetVelocity();
	FLOAT len = D3DXVec3Length(&vel);
	if(len > 200.0f)
	{
		vel = vel/len * 200.0f;
		SetVelocity(vel);
	}

	/*if(m_bAudioPlayBreak == true)
	{
		CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_CRATEBREAK, CRATE);
		m_bAudioPlayBreak = false;
	}*/

	//helios
	if(!GetIsAffectedByGrav() && m_bFloating == false)
	{
		D3DXMATRIX position;
		D3DXMatrixIdentity(&position);
		D3DXMatrixTranslation(&position, 0.0f, 15.0f, 0.0f);
		GetShape(1).GetFrame().GetLocalMatrix() = position;

		SetVelocity(((rand()%2)-2)*100.0f, 100.0f, 0.0f);
		m_bFloating = true;
	}
	GetShape(1).GetAnimCtrl().update(fElapsedTime);

	//crate
	unsigned int loop = GetShape(0).GetAnimCtrl().getLoopCount();
	if(m_bExplode && loop < 1)
	{
		GetShape(0).GetAnimCtrl().update(fElapsedTime);
	}

	loop = GetShape(0).GetAnimCtrl().getLoopCount();
	if(loop == 1)
	{
		GetShape(0).GetAnimCtrl().update(-fElapsedTime);
		SetIsActive(false);
		CParticleManager::GetInstance()->SpawnParticle("Explosion2", GetFrame().GetWorldMatrix());
		CFXManager::GetInstance()->LoadEffect(FX_HELIOS_EXPLODE, GetFrame().GetWorldMatrix(), 0.7f,	false);
	}

	CBase::Update(fElapsedTime);
}

void CCrate::Render()
{
}

bool CCrate::CheckCollision(CBase* pBase)
{
	return CBase::CheckCollision(pBase);
}

void CCrate::CollisionReact(CBase* pBase)
{
	if(!m_bExplode)
	{
		switch(pBase->GetType())
		{
		case OBJ_MACHINE_GUN_BULLET:
		case OBJ_SHOTGUN_BULLET:
		case OBJ_SHOTGUN_ALTERNATE:
		case OBJ_SHOTGUN_ULTIMATE:
		case OBJ_FLAME:
		case OBJ_FLAME_ALTERNATE:
		case OBJ_FLAME_ULTIMATE:
		case OBJ_ROCKET:
		case OBJ_ROCKET_ALTERNATE:
		case OBJ_ROCKET_ULTIMATE:
		case OBJ_LIGHTNING:
		case OBJ_LIGHTNING_ALTERNATE:
		case OBJ_LIGHTNING_ULTIMATE:
			{
				if(GetIsAffectedByGrav())
				{
					CreatePickup();
					m_bExplode = true;
					if(m_bAudioPlayBreak == false)
						m_bAudioPlayBreak = true;
					break;
				}
				else
				{
					SetIsAffectedByGrav(true);
					GetShape(1).GetAnimCtrl().SetAnimation("die");
				}
			}
		};

		if(m_bAudioPlayBreak == true)
		{
			CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_CRATEBREAK, CRATE);
			m_bAudioPlayBreak = false;
		}
	}
}

void CCrate::CreatePickup(void)
{
	int choice = rand() % 16;

	CPickUp* pickup;

	//Turning off the rocket launcher and lightning gun for now.
	if(choice <= 10)
	{
		pickup = CObjectFactory::GetInstance()->GetObject<CShotGunPickUp>(OBJ_SHOTGUN_PICKUP);
	}
	else if(choice >= 11 && choice <= 15)
	{
		pickup = CObjectFactory::GetInstance()->GetObject<CFlameThrowerPickUp>(OBJ_FLAMETHROWER_PICKUP);
	}
	//else if(choice >= 16 && choice <= 19)
	//{
	//	pickup = CObjectFactory::GetInstance()->GetObject<CRocketLauncherPickUp>(OBJ_ROCKET_PICKUP);
	//}
	/*else if(choice >= 16 && choice <= 19)
	{
		pickup = CObjectFactory::GetInstance()->GetObject<CLightningGunPickUp>(OBJ_LIGHTNING_PICKUP);
	}*/

	pickup->SetPosition(D3DXVECTOR3(GetPosition().x, GetPosition().y + 10.0f, GetPosition().z));
	pickup->SetRotationY(-D3DX_PI*0.5f);
	pickup->Update(0.2f);
	pickup->SetIsActive(true);

	CObjectManager::GetInstance()->AddObject(pickup);
}