#include "Barron.h"

#include "../Camera/Camera.h"

const AkGameObjectID BARRON = 4;

#define WALKTIME 0.27f

CBarron::CBarron()
{
	SetType(OBJ_BARRON);
	CWwiseManager::GetInstance()->RegisterObject(BARRON, "Barron");
	//CWwiseManager::GetInstance()->SetAttenuation(BARRON, 100);
}

CBarron::~CBarron()
{
}

void CBarron::Update(float fElapsedTime)
{
	// REMOVE: USED FOR AUDIO TESTING
	//SetCurrentHealth(100);

	CPlayer::Update(fElapsedTime);

	pos.Position.X = GetPosition().x;
	pos.Position.Y = GetPosition().y;
	pos.Position.Z = GetPosition().z;
	D3DXVECTOR3 ori = CCamera::GetInstance()->GetViewPosition() - GetPosition();
	pos.Orientation.X = ori.x;
	pos.Orientation.Y = ori.y;
	pos.Orientation.Z = ori.z;
	CWwiseManager::GetInstance()->SetObjectPosition(BARRON, pos);

	static float fWalk = 0.0f;
	fWalk += fElapsedTime;

	static bool bPrevSpace = false;
	static bool bCurrSpace = false;
	bPrevSpace = bCurrSpace;
	bCurrSpace = m_bIsOnGround;
	if(bPrevSpace == false && bCurrSpace == true)
	{
		CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_DX_BARRON_LAND, BARRON);
	}

	if(m_bAudioPlayWalk && fWalk >= WALKTIME)
	{
		CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_FOOTSTEPS, BARRON);
		m_bAudioPlayWalk = false;
		fWalk = 0.0f;
	}

	if(m_bAudioPlayJump == true)
	{
		CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_BARRON_JUMP, BARRON);
		m_bAudioPlayJump = false;
	}
	m_bAudioPlayJump = false;

	/*if(m_bAudioPlayLand)
	{
		CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_3D_LAND, BARRON);
		m_bAudioPlayLand = false;
	}*/

	if(m_bAudioPlayHurt)
	{
		CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_BARRON_HIT, BARRON);
		m_bAudioPlayHurt = false;
	}

	if(m_bAudioPlayDie)
	{
		CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_BARRON_DEATH, BARRON);
		m_bAudioPlayDie = false;
	}

	if(m_bAudioPlayUlt && m_nUltHitCount < 1)
	{
		//CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_3D_BODYFALL, BARRON);
		m_bAudioPlayUlt = false;
	}

	if(m_bAudioPlayPickUp)
	{
		//CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_2D_HEALTHPICKUP, BARRON);
		m_bAudioPlayPickUp = false;
	}

	if(m_bAudioPlayTransMGun)
	{
		CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_SFX_PICKUP_MACHINEGUN, BARRON);
		m_bAudioPlayTransMGun = false;
		CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_DX_BARRON_POSITIVE_FEEDBACK, BARRON);
	}

	if(m_bAudioPlayTransSGun)
	{
		CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_SFX_PICKUP_SHOTGUN, BARRON);
		CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_DX_ANNOUNCER_SHOTGUN, BARRON);
		m_bAudioPlayTransSGun = false;
		CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_DX_BARRON_POSITIVE_FEEDBACK, BARRON);
	}

	if(m_bAudioPlayTransFGun)
	{
		CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_SFX_PICKUP_FLAMETHROWER, BARRON);
		CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_DX_ANNOUNCER_FLAMETHROWER, BARRON);
		m_bAudioPlayTransFGun = false;
		CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_DX_BARRON_POSITIVE_FEEDBACK, BARRON);
	}

	if(m_bAudioPlayTransLGun)
	{
		CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_SFX_PICKUP_LIGHTNINGGUN, BARRON);
		CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_DX_ANNOUNCER_LIGHTNINGGUN, BARRON);
		m_bAudioPlayTransLGun = false;
		CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_DX_BARRON_POSITIVE_FEEDBACK, BARRON);
	}

	if(m_bAudioPlayTransRGun)
	{
		CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_SFX_PICKUP_ROCKETLAUNCHER, BARRON);
		CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_DX_ANNOUNCER_ROCKETLAUNCHER, BARRON);
		m_bAudioPlayTransRGun = false;
		CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_DX_BARRON_POSITIVE_FEEDBACK, BARRON);
	}

	if(m_bAudioPlayDash)
	{
		CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_PLAYER_DASH, BARRON);
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