#include "CMotorcycle.h"

#include "../Resources/Sound/Wwise_IDs.h"
#include "../Sound/WwiseManager.h"

CMotorcycleObject::CMotorcycleObject()
{
	SetType(OBJ_MOTORCYCLE);
	SetIsAffectedByGrav(false);
	m_resetRevSound = false;
	pParent = NULL;
}

CMotorcycleObject::~CMotorcycleObject()
{

}

void CMotorcycleObject::Update(float fElapsedTime)
{
	CBase::Update(fElapsedTime);

	if(m_vVelocity.x > 0.0f)
	{
		//rev motorcycle
		if(pParent->GetType() == OBJ_BARRON)
		{
			//CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_MOTORCYCLE, 4);
		}
		else if(pParent->GetType() == OBJ_CLAIRE)
		{
			//CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_MOTORCYCLE, 5);
		}

	}
	else if(m_vVelocity.x < 0.0f)
	{
		//lower motorcycle rev
		if(pParent->GetType() == OBJ_BARRON)
		{
			//CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_MOTORCYCLE, 4);
		}
		else if(pParent->GetType() == OBJ_CLAIRE)
		{
			//CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_MOTORCYCLE, 5);
		}

	}
	else if(m_vVelocity.y > 0.0f)
	{
		//motorcycle jump rev
		if(pParent->GetType() == OBJ_BARRON)
		{
			//CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_MOTORCYCLE, 4);
		}
		else if(pParent->GetType() == OBJ_CLAIRE)
		{
			//CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_MOTORCYCLE, 5);
		}

	}
	else
	{
		//reset rev sound to normal
		if(pParent->GetType() == OBJ_BARRON)
		{
			//CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_MOTORCYCLE, 4);
		}
		else if(pParent->GetType() == OBJ_CLAIRE)
		{
			//CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_MOTORCYCLE, 5);
		}

	}
}

void CMotorcycleObject::Render()
{
	CBase::Render();
}

bool CMotorcycleObject::CheckCollision(CBase* pBase)
{
	return CBase::CheckCollision(pBase);
}

void CMotorcycleObject::CollisionReact(CBase* pBase)
{

}