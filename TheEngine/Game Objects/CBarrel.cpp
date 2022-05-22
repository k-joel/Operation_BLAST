#include "CBarrel.h"
#include "../Camera/Camera.h"
#include "../FX/CFXManager.h"

const AkGameObjectID BARREL = 21;

CBarrel::CBarrel()
{
	SetType(OBJ_BARREL);
	m_bAudioPlayExplode = false;
	SetIsActive(true);
	SetIsAffectedByGrav(true);
	CWwiseManager::GetInstance()->RegisterObject(BARREL, "Barrel");
	//CWwiseManager::GetInstance()->SetAttenuation(BARREL, 75);
}

void CBarrel::Initialize()
{
	CBase::Initialize();
	SetType(OBJ_BARREL);
	m_bAudioPlayExplode = false;
	CWwiseManager::GetInstance()->RegisterObject(BARREL, "Barrel");
	//CWwiseManager::GetInstance()->SetAttenuation(BARREL, 75);

	GetShape(0).GetAnimCtrl().reset();
	GetShape(0).GetAnimCtrl().update(0.01);
	m_bExplode = false;
}

CBarrel::~CBarrel()
{
}

void CBarrel::Update(float fElapsedTime)
{
	pos.Position.X = GetPosition().x;
	pos.Position.Y = GetPosition().y;
	pos.Position.Z = GetPosition().z;
	D3DXVECTOR3 ori = CCamera::GetInstance()->GetViewPosition() - GetPosition();
	pos.Orientation.X = ori.x;
	pos.Orientation.Y = ori.y;
	pos.Orientation.Z = ori.z;
	CWwiseManager::GetInstance()->SetObjectPosition(BARREL, pos);

	/*if(m_bAudioPlayExplode == true)
	{
		CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_BARRELSHOT, BARREL);
		m_bAudioPlayExplode = false;
	}*/

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
	}

	CBase::Update(fElapsedTime);
}

void CBarrel::Render()
{
}

bool CBarrel::CheckCollision(CBase* pBase)
{
	return CBase::CheckCollision(pBase);
}

void CBarrel::CollisionReact(CBase* pBase)
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
		case OBJ_ROCKET_BULLET:
		case OBJ_ROCKET_ALTERNATE:
		case OBJ_ROCKET_ULTIMATE:
		case OBJ_LIGHTNING:
		case OBJ_LIGHTNING_ALTERNATE:
		case OBJ_LIGHTNING_ULTIMATE:
			{
				//effect stuff here eventualy
				m_bAudioPlayExplode = true;
				m_bExplode = true;
				CFXManager::GetInstance()->LoadEffect(FX_EXPLOSION_BARREL, GetFrame().GetWorldMatrix(), 0.5f, false);
				break;
			}
		};

		if(m_bAudioPlayExplode == true)
		{
			CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_BARRELSHOT, BARREL);
			m_bAudioPlayExplode = false;
		}
	}
}