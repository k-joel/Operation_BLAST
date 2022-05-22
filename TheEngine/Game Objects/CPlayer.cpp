#include "CPlayer.h"
#include "../Input/CInput.h"
#include "Enemy/CStriker.h"
#include "Enemy/CAssaulter.h"
#include "Enemy/CHelios.h"

#include "../Resources/Sound/Wwise_IDs.h"
#include "../Sound/WwiseManager.h"

//Factory and manager
#include "../Game Objects/CObjectManager.h"
#include "../Game Objects/CObjectFactory.h"
#include "../Game Objects/CHUD.h"
#include "../Game Objects/Bullet/CMachineGunBullet.h"
#include "../Game Objects/Bullet/CShotgunBullet.h"
#include "../Game Objects/Bullet/CFlamethrowerBullet.h"
#include "../Game Objects/Bullet/CLightningBullet.h"
#include "../Game Objects/Bullet/CRocketBullet.h"
#include "../Game Objects/CSplit.h"
#include "../Game Objects/CMerge.h"
#include "../Game Objects/CSpawner.h"
#include "../Debug System/DebugSystem.h"
#include "../../Core/CGame.h"
#include "../Mesh/modelImporter.h"
#include "../FX/CParticleManager.h"
#include "../States/CStateMachine.h"
#include "../States/CPauseState.h"
#include "../States/CGameState.h"
#include "../States/CCharacterSelectionState.h"
#include "../Rendering/CRenderer.h"

#include "../Collision/CCollisionTests.h"

//Includes for the weapon states
#include "Weapon States/CMachineGunState.h"
#include "Weapon States/CFlamethrowerState.h"
#include "Weapon States/CShotgunState.h"
#include "Weapon States/CLightningGunState.h"

#include "CPickUps.h"

using namespace std;

//const AkGameObjectID PLAYER_ULT_HIT = 600;
//const AkGameObjectID PLAYER_DIE_SFX = 500;
//const AkGameObjectID PLAYER_LAND_SFX = 400;
//const AkGameObjectID PLAYER_WALK_SFX = 200;
//const AkGameObjectID PLAYER_JUMP_SFX = 300;
//const AkGameObjectID PLAYER_TAKE_HIT = 100;
//const AkGameObjectID PLAYER = 2;

static int preState = IDLE;

#define ULT_CHARGING_RATE 1.5f

#define DASH_TIMER 0.05f
#define DASH_DIST 48
#define DUST_THRESHOLD 0.1f

//-----------------------------------------------------------------------------
// Name: CPlayer::CPlayer
// Desc: Constructor for CPlayer
//-----------------------------------------------------------------------------
CPlayer::CPlayer()
{
	m_fMultTimer = 0.0f;
	m_myHUD = NULL;
	m_bRespawn = false;
	m_bReverse = false;
	state = IDLE;

	m_bFiring = false;

	m_bAimSet = false;

	m_fHitTimer = 0.0;
	m_bHit = false;
	// Sound Stuff
	/*CWwiseManager::GetInstance()->RegisterObject(PLAYER_TAKE_HIT, "Player Hit");
	CWwiseManager::GetInstance()->RegisterObject(PLAYER_JUMP_SFX, "Player Jump");
	CWwiseManager::GetInstance()->RegisterObject(PLAYER_LAND_SFX, "Player Land");
	CWwiseManager::GetInstance()->RegisterObject(PLAYER_DIE_SFX, "Player Death");
	CWwiseManager::GetInstance()->RegisterObject(PLAYER_WALK_SFX, "Player Walk");
	CWwiseManager::GetInstance()->RegisterObject(PLAYER_ULT_HIT, "Player Ult Hit");*/
	//CWwiseManager::GetInstance()->RegisterObject(PLAYER, "Player");

	m_eLoseEvent = CEventSystem::GetInstance()->FindEvent("LoseEvent");
	m_eWinEvent = CEventSystem::GetInstance()->FindEvent("WinEvent");
	m_eSplitEvent = CEventSystem::GetInstance()->FindEvent("SplitEvent");
	m_eMergeEvent = CEventSystem::GetInstance()->FindEvent("MergeEvent");
	m_eShowPlayerID = CEventSystem::GetInstance()->FindEvent("ShowPlayerID");

	m_pInput = CInput::GetInstance();

	Register();

	m_bInvincible = false;
	readyToFire = true;
	m_bAltReady = true;
	m_bUltReady = false;
	m_bIsDashing = false;
	SetIsJumping(false);
	fireTimer = 0.0f;
	m_fAltTimer = 0.0f;
	m_fUltTimer = 0.0f;
	m_fUltGauge = 0.0f;
	m_fInvincibleTimer = 0.0f;
	m_fBlink = 0.0f;
	m_fReverseTimer = 0.0f;
	m_fMult = 1.0f;
	m_nDashCount = 0;
	m_fCount = 0.0f;
	m_nHat = 0;
	m_nUltHitCount = 0;

	//Setting the weapon state to the default machine gun
	m_pWeaponState = CMachineGunState::GetInstance();
	m_nAmmo = m_pWeaponState->GetMaxAmmo();

	/************************************************************************/
	/* SOUND STUFF                                                          */
	/************************************************************************/
	m_bAudioPlayHurt	=	false;
	m_bAudioPlayDie		=	false;
	m_bAudioPlayJump	=	false;
	m_bAudioPlayLand	=	false;
	m_bAudioPlayWalk	=	false;
	m_bAudioPlayUlt		=	false;
	m_bAudioPlayPickUp	=	false;
	m_bAudioIsFlamePlay =	false;
	m_bAudioPlayDash	=	false;

	m_bAudioPlayTransMGun	=	false;
	m_bAudioPlayTransSGun	=	false;
	m_bAudioPlayTransFGun	=	false;
	m_bAudioPlayTransLGun	=	false;
	m_bAudioPlayTransRGun	=	false;
	m_bRocketDrop = false;

	m_bFlameOn			=	false;

	/************************************************************************/
	/* AI STUFF                                                             */
	/************************************************************************/
	m_bAI = false;

	/************************************************************************/
	/* GOAL STUFF                                                           */
	/************************************************************************/
	m_goal = NULL;

	/************************************************************************/
	/* LUA THANGS                                                           */
	/************************************************************************/
	luaState = lua_open();
	luaL_openlibs(luaState);

	luaL_dofile(luaState, "Resources/Lua/Game Objects/Player.lua");
}

void CPlayer::Initialize()
{
	CBase::Initialize();
	m_fMultTimer = 0.0f;
	m_myHUD = NULL;
	m_bRespawn = false;
	m_bReverse = false;
	m_bSlow = false;
	state = IDLE;
	m_fGroundTimer = 0.0f;
	m_bChargePartOut = false;
	m_bFiring = false;

	m_bAimSet = false;

	m_eLoseEvent = CEventSystem::GetInstance()->FindEvent("LoseEvent");
	m_eWinEvent = CEventSystem::GetInstance()->FindEvent("WinEvent");
	m_eSplitEvent = CEventSystem::GetInstance()->FindEvent("SplitEvent");
	m_eMergeEvent = CEventSystem::GetInstance()->FindEvent("MergeEvent");

	m_pInput = CInput::GetInstance();

	Register();

	m_bInvincible = false;
	readyToFire = true;
	m_bAltReady = true;
	m_bUltReady = false;
	m_bIsDashing = false;
	m_bIsDownJumping = false;
	SetIsJumping(false);
	fireTimer = 0.0f;
	m_fAltTimer = 0.0f;
	m_fUltTimer = 0.0f;
	m_fUltGauge = 0.0f;
	m_fInvincibleTimer = 0.0f;
	m_fBlink = 0.0f;
	m_fReverseTimer = 0.0f;
	m_fSlowTimer = 0.0f;
	m_fMult = 1.0f;
	//m_fDashTimer = 6.0f;
	m_nDashCount = 1;
	m_fCount = 0.0f;
	m_nHat = 1;
	m_nUltHitCount = 0;

	//Setting the weapon state to the default machine gun
	m_pWeaponState = CMachineGunState::GetInstance();
	m_nAmmo = m_pWeaponState->GetMaxAmmo();

	// Audio checks
	m_bAudioFlameNorm	=	false;
	m_bAudioPlayHurt	=	false;
	m_bAudioPlayDie		=	false;
	m_bAudioPlayJump	=	false;
	m_bAudioPlayLand	=	false;
	m_bAudioPlayWalk	=	false;
	m_bAudioPlayUlt		=	false;
	m_bAudioPlayPickUp	=	false;
	m_bAudioIsFlamePlay =	false;
	m_bAudioPlayDash	=	false;

	m_bAudioPlayTransMGun	=	false;
	m_bAudioPlayTransSGun	=	false;
	m_bAudioPlayTransFGun	=	false;
	m_bAudioPlayTransLGun	=	false;
	m_bAudioPlayTransRGun	=	false;
	m_bRocketDrop = false;

	m_bFlameOn			=	false;
	m_goal = NULL;
	/************************************************************************/
	/* AI STUFF                                                             */
	/************************************************************************/
	m_bAI = false;

	m_fStormDamage = 0.0f;
	m_fDelay = 0.0f;
	m_fDelayDecay = 0.0f;
	m_nBoltCount = 0;
	m_nMaxBolts = 0;
	m_fBoltSpeed = 0.0f;
	m_fBoltTimer = 0.0f;
	m_bLightningStorm = false;
	m_bGrounded = false;

	m_fHitTimer = 0.0;
	m_bHit = false;

	m_bIsProne = false;

	D3DXMATRIX matIdentity;
	D3DXMatrixIdentity(&matIdentity);
	m_nShadowEffect = CFXManager::GetInstance()->LoadEffect(FX_SHADOW_BLOB, matIdentity, -1.0, true);

	m_nHatStandIn = CFXManager::GetInstance()->LoadEffect(FX_HAT_STANDIN, matIdentity, -1.0, true);

	m_pPilotLightParticle = NULL;

	m_fLifeTime = 0.0f;
	m_fShowID = 5.0f;
	m_nIDEffect = CFXManager::GetInstance()->LoadEffect(FX_P1_ID, matIdentity, -1.0, true);

	m_nCurrentAnimation = 0;
	m_nPreviousAnimation = 0;
	m_fDustTimer = 0.0f;
	m_pDustEffect = CParticleManager::GetInstance()->SpawnParticle("Dust_kickup", GetFrame().GetWorldMatrix());

	m_fUltChargeUp = 2.1f;

	m_pDashEffect = NULL;

	switch(GetType())
	{
	case OBJ_BARRON:
		m_nHat = CCharacterSelectionState::GetInstance()->GetPlayer1Hat();
		break;
	case OBJ_CLAIRE:
		m_nHat = CCharacterSelectionState::GetInstance()->GetPlayer2Hat();
		break;
	}

	switch(m_nHat)
	{
	case 1:
		{
			if(GetType() == OBJ_BARRON)
			{
				CFXManager::GetInstance()->GetEffect(m_nHatStandIn)->GetShape(0).SetMesh(&CModelManager::GetInstance()->GetModel(CLoadState::GetInstance()->GetModelID("armyhatShape"))->GetMesh());
			}
			else
			{
				CFXManager::GetInstance()->GetEffect(m_nHatStandIn)->GetShape(0).SetMesh(&CModelManager::GetInstance()->GetModel(CLoadState::GetInstance()->GetModelID("Claire_hatShape"))->GetMesh());
			}
		}
		break;
	case BEAR:
			CFXManager::GetInstance()->GetEffect(m_nHatStandIn)->GetShape(0).SetMesh(&CModelManager::GetInstance()->GetModel(CLoadState::GetInstance()->GetModelID("bearhatShape"))->GetMesh());
		break;
	case CAT:
			CFXManager::GetInstance()->GetEffect(m_nHatStandIn)->GetShape(0).SetMesh(&CModelManager::GetInstance()->GetModel(CLoadState::GetInstance()->GetModelID("cathatShape"))->GetMesh());
		break;
	case COW:
			CFXManager::GetInstance()->GetEffect(m_nHatStandIn)->GetShape(0).SetMesh(&CModelManager::GetInstance()->GetModel(CLoadState::GetInstance()->GetModelID("cowboyhatShape"))->GetMesh());
		break;
	case FANCY:
			CFXManager::GetInstance()->GetEffect(m_nHatStandIn)->GetShape(0).SetMesh(&CModelManager::GetInstance()->GetModel(CLoadState::GetInstance()->GetModelID("fancyhatShape"))->GetMesh());
		break;
	case FRUIT:
			CFXManager::GetInstance()->GetEffect(m_nHatStandIn)->GetShape(0).SetMesh(&CModelManager::GetInstance()->GetModel(CLoadState::GetInstance()->GetModelID("fruithatShape"))->GetMesh());
		break;
	case NAR:
			CFXManager::GetInstance()->GetEffect(m_nHatStandIn)->GetShape(0).SetMesh(&CModelManager::GetInstance()->GetModel(CLoadState::GetInstance()->GetModelID("narwhalhatShape"))->GetMesh());
		break;
	case MAR:
			CFXManager::GetInstance()->GetEffect(m_nHatStandIn)->GetShape(0).SetMesh(&CModelManager::GetInstance()->GetModel(CLoadState::GetInstance()->GetModelID("marchinghatShape"))->GetMesh());
		break;
	case POKE:
			CFXManager::GetInstance()->GetEffect(m_nHatStandIn)->GetShape(0).SetMesh(&CModelManager::GetInstance()->GetModel(CLoadState::GetInstance()->GetModelID("pokehatShape"))->GetMesh());
		break;
	case POT:
			CFXManager::GetInstance()->GetEffect(m_nHatStandIn)->GetShape(0).SetMesh(&CModelManager::GetInstance()->GetModel(CLoadState::GetInstance()->GetModelID("pothatShape"))->GetMesh());
		break;
	case SHARK:
			CFXManager::GetInstance()->GetEffect(m_nHatStandIn)->GetShape(0).SetMesh(&CModelManager::GetInstance()->GetModel(CLoadState::GetInstance()->GetModelID("sharkhatShape"))->GetMesh());
		break;
	case SOM:
			CFXManager::GetInstance()->GetEffect(m_nHatStandIn)->GetShape(0).SetMesh(&CModelManager::GetInstance()->GetModel(CLoadState::GetInstance()->GetModelID("sombrerohatShape"))->GetMesh());
		break;
	default:
		CFXManager::GetInstance()->GetEffect(m_nHatStandIn)->SetRenderType(RT_DONOTRENDER);
		break;
	}
}

void CPlayer::Shutdown()
{
	CBase::Shutdown();

	/*if(CGameState::GetInstance()->GetShootingGallery())
	{
		CObjectManager::GetInstance()->DestroyAllGameObjects();

		if(m_nPlayerNum == 1)
		{
			if(CGameState::GetInstance()->GetPlayerTwo())
			CGameState::GetInstance()->GetPlayerTwo()->AddScore(5000);
		}
		else
		{
			if(CGameState::GetInstance()->GetPlayerOne())
			CGameState::GetInstance()->GetPlayerOne()->AddScore(5000);
		}
	}*/

	if(m_pPilotLightParticle)
	{
		m_pPilotLightParticle->SetComplete(true);
		m_pPilotLightParticle = NULL;
	}

	CFXManager::GetInstance()->UnloadEffect(m_nShadowEffect);
	CFXManager::GetInstance()->UnloadEffect(m_nHatStandIn);
	CFXManager::GetInstance()->UnloadEffect(m_nIDEffect);
}

//-----------------------------------------------------------------------------
// Name: CPlayer::~CPlayer
// Desc: Destructor for CPlayer%
//-----------------------------------------------------------------------------
CPlayer::~CPlayer()
{
	Deregister();
	if(m_bAI)
		m_AIsm.Shutdown();

	lua_close(luaState);
}

void CPlayer::Input(int nPlayer)
{
	m_bAimSet = false;

	SetVelocity(D3DXVECTOR3(0.0f, GetVelocity().y,0.0f));
	KeyboardDevice* keyboard = (KeyboardDevice*)CInput::GetInstance()->GetDevice(DEV_KEYBOARD);
	D3DXVECTOR3 vBulletVel(0, 0, 0);
	float rotY = 0.0f;
	float rotX = 0.0f;

	if(!CCamera::GetInstance()->GetIsPanning())
	{
		if(CInput::GetInstance()->GetAction(ACT_BACK,nPlayer))
		{
			CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_MENU_PAUSE, 200);
			CGameState::GetInstance()->SetIsPaused(true);
			CStateSystem::GetInstance()->AddState(CPauseState::GetInstance());
		}

		if(CInput::GetInstance()->GetAction(ACT_PAUSE, nPlayer) )
		{
			//CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PAUSE_ALL, 200);
			CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_MENU_PAUSE, 200);
			CGameState::GetInstance()->SetIsPaused(true);
			CStateSystem::GetInstance()->AddState(CPauseState::GetInstance());
		}

		if(m_bRespawn == false)
		{
			if(m_pInput->GetActionAmount(ACT_MOVEX,nPlayer) < -16000.0f)
			{
				if(!m_bReverse)
				{
					m_vVelocity.x	= -((float)lua_tonumber(luaState, -5));
					if(m_bSlow == true)
					{
						m_vVelocity.x *= 0.5f;
					}
				}
				else
				{
					m_vVelocity.x	= ((float)lua_tonumber(luaState, -5));
					if(m_bSlow == true)
					{
						m_vVelocity.x *= 0.5f;
					}
				}
			}

			if(m_pInput->GetActionAmount(ACT_MOVEX,nPlayer) > 16000.0f)
			{
				if(!m_bReverse)
				{
					m_vVelocity.x	= ((float)lua_tonumber(luaState, -5));
					if(m_bSlow == true)
					{
						m_vVelocity.x *= 0.5f;
					}
				}
				else
				{
					m_vVelocity.x	= -((float)lua_tonumber(luaState, -5));
					if(m_bSlow == true)
					{
						m_vVelocity.x *= 0.5f;
					}
				}
			}

			if(m_vVelocity.x < 0.0f)
			{
				SetRotationY(D3DX_PI*0.5);
				SetIsFacingLeft(true);
			}
			else if(m_vVelocity.x > 0.0f)
			{
				SetRotationY(-D3DX_PI*0.5);
				SetIsFacingLeft(false);
			}

			if(m_pInput->GetAction(ACT_SHOOT,nPlayer))
				m_bFiring = true;
			else
				m_bFiring = false;

			if(!m_bIsProne)
			{
				//Firing a normal shot angled up
				if( (m_pInput->GetAction(ACT_HOLDUP,nPlayer) && (m_pInput->GetAction(ACT_HOLDRIGHT,nPlayer) || m_pInput->GetAction(ACT_HOLDLEFT,nPlayer) ) && m_pInput->GetAction(ACT_SHOOT,nPlayer)   )
					|| (m_pInput->GetActionAmount(ACT_MOVEY,nPlayer) < 22500 && m_pInput->GetActionAmount(ACT_MOVEY,nPlayer) > 7000 && m_pInput->GetAction(ACT_SHOOT,nPlayer)))
				{
					Shoot(ANGLE_UP, NORMAL_FIRE);
				}

				//Firing an alt shot angled up
				if( (m_pInput->GetAction(ACT_HOLDUP,nPlayer) && (m_pInput->GetAction(ACT_HOLDRIGHT,nPlayer) || m_pInput->GetAction(ACT_HOLDLEFT,nPlayer) ) && m_pInput->GetAction(ACT_ALTFIRE,nPlayer)   )
					|| (m_pInput->GetActionAmount(ACT_MOVEY,nPlayer) < 22500 && m_pInput->GetActionAmount(ACT_MOVEY,nPlayer) > 7000 && m_pInput->GetAction(ACT_ALTFIRE,nPlayer)))
				{
					Shoot(ANGLE_UP, ALTERNATE_FIRE);
				}

				//Firing a normal shot angled down
				if((m_pInput->GetAction(ACT_HOLDDOWN,nPlayer) && (m_pInput->GetAction(ACT_HOLDRIGHT,nPlayer) || m_pInput->GetAction(ACT_HOLDLEFT,nPlayer) ) && m_pInput->GetAction(ACT_SHOOT,nPlayer)   )
					|| (m_pInput->GetActionAmount(ACT_MOVEY,nPlayer) > -22500 && m_pInput->GetActionAmount(ACT_MOVEY,nPlayer) < -7000 && m_pInput->GetAction(ACT_SHOOT,nPlayer)))
				{
					Shoot(ANGLE_DOWN, NORMAL_FIRE);
				}

				//Firing an alt shot angled down
				if((m_pInput->GetAction(ACT_HOLDDOWN,nPlayer) && (m_pInput->GetAction(ACT_HOLDRIGHT,nPlayer) || m_pInput->GetAction(ACT_HOLDLEFT,nPlayer) ) && m_pInput->GetAction(ACT_ALTFIRE,nPlayer)   )
					|| (m_pInput->GetActionAmount(ACT_MOVEY,nPlayer) > -22500 && m_pInput->GetActionAmount(ACT_MOVEY,nPlayer) < -7000 && m_pInput->GetAction(ACT_ALTFIRE,nPlayer)))
				{
					Shoot(ANGLE_DOWN, ALTERNATE_FIRE);
				}

				//Firing a normal shot straight up
				if((m_pInput->GetAction(ACT_HOLDUP,nPlayer) && m_pInput->GetAction(ACT_SHOOT,nPlayer)) || (m_pInput->GetActionAmount(ACT_MOVEY,nPlayer) > 22500 && m_pInput->GetAction(ACT_SHOOT,nPlayer)) )
				{
					Shoot(STRAIGHT_UP, NORMAL_FIRE);
				}

				//Firing an alt shot straight up
				if((m_pInput->GetAction(ACT_HOLDUP,nPlayer) && m_pInput->GetAction(ACT_ALTFIRE,nPlayer)) || (m_pInput->GetActionAmount(ACT_MOVEY,nPlayer) > 22500 && m_pInput->GetAction(ACT_ALTFIRE,nPlayer)) )
				{
					Shoot(STRAIGHT_UP, ALTERNATE_FIRE);
				}

				//Firing a normal shot straight down
				if((m_pInput->GetAction(ACT_HOLDDOWN,nPlayer)  && m_pInput->GetAction(ACT_SHOOT,nPlayer) ) || (m_pInput->GetActionAmount(ACT_MOVEY,nPlayer) < -22500 && m_pInput->GetAction(ACT_SHOOT,nPlayer)) )
				{
					Shoot(STRAIGHT_DOWN, NORMAL_FIRE);
				}

				//Firing an alt shot straight down
				if((m_pInput->GetAction(ACT_HOLDDOWN,nPlayer)  && m_pInput->GetAction(ACT_ALTFIRE,nPlayer) ) || (m_pInput->GetActionAmount(ACT_MOVEY,nPlayer) < -22500 && m_pInput->GetAction(ACT_ALTFIRE,nPlayer)) )
				{
					Shoot(STRAIGHT_DOWN, ALTERNATE_FIRE);
				}

				if(m_pInput->GetAction(ACT_HOLDLEFT,nPlayer))
				{
					if(!m_bReverse)
					{
						m_vVelocity.x = -((float)lua_tonumber(luaState, -5));
						state = WALK;
						SetIsFacingLeft(true);
						SetRotationY(D3DX_PI * 0.5);
					}
					else
					{
						m_vVelocity.x = ((float)lua_tonumber(luaState, -5));
						state = WALK;
						SetIsFacingLeft(false);
						SetRotationY(D3DX_PI * -0.5);
					}

					if(GetIsOnGround() && m_fDustTimer >= DUST_THRESHOLD)
					{
						m_pDustEffect = CParticleManager::GetInstance()->SpawnParticle("Dust_kickup", GetFrame().GetWorldMatrix());
						m_fDustTimer = 0.0f;
					}
				}

				if(m_pInput->GetAction(ACT_HOLDRIGHT,nPlayer))
				{
					if(!m_bReverse)
					{
						m_vVelocity.x = ((float)lua_tonumber(luaState, -5));
						state = WALK;
						SetIsFacingLeft(false);
						SetRotationY(-D3DX_PI*0.5f);
					}
					else
					{
						m_vVelocity.x = -((float)lua_tonumber(luaState, -5));
						state = WALK;
						SetIsFacingLeft(true);
						SetRotationY(D3DX_PI * 0.5f);
					}

					if(GetIsOnGround() && m_fDustTimer >= DUST_THRESHOLD)
					{
						m_pDustEffect = CParticleManager::GetInstance()->SpawnParticle("Dust_kickup", GetFrame().GetWorldMatrix());
						m_fDustTimer = 0.0f;
					}
				}

				if(!m_bIsDashing && !m_bSlow && m_nDashCount > 0)
				{
					if(m_pInput->GetAction(ACT_LDASH, nPlayer))
					{
						SetRotationY(D3DX_PI*0.5);
						SetIsFacingLeft(true);
						m_vStartDashPos = GetPosition();
						m_vEndDashPos.x = m_vStartDashPos.x - DASH_DIST;
						m_bIsDashing = true;
						m_pDashEffect = CParticleManager::GetInstance()->SpawnParticle("Dash_steak", GetFrame().GetWorldMatrix());
						m_bAudioPlayDash = true;
					}
					if(m_pInput->GetAction(ACT_RDASH, nPlayer))
					{
						SetRotationY(-D3DX_PI*0.5);
						m_vStartDashPos = GetPosition();
						m_vEndDashPos.x = m_vStartDashPos.x + DASH_DIST;
						SetIsFacingLeft(false);
						m_bIsDashing = true;
						m_pDashEffect = CParticleManager::GetInstance()->SpawnParticle("Dash_steak", GetFrame().GetWorldMatrix());
						m_bAudioPlayDash = true;
					}
				}
			}

			if(!(GetIsOnGround() || GetIsOnPlatform()) ||
				m_bIsProne && (!(m_pInput->GetAction(ACT_HOLDDOWN,nPlayer)) || !(m_pInput->GetActionAmount(ACT_MOVEY,nPlayer) < -22500)))
			{
				m_bIsProne = false;
			}

			if((GetIsOnGround() || GetIsOnPlatform()) &&
				((m_pInput->GetAction(ACT_HOLDDOWN,nPlayer)) && !(m_pInput->GetAction(ACT_HOLDLEFT,nPlayer)) && !(m_pInput->GetAction(ACT_HOLDRIGHT,nPlayer)) ||
				((m_pInput->GetActionAmount(ACT_MOVEY,nPlayer) < -22500) && !(m_pInput->GetActionAmount(ACT_MOVEX,nPlayer) < -22500) && !(m_pInput->GetActionAmount(ACT_MOVEX,nPlayer) > 22500) )))
			{
				m_bIsProne = true;
			}

			if(m_pInput->GetAction(ACT_JUMP,nPlayer) && (!(m_pInput->GetAction(ACT_HOLDDOWN, nPlayer)) && !(m_pInput->GetActionAmount(ACT_MOVEY,nPlayer) < -22500)))
			{
				if(!m_bGrounded)
				{
					if(m_bIsOnGround)
					{
						m_vPosition.y += JUMP_EPSILON + 1.9f;
						m_vVelocity.y += ((float)lua_tonumber(luaState, -4));
						m_bIsOnGround = false;
						m_bIsJumping = true;
						//CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_3D_JUMP, PLAYER);
						if(m_bAudioPlayJump == false)
						{
							m_bAudioPlayJump = true;
						}
					}
				}
			}
			else if(m_pInput->GetAction(ACT_JUMP,nPlayer) && (m_pInput->GetAction(ACT_HOLDDOWN, nPlayer) || (m_pInput->GetActionAmount(ACT_MOVEY,nPlayer) < -22500)))
			{
				//down jumping through platforms
				if(GetIsOnPlatform() == true)
				{
					m_vVelocity.y -= ((float)lua_tonumber(luaState, -4))*0.5f;
					m_bIsOnGround = false;
					m_bIsDownJumping = true;
					SetIsonPlatform(false);
					if(m_bAudioPlayJump == false)
					{
						m_bAudioPlayJump = true;
					}
				}
			}

			//Normal shot fired forwards
			if(m_pInput->GetAction(ACT_SHOOT,nPlayer))
			{
				Shoot(STRAIGHT_FORWARD, NORMAL_FIRE);

				if(m_pWeaponState == CFlamethrowerState::GetInstance())
				{
					if(m_bAudioFlameNorm == false)
					{
						m_bAudioFlameNorm = true;
						if(GetType() == OBJ_BARRON)
						{
							CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_FLAMETHROWER_LOOP, 4); // Playing the sound.  Pew pew!
							m_bAudioFlameNorm = true;
						}
						else if(GetType() == OBJ_CLAIRE)
						{
							CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_FLAMETHROWER_LOOP, 5); // Playing the sound.  Pew pew!
							m_bAudioFlameNorm = true;
						}
					}
				}
			}

			if(m_pInput->GetAction(ACT_RELEASESHOOT, nPlayer) && m_pWeaponState == CFlamethrowerState::GetInstance())
			{
				m_bAudioFlameNorm = false;
				if(GetType() == OBJ_BARRON)
				{
					CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::STOP_FX_FLAMETHROWER_LOOP, 4); // Playing the sound.  Pew pew!
				}
				else if(GetType() == OBJ_CLAIRE)
				{
					CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::STOP_FX_FLAMETHROWER_LOOP, 5); // Playing the sound.  Pew pew!
				}
			}

			//Alt shot fired forwards
			if(m_pInput->GetAction(ACT_ALTFIRE, nPlayer))
			{
				Shoot(STRAIGHT_FORWARD, ALTERNATE_FIRE);
			}

			if(m_pInput->GetAction(ACT_ULTFIRE, nPlayer))
			{
				if(m_bUltReady && (m_nAmmo != -100) && (m_pWeaponState->GetWeaponType() != ROCKET_LAUNCHER))
				{
					m_bUltReady = false;
					m_fUltGauge = 0.0f;
					//D3DXVECTOR3 tempVec = GetPosition();
					//tempVec.y += 17.0f;
					//m_pWeaponState->FireUltimate(this, tempVec);
					m_fUltChargeUp = 0.0f;
				}
				else if(m_bUltReady && m_pWeaponState->GetWeaponType() == ROCKET_LAUNCHER)
				{
					m_bUltReady = false;
					m_bRocketDrop = true;
					m_fUltGauge = 0.0f;
					m_fRocketTimer = 2.0;
				}
			}

			if(m_bRocketDrop && m_fRocketTimer <= 0.0)
			{
				D3DXVECTOR3 tempVec = GetPosition();
				tempVec.y += 17.0f;
				m_pWeaponState->FireUltimate(this, tempVec);
				m_bRocketDrop = false;
			}

			if(!m_pInput->GetAction(ACT_SHOOT,nPlayer))
			{
				state = WALK;
			}

			if(m_bIsDashing && (m_nDashCount > 0) )
			{
				if(!(GetIsOnPlatform()||GetIsOnGround()))
					m_nDashCount--;
			}
		}
	}
}

void CPlayer::Update(float fElapsedTime)
{
	if(m_fCount > 0.3f)
	{
		m_bIsDashing = false;
		m_fCount = 0.0f;
	}

	if(m_bIsDashing)
	{
		m_fCount += fElapsedTime;
		if(GetIsFacingLeft())
			SetVelocity(-250.0f, 0.0f, 0.0f);
		else
			SetVelocity(250.0f, 0.0f, 0.0f);
	}

	CBase::Update(fElapsedTime);
	m_fDustTimer += fElapsedTime;

	if(m_bIsProne)
	{
		GetBVAABB().GetAABB().maxPt.y = GetBVAABB().GetAABB().minPt.y + 10.0f;
	}
	else
	{
		GetBVAABB().GetAABB().maxPt.y = GetBVAABB().GetAABB().minPt.y + 30.0f;
	}

	//Pulling down the lua table
	lua_getglobal(luaState, "Player");
	lua_pushstring(luaState, "MoveSpeed");
	lua_gettable(luaState, -2);
	lua_pushstring(luaState, "JumpSpeed");
	lua_gettable(luaState, -3);
	lua_pushstring(luaState, "MultDecay");
	lua_gettable(luaState, -4);
	lua_pushstring(luaState, "InvulnLimit");
	lua_gettable(luaState, -5);
	lua_pushstring(luaState, "Reverse");
	lua_gettable(luaState, -6);

	if(m_fHitTimer > 0.0)
	{
		m_fHitTimer -= fElapsedTime;

		if(m_fHitTimer <= 0.0)
		{
			//CParticleManager::GetInstance()->DestroyParticle(m_nBlood);
			m_bHit = false;
		}
	}

	m_fMultTimer += fElapsedTime;
	if(!m_bSlow)
	{
		fireTimer += fElapsedTime;
		m_fUltTimer += fElapsedTime;
		m_fAltTimer += fElapsedTime;
	}
	else
	{
		fireTimer += fElapsedTime;
		m_fUltTimer += fElapsedTime;
		m_fAltTimer += fElapsedTime;
	}

	m_fLifeTime += fElapsedTime;

	if(m_bGrounded)
	{
		m_fGroundTimer += fElapsedTime;

		if(m_fGroundTimer > 4.0f)
		{
			m_bGrounded = false;
			m_fGroundTimer = 0.0f;
		}
	}

	if(m_fRocketTimer > 0.0 && m_bRocketDrop)
	{
		m_fRocketTimer -= fElapsedTime;
	}

	//If the timer is more than a second, DROP THAT MULTIPLIER LIKE IT AIN'T NO THANG
	if(m_fMultTimer > (float)lua_tonumber(luaState, -3))
	{
		if(m_fMult > 1.0f)
		{
			m_fMult -= 0.1f;
			if(m_myHUD)
			{
				m_myHUD->DecreaseMultiplier();
			}
		}
		m_fMultTimer = 0.0f;

		if(m_fMult < 1.0)
		{
			m_fMult = 1.0;
		}
	}

	if(GetIsOnGround() == true)
	{
		m_bIsJumping = false;
		m_nDashCount = 1;
	}

	if(m_bInvincible)
	{
		m_fInvincibleTimer += fElapsedTime;
		m_fBlink += fElapsedTime;
		if(m_fInvincibleTimer > (float)lua_tonumber(luaState, -2))
		{
			m_bInvincible = false;
			m_fInvincibleTimer = 0.0f;
			m_fBlink = 0.0f;
		}
	}

	if(m_bSlow == false)
	{
		if(fireTimer >= m_pWeaponState->GetFiringRate())
		{
			readyToFire = true;
			fireTimer = 0.0f;
		}

		if(m_fAltTimer > m_pWeaponState->GetAlternateRate())
		{
			m_bAltReady = true;
			m_fAltTimer = 0.0f;
		}
	}
	else
	{
		if(fireTimer >= (m_pWeaponState->GetFiringRate() * 1.5f))
		{
			readyToFire = true;
			fireTimer = 0.0f;
		}

		if(m_fAltTimer > (m_pWeaponState->GetAlternateRate() * 1.5f))
		{
			m_bAltReady = true;
			m_fAltTimer = 0.0f;
		}
	}

	if(m_bReverse)
	{
		m_fReverseTimer += fElapsedTime;

		if(m_fReverseTimer >= (float)lua_tonumber(luaState, -1))
		{
			m_fReverseTimer = 0.0f;
			m_bReverse = false;
		}
	}

	if(m_bSlow)
	{
		m_fSlowTimer += fElapsedTime;

		if(m_fSlowTimer >= 3.0)
		{
			m_fSlowTimer = 0.0f;
			m_bSlow = false;
		}
	}

	//dash effect
	if(m_bIsDashing)
	{
		D3DXMATRIX trans;
		D3DXMatrixTranslation(&trans, 0.0f, 15.0f, 10.0f);
		m_pDashEffect->SetWorld(trans * GetFrame().GetWorldMatrix());
	}
	else if(m_pDashEffect)
	{
		m_pDashEffect->SetIsActive(false);
		m_pDashEffect = NULL;
	}

	//Ult filling up
	if(!m_bUltReady)
	{
		int mult = (int)(m_fMult);
		if(mult > 3)
			mult = 3;

		m_fUltGauge += (ULT_CHARGING_RATE * fElapsedTime * mult); //Updating the gauge by the current multiplier times time times the rate

		if(m_fUltGauge >= 100.0f)
		{
			m_fUltGauge = 100.f;
			m_bUltReady = true;
			m_nUltHitCount = 0;
			if(m_nType == OBJ_BARRON)
			{
				CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_DX_ANNOUNCER_ULTIMATEBARON, 200);
			}
			else if(m_nType == OBJ_CLAIRE)
				CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_DX_ANNOUNCER_ULTIMATECLAIRE, 200);
		}
	}

	//Ult Charge up to fire
	m_fUltChargeUp += fElapsedTime;

	if(m_fUltChargeUp < 0.5f)
	{
		//Setting these so they don't blink
		m_fBlink = 5.0f;
		m_fInvincibleTimer = 5.0f;

		SetVelocity(0.0f, 0.0f, 0.0f);
		SetPosition(m_vPrevPosition);
		D3DXMATRIX partMat;
		D3DXMatrixIdentity(&partMat);
		partMat._41 = GetBVAABB().GetAABB().center.x;
		partMat._42 = GetBVAABB().GetAABB().center.y;
		partMat._43 = GetBVAABB().GetAABB().center.z;

		if(!m_bChargePartOut)
		{
			switch(m_pWeaponState->GetWeaponType())
			{
			case SHOTGUN:
				CParticleManager::GetInstance()->SpawnParticle("Shotgun_windup", partMat);//GetFrame().GetWorldMatrix());
				break;
			case FLAMETHROWER:
				CParticleManager::GetInstance()->SpawnParticle("Flamethrower_windup", partMat);//GetFrame().GetWorldMatrix());
				break;
			case LIGHTNING_GUN:
				CParticleManager::GetInstance()->SpawnParticle("Robot_spark2", GetFrame().GetWorldMatrix());
				break;
			default:
				CParticleManager::GetInstance()->SpawnParticle("HUD_fire", GetFrame().GetWorldMatrix());
			}
			m_bChargePartOut = true;
		}
		m_bInvincible = true;
	}

	if(m_fUltChargeUp >= 0.5f && m_fUltChargeUp < 2.0f && (m_pWeaponState->GetWeaponType() != ROCKET_LAUNCHER))
	{
		m_fUltChargeUp = 2.1f;
		D3DXVECTOR3 tempVec = GetPosition();
		tempVec.y += 17.0f;
		m_pWeaponState->FireUltimate(this, tempVec);
		m_bInvincible = false;
		m_bChargePartOut = false;
		m_fBlink = 0.0f;
		m_fInvincibleTimer = 0.0f;
	}

	//If we're not using the machine gun, check to see if we ran out of ammo for our current gun.  If we have, make the weapon the
	//machine gun again.
	if(m_nAmmo != -100)
	{
		if(m_nAmmo <= 0)
		{
			m_bAudioPlayTransMGun = true;
			if(GetType() == OBJ_BARRON)
			{
				CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::STOP_FX_FLAMETHROWER_LOOP, 4);
			}
			else if(GetType() == OBJ_CLAIRE)
			{
				CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::STOP_FX_FLAMETHROWER_LOOP, 5);
			}
			SwitchWeapon(CMachineGunState::GetInstance());
		}
	}
	GetShape(0).GetAnimCtrl().update(fElapsedTime);

	if(m_goal != NULL)
	{
		m_bIsProne = false;
		//move as close as possable to goal, ignore y axis
		if(GetPosition().x <= m_goal->GetPosition().x -2.0f || GetPosition().z <= m_goal->GetPosition().z - 2.0f)
		{
			D3DXVECTOR3 toGoal = m_goal->GetPosition()-GetPosition();
			toGoal.y = 0.0f;
			float goalMag = D3DXVec3Length(&toGoal);
			D3DXVec3Normalize(&toGoal, &toGoal);
			float playerMag = ((float)lua_tonumber(luaState, -5));
			toGoal *= playerMag;
			D3DXVECTOR3 withGravity = GetVelocity();
			withGravity.x = toGoal.x;
			withGravity.z = toGoal.z;
			SetVelocity(withGravity);
			UpdateAnimation(fElapsedTime);
		}
		else
		{
			m_goal = NULL;
			SetVelocity(D3DXVECTOR3(0.0f,0.0f,0.0f));
		}
	}
	else if(m_bAI)
	{
		//m_AIsm.Update(fElapsedTime);
		SetVelocity(GetCurrentTarget()->GetVelocity().x,GetVelocity().y,GetVelocity().z);

		if(GetCurrentTarget()->GetVelocity().x != 0.0f)
			GetShape(0).GetAnimCtrl().SetAnimation("walk");
		else
			GetShape(0).GetAnimCtrl().SetAnimation("idle");

		if(!GetCurrentTarget()->GetIsFacingLeft())
		{
			SetRotationY(D3DX_PI * 0.5f);
			SetIsFacingLeft(true);
		}
		else
		{
			SetIsFacingLeft(false);
			SetRotationY(-D3DX_PI * 0.5f);
		}

		//enum player_aim_dir {AIM_FORWARD = 0, AIM_UP, AIM_DOWN, AIM_ANGLE_UP, AIM_ANGLE_DOWN };

		if(((CPlayer*)GetCurrentTarget())->GetIsFiring())
		{
			m_bFlameOn = true;
			switch(((CPlayer*)GetCurrentTarget())->GetAimDir())
			{
			case AIM_FORWARD:
				{
					if(GetVelocity().x != 0.0f)
						GetShape(0).GetAnimCtrl().SetAnimation("runShootForward");
					else
						GetShape(0).GetAnimCtrl().SetAnimation("shootForward");
				}
				break;
			case AIM_UP:
				{
					if(GetVelocity().x != 0.0f)
						GetShape(0).GetAnimCtrl().SetAnimation("runShootUp");
					else
						GetShape(0).GetAnimCtrl().SetAnimation("shootUp");
				}
				break;
			case AIM_DOWN:
				{
					if(GetVelocity().x != 0.0f)
						GetShape(0).GetAnimCtrl().SetAnimation("runShootDown");
					else
						GetShape(0).GetAnimCtrl().SetAnimation("shootDown");
				}
				break;
			case AIM_ANGLE_UP:
				{
					if(GetVelocity().x != 0.0f)
						GetShape(0).GetAnimCtrl().SetAnimation("runShootDiagonalUp");
					else
						GetShape(0).GetAnimCtrl().SetAnimation("shootDiagonalUp");
				}
				break;
			case AIM_ANGLE_DOWN:
				{
					if(GetVelocity().x != 0.0f)
						GetShape(0).GetAnimCtrl().SetAnimation("shootDiagonalDown");
					else
						GetShape(0).GetAnimCtrl().SetAnimation("shootDiagonalDown");
				}
				break;
			}
			Shoot(((CPlayer*)GetCurrentTarget())->GetAimDir(),NORMAL_FIRE);
		}
		else
			m_bFlameOn = false;
	}
	else
	{
		Input(m_nPlayerNum);
		UpdateAnimation(fElapsedTime);
	}

	m_myHUD->SetUltScale(m_fUltGauge/100.0f);
	m_myHUD->SetAmmo(m_nAmmo);
	m_myHUD->SetWeaponType(m_pWeaponState->GetWeaponType());

	//stay in frustum
	int frustumTest = CCollisionTests::IntersectFrustrumSphere(m_Sphere.GetSphere(), CCamera::GetInstance()->GetFrustum());
	if( frustumTest == CT_INTERSECT ||
		frustumTest == CT_OUT)
	{
		D3DXVECTOR3 stayInFrustum;
		stayInFrustum = GetPosition() - CCamera::GetInstance()->GetViewPosition();
		stayInFrustum.y = 0.0;
		stayInFrustum.z = 0.0;
		D3DXVec3Normalize(&stayInFrustum, &stayInFrustum);
		SetPosition(GetPosition()-stayInFrustum*2);
	}

	//id effect
	D3DXMATRIX identityID;
	if(m_nPlayerNum == 1 && CFXManager::GetInstance()->GetEffect(m_nIDEffect)->GetFXType() == FX_P1_ID)
	{
		CFXManager::GetInstance()->KillEffect(m_nIDEffect);
		m_nIDEffect = CFXManager::GetInstance()->LoadEffect(FX_P2_ID, identityID, -1.0, true);
	}

	D3DXMATRIX pointer = *(GetShape(0).GetAnimCtrl().getMatrixByID(2)) * GetFrame().GetWorldMatrix();
	D3DXMatrixTranslation(&identityID,0.0f, sin(m_fLifeTime*8.0f)*5.0f+10.0f,0.0f);
	identityID._41 = pointer._41;
	identityID._42 += pointer._42;
	identityID._43 = pointer._43;
	CFXManager::GetInstance()->GetEffect(m_nIDEffect)->GetFrame().GetLocalMatrix() = identityID;

	m_fShowID -= fElapsedTime;
	if(m_fShowID < 0.0f)
	{
		CFXManager::GetInstance()->GetEffect(m_nIDEffect)->SetRenderType(RT_DONOTRENDER);
	}
	else
	{
		CFXManager::GetInstance()->GetEffect(m_nIDEffect)->SetRenderType(RT_TRANSPARENT);
	}

	//hats
	CFXManager::GetInstance()->GetEffect(m_nHatStandIn)->GetFrame().GetLocalMatrix() = *(GetShape(0).GetAnimCtrl().getMatrixByID(2)) * GetFrame().GetWorldMatrix();

	//shadow blob
	if(GetIsOnGround())
	{
		CFXManager::GetInstance()->GetEffect(m_nShadowEffect)->SetRenderType(RT_TRANSPARENT);
		D3DXVECTOR3 worldUp(0,1,0);
		float rot = acos(D3DXVec3Dot(&worldUp, &m_faceNormal));
		D3DXVECTOR3 cross;
		D3DXVec3Cross(&cross, &worldUp, &m_faceNormal);

		D3DXMATRIX rotMat;
		D3DXMatrixRotationAxis(&rotMat, &cross, rot);

		D3DXMATRIX newMat;
		D3DXMatrixTranslation(&newMat, GetFrame().GetWorldMatrix()._41, GetFrame().GetWorldMatrix()._42 + 1.0f,
			GetFrame().GetWorldMatrix()._43);

		CFXManager::GetInstance()->GetEffect(m_nShadowEffect)->GetFrame().GetLocalMatrix() =
			rotMat * newMat ;
	}
	else
	{
		CFXManager::GetInstance()->GetEffect(m_nShadowEffect)->SetRenderType(RT_DONOTRENDER);
	}

	//-----------------------------------------------------
	//gun holding
	// this should be temporary math

	D3DXMATRIX gunAimMat = *GetShape(0).GetAnimCtrl().getMatrixByID(1);

	GetShape(1).GetFrame().GetLocalMatrix() = gunAimMat;

	//pilot light
	D3DXMATRIX gunNozzle;
	D3DXMatrixTranslation(&gunNozzle, 0.0f, 5.0f, -17.0f);
	m_gunWorldPos = gunNozzle * gunAimMat * GetFrame().GetWorldMatrix();

	if(m_pWeaponState->GetWeaponType() == weapon_type::FLAMETHROWER && m_pPilotLightParticle == NULL)
	{
		D3DXMATRIX matIdentity;
		D3DXMatrixIdentity(&matIdentity);
		m_pPilotLightParticle =  CParticleManager::GetInstance()->SpawnParticle("Flamethrower_idle", matIdentity);
	}

	if(m_pWeaponState->GetWeaponType() == weapon_type::FLAMETHROWER && m_pPilotLightParticle != NULL && m_pInput->GetAction(ACT_SHOOT,m_nPlayerNum))
	{
		m_pPilotLightParticle->SetIsActive(false);
		m_pPilotLightParticle = NULL;
	}

	if(m_pWeaponState->GetWeaponType() != weapon_type::FLAMETHROWER && m_pPilotLightParticle != NULL)
	{
		m_pPilotLightParticle->SetIsActive(false);
		m_pPilotLightParticle = NULL;
	}

	if(m_pPilotLightParticle != NULL)
	{
		m_pPilotLightParticle->SetWorld(m_gunWorldPos);
	}

	lua_pop(luaState, 6);

	if(m_bLightningStorm)
	{
		m_fBoltTimer += fElapsedTime;
		if(m_fBoltTimer > m_fDelay)
		{
			CLightningUltimate* bullet = (CLightningUltimate*)CObjectFactory::GetInstance()->GetObject<CLightningUltimate>(OBJ_LIGHTNING_ULTIMATE);
			bullet->SetOwner(this);
			bullet->SetDamage(m_fStormDamage);
			bullet->SetVelocity(0.0f, -m_fBoltSpeed, 0.0f);
			bullet->SetIsActive(true);
			bullet->SetIsAffectedByGrav(false);
			bullet->SetType(OBJ_LIGHTNING_ULTIMATE);

			float xOffset = CCamera::GetInstance()->GetViewPosition().x + ((rand() % 360) - 180.0f);

			bullet->SetPosition(xOffset, 300.0f, GetPosition().z);
			bullet->SetRotationY(D3DXToRadian(90.0f));
			CObjectManager::GetInstance()->AddObject(bullet);

			m_fBoltTimer = 0.0f;
			m_fDelay -= (m_fDelay * m_fDelayDecay);

			++m_nBoltCount;

			if(m_nBoltCount >= m_nMaxBolts)
			{
				m_bLightningStorm = false;
				m_nBoltCount = 0;
			}
		}
	}
}

void CPlayer::Render()
{
	//CDebugSystem::GetInstance()->DisplayInfo();
	//CBase::Render();
	if(!m_bInvincible)
	{
		GetShape(0).SetTexID(GetShape(0).GetMesh()->GetTextureID());
	}
	else if(m_bInvincible && m_fBlink < 0.25 && !m_bRespawn)
	{
		GetShape(0).SetTexID(CLoadState::GetInstance()->GetImageID("Hit"));
	}
	else if(m_bInvincible && m_fBlink > 0.25 && !m_bRespawn)
	{
		GetShape(0).SetTexID(GetShape(0).GetMesh()->GetTextureID());

		if(m_fBlink > 0.50)
		{
			m_fBlink = 0.0f;
		}
	}
}

void CPlayer::AddScore(int score)
{
	m_nScore += (score * (int)m_fMult);

	if(m_myHUD)
	{
		m_myHUD->UpdateDials(score);
	}
}

void CPlayer::IncreaseMultiplier(float increase)
{
	m_fMult += increase;

	if(m_myHUD)
	{
		m_myHUD->IncreaseMultiplier();
	}
}

bool CPlayer::CheckCollision(CBase *pBase)
{
	return CBase::CheckCollision(pBase);
}

void CPlayer::CollisionReact(CBase *pBase)
{
	//The player will handle any collisions with the enemy, killing it and all that good stuff.
	//The player is also handling colliding with the ground
	//CBase::CollisionReact(pBase);
	switch(pBase->GetType())
	{
	case OBJ_STRIKER:
		{
			if(CGame::GetInstance()->GetGodLikeCheat())
				break;

			if(!m_bHit)
			{
				//m_nHit = CParticleManager::GetInstance()->SpawnParticle("blood",this->GetFrame().GetWorldMatrix());
			}
			m_bHit = true;

			if(CGameState::GetInstance()->GetEnemiesAlive() > 0)
			{
				CGameState::GetInstance()->SetEnemiesAlive(CGameState::GetInstance()->GetEnemiesAlive() - 1);
			}

			//Decrementing the lock
			if(((CStriker*)(pBase))->GetIsLockEnemy())
			{
				CGameState::GetInstance()->SetEnemyLockCount(CGameState::GetInstance()->GetEnemyLockCount() - 1);
			}

			if(!m_bInvincible && !((CStriker*)(pBase))->GetIsDead())
			{
				SetCurrentHealth(GetCurrentHealth() - 1);
				if(m_myHUD)
				{
					m_myHUD->SubtractHealth(1);
				}
				//CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_DX_3D_HURT, PLAYER);
				m_bAudioPlayHurt = true;
				D3DXMATRIX playerMat = GetFrame().GetWorldMatrix();
				playerMat._42 += 35.0f;
				CParticleManager::GetInstance()->SpawnParticle("Blood_splurt", playerMat);
				if(GetCurrentHealth() <= 0)
				{
					//CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_DX_3D_DEATH, PLAYER);
					m_bAudioPlayDie = true;
					SetCurrentHealth(0);
					if(GetNumLives() == 0)
					{
						//HEY I'M DEAD AND OUTTA' LIVES.
						char* buffer = "whatever";
						CEventSystem::GetInstance()->DispatchEvent(*m_eLoseEvent, NULL, 2.0f);
						SetIsActive(false);
					}
					else
					{
						SetNumLives(GetNumLives() - 1);
						SetCurrentHealth(3);
						m_bRespawn = true;
					}
				}

				m_bInvincible = true;
			}

			((CStriker*)(pBase))->SetIsDead(true);
			//SPAWN EXPLOSION AT THE LOCATION OF THE ENEMY
			//Play a sound probably.
			break;
		}
	case OBJ_ASSAULTER:
		{
			if(CGame::GetInstance()->GetGodLikeCheat())
				break;

			if(!((CAssaulter*)(pBase))->GetIsDead())
			{
				//Decrementing the lock
				if(((CAssaulter*)(pBase))->GetIsLockEnemy())
				{
					CGameState::GetInstance()->SetEnemyLockCount(CGameState::GetInstance()->GetEnemyLockCount() - 1);
				}
			}

			if(!m_bInvincible && !((CAssaulter*)(pBase))->GetIsDead())
			{
				SetCurrentHealth(GetCurrentHealth() - 1);
				if(m_myHUD)
				{
					m_myHUD->SubtractHealth(1);
				}
				//CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_DX_3D_HURT, PLAYER);
				m_bAudioPlayHurt = true;
				CParticleManager::GetInstance()->SpawnParticle("Blood_splurt", GetFrame().GetWorldMatrix());
				if(GetCurrentHealth() <= 0)
				{
					//CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_DX_3D_DEATH, PLAYER);
					m_bAudioPlayDie = true;
					SetCurrentHealth(0);
					if(GetNumLives() == 0)
					{
						CEventSystem::GetInstance()->DispatchEvent(*m_eLoseEvent, NULL, 2.0f);
						SetIsActive(false);
					}
					else
					{
						SetNumLives(GetNumLives() - 1);
						SetCurrentHealth(3);
						m_bRespawn = true;
					}
				}

				m_bInvincible = true;
			}
			((CAssaulter*)(pBase))->SetIsDead(true);
			break;
		}
	case OBJ_HELIOS:
		{
			if(CGame::GetInstance()->GetGodLikeCheat())
				break;

			if(!((CHelios*)(pBase))->GetIsDead())
			{
				//Decrementing the lock
				if(((CHelios*)(pBase))->GetIsLockEnemy())
				{
					CGameState::GetInstance()->SetEnemyLockCount(CGameState::GetInstance()->GetEnemyLockCount() - 1);
				}
			}

			if(!m_bInvincible && !((CHelios*)(pBase))->GetIsDead())
			{
				SetCurrentHealth(GetCurrentHealth() - 1);
				if(m_myHUD)
				{
					m_myHUD->SubtractHealth(1);
				}
				//CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_DX_3D_HURT, PLAYER);
				m_bAudioPlayHurt = true;
				CParticleManager::GetInstance()->SpawnParticle("Blood_splurt", GetFrame().GetWorldMatrix());
				if(GetCurrentHealth() <= 0)
				{
					//CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_DX_3D_DEATH, PLAYER);
					m_bAudioPlayDie = true;
					SetCurrentHealth(0);
					if(GetNumLives() == 0)
					{
						CEventSystem::GetInstance()->DispatchEvent(*m_eLoseEvent, NULL, 2.0f);
						SetIsActive(false);
					}
					else
					{
						SetNumLives(GetNumLives() - 1);
						SetCurrentHealth(3);
						m_bRespawn = true;
					}
				}

				m_bInvincible = true;
			}
			((CHelios*)(pBase))->SetIsDead(true);
			break;
		}
	case OBJ_ULTIMUS:
		{
			if(CGame::GetInstance()->GetGodLikeCheat())
				break;

			if(!m_bInvincible)
			{
				SetCurrentHealth(GetCurrentHealth() - 1);
				if(m_myHUD)
				{
					m_myHUD->SubtractHealth(1);
				}
				//CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_DX_3D_HURT, PLAYER);
				m_bAudioPlayHurt = true;
				CParticleManager::GetInstance()->SpawnParticle("Blood_splurt", GetFrame().GetWorldMatrix());
				if(GetCurrentHealth() <= 0)
				{
					//CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_DX_3D_DEATH, PLAYER);
					m_bAudioPlayDie = true;
					SetCurrentHealth(0);
					if(GetNumLives() == 0)
					{
						CEventSystem::GetInstance()->DispatchEvent(*m_eLoseEvent, NULL, 2.0f);
						SetIsActive(false);
					}
					else
					{
						SetNumLives(GetNumLives() - 1);
						SetCurrentHealth(3);
						m_bRespawn = true;
					}
				}

				m_bInvincible = true;
			}
			break;
		}
	case OBJ_FLAG:
		{
			char* buffer = "whatever";
			CEvent* test = CEventSystem::GetInstance()->FindEvent("WinEvent");
			CEventSystem::GetInstance()->DispatchEvent(*test, NULL, 2.0f);
			pBase->SetIsActive(false);
			break;
		}
	case OBJ_LASER_BULLET:
		{
			if(CGame::GetInstance()->GetGodLikeCheat())
				break;

			pBase->SetIsActive(false);
			if(!m_bInvincible)
			{
				SetCurrentHealth(GetCurrentHealth() - 1);
				if(m_myHUD)
				{
					m_myHUD->SubtractHealth(1);
				}
				//CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_DX_3D_HURT, PLAYER);
				m_bAudioPlayHurt = true;
				if(GetCurrentHealth() <= 0)
				{
					//CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_DX_3D_DEATH, PLAYER);
					m_bAudioPlayHurt = true;
					CParticleManager::GetInstance()->SpawnParticle("Blood_splurt", GetFrame().GetWorldMatrix());
					SetCurrentHealth(0);
					if(GetNumLives() == 0)
					{
						//HEY I'M DEAD AND OUTTA' LIVES.
						char* buffer = "whatever";
						CEventSystem::GetInstance()->DispatchEvent(*m_eLoseEvent, NULL, 2.0f);
						SetIsActive(false);
					}
					else
					{
						SetNumLives(GetNumLives() - 1);
						SetCurrentHealth(3);
						m_bRespawn = true;
					}
				}

				m_bInvincible = true;
			}
			break;
		}
	case OBJ_ROCKET_ALTERNATE:
		{
			CParticleManager::GetInstance()->SpawnParticle("Explosion2", GetFrame().GetWorldMatrix());
			if(CGame::GetInstance()->GetGodLikeCheat() || ((CRocketAlternate*)pBase)->GetOwner()->GetType() != OBJ_ULTIMUS)
				break;

			pBase->SetIsActive(false);
			if(!m_bInvincible)
			{
				SetCurrentHealth(GetCurrentHealth() - 1);
				if(m_myHUD)
				{
					m_myHUD->SubtractHealth(1);
				}
				//CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_DX_3D_HURT, PLAYER);
				m_bAudioPlayHurt = true;
				if(GetCurrentHealth() <= 0)
				{
					//CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_DX_3D_DEATH, PLAYER);
					m_bAudioPlayHurt = true;
					CParticleManager::GetInstance()->SpawnParticle("Blood_splurt", GetFrame().GetWorldMatrix());
					SetCurrentHealth(0);
					if(GetNumLives() == 0)
					{
						//HEY I'M DEAD AND OUTTA' LIVES.
						char* buffer = "whatever";
						CEventSystem::GetInstance()->DispatchEvent(*m_eLoseEvent, NULL, 2.0f);
						SetIsActive(false);
					}
					else
					{
						SetNumLives(GetNumLives() - 1);
						SetCurrentHealth(3);
						m_bRespawn = true;
					}
				}

				m_bInvincible = true;
			}
			break;
		}
	case OBJ_SMARTLASER:
		{
			if(CGame::GetInstance()->GetGodLikeCheat())
				break;

			pBase->SetIsActive(false);
			if(!m_bInvincible)
			{
				SetCurrentHealth(GetCurrentHealth() - 1);
				if(m_myHUD)
				{
					m_myHUD->SubtractHealth(1);
				}
				//CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_DX_3D_HURT, PLAYER);
				m_bAudioPlayHurt = true;
				CParticleManager::GetInstance()->SpawnParticle("Blood_splurt", GetFrame().GetWorldMatrix());
				if(GetCurrentHealth() <= 0)
				{
					//CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_DX_3D_DEATH, PLAYER);
					m_bAudioPlayHurt = true;
					SetCurrentHealth(0);
					if(GetNumLives() == 0)
					{
						//HEY I'M DEAD AND OUTTA' LIVES.
						char* buffer = "whatever";
						CEventSystem::GetInstance()->DispatchEvent(*m_eLoseEvent, NULL, 2.0f);
						SetIsActive(false);
					}
					else
					{
						SetNumLives(GetNumLives() - 1);
						SetCurrentHealth(3);
						m_bRespawn = true;
					}
				}

				m_bInvincible = true;
			}
			break;
		}
	case OBJ_CLASER:
		{
			if(CGame::GetInstance()->GetGodLikeCheat())
				break;

			//pBase->SetIsActive(false);
			if(!m_bInvincible)
			{
				SetCurrentHealth(GetCurrentHealth() - 1);
				if(m_myHUD)
				{
					m_myHUD->SubtractHealth(1);
				}
				//CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_DX_3D_HURT, PLAYER);
				m_bAudioPlayHurt = true;
				CParticleManager::GetInstance()->SpawnParticle("Blood_splurt", GetFrame().GetWorldMatrix());
				if(GetCurrentHealth() <= 0)
				{
					//CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_DX_3D_DEATH, PLAYER);
					m_bAudioPlayHurt = true;
					SetCurrentHealth(0);
					if(GetNumLives() == 0)
					{
						//HEY I'M DEAD AND OUTTA' LIVES.
						char* buffer = "whatever";
						CEventSystem::GetInstance()->DispatchEvent(*m_eLoseEvent, NULL, 2.0f);
						SetIsActive(false);
					}
					else
					{
						SetNumLives(GetNumLives() - 1);
						SetCurrentHealth(3);
						m_bRespawn = true;
					}
				}

				m_bInvincible = true;
			}
			break;
		}
	case OBJ_PICKUP:
		{
			//CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_2D_HEALTHPICKUP, PLAYER);
			break;
		}
	case OBJ_SHOTGUN_PICKUP:
		{
			//m_bAudioPlayPickUp = true;
			m_bAudioPlayTransSGun = true;
			AddScore(500);
			SwitchWeapon(((CPickUp*)(pBase))->GetWeaponState());
			pBase->SetIsActive(false);
			break;
		}
	case OBJ_FLAMETHROWER_PICKUP:
		{
			//m_bAudioPlayPickUp = true;
			m_bAudioPlayTransFGun = true;
			AddScore(500);
			SwitchWeapon(((CPickUp*)(pBase))->GetWeaponState());
			pBase->SetIsActive(false);
			break;
		}
	case OBJ_LIGHTNING_PICKUP:
		{
			m_bAudioPlayTransLGun = true;
			AddScore(500);
			SwitchWeapon(((CPickUp*)(pBase))->GetWeaponState());
			pBase->SetIsActive(false);
			break;
		}
	case OBJ_ROCKET_PICKUP:
		{
			m_bAudioPlayTransRGun = true;
			AddScore(500);
			SwitchWeapon(((CPickUp*)(pBase))->GetWeaponState());
			pBase->SetIsActive(false);
			break;
		}
	case OBJ_SPLIT:
		{
			CSpawner::GetInstance()->SetTrickle(false);

			CObjectManager::GetInstance()->DestroyAllGameObjects();

			char* buffer = new char[sizeof(unsigned int)];
			*((unsigned int*)(buffer)) = ((CSplitObject*)pBase)->m_goalID;
			CEventSystem::GetInstance()->DispatchEvent(*m_eSplitEvent, buffer, 1.0f);

			break;
		}
	case OBJ_MERGE:
		{
			CSpawner::GetInstance()->SetTrickle(false);

			CObjectManager::GetInstance()->DestroyAllGameObjects();

			char* buffer = new char[sizeof(unsigned int)];
			*((unsigned int*)(buffer)) = ((CMergeObject*)pBase)->m_goalID;
			CEventSystem::GetInstance()->DispatchEvent(*m_eMergeEvent, buffer, 1.0f);

			break;
		}
	case OBJ_DEBRIS:
		{
			if(CGame::GetInstance()->GetGodLikeCheat())
				break;

			if(!m_bInvincible)
			{
				SetCurrentHealth(GetCurrentHealth() - 1);
				if(m_myHUD)
				{
					m_myHUD->SubtractHealth(1);
				}
				//CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_DX_3D_HURT, PLAYER);
				m_bAudioPlayHurt = true;
				if(GetCurrentHealth() <= 0)
				{
					//CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_DX_3D_DEATH, PLAYER);
					m_bAudioPlayHurt = true;
					SetCurrentHealth(0);
					if(GetNumLives() == 0)
					{
						//HEY I'M DEAD AND OUTTA' LIVES.
						char* buffer = "whatever";
						CEventSystem::GetInstance()->DispatchEvent(*m_eLoseEvent, NULL, 2.0f);
						SetIsActive(false);
					}
					else
					{
						SetNumLives(GetNumLives() - 1);
						SetCurrentHealth(3);
						m_bRespawn = true;
					}
				}

				m_bInvincible = true;
			}
			break;
		}
	case OBJ_AUDIO_TRIGGER:
		{
			switch(CGameState::GetInstance()->GetTriggerCount())
			{
			case 0:
				{
					CWwiseManager::GetInstance()->SetSwitch(AK::SWITCHES::AMBIENCE::GROUP, AK::SWITCHES::AMBIENCE::SWITCH::JUNGLE, 100);
					CGameState::GetInstance()->SetTriggerCount(1);
					CGameState::GetInstance()->SetLevelArea(JUNGLE);
					pBase->SetIsActive(false);
					break;
				}
			case 1:
				{
					CWwiseManager::GetInstance()->SetSwitch(AK::SWITCHES::AMBIENCE::GROUP, AK::SWITCHES::AMBIENCE::SWITCH::TRAIN, 100);
					CGameState::GetInstance()->SetTriggerCount(2);
					CGameState::GetInstance()->SetLevelArea(TRAIN_YARD);
					pBase->SetIsActive(false);
					break;
				}
			case 2:
				{
					CWwiseManager::GetInstance()->SetSwitch(AK::SWITCHES::AMBIENCE::GROUP, AK::SWITCHES::AMBIENCE::SWITCH::TRAIN, 100);
					CGameState::GetInstance()->SetTriggerCount(3);
					pBase->SetIsActive(false);
					break;
				}
			};
			break;
		}
	case OBJ_BOSSBODY:
		{
			float mathstuff = GetBVAABB().GetAABB().maxPt.x - pBase->GetBVAABB().GetAABB().minPt.x;
			SetPosition(GetPosition().x - mathstuff,GetPosition().y ,GetPosition().z );
			if(!m_bInvincible )
			{
				SetCurrentHealth(GetCurrentHealth() - 1);
				if(m_myHUD)
				{
					m_myHUD->SubtractHealth(1);
				}
				m_bAudioPlayHurt = true;
				D3DXMATRIX playerMat = GetFrame().GetWorldMatrix();
				playerMat._42 += 35.0f;
				CParticleManager::GetInstance()->SpawnParticle("Blood_splurt", playerMat);
				if(GetCurrentHealth() <= 0)
				{
					m_bAudioPlayDie = true;
					SetCurrentHealth(0);
					if(GetNumLives() == 0)
					{
						CEventSystem::GetInstance()->DispatchEvent(*m_eLoseEvent, NULL, 2.0f);
						SetIsActive(false);
					}
					else
					{
						SetNumLives(GetNumLives() - 1);
						SetCurrentHealth(3);
						m_bRespawn = true;
					}
				}

				m_bInvincible = true;
			}
		}
		break;
	}
}

void CPlayer::Register()
{
	CEventSystem::GetInstance()->RegisterForEvent(*this, *m_eLoseEvent, 2.0f);
	CEventSystem::GetInstance()->RegisterForEvent(*this, *m_eWinEvent,2.0f);
	CEventSystem::GetInstance()->RegisterForEvent(*this, *m_eSplitEvent,1.0f);
	CEventSystem::GetInstance()->RegisterForEvent(*this, *m_eMergeEvent,1.0f);
	CEventSystem::GetInstance()->RegisterForEvent(*this, *m_eShowPlayerID,1.0f);
}

void CPlayer::Deregister()
{
	CEventSystem::GetInstance()->DeregisterForEvent(*this, *m_eLoseEvent);
	CEventSystem::GetInstance()->DeregisterForEvent(*this, *m_eWinEvent);
	CEventSystem::GetInstance()->DeregisterForEvent(*this, *m_eSplitEvent);
	CEventSystem::GetInstance()->DeregisterForEvent(*this, *m_eMergeEvent);
	CEventSystem::GetInstance()->DeregisterForEvent(*this, *m_eShowPlayerID);
}

void CPlayer::React(const tDispatch& toReactTo)
{
	char buffer[16];
	strcpy_s(buffer, 16, (((tDispatch*)(&toReactTo))->m_pEvent->GetEventName()));

	if(strcmp(buffer, "LoseEvent") == 0)
	{
	}

	if(strcmp(buffer, "SplitEvent") == 0)
	{
		CGameState::GetInstance()->SetPlayState(SPLIT);
		m_goal = CObjectManager::GetInstance()->GetGoalObject(*((unsigned int*)(toReactTo.m_buffer)),m_nPlayerNum);
		m_fShowID = 5.0f;
	}

	if(strcmp(buffer, "MergeEvent") == 0)
	{
		CGameState::GetInstance()->SetPlayState(LOCKED);
		m_goal = CObjectManager::GetInstance()->GetGoalObject(*((unsigned int*)(toReactTo.m_buffer)),m_nPlayerNum);
		m_fShowID = 5.0f;
	}

	if(strcmp(buffer, "ShowPlayerID") == 0)
	{
		m_fShowID = 5.0f;
	}
}
void CPlayer::Jump()
{
	const float fJumpStr = 4.0f;

	SetVelocity(D3DXVECTOR3(GetVelocity().x, fJumpStr, GetVelocity().z));

	SetIsJumping(true);
}

void CPlayer::Respawn(void)
{
	SwitchWeapon(CMachineGunState::GetInstance());
	SetPosition(D3DXVECTOR3(GetPosition().x-25.0f,GetPosition().y +100.0f,GetPosition().z));
	SetIsOnGround(false);
	m_fUltGauge = 0.0f;
	m_myHUD->SetUltScale(m_fUltGauge/100.0f);
	m_myHUD->ResetMultiplyer();
	m_bUltReady = false;
	m_fMult = 1.0;
	m_fShowID = 5.0f;
}

void CPlayer::Shoot(int nAngle, int nTypeFire)
{
	if(m_bIsDashing == false)
	{
		if(readyToFire && nTypeFire == NORMAL_FIRE)
		{
			state = SHOOT;
			readyToFire = false;
			fireTimer = 0.0f;

			D3DXVECTOR3 irrelevant(42.0f,42.0f,42.0f);

			m_pWeaponState->Fire(this, irrelevant, GetIsFacingLeft(), nAngle, nTypeFire);
			//Only deducting ammo if this is NOT the machine gun we're using.
			if(m_nAmmo != -100 && !CGame::GetInstance()->GetUnlimtedAmmoCheat())
				--m_nAmmo;
		}

		if(m_nAmmo != -100) //Can only fire an alt if we're not using the machine gun
		{
			if(m_bAltReady && nTypeFire == ALTERNATE_FIRE)
			{
				state = SHOOT;
				m_bAltReady = false;
				m_fAltTimer = 0.0f;

				D3DXVECTOR3 irrelevant(42.0f,42.0f,42.0f);

				m_pWeaponState->Fire(this, irrelevant, GetIsFacingLeft(), nAngle, nTypeFire);
				if(!CGame::GetInstance()->GetUnlimtedAmmoCheat())
					m_nAmmo -= m_pWeaponState->GetAlternateCost();
			}
		}
	}
}

void CPlayer::SwitchWeapon(IBaseWeaponState* state)
{
	m_pWeaponState = state;
	m_nAmmo = m_pWeaponState->GetMaxAmmo();
	m_bFlameOn = false;

	//D3DXMATRIX gunMat, gunTrans, gunScale, gunRot;
	//D3DXMatrixTranslation(&gunTrans, -5.0f, 15.0f, -2.0f);
	//D3DXMatrixScaling(&gunScale, 3.0f, 3.0f, 3.0f);
	//gunMat =  gunScale* gunTrans;

	if(state->GetWeaponType() != MACHINE_GUN)
	{
		GetShape(1).SetMesh(m_pWeaponState->GetMesh());
	}
	else
	{
		if(GetType() == OBJ_BARRON)
		{
			GetShape(1).SetMesh(&CModelManager::GetInstance()->GetModel(CLoadState::GetInstance()->GetModelID("BaronsGunShape"))->GetMesh());
		}
		else
		{
			GetShape(1).SetMesh(&CModelManager::GetInstance()->GetModel(CLoadState::GetInstance()->GetModelID("ClairesGunShape"))->GetMesh());
		}
	}
}

void CPlayer::InitializeAI()
{
	m_bAI = true;
	//m_AIsm.Initialize(this,CObjectManager::GetInstance()->GetEnemyList());
}
void CPlayer::UpdateAnimation(float fElapsedTime)
{
	m_nPreviousAnimation = m_nCurrentAnimation;
	//animation outline
	if(!m_bRespawn)
	{
		//GetShape(0).GetAnimCtrl().SetAnimation("idle");
		m_nCurrentAnimation = 1;

		if(m_bIsDashing)
		{
			//GetShape(0).GetAnimCtrl().SetAnimation("dashForward");
			m_nCurrentAnimation = 16;
		}
		else if(!GetIsJumping())
		{
			if(m_bIsProne)
			{
				//GetShape(0).GetAnimCtrl().SetAnimation("prone");
				m_nCurrentAnimation = 2;
				if(m_pInput->GetAction(ACT_SHOOT,m_nPlayerNum))
				{
					//GetShape(0).GetAnimCtrl().SetAnimation("prone");
					m_nCurrentAnimation = 3;
				}
			}
			else if(m_bIsDownJumping)
			{
				//GetShape(0).GetAnimCtrl().SetAnimation("burrow");
				m_nCurrentAnimation = 4;
			}
			else if(m_vVelocity.x != 0.0f)
			{
				//moving
				//GetShape(0).GetAnimCtrl().SetAnimation("walk");
				m_nCurrentAnimation = 5;
				//CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_3D_FOOTSTEPS, PLAYER);
				m_bAudioPlayWalk = true;
				if(m_pInput->GetAction(ACT_SHOOT,m_nPlayerNum))
				{
					//GetShape(0).GetAnimCtrl().SetAnimation("runShootForward");
					m_nCurrentAnimation = 6;

					//shooting
					//5 directions
					if(
						(m_pInput->GetAction(ACT_HOLDUP,m_nPlayerNum) ) ||
						(m_pInput->GetActionAmount(ACT_MOVEY,m_nPlayerNum) > 22500)
						)
					{
						//GetShape(0).GetAnimCtrl().SetAnimation("runShootUp");
						m_nCurrentAnimation = 7;
					}

					if(
						(m_pInput->GetAction(ACT_HOLDDOWN,m_nPlayerNum) ) ||
						(m_pInput->GetActionAmount(ACT_MOVEY,m_nPlayerNum) < -22500)
						)
					{
						// this one is up for debate
						//GetShape(0).GetAnimCtrl().SetAnimation("runShootDown");
						m_nCurrentAnimation = 8;
					}

					if(
						(m_pInput->GetAction(ACT_HOLDUP,m_nPlayerNum) && (m_pInput->GetAction(ACT_HOLDRIGHT,m_nPlayerNum) || m_pInput->GetAction(ACT_HOLDLEFT,m_nPlayerNum) ) ) ||
						(m_pInput->GetActionAmount(ACT_MOVEY,m_nPlayerNum) < 22500 && m_pInput->GetActionAmount(ACT_MOVEY,m_nPlayerNum) > 7000)
						)
					{
						//GetShape(0).GetAnimCtrl().SetAnimation("runShootDiagonalUp");
						m_nCurrentAnimation = 9;
					}

					if(
						(m_pInput->GetAction(ACT_HOLDDOWN,m_nPlayerNum) && (m_pInput->GetAction(ACT_HOLDRIGHT,m_nPlayerNum) || m_pInput->GetAction(ACT_HOLDLEFT,m_nPlayerNum) ) ) ||
						(m_pInput->GetActionAmount(ACT_MOVEY,m_nPlayerNum) > -22500 && m_pInput->GetActionAmount(ACT_MOVEY,m_nPlayerNum) < -7000)
						)
					{
						//GetShape(0).GetAnimCtrl().SetAnimation("runShootDiagonalDown");
						m_nCurrentAnimation = 10;
					}
				}
				else
				{
					//aiming
					//5 directions
					//GetShape(0).GetAnimCtrl().SetAnimation("runAimForward");
					m_nCurrentAnimation = 11;

					if(
						(m_pInput->GetAction(ACT_HOLDUP,m_nPlayerNum) ) ||
						(m_pInput->GetActionAmount(ACT_MOVEY,m_nPlayerNum) > 22500)
						)
					{
						//GetShape(0).GetAnimCtrl().SetAnimation("runAimUp");
						m_nCurrentAnimation = 12;
					}

					if(
						(m_pInput->GetAction(ACT_HOLDDOWN,m_nPlayerNum) ) ||
						(m_pInput->GetActionAmount(ACT_MOVEY,m_nPlayerNum) < -22500)
						)
					{
						// this one is up for debate
						//GetShape(0).GetAnimCtrl().SetAnimation("runAimDown");
						m_nCurrentAnimation = 13;
					}

					if(
						(m_pInput->GetAction(ACT_HOLDUP,m_nPlayerNum) && (m_pInput->GetAction(ACT_HOLDRIGHT,m_nPlayerNum) || m_pInput->GetAction(ACT_HOLDLEFT,m_nPlayerNum) ) ) ||
						(m_pInput->GetActionAmount(ACT_MOVEY,m_nPlayerNum) < 22500 && m_pInput->GetActionAmount(ACT_MOVEY,m_nPlayerNum) > 7000)
						)
					{
						//GetShape(0).GetAnimCtrl().SetAnimation("runAimDiagonalUp");
						m_nCurrentAnimation = 14;
					}

					if(
						(m_pInput->GetAction(ACT_HOLDDOWN,m_nPlayerNum) && (m_pInput->GetAction(ACT_HOLDRIGHT,m_nPlayerNum) || m_pInput->GetAction(ACT_HOLDLEFT,m_nPlayerNum) ) ) ||
						(m_pInput->GetActionAmount(ACT_MOVEY,m_nPlayerNum) > -22500 && m_pInput->GetActionAmount(ACT_MOVEY,m_nPlayerNum) < -7000)
						)
					{
						//GetShape(0).GetAnimCtrl().SetAnimation("runAimDiagonalDown");
						m_nCurrentAnimation = 15;
					}
				}
			}
			else
			{
				//standing
				if(m_pInput->GetAction(ACT_SHOOT,m_nPlayerNum))
				{
					//GetShape(0).GetAnimCtrl().SetAnimation("shootForward");
					m_nCurrentAnimation = 17;

					//shooting
					//5 directions
					if(
						(m_pInput->GetAction(ACT_HOLDUP,m_nPlayerNum) ) ||
						(m_pInput->GetActionAmount(ACT_MOVEY,m_nPlayerNum) > 22500)
						)
					{
						//GetShape(0).GetAnimCtrl().SetAnimation("shootUp");
						m_nCurrentAnimation = 18;
					}

					if(
						(m_pInput->GetAction(ACT_HOLDDOWN,m_nPlayerNum) ) ||
						(m_pInput->GetActionAmount(ACT_MOVEY,m_nPlayerNum) < -22500)
						)
					{
						// this one is up for debate
						//GetShape(0).GetAnimCtrl().SetAnimation("shootDown");
						m_nCurrentAnimation = 19;
					}

					if(
						(m_pInput->GetAction(ACT_HOLDUP,m_nPlayerNum) && (m_pInput->GetAction(ACT_HOLDRIGHT,m_nPlayerNum) || m_pInput->GetAction(ACT_HOLDLEFT,m_nPlayerNum) ) ) ||
						(m_pInput->GetActionAmount(ACT_MOVEY,m_nPlayerNum) < 22500 && m_pInput->GetActionAmount(ACT_MOVEY,m_nPlayerNum) > 7000)
						)
					{
						//GetShape(0).GetAnimCtrl().SetAnimation("shootDiagonalUp");
						m_nCurrentAnimation = 20;
					}

					if(
						(m_pInput->GetAction(ACT_HOLDDOWN,m_nPlayerNum) && (m_pInput->GetAction(ACT_HOLDRIGHT,m_nPlayerNum) || m_pInput->GetAction(ACT_HOLDLEFT,m_nPlayerNum) ) ) ||
						(m_pInput->GetActionAmount(ACT_MOVEY,m_nPlayerNum) > -22500 && m_pInput->GetActionAmount(ACT_MOVEY,m_nPlayerNum) < -7000)
						)
					{
						//GetShape(0).GetAnimCtrl().SetAnimation("shootDiagonalDown");
						m_nCurrentAnimation = 21;
					}
				}
				else
				{
					//aiming
					//4 directions
					//aimForward is idle
					if(
						(m_pInput->GetAction(ACT_HOLDUP,m_nPlayerNum) ) ||
						(m_pInput->GetActionAmount(ACT_MOVEY,m_nPlayerNum) > 22500)
						)
					{
						//GetShape(0).GetAnimCtrl().SetAnimation("aimUp");
						m_nCurrentAnimation = 22;
					}

					if(
						(m_pInput->GetAction(ACT_HOLDDOWN,m_nPlayerNum) ) ||
						(m_pInput->GetActionAmount(ACT_MOVEY,m_nPlayerNum) < -22500)
						)
					{
						// this one is up for debate
						//GetShape(0).GetAnimCtrl().SetAnimation("prone");
						m_nCurrentAnimation = 23;
					}

					if(
						(m_pInput->GetAction(ACT_HOLDUP,m_nPlayerNum) && (m_pInput->GetAction(ACT_HOLDRIGHT,m_nPlayerNum) || m_pInput->GetAction(ACT_HOLDLEFT,m_nPlayerNum) ) ) ||
						(m_pInput->GetActionAmount(ACT_MOVEY,m_nPlayerNum) < 22500 && m_pInput->GetActionAmount(ACT_MOVEY,m_nPlayerNum) > 7000)
						)
					{
						//GetShape(0).GetAnimCtrl().SetAnimation("aimDiagonalUp");
						m_nCurrentAnimation = 24;
					}

					if(
						(m_pInput->GetAction(ACT_HOLDDOWN,m_nPlayerNum) && (m_pInput->GetAction(ACT_HOLDRIGHT,m_nPlayerNum) || m_pInput->GetAction(ACT_HOLDLEFT,m_nPlayerNum) ) ) ||
						(m_pInput->GetActionAmount(ACT_MOVEY,m_nPlayerNum) > -22500 && m_pInput->GetActionAmount(ACT_MOVEY,m_nPlayerNum) < -7000)
						)
					{
						//GetShape(0).GetAnimCtrl().SetAnimation("aimDiagonalDown");
						m_nCurrentAnimation = 25;
					}
				}
			}
		}
		else if(m_vVelocity.y > 0.0f)
		{
			//rising
			if(m_pInput->GetAction(ACT_SHOOT,m_nPlayerNum))
			{
				//GetShape(0).GetAnimCtrl().SetAnimation("jumpShootForward");
				m_nCurrentAnimation = 26;

				//shooting
				//5 directions
				if(
					(m_pInput->GetAction(ACT_HOLDUP,m_nPlayerNum) ) ||
					(m_pInput->GetActionAmount(ACT_MOVEY,m_nPlayerNum) > 22500)
					)
				{
					//GetShape(0).GetAnimCtrl().SetAnimation("jumpShootUp");
					m_nCurrentAnimation = 27;
				}

				if(
					(m_pInput->GetAction(ACT_HOLDDOWN,m_nPlayerNum) ) ||
					(m_pInput->GetActionAmount(ACT_MOVEY,m_nPlayerNum) < -22500)
					)
				{
					// this one is up for debate
					//GetShape(0).GetAnimCtrl().SetAnimation("jumpShootDown");
					m_nCurrentAnimation = 28;
				}

				if(
					(m_pInput->GetAction(ACT_HOLDUP,m_nPlayerNum) && (m_pInput->GetAction(ACT_HOLDRIGHT,m_nPlayerNum) || m_pInput->GetAction(ACT_HOLDLEFT,m_nPlayerNum) ) ) ||
					(m_pInput->GetActionAmount(ACT_MOVEY,m_nPlayerNum) < 22500 && m_pInput->GetActionAmount(ACT_MOVEY,m_nPlayerNum) > 7000)
					)
				{
					//GetShape(0).GetAnimCtrl().SetAnimation("jumpShootDiagonalUp");
					m_nCurrentAnimation = 29;
				}

				if(
					(m_pInput->GetAction(ACT_HOLDDOWN,m_nPlayerNum) && (m_pInput->GetAction(ACT_HOLDRIGHT,m_nPlayerNum) || m_pInput->GetAction(ACT_HOLDLEFT,m_nPlayerNum) ) ) ||
					(m_pInput->GetActionAmount(ACT_MOVEY,m_nPlayerNum) > -22500 && m_pInput->GetActionAmount(ACT_MOVEY,m_nPlayerNum) < -7000)
					)
				{
					//GetShape(0).GetAnimCtrl().SetAnimation("jumpShootDiagonalDown");
					m_nCurrentAnimation = 30;
				}
			}
			else
			{
				//aiming
				//5 directions
				//GetShape(0).GetAnimCtrl().SetAnimation("jumpAimForward");
				m_nCurrentAnimation = 31;

				if(
					(m_pInput->GetAction(ACT_HOLDUP,m_nPlayerNum) ) ||
					(m_pInput->GetActionAmount(ACT_MOVEY,m_nPlayerNum) > 22500)
					)
				{
					//GetShape(0).GetAnimCtrl().SetAnimation("jumpAimUp");
					m_nCurrentAnimation = 32;
				}

				if(
					(m_pInput->GetAction(ACT_HOLDDOWN,m_nPlayerNum) ) ||
					(m_pInput->GetActionAmount(ACT_MOVEY,m_nPlayerNum) < -22500)
					)
				{
					// this one is up for debate
					//GetShape(0).GetAnimCtrl().SetAnimation("jumpAimDown");
					m_nCurrentAnimation = 33;
				}

				if(
					(m_pInput->GetAction(ACT_HOLDUP,m_nPlayerNum) && (m_pInput->GetAction(ACT_HOLDRIGHT,m_nPlayerNum) || m_pInput->GetAction(ACT_HOLDLEFT,m_nPlayerNum) ) ) ||
					(m_pInput->GetActionAmount(ACT_MOVEY,m_nPlayerNum) < 22500 && m_pInput->GetActionAmount(ACT_MOVEY,m_nPlayerNum) > 7000)
					)
				{
					//GetShape(0).GetAnimCtrl().SetAnimation("jumpAimDiagonalUp");
					m_nCurrentAnimation = 34;
				}

				if(
					(m_pInput->GetAction(ACT_HOLDDOWN,m_nPlayerNum) && (m_pInput->GetAction(ACT_HOLDRIGHT,m_nPlayerNum) || m_pInput->GetAction(ACT_HOLDLEFT,m_nPlayerNum) ) ) ||
					(m_pInput->GetActionAmount(ACT_MOVEY,m_nPlayerNum) > -22500 && m_pInput->GetActionAmount(ACT_MOVEY,m_nPlayerNum) < -7000)
					)
				{
					//GetShape(0).GetAnimCtrl().SetAnimation("jumpAimDiagonalDown");
					m_nCurrentAnimation = 35;
				}
			}
		}
		else if(m_vVelocity.y < 0.0f)
		{
			//falling
			if(m_pInput->GetAction(ACT_SHOOT,m_nPlayerNum))
			{
				//GetShape(0).GetAnimCtrl().SetAnimation("fallShootForward");
				m_nCurrentAnimation = 36;

				//shooting
				//5 directions
				if(
					(m_pInput->GetAction(ACT_HOLDUP,m_nPlayerNum) ) ||
					(m_pInput->GetActionAmount(ACT_MOVEY,m_nPlayerNum) > 22500)
					)
				{
					//GetShape(0).GetAnimCtrl().SetAnimation("fallShootUp");
					m_nCurrentAnimation = 37;
				}

				if(
					(m_pInput->GetAction(ACT_HOLDDOWN,m_nPlayerNum) ) ||
					(m_pInput->GetActionAmount(ACT_MOVEY,m_nPlayerNum) < -22500)
					)
				{
					// this one is up for debate
					//GetShape(0).GetAnimCtrl().SetAnimation("fallShootDown");
					m_nCurrentAnimation = 38;
				}

				if(
					(m_pInput->GetAction(ACT_HOLDUP,m_nPlayerNum) && (m_pInput->GetAction(ACT_HOLDRIGHT,m_nPlayerNum) || m_pInput->GetAction(ACT_HOLDLEFT,m_nPlayerNum) ) ) ||
					(m_pInput->GetActionAmount(ACT_MOVEY,m_nPlayerNum) < 22500 && m_pInput->GetActionAmount(ACT_MOVEY,m_nPlayerNum) > 7000)
					)
				{
					//GetShape(0).GetAnimCtrl().SetAnimation("fallShootDiagonalUp");
					m_nCurrentAnimation = 39;
				}

				if(
					(m_pInput->GetAction(ACT_HOLDDOWN,m_nPlayerNum) && (m_pInput->GetAction(ACT_HOLDRIGHT,m_nPlayerNum) || m_pInput->GetAction(ACT_HOLDLEFT,m_nPlayerNum) ) ) ||
					(m_pInput->GetActionAmount(ACT_MOVEY,m_nPlayerNum) > -22500 && m_pInput->GetActionAmount(ACT_MOVEY,m_nPlayerNum) < -7000)
					)
				{
					//GetShape(0).GetAnimCtrl().SetAnimation("fallShootDiagonalDown");
					m_nCurrentAnimation = 40;
				}
			}
			else
			{
				//aiming
				//5 directions
				//GetShape(0).GetAnimCtrl().SetAnimation("fallAimForward");
				m_nCurrentAnimation = 41;

				if(
					(m_pInput->GetAction(ACT_HOLDUP,m_nPlayerNum) ) ||
					(m_pInput->GetActionAmount(ACT_MOVEY,m_nPlayerNum) > 22500)
					)
				{
					//GetShape(0).GetAnimCtrl().SetAnimation("fallAimUp");
					m_nCurrentAnimation = 42;
				}

				if(
					(m_pInput->GetAction(ACT_HOLDDOWN,m_nPlayerNum) ) ||
					(m_pInput->GetActionAmount(ACT_MOVEY,m_nPlayerNum) < -22500)
					)
				{
					// this one is up for debate
					//GetShape(0).GetAnimCtrl().SetAnimation("fallAimDown");
					m_nCurrentAnimation = 43;
				}

				if(
					(m_pInput->GetAction(ACT_HOLDUP,m_nPlayerNum) && (m_pInput->GetAction(ACT_HOLDRIGHT,m_nPlayerNum) || m_pInput->GetAction(ACT_HOLDLEFT,m_nPlayerNum) ) ) ||
					(m_pInput->GetActionAmount(ACT_MOVEY,m_nPlayerNum) < 22500 && m_pInput->GetActionAmount(ACT_MOVEY,m_nPlayerNum) > 7000)
					)
				{
					//GetShape(0).GetAnimCtrl().SetAnimation("fallAimDiagonalUp");
					m_nCurrentAnimation = 44;
				}

				if(
					(m_pInput->GetAction(ACT_HOLDDOWN,m_nPlayerNum) && (m_pInput->GetAction(ACT_HOLDRIGHT,m_nPlayerNum) || m_pInput->GetAction(ACT_HOLDLEFT,m_nPlayerNum) ) ) ||
					(m_pInput->GetActionAmount(ACT_MOVEY,m_nPlayerNum) > -22500 && m_pInput->GetActionAmount(ACT_MOVEY,m_nPlayerNum) < -7000)
					)
				{
					//GetShape(0).GetAnimCtrl().SetAnimation("fallAimDiagonalDown");
					m_nCurrentAnimation = 45;
				}
			}
		}
	}
	else
	{
		//death
		//GetShape(0).GetAnimCtrl().SetAnimation("death");
		m_nCurrentAnimation = 46;
	}

	if(m_nPreviousAnimation != m_nCurrentAnimation)
	{
		switch(m_nCurrentAnimation)
		{
		case 1:
			GetShape(0).GetAnimCtrl().BlendAnimation("idle", 0.1f);
			break;
		case 2:
			GetShape(0).GetAnimCtrl().BlendAnimation("prone", 0.1f);
			break;
		case 3:
			GetShape(0).GetAnimCtrl().BlendAnimation("prone", 0.1f);
			break;
		case 4:
			GetShape(0).GetAnimCtrl().BlendAnimation("burrow", 0.1f);
			break;
		case 5:
			GetShape(0).GetAnimCtrl().BlendAnimation("walk", 0.1f);
			break;
		case 6:
			GetShape(0).GetAnimCtrl().BlendAnimation("runShootForward", 0.1f);
			break;
		case 7:
			GetShape(0).GetAnimCtrl().BlendAnimation("runShootUp", 0.1f);
			break;
		case 8:
			GetShape(0).GetAnimCtrl().BlendAnimation("runShootDown", 0.1f);
			break;
		case 9:
			GetShape(0).GetAnimCtrl().BlendAnimation("runShootDiagonalUp", 0.1f);
			break;
		case 10:
			GetShape(0).GetAnimCtrl().BlendAnimation("runShootDiagonalDown", 0.1f);
			break;
		case 11:
			GetShape(0).GetAnimCtrl().BlendAnimation("runAimForward", 0.1f);
			break;
		case 12:
			GetShape(0).GetAnimCtrl().BlendAnimation("runAimUp", 0.1f);
			break;
		case 13:
			GetShape(0).GetAnimCtrl().BlendAnimation("runAimDown", 0.1f);
			break;
		case 14:
			GetShape(0).GetAnimCtrl().BlendAnimation("runAimDiagonalUp", 0.1f);
			break;
		case 15:
			GetShape(0).GetAnimCtrl().BlendAnimation("runAimDiagonalDown", 0.1f);
			break;
		case 16:
			GetShape(0).GetAnimCtrl().BlendAnimation("dashForward", 0.1f);
			break;
		case 17:
			GetShape(0).GetAnimCtrl().BlendAnimation("shootForward", 0.1f);
			break;
		case 18:
			GetShape(0).GetAnimCtrl().BlendAnimation("shootUp", 0.1f);
			break;
		case 19:
			GetShape(0).GetAnimCtrl().BlendAnimation("shootDown", 0.1f);
			break;
		case 20:
			GetShape(0).GetAnimCtrl().BlendAnimation("shootDiagonalUp", 0.1f);
			break;
		case 21:
			GetShape(0).GetAnimCtrl().BlendAnimation("shootDiagonalDown", 0.1f);
			break;
		case 22:
			GetShape(0).GetAnimCtrl().BlendAnimation("aimUp", 0.1f);
			break;
		case 23:
			GetShape(0).GetAnimCtrl().BlendAnimation("prone", 0.1f);
			break;
		case 24:
			GetShape(0).GetAnimCtrl().BlendAnimation("aimDiagonalUp", 0.1f);
			break;
		case 25:
			GetShape(0).GetAnimCtrl().BlendAnimation("aimDiagonalDown", 0.1f);
			break;
		case 26:
			GetShape(0).GetAnimCtrl().BlendAnimation("jumpShootForward", 0.1f);
			break;
		case 27:
			GetShape(0).GetAnimCtrl().BlendAnimation("jumpShootUp", 0.1f);
			break;
		case 28:
			GetShape(0).GetAnimCtrl().BlendAnimation("jumpShootDown", 0.1f);
			break;
		case 29:
			GetShape(0).GetAnimCtrl().BlendAnimation("jumpShootDiagonalUp", 0.1f);
			break;
		case 30:
			GetShape(0).GetAnimCtrl().BlendAnimation("jumpShootDiagonalDown", 0.1f);
			break;
		case 31:
			GetShape(0).GetAnimCtrl().BlendAnimation("jumpAimForward", 0.1f);
			break;
		case 32:
			GetShape(0).GetAnimCtrl().BlendAnimation("jumpAimUp", 0.1f);
			break;
		case 33:
			GetShape(0).GetAnimCtrl().BlendAnimation("jumpAimDown", 0.1f);
			break;
		case 34:
			GetShape(0).GetAnimCtrl().BlendAnimation("jumpAimDiagonalUp", 0.1f);
			break;
		case 35:
			GetShape(0).GetAnimCtrl().BlendAnimation("jumpAimDiagonalDown", 0.1f);
			break;
		case 36:
			GetShape(0).GetAnimCtrl().BlendAnimation("fallShootForward", 0.1f);
			break;
		case 37:
			GetShape(0).GetAnimCtrl().BlendAnimation("fallShootUp", 0.1f);
			break;
		case 38:
			GetShape(0).GetAnimCtrl().BlendAnimation("fallShootDown", 0.1f);
			break;
		case 39:
			GetShape(0).GetAnimCtrl().BlendAnimation("fallShootDiagonalUp", 0.1f);
			break;
		case 40:
			GetShape(0).GetAnimCtrl().BlendAnimation("fallShootDiagonalDown", 0.1f);
			break;
		case 41:
			GetShape(0).GetAnimCtrl().BlendAnimation("fallAimForward", 0.1f);
			break;
		case 42:
			GetShape(0).GetAnimCtrl().BlendAnimation("fallAimUp", 0.1f);
			break;
		case 43:
			GetShape(0).GetAnimCtrl().BlendAnimation("fallAimDown", 0.1f);
			break;
		case 44:
			GetShape(0).GetAnimCtrl().BlendAnimation("fallAimDiagonalUp", 0.1f);
			break;
		case 45:
			GetShape(0).GetAnimCtrl().BlendAnimation("fallAimDiagonalDown", 0.1f);
			break;
		case 46:
			GetShape(0).GetAnimCtrl().SetAnimation("death");
			break;
		}
	}

	if(m_bRespawn == true && m_nCurrentAnimation == 46 && GetShape(0).GetAnimCtrl().getLoopCount() >= 1)
	{
		Respawn();
		m_bRespawn = false;
	}
}

bool CPlayer::CheckSceneCollision(CBase* pBase)
{
	bool toReturn = false;
	if(m_bIsDownJumping && pBase->GetType() != OBJ_PLATFORM)
	{
		toReturn = CBase::CheckSceneCollision(pBase);
	}
	else
	{
		toReturn = CBase::CheckSceneCollision(pBase);
	}

	SetIsonPlatform(false);
	return toReturn;
}

void CPlayer::SceneCollisionReact(CBase* pBase)
{
	if(pBase->GetType() == OBJ_PLATFORM)
		SetIsonPlatform(true);

	m_bIsDownJumping = false;
}