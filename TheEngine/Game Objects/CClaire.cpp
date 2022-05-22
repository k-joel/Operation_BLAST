#include "CClaire.h"

#include "../Camera/Camera.h"

const AkGameObjectID CLAIRE = 5;

#define WALKTIME 0.27f

CClaire::CClaire()
{
	SetType(OBJ_CLAIRE);
	CWwiseManager::GetInstance()->RegisterObject(CLAIRE, "Claire");
	//CWwiseManager::GetInstance()->SetAttenuation(CLAIRE, 100);
}

CClaire::~CClaire()
{
}

void CClaire::Update(float fElapsedTime)
{
	
	CPlayer::Update(fElapsedTime);

	pos.Position.X = GetPosition().x;
	pos.Position.Y = GetPosition().y;
	pos.Position.Z = GetPosition().z;
	D3DXVECTOR3 ori = CCamera::GetInstance()->GetViewPosition() - GetPosition();
	pos.Orientation.X = ori.x;
	pos.Orientation.Y = ori.y;
	pos.Orientation.Z = ori.z;
	CWwiseManager::GetInstance()->SetObjectPosition(CLAIRE, pos);

	static float fWalk = 0.0f;
	fWalk += fElapsedTime;

	static bool bPrevSpace = false;
	static bool bCurrSpace = false;
	bPrevSpace = bCurrSpace;
	bCurrSpace = m_bIsOnGround;
	if(bPrevSpace == false && bCurrSpace == true)
	{
		CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_DX_CLAIRE_LAND, CLAIRE);
		//m_bAudioPlayLand = true; 
	}

	if(m_bAudioPlayWalk && fWalk >= WALKTIME)
	{
		CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_FOOTSTEPS, CLAIRE);
		m_bAudioPlayWalk = false;
		fWalk = 0.0f;
	}

	if(m_bAudioPlayJump == true)
	{
		CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_DX_CLAIRE_JUMP, CLAIRE);
		m_bAudioPlayJump = false;
	}
	m_bAudioPlayJump = false;

	/*if(m_bAudioPlayLand)
	{
		CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_3D_LAND, CLAIRE);
		m_bAudioPlayLand = false;
	}*/

	if(m_bAudioPlayHurt)
	{
		CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_DX_CLAIRE_HIT, CLAIRE);
		m_bAudioPlayHurt = false;
	}

	if(m_bAudioPlayDie)
	{
		CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_DX_CLAIRE_DEATH, CLAIRE);
		m_bAudioPlayDie = false;
	}

	if(m_bAudioPlayUlt && m_nUltHitCount < 1)
	{
		//CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_3D_BODYFALL, CLAIRE);
		m_bAudioPlayUlt = false;
	}

	if(m_bAudioPlayPickUp)
	{
		//CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_2D_HEALTHPICKUP, CLAIRE);
		m_bAudioPlayPickUp = false;
	}

	if(m_bAudioPlayTransMGun)
	{
		CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_SFX_PICKUP_MACHINEGUN, CLAIRE);
		m_bAudioPlayTransMGun = false;
		CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_DX_CLAIRE_POSITIVE_FEEDBACK, CLAIRE);
	}

	if(m_bAudioPlayTransSGun)
	{
		CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_SFX_PICKUP_SHOTGUN, CLAIRE);
		CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_DX_ANNOUNCER_SHOTGUN, CLAIRE);
		m_bAudioPlayTransSGun = false;
		CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_DX_CLAIRE_POSITIVE_FEEDBACK, CLAIRE);
	}

	if(m_bAudioPlayTransFGun)
	{
		CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_SFX_PICKUP_FLAMETHROWER, CLAIRE);
		CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_DX_ANNOUNCER_FLAMETHROWER, CLAIRE);
		m_bAudioPlayTransFGun = false;
		CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_DX_CLAIRE_POSITIVE_FEEDBACK, CLAIRE);
	}

	if(m_bAudioPlayTransLGun)
	{
		CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_SFX_PICKUP_LIGHTNINGGUN, CLAIRE);
		CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_DX_ANNOUNCER_LIGHTNINGGUN, CLAIRE);
		m_bAudioPlayTransLGun = false;
		CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_DX_CLAIRE_POSITIVE_FEEDBACK, CLAIRE);
	}

	if(m_bAudioPlayTransRGun)
	{
		CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_SFX_PICKUP_ROCKETLAUNCHER, CLAIRE);
		CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_DX_ANNOUNCER_ROCKETLAUNCHER, CLAIRE);
		m_bAudioPlayTransRGun = false;
		CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_DX_CLAIRE_POSITIVE_FEEDBACK, CLAIRE);
	}

	if(m_bAudioPlayDash)
	{
		CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_PLAYER_DASH, CLAIRE);
		m_bAudioPlayDash = false;
	}

	//if(m_bIsDashing)
	//{
	//	m_fCount += 0.015f;
	//	
	//	SetPosition(D3DXVECTOR3(LERP(m_vStartDashPos.x, m_vEndDashPos.x, m_fCount/*0.5f*/), GetPosition().y, GetPosition().z));
	//	
	//	if(m_fCount > 1.0f)
	//	{
	//		m_bIsDashing = false;
	//		m_fCount = 0.0f;
	//	}

	//}

}