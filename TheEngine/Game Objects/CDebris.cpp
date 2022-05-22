#include "CDebris.h"
#include "../Camera/Camera.h"
#include "../FX/CParticleManager.h"

const AkGameObjectID DEBRIS = 24;

CDebris::CDebris()
{
	SetType(OBJ_DEBRIS);
	CWwiseManager::GetInstance()->RegisterObject(DEBRIS, "Debris");
	//CWwiseManager::GetInstance()->SetAttenuation(DEBRIS, 100);
	//m_fWaitTimer = 0.0f;
}

CDebris::~CDebris()
{
}

void CDebris::Update(float fElapsedTime)
{
	CBase::Update(fElapsedTime);

	D3DXMATRIX rotMat;

	D3DXMatrixRotationY(&rotMat, fElapsedTime);

	GetFrame().GetLocalMatrix() = rotMat * GetFrame().GetLocalMatrix();

	pos.Position.X = GetPosition().x;
	pos.Position.Y = GetPosition().y;
	pos.Position.Z = GetPosition().z;
	D3DXVECTOR3 ori = CCamera::GetInstance()->GetViewPosition() - GetPosition();
	pos.Orientation.X = ori.x;
	pos.Orientation.Y = ori.y;
	pos.Orientation.Z = ori.z;
	CWwiseManager::GetInstance()->SetObjectPosition(DEBRIS, pos);

	if(GetIsOnGround())
	{
		//Hey hey, I'm on the ground.  I should explode here.
		{
			CParticleManager::GetInstance()->SpawnParticle("Explosion2", GetFrame().GetWorldMatrix());
			SetIsActive(false);
			//CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_BOSS_METAL_DEBRIS, DEBRIS); // AudioID 200 is menu, 2D space
		}
	}

	CBase::Update(fElapsedTime);
}

void CDebris::CollisionReact(CBase* pBase)
{

}