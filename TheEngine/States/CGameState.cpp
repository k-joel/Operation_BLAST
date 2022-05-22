#include "CGameState.h"
#include "../Input/CInput.h"
//additional #includes here
#include "../Core/CGame.h"
#include "../Rendering/CTextureManager.h"
#include "../Rendering/CDirect3D.h"
#include "../Rendering/CShaderManager.h"
#include "../Font/CBitMapFont.h"
#include "../Input/CInput.h"
#include "CWinState.h"
#include "CGameOverState.h"
#include "CStateMachine.h"
#include "CMainMenuState.h"
#include "CStatsState.h"
#include "CPauseState.h"
#include "CCharacterSelectionState.h"
#include "CHowToState.h"
#include "../Debug System/DebugSystem.h"
#include "../Game Objects/CCrate.h"
#include "../Game Objects/CBarrel.h"
#include "../Game Objects/CSpawner.h"
#include "../Resources/Sound/Wwise_IDs.h"
#include "../Sound/WwiseManager.h"

#include "../Mesh/modelImporter.h"

#include "../Game Objects/layoutImporter.h"
#include "../Game Objects/motionImporter.h"
#include "../Game Objects/Scene/CFlag.h"
#include "../Game Objects/Weapon States/CMachineGunState.h"
#include "../Game Objects/Weapon States/CFlamethrowerState.h"
#include "../Game Objects/Weapon States/CRocketState.h"
#include "../Game Objects/Weapon States/CLightningGunState.h"
#include "../Game Objects/Weapon States/CShotgunState.h"

#include "../Game Objects/CGoal.h"

#include "../Rendering/CRenderer.h"
#include "../Collision/CCollisionTests.h"
#include "../LuaEngine/CLuaEngine.h"

//Factory and manager
#include "../Game Objects/CObjectManager.h"
#include "../Game Objects/CObjectFactory.h"

#include "../Game Objects/Enemy/CAssaulter.h"
#include "../Game Objects/Enemy/CStriker.h"
#include "../Game Objects/Enemy/CUltimus.h"
#include "../Game Objects/CDebris.h"

#include "../Game Objects/CPickUps.h"

#include "../fx/CParticleManager.h"

#define CAM_PAN_X 275.0f //How far the camera will pan after a plan split
#define MULT_FIRST_GAL_AMOUNT 12
#define MULT_SECOND_GAL_AMOUNT 16
#define SOLO_FIRST_GAL_AMOUNT 8
#define SOLO_SECOND_GAL_AMOUNT 12

#define BOSS_YEAH 55
//#define TESTINGAI
//#define TESTINGBOSS

const AkGameObjectID GAME_OBJECT_MUSIC = 200;

CGameState::CGameState(void)
{
	m_nDebrisOffset = 0;
	m_Cam = CCamera::GetInstance();

	//initialize all values
	m_pTM = NULL;

	m_eCameraFollow = CEventSystem::GetInstance()->FindEvent("FollowCamera");
	m_eCameraLock = CEventSystem::GetInstance()->FindEvent("LockCamera");
	m_eSpawnLightWave = CEventSystem::GetInstance()->FindEvent("SpawnLightWave");
	m_eLightLock = CEventSystem::GetInstance()->FindEvent("LightLock");
	m_eSpawnMediumWave = CEventSystem::GetInstance()->FindEvent("SpawnMediumWave");
	m_eMediumLock = CEventSystem::GetInstance()->FindEvent("MediumLock");
	m_eSpawnHeavyWave = CEventSystem::GetInstance()->FindEvent("SpawnHeavyWave");
	m_eHeavyLock = CEventSystem::GetInstance()->FindEvent("HeavyLock");
	m_eWinEvent = CEventSystem::GetInstance()->FindEvent("WinEvent");
	m_eLoseEvent = CEventSystem::GetInstance()->FindEvent("LoseEvent");
	m_eTutorialCompleted = CEventSystem::GetInstance()->FindEvent("IntroCompleted");
	m_eSplitEvent = CEventSystem::GetInstance()->FindEvent("SplitEvent");
	m_eMergeEvent = CEventSystem::GetInstance()->FindEvent("MergeEvent");
	m_eShootingGal = CEventSystem::GetInstance()->FindEvent("ShootingGal");
	m_eRainDebris = CEventSystem::GetInstance()->FindEvent("RainDebris");
	m_eStopDebris = CEventSystem::GetInstance()->FindEvent("StopDebris");
	m_eSpawnBoss = CEventSystem::GetInstance()->FindEvent("SpawnBoss");
	m_eLockClear = CEventSystem::GetInstance()->FindEvent("LockClear");
	m_eShowPlayerID = CEventSystem::GetInstance()->FindEvent("ShowPlayerID");

	m_bIsBarron = true;
	m_bAudioPlayPlayerScoreSwitch = false;
	m_bUltimateCheat = false;
	m_bPlayer2Entered = false;
	m_bAudioPlayScore1 = false;
	m_bAudioPlayScore2 = false;

	m_nPrevPlayer1Score = 0;
	m_nPrevPlayer2Score = 0;

	Register();

	D3DXMatrixIdentity(&mxWorld);
	D3DXMatrixTranslation(&mxWorld, 30.0f, 20.0f, 30.0f);

	m_fCrateTimer = 0.0f;
	m_nPlayState = LOCKED;
	m_nTriggerCount = 0;
	m_fDebrisTimer = 0.0f;

	m_nDebrisAddAmount = 1;
}

CGameState::~CGameState(void)
{
}

void CGameState::Enter(void)
{
	//PROFILER_START

	CWwiseManager::GetInstance()->RegisterObject(GAME_OBJECT_MUSIC, "GameMusic");
	m_nPlayState = LOCKED;

	//CWwiseManager::GetInstance()->SetMXVolume(100);
	//CWwiseManager::GetInstance()->SetFXVolume(100);

	CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::STOP_ALL, AK_UnknownObject);
	CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_MX_LEVEL_MUSIC, GAME_OBJECT_MUSIC);
	CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_GAMEPLAY_AMBIENCE_LOOP, GAME_OBJECT_MUSIC);
	CWwiseManager::GetInstance()->SetSwitch(AK::SWITCHES::AMBIENCE::GROUP, AK::SWITCHES::AMBIENCE::SWITCH::BEACH, GAME_OBJECT_MUSIC);

	//get instance of systems here
	m_pTM = CTextureManager::GetInstance();
	m_pD3D = CDirect3D::GetInstance();
	m_pDI = CInput::GetInstance();
	m_pOM = CObjectManager::GetInstance();
	float w = (float)m_pD3D->GetPresentParams()->BackBufferWidth;
	float h = (float)m_pD3D->GetPresentParams()->BackBufferHeight;
	m_Cam->BuildPerspective(D3DX_PI * 0.25f, w/h, 1.0f, 5000.0f);
	m_bShootingGal = false;
	m_bPlayer2Entered = false;
	m_bAudioPlayPlayerScoreSwitch = false;
	m_bAudioPlayScore1 = false;
	m_bAudioPlayScore2 = false;

	m_nPrevPlayer1Score = 0;
	m_nPrevPlayer2Score = 0;

	m_nFade = CLoadState::GetInstance()->GetImageID("fade");
	{
		//player feedback setup
		m_fShootEverything = 0.0f;
		m_nShootEverything = CLoadState::GetInstance()->GetImageID("shootEverything");

		m_fMoveForward = 0.0f;
		m_nMoveForward = CLoadState::GetInstance()->GetImageID("moveForward");

		m_fSplitUp = 0.0f;
		m_nSplitUp = CLoadState::GetInstance()->GetImageID("splitUp");

		m_fRegroup = 0.0f;
		m_nRegroup = CLoadState::GetInstance()->GetImageID("regroup");

		m_fWinner1 = 0.0f;
		m_nWinner1 = CLoadState::GetInstance()->GetImageID("player1win");

		m_fWinner2 = 0.0f;
		m_nWinner2 = CLoadState::GetInstance()->GetImageID("player2win");
	}

	// reset all triggers ( merge split etc )
	m_pOM->ResetTriggers();

	// Setting up the victory flag
	m_Flag = (CFlag*)CObjectFactory::GetInstance()->GetObject<CFlag>(OBJ_FLAG);
	m_Flag->SetPosition(D3DXVECTOR3(9770.0f,1050.0f,0.0f));
	m_Flag->m_vgoal = D3DXVECTOR3(9770.0f,1060.0f,0.0f);
	m_Flag->SetIsActive(true);

	//m_pBossMan = (CUltimus*)(CObjectFactory::GetInstance()->GetObject<CUltimus>(OBJ_ULTIMUS));
	//m_pBossMan->Init(D3DXVECTOR3(8300.0f,50.0f,0.0f),false,600);
	//CObjectManager::GetInstance()->AddObject(m_pBossMan);

	CObjectManager::GetInstance()->AddObject(m_Flag);

	//Player 1 HUD
	hud.SetType(PLAYER1);
	if(CCharacterSelectionState::GetInstance()->GetSelection() == 0)
		hud.SetPortraitType(0);
	else
		hud.SetPortraitType(1);
	hud.Initialize();

	//Player 2 HUD
	hud2.SetType(PLAYER2);
	if(CCharacterSelectionState::GetInstance()->GetSelection() == 0)
		hud2.SetPortraitType(1);
	else
		hud2.SetPortraitType(0);

	hud2.Initialize();

	D3DXVECTOR3 cPos;
	CLuaEngine::GetInstance()->GetData(cPos, "camStart");
	D3DXMATRIX reset;
	D3DXMatrixIdentity(&reset);
	m_Cam->SetViewMatrix(&reset);
	m_Cam->SetViewPosition(cPos);
	m_Cam->SetBaseY(cPos.y);
	float camRot;
	CLuaEngine::GetInstance()->GetData(camRot, "camStartRot");
	m_Cam->ViewRotateGlobalX(camRot);

	// Adding the player
	CreatePlayer();
	m_Cam->SetPlayerOne(m_pPlayer);

	m_Cam->ShakeCamera(0.0f);
	reset = m_Cam->m_MC.GetWorldMatrix(0);
	m_Cam->SetViewMatrix(&reset);
	m_Cam->Update(0.015f);
	//m_Cam->Update(0.0f);

	m_fSpawnTimer = 0.0f;
	m_bIntroDone = false;

	m_bWaveOneOut = false;
	m_bWaveTwoOut = false;
	m_bWaveThreeOut = false;

	m_bRender = true;

	m_bGameOver = false;
	m_bWin = false;
	m_fFadeSet = 0.0;
	m_fFadeTimer = 0.0;
	m_bPaused = false;

	m_nEnemiesAlive = 3;

	//CreateTutorialEnemies();

	m_bIntroComplete = false;

	m_nPlayState = LOCKED;
	m_nTriggerCount = 0;
	m_Cam->SetState(CCamera::LOCK);

	m_bIsBarron = true;

	m_pPlayer->SwitchWeapon(CMachineGunState::GetInstance());

	if(CGame::GetInstance()->GetMutliplayer())
		m_pPlayer2->SwitchWeapon(CMachineGunState::GetInstance());

	m_startsAsMultiPlayer = CGame::GetInstance()->GetMutliplayer();

	CEventSystem::GetInstance()->DispatchEvent(*m_eStopDebris, NULL, 1.0f);

	m_nEnemyLockCount = 0;
	m_nPrevEnemyLockCount = 0;
	m_bTutorial = true;
	m_bEnemiesPresent = false;
	m_bTutorialEnemiesCleared = false;
	m_fTutorialEnemyTimer = 0.0f;
	m_fStartCountdown = 0.0f;
	CSpawner::GetInstance()->SetTrickle(false);

	m_PressStart.Initialize("PressStart","PressStartOffset",150.0f,225.0f,50.0f,50,50,1,false);

	//Setting the default level type to the beach
	m_nLevelArea = BEACH;
	m_nGalleryCount = 1;
	m_fAugmentTimer = 0.0f;
	m_bAugmentOut = false;

	m_fGameTime = 0.0f;
}

bool CGameState::Input(void)
{
	KeyboardDevice* keyboard = (KeyboardDevice*)CInput::GetInstance()->GetDevice(DEV_KEYBOARD);

	if(m_bGameOver == false)
	{
		if(m_startsAsMultiPlayer == false && m_bPlayer2Entered == false && CInput::GetInstance()->GetAction(ACT_PAUSE,1))
		{
			m_bPlayer2Entered = true;
			CreatePlayer2();
		}

		if(keyboard->KeyPressed(DIK_F1))
		{
			CStateSystem::GetInstance()->AddState(CHowToState::GetInstance());
			m_bPaused = true;
		}
	}

	return true;
}

void CGameState::Update(float fElapsedTime)
{
	m_fGameTime += fElapsedTime;

	//Cheats for the weapons
	if(CGame::GetInstance()->GetMutliplayer())
	{
		if(CGame::GetInstance()->GetFlamethrowerCheat())
			m_pPlayer2->SwitchWeapon(CFlamethrowerState::GetInstance());

		if(CGame::GetInstance()->GetShotgunCheat())
			m_pPlayer2->SwitchWeapon(CShotgunState::GetInstance());

		if(CGame::GetInstance()->GetRocketLauncherCheat())
			m_pPlayer2->SwitchWeapon(CRocketLauncherState::GetInstance());

		if(CGame::GetInstance()->GetLightningGunCheat())
			m_pPlayer2->SwitchWeapon(CLightningGunState::GetInstance());
	}

	if(CGame::GetInstance()->GetFlamethrowerCheat())
	{
		m_pPlayer->SwitchWeapon(CFlamethrowerState::GetInstance());
		CGame::GetInstance()->SetFlamethrowerCheat(false);
	}

	if(CGame::GetInstance()->GetShotgunCheat())
	{
		m_pPlayer->SwitchWeapon(CShotgunState::GetInstance());
		CGame::GetInstance()->SetShotgunCheat(false);
	}

	if(CGame::GetInstance()->GetRocketLauncherCheat())
	{
		m_pPlayer->SwitchWeapon(CRocketLauncherState::GetInstance());
		CGame::GetInstance()->SetRocketLauncherCheat(false);
	}

	if(CGame::GetInstance()->GetLightningGunCheat())
	{
		m_pPlayer->SwitchWeapon(CLightningGunState::GetInstance());
		CGame::GetInstance()->SetLightningGunCheat(false);
	}

	if(m_bTutorial)
	{
		//First, displaying the movement things.

		//After moving around, display the fire controls.

		if(!m_bEnemiesPresent)
		{
			m_fTutorialEnemyTimer += fElapsedTime;
			if(m_fTutorialEnemyTimer >= 5.0f) //Possibly change this time - TJ
			{
				m_fShootEverything = 4.0f;
				m_bEnemiesPresent = true;
				CSpawner::GetInstance()->RunScript("Tutorial", false, LOCK_WAVE);
				m_nTutorialEnemiesAlive = 3;
				CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_DX_ANNOUNCER_KILLEVERYTHING, 200);
			}
		}

		if(m_bEnemiesPresent)
		{
			m_fTutorialEnemyTimer += fElapsedTime;
			if(m_fTutorialEnemyTimer > 10.0f)
			{
				m_Cam->SetState(CCamera::FOLLOW);
				m_bTutorial = false;
			}

		}

		if(m_bEnemiesPresent && m_nTutorialEnemiesAlive <= 0)
		{
			m_bTutorialEnemiesCleared = true;
			m_fMoveForward = 3.0f;
		}

		if(m_bTutorialEnemiesCleared)
		{
			m_fStartCountdown += fElapsedTime;
			if(m_fStartCountdown > 3.0f) //Possibly change this time - TJ
			{
				m_bTutorial = false;
				m_Cam->SetState(CCamera::FOLLOW);
				CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_DX_ANNOUNCER_GOGOGO, 200);
			}
		}
	}

#ifdef _DEBUG
	{
		static float lastTime = 0.0f;
		float curTime = (float)GetTickCount() / 1000.0f;
		static DWORD dwFrames = 0;
		dwFrames++;
		static float fFPS = 0.0f;

		if( curTime - lastTime > 1.0f )
		{
			fFPS = (float)dwFrames / (curTime - lastTime);
			dwFrames = 0;
			lastTime = curTime;
		}

		CDebugSystem::GetInstance()->RegisterData("FPS: ", fFPS);
		CDebugSystem::GetInstance()->RegisterData("Previous Enemy: ",m_nPrevEnemyLockCount);
		CDebugSystem::GetInstance()->RegisterData("Enemy Count: ", m_nEnemyLockCount);
	}
#endif

	if(m_pPlayer && !m_pPlayer->GetIsActive())
	{
		m_pPlayer = m_pPlayer2;
		m_pPlayer2 = NULL;
	}

	if(m_pPlayer2 && !m_pPlayer2->GetIsActive())
	{
		m_pPlayer2 = NULL;
	}

	if(!m_pPlayer2 && !m_bGameOver && !m_bWin)
	{
		CGame::GetInstance()->SetMultiplayer(false);
	}

	if(CGame::GetInstance()->GetUnlimitedUltCheat())
	{
		m_pPlayer->SetUltGauge(100.0f);

		if(CGame::GetInstance()->GetMutliplayer())
			m_pPlayer2->SetUltGauge(100.0f);
	}

	m_mxViewProj  = CCamera::GetInstance()->GetViewMatrix() * CCamera::GetInstance()->GetProjectionMatrix();
	if(!((XboxGamePadDevice*)CInput::GetInstance()->GetDevice(DEV_GAMEPAD))->ControllerConnected(1) && CGame::GetInstance()->GetMutliplayer())
	{
		m_bPaused = true;
		CStateSystem::GetInstance()->AddState(CPauseState::GetInstance());
	}

	if(m_bGameOver == false && m_bWin == false)
	{
		hud.Update(fElapsedTime);
		hud2.Update(fElapsedTime);
		m_Cam->Update(fElapsedTime);

		if(CGame::GetInstance()->GetMutliplayer() == true)
		{
			PlayPlayerSwitch();

			if(m_bAudioPlayPlayerScoreSwitch == true)
			{
				m_bAudioPlayPlayerScoreSwitch = false;
				if(m_bAudioPlayScore1 == true)
				{
					CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_DX_ANNOUNCER_LEADBARON, 200);
					m_bAudioPlayScore1 = false;
				}
				if(m_bAudioPlayScore2 == true)
				{
					CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_DX_ANNOUNCER_LEADCLAIRE, 200);
					m_bAudioPlayScore2 = false;
				}
			}
			//if( m_bAudioPlayPlayerScoreSwitch == true )
			//{
			//	if(hud.GetHudPortrate() == 0 )	// 0 == Barron
			//	{
			//		CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_DX_ANNOUNCER_LEADBARON, 200);
			//		m_bAudioPlayPlayerScoreSwitch = false;
			//	}
			//	else if(hud.GetHudPortrate() == 1 )	// 1 == Claire
			//	{
			//		CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_DX_ANNOUNCER_LEADCLAIRE, 200);
			//		m_bAudioPlayPlayerScoreSwitch = false;
			//	}

			//}
			//if( (hud.GetScore() > m_nPrevPlayer2Score && m_nPrevPlayer1Score < hud2.GetScore() )|| (hud2.GetScore() > m_nPrevPlayer1Score && m_nPrevPlayer2Score < hud.GetScore() ) )
			//{
			//	m_bAudioPlayPlayerScoreSwitch = true;
			//}
		}

		m_pOM->UpdateObjects(fElapsedTime);
		m_pOM->CheckCollisions();
	}

	if(m_bGameOver == true || m_bWin == true)
	{
		m_fFadeTimer += fElapsedTime;
		if(m_fFadeTimer >= 0.05f)
		{
			m_fFadeSet += 0.1f;
			m_fFadeTimer = 0.0f;
		}

		if(m_fFadeSet >= 1.0f && m_bGameOver)
		{
			CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::STOP_ALL, GAME_OBJECT_MUSIC);
			//CWwiseManager::GetInstance()->UnregisterObject(GAME_OBJECT_MUSIC);
			m_bRender = false;
			CStateSystem::GetInstance()->AddState(CGameOverState::GetInstance());
		}
		else if(m_fFadeSet >= 1.0f && m_bWin)
		{
			CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::STOP_ALL, GAME_OBJECT_MUSIC);
			//CWwiseManager::GetInstance()->UnregisterObject(GAME_OBJECT_MUSIC);
			m_bRender = false;
			CStateSystem::GetInstance()->AddState(CWinState::GetInstance());
		}
	}

	/*
	if(m_nEnemiesAlive == 0 && !m_bIntroComplete)
	{
	char* lions = "whatever";
	CEventSystem::GetInstance()->DispatchEvent(*m_eTutorialCompleted, NULL, 1.0f);
	CSpawner::GetInstance()->SetTrickle(true);
	m_bIntroComplete = true;
	}
	*/

	if(m_bShootingGal)
	{
		if(!m_bAugmentOut)
		{
			m_fAugmentTimer += fElapsedTime;
			if(m_fAugmentTimer > 4.0f)
			{
				if(CGame::GetInstance()->GetMutliplayer())
				{
					char buffer[20];
					sprintf(buffer, "ShootingGal%iAugment", m_nGalleryCount);
					CSpawner::GetInstance()->RunScript(buffer, false, GAL_WAVE);
					CSpawner::GetInstance()->RunScript(buffer, true, GAL_WAVE);
				}

				m_bAugmentOut = true;
				m_fAugmentTimer = 0.0f;
			}
		}

		if(CGame::GetInstance()->GetMutliplayer() && m_nShootingGalleryEnemiesAliveFar == 0 )
		{
			m_bShootingGal = false;
			m_bAugmentOut = false;
			++m_nGalleryCount;

			CSpawner::GetInstance()->SetTrickle(true);
			char* buffer = "KahranIsAJerk";
			// that's not how char* works. should be char buffer[] = "TJisAGiantDouchebag"
			// Now see here, you dunder head, the snooty board of directors dictates how strings are used

			CEventSystem::GetInstance()->DispatchEvent(*m_eCameraFollow, NULL, 1.0f);
			CObjectManager::GetInstance()->DestroyAllGameObjects();

			//Now we add points to the player that was over here.
			if(CGame::GetInstance()->GetMutliplayer())
			{
				m_pPlayer2->AddScore(5000);

				D3DXMATRIX locMat = m_pPlayer2->GetFrame().GetWorldMatrix();
				locMat._42 += 30.0f;
				CParticleManager::GetInstance()->SpawnParticle("Plus_win", locMat);
			}
		}
		else if(m_nShootingGalleryEnemiesAliveNear == 0)
		{
			m_bShootingGal = false;
			CSpawner::GetInstance()->SetTrickle(true);
			++m_nGalleryCount;

			char* buffer = "IWantToGoHome";
			CEventSystem::GetInstance()->DispatchEvent(*m_eCameraFollow, NULL, 1.0f);
			CObjectManager::GetInstance()->DestroyAllGameObjects();

			//Adding points to the player that was here ONLY in multiplayer
			if(CGame::GetInstance()->GetMutliplayer())
			{
				m_pPlayer->AddScore(5000);

				D3DXMATRIX locMat = m_pPlayer->GetFrame().GetWorldMatrix();
				locMat._42 += 30.0f;
				CParticleManager::GetInstance()->SpawnParticle("Plus_winb", locMat);
			}
		}
	}

	//Raining debris right here
	if(m_bRainDebris)
	{
		m_fDebrisTimer += fElapsedTime;

		if(m_fDebrisTimer > 0.5f)
		{
			CDebris* debris = CObjectFactory::GetInstance()->GetObject<CDebris>(OBJ_DEBRIS);
			D3DXVECTOR3 pos;
			pos.y = 600.0f;
			pos.z = 0.0f;
			pos.x = CCamera::GetInstance()->GetViewPosition().x - 60.0f;
			pos.x += (60.0f * m_nDebrisOffset);

			m_nDebrisOffset += m_nDebrisAddAmount;

			if(m_nDebrisOffset == 2)
			{
				m_nDebrisAddAmount = -1;
			}

			if(m_nDebrisOffset == 0)
			{
				m_nDebrisAddAmount = 1;
			}

			debris->SetPosition(pos);
			debris->SetType(OBJ_DEBRIS);
			debris->SetIsOnGround(false);
			debris->SetIsActive(true);
			debris->Update(fElapsedTime);

			CObjectManager::GetInstance()->AddObject(debris);

			m_fDebrisTimer = 0.0f;
		}
	}

	if(m_fShootEverything > 0.0f)
	{
		m_fShootEverything -= fElapsedTime;
	}

	if(m_fMoveForward > 0.0f)
	{
		m_fMoveForward -= fElapsedTime;
	}

	if(m_fSplitUp > 0.0f)
	{
		m_fSplitUp -= fElapsedTime;
	}

	if(m_fRegroup > 0.0f)
	{
		m_fRegroup -= fElapsedTime;
	}

	
		if(m_fUnlock > 0.0f)
		{
			m_fUnlock -= fElapsedTime;
		}
	

	if(m_fWinner1 > 0.0f)
	{
		m_fWinner1 -= fElapsedTime;
	}

	if(m_fWinner2 > 0.0f)
	{
		m_fWinner2 -= fElapsedTime;
	}

	CSpawner::GetInstance()->Update(fElapsedTime);

	if(m_Cam->GetState() == CCamera::LOCK && m_bShootingGal)
	{
		if(m_fUnlock <= 0.0f)
		{
				CEventSystem::GetInstance()->DispatchEvent(*m_eLockClear, NULL, 1.0f);
				m_fMoveForward = 2.0;
				CSpawner::GetInstance()->SetTrickle(true);
		}
	}

	if(m_Cam->GetState() == CCamera::LOCK && !m_bTutorial && !m_bShootingGal)
	{
		if(m_fUnlock <= 0.0f)
		{
			CEventSystem::GetInstance()->DispatchEvent(*m_eLockClear, NULL, 1.0f);
			m_fMoveForward = 2.0;
			CSpawner::GetInstance()->SetTrickle(true);
		}

		if(m_nEnemyLockCount <= 0 && m_nPrevEnemyLockCount != 0)
		{
			m_fMoveForward = 2.0;
			CSpawner::GetInstance()->SetTrickle(true);
			CEventSystem::GetInstance()->DispatchEvent(*m_eLockClear, NULL, 1.0f);
		}

		m_nPrevEnemyLockCount = m_nEnemyLockCount;
	}

	m_PressStart.UpdateBlink(fElapsedTime,0.5);

	if(m_pPlayer && m_pPlayer->GetPosition().x > 9640)
	{
		m_Cam->SetState(CCamera::LOCK);
		CSpawner::GetInstance()->SetTrickle(false);
		m_pBossMan->SetBossFighting(true);
	}
	if(m_pBossMan->GetBossFighting())
		CSpawner::GetInstance()->SetTrickle(false);

	if(m_pPlayer && m_pPlayer->GetPosition().x > 9450)
		CSpawner::GetInstance()->SetTrickle(false);

	if(m_pBossMan->GetIsDead())
	{
		m_Flag->SetRotationY(D3DX_PI * -0.30);
		m_Flag->m_vgoal = D3DXVECTOR3(m_pBossMan->GetPosition().x,25.0f,0.0f);
		CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_WEEBOS_HELICOPTER_LOOP, 200);
	}


	CParticleManager::GetInstance()->Run(fElapsedTime);
}

void CGameState::Render(void)
{
#ifdef _DEBUG
	CDebugSystem::GetInstance()->DisplayInfo();
#endif
	// prototype test

	if(m_bRender)
	{
		m_pOM->RenderObjects();
		if(!m_bPaused)
		{
			hud.Render();

			float w = (float)CDirect3D::GetInstance()->GetPresentParams()->BackBufferWidth;
			float h = (float)CDirect3D::GetInstance()->GetPresentParams()->BackBufferHeight;
			D3DXMATRIX OrthoProjection;
			D3DXMatrixOrthoLH(&OrthoProjection, w, h , 1.0f, 500.0f);

			if(m_startsAsMultiPlayer==true)
				hud2.Render();
			else if(((XboxGamePadDevice*)CInput::GetInstance()->GetDevice(DEV_GAMEPAD))->ControllerConnected(1))
			{
				m_PressStart.RenderBlink(OrthoProjection);
			}

				if(CCharacterSelectionState::GetInstance()->GetSelection() == 0)
			{
				CBitMapFont::GetInstance()->Print("P1: Barron",205,120,false,false,0.85f,D3DXCOLOR(0,0,0.5f,255));
				if(m_startsAsMultiPlayer==true)
					CBitMapFont::GetInstance()->Print("P2: Claire",660,120,false,false,0.85f,D3DXCOLOR(0.5f,0,0,255));
			}
			else
			{
				CBitMapFont::GetInstance()->Print("P1: Claire",205,120,false,false,0.85f,D3DXCOLOR(0,0,0.5f,255));
				if(m_startsAsMultiPlayer==true)
					CBitMapFont::GetInstance()->Print("P2: Barron",660,120,false,false,0.85f,D3DXCOLOR(0.5f,0,0,255));
			}
		}

		if(m_fWinner1 > 0.0f)
		{
			CTextureManager::GetInstance()->Draw(m_nWinner1, 400, 150, 0.2f, 0.2f);
		}

		else if(m_fWinner2 > 0.0f)
		{
			CTextureManager::GetInstance()->Draw(m_nWinner2, 400, 150, 0.2f, 0.2f);
		}

		else if(m_fShootEverything > 0.0f)
		{
			CTextureManager::GetInstance()->Draw(m_nShootEverything, 400, 150, 0.6f, 0.6f);
		}

		else if(m_fMoveForward > 0.0f)
		{
			CTextureManager::GetInstance()->Draw(m_nMoveForward, 400, 150, 0.6f, 0.6f);
		}

		else if(m_fSplitUp > 0.0f)
		{
			CTextureManager::GetInstance()->Draw(m_nSplitUp, 400, 150, 0.6f, 0.6f);
		}

		else if(m_fRegroup > 0.0f)
		{
			CTextureManager::GetInstance()->Draw(m_nRegroup, 400, 150, 0.6f, 0.6f);
		}


		if(m_bGameOver || m_bWin)
		{
			CTextureManager::GetInstance()->Draw(m_nFade,0,0,1.0,1.0,0,0,0,0,D3DXCOLOR(1.0,1.0,1.0,m_fFadeSet));
		}
	}

	CRenderContextManager::GetInstance()->Render();
}

bool CGameState::Exit(void)
{
	CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::STOP_MX_LEVEL_MUSIC, GAME_OBJECT_MUSIC);
	hud.Shutdown();
	m_Cam->SetPlayerOne(NULL);
	m_Cam->SetPlayerTwo(NULL);
	m_pOM->RemoveAllObjects();
	CParticleManager::GetInstance()->RemoveParticles();

	return false;
}

CGameState* CGameState::GetInstance(void)
{
	static CGameState instance;
	return &instance;
}

//Definition of event system events
void CGameState::Register(void)
{
	CEventSystem::GetInstance()->RegisterForEvent(*this, *m_eCameraLock, 1.0f);
	CEventSystem::GetInstance()->RegisterForEvent(*this, *m_eCameraFollow, 1.0f);
	CEventSystem::GetInstance()->RegisterForEvent(*this, *m_eSpawnLightWave, 1.0f);
	CEventSystem::GetInstance()->RegisterForEvent(*this, *m_eSpawnMediumWave, 1.0f);
	CEventSystem::GetInstance()->RegisterForEvent(*this, *m_eSpawnHeavyWave, 1.0f);
	CEventSystem::GetInstance()->RegisterForEvent(*this, *m_eWinEvent, 1.0f);
	CEventSystem::GetInstance()->RegisterForEvent(*this, *m_eLoseEvent,1.0f);
	CEventSystem::GetInstance()->RegisterForEvent(*this, *m_eTutorialCompleted,1.0f);
	CEventSystem::GetInstance()->RegisterForEvent(*this, *m_eSplitEvent,1.0f);
	CEventSystem::GetInstance()->RegisterForEvent(*this, *m_eMergeEvent,1.0f);
	CEventSystem::GetInstance()->RegisterForEvent(*this, *m_eShootingGal,1.0f);
	CEventSystem::GetInstance()->RegisterForEvent(*this, *m_eRainDebris,1.0f);
	CEventSystem::GetInstance()->RegisterForEvent(*this, *m_eStopDebris,1.0f);
	CEventSystem::GetInstance()->RegisterForEvent(*this, *m_eSpawnBoss,1.0f);
	CEventSystem::GetInstance()->RegisterForEvent(*this, *m_eLightLock,1.0f);
	CEventSystem::GetInstance()->RegisterForEvent(*this, *m_eMediumLock,1.0f);
	CEventSystem::GetInstance()->RegisterForEvent(*this, *m_eHeavyLock,1.0f);
	CEventSystem::GetInstance()->RegisterForEvent(*this, *m_eLockClear,1.0f);
}

void CGameState::Deregister(void)
{
	CEventSystem::GetInstance()->DeregisterForEvent(*this, *m_eCameraLock);
	CEventSystem::GetInstance()->DeregisterForEvent(*this, *m_eCameraFollow);
	CEventSystem::GetInstance()->DeregisterForEvent(*this, *m_eSpawnLightWave);
	CEventSystem::GetInstance()->DeregisterForEvent(*this, *m_eSpawnMediumWave);
	CEventSystem::GetInstance()->DeregisterForEvent(*this, *m_eSpawnHeavyWave);
	CEventSystem::GetInstance()->DeregisterForEvent(*this, *m_eWinEvent);
	CEventSystem::GetInstance()->DeregisterForEvent(*this, *m_eLoseEvent);
	CEventSystem::GetInstance()->DeregisterForEvent(*this, *m_eTutorialCompleted);
	CEventSystem::GetInstance()->DeregisterForEvent(*this, *m_eSplitEvent);
	CEventSystem::GetInstance()->DeregisterForEvent(*this, *m_eMergeEvent);
	CEventSystem::GetInstance()->DeregisterForEvent(*this, *m_eShootingGal);
	CEventSystem::GetInstance()->DeregisterForEvent(*this, *m_eRainDebris);
	CEventSystem::GetInstance()->DeregisterForEvent(*this, *m_eStopDebris);
	CEventSystem::GetInstance()->DeregisterForEvent(*this, *m_eSpawnBoss);
	CEventSystem::GetInstance()->DeregisterForEvent(*this, *m_eLightLock);
	CEventSystem::GetInstance()->DeregisterForEvent(*this, *m_eMediumLock);
	CEventSystem::GetInstance()->DeregisterForEvent(*this, *m_eHeavyLock);
	CEventSystem::GetInstance()->DeregisterForEvent(*this, *m_eLockClear);
}

void CGameState::React(const tDispatch& toReactTo)
{
	char buffer[16];
	strcpy_s(buffer, 16,(((tDispatch*)(&toReactTo))->m_pEvent->GetEventName()));

	if(strcmp(buffer, "SpawnLightWave") == 0)
	{
		if(m_bTutorial)
			m_bTutorial = false;

		if(m_nPlayState == SPLIT)
		{
			if(!CGame::GetInstance()->GetMutliplayer())
			{
				if(m_pPlayer->GetPosition().z < 30.0f)
					CSpawner::GetInstance()->RunScript("BasicLightWave", false, NORMAL_WAVE);
				else
					CSpawner::GetInstance()->RunScript("BasicLightWave", true, NORMAL_WAVE);
			}
			else
			{
				CSpawner::GetInstance()->RunScript("BasicLightWave", true, NORMAL_WAVE);
				CSpawner::GetInstance()->RunScript("BasicLightWave", false, NORMAL_WAVE);
			}
		}
		else
		{
			CSpawner::GetInstance()->RunScript("BasicLightWave", false, NORMAL_WAVE);
		}
		return;
	}

	if(strcmp(buffer, "LightLock") == 0)
	{
		if(m_bTutorial)
			m_bTutorial = false;

		/*
		LOCK THE CAM
		*/
		m_fUnlock = 8.0;
		m_Cam->SetState(CCamera::LOCK);
		CSpawner::GetInstance()->SetTrickle(false);
		if(m_nPlayState == SPLIT)
		{
			if(!CGame::GetInstance()->GetMutliplayer())
			{
				if(m_pPlayer->GetPosition().z < 30.0f)
					CSpawner::GetInstance()->RunScript("BasicLightWave", false, LOCK_WAVE);
				else
					CSpawner::GetInstance()->RunScript("BasicLightWave", true, LOCK_WAVE);

				m_nEnemyLockCount = CSpawner::GetInstance()->GetNumberCreated();
			}
			else
			{
				CSpawner::GetInstance()->RunScript("BasicLightWave", true, LOCK_WAVE);
				m_nEnemyLockCount = CSpawner::GetInstance()->GetNumberCreated();
				CSpawner::GetInstance()->RunScript("BasicLightWave", false, LOCK_WAVE);
				m_nEnemyLockCount += CSpawner::GetInstance()->GetNumberCreated();
			}
		}
		else
		{
			CSpawner::GetInstance()->RunScript("BasicLightWave", false, LOCK_WAVE);
			m_nEnemyLockCount = CSpawner::GetInstance()->GetNumberCreated();
		}
		return;
	}

	if(strcmp(buffer, "SpawnMediumWave") == 0)
	{
		if(m_bTutorial)
			m_bTutorial = false;

		if(m_nPlayState == SPLIT)
		{
			if(!CGame::GetInstance()->GetMutliplayer())
			{
				if(m_pPlayer->GetPosition().z < 30.0f)
					CSpawner::GetInstance()->RunScript("BasicMediumWave", false, NORMAL_WAVE);
				else
					CSpawner::GetInstance()->RunScript("BasicMediumWave", true, NORMAL_WAVE);
			}
			else
			{
				CSpawner::GetInstance()->RunScript("BasicMediumWave", true, NORMAL_WAVE);
				CSpawner::GetInstance()->RunScript("BasicMediumWave", false, NORMAL_WAVE);
			}
		}
		else
		{
			CSpawner::GetInstance()->RunScript("BasicMediumWave", false, NORMAL_WAVE);
		}

		return;
	}

	if(strcmp(buffer, "MediumLock") == 0)
	{
		if(m_bTutorial)
			m_bTutorial = false;

		/*
		Add lock stuff here
		*/
		m_fUnlock = 8.0;
		m_Cam->SetState(CCamera::LOCK);
		CSpawner::GetInstance()->SetTrickle(false);
		if(m_nPlayState == SPLIT)
		{
			if(!CGame::GetInstance()->GetMutliplayer())
			{
				if(m_pPlayer->GetPosition().z < 30.0f)
					CSpawner::GetInstance()->RunScript("BasicMediumWave", false, LOCK_WAVE);
				else
					CSpawner::GetInstance()->RunScript("BasicMediumWave", true, LOCK_WAVE);

				m_nEnemyLockCount = CSpawner::GetInstance()->GetNumberCreated();
			}
			else
			{
				CSpawner::GetInstance()->RunScript("BasicMediumWave", true, LOCK_WAVE);
				m_nEnemyLockCount = CSpawner::GetInstance()->GetNumberCreated();
				CSpawner::GetInstance()->RunScript("BasicMediumWave", false, LOCK_WAVE);
				m_nEnemyLockCount += CSpawner::GetInstance()->GetNumberCreated();
			}
		}
		else
		{
			CSpawner::GetInstance()->RunScript("BasicMediumWave", false, LOCK_WAVE);
			m_nEnemyLockCount = CSpawner::GetInstance()->GetNumberCreated();
		}

		return;
	}

	if(strcmp(buffer, "SpawnHeavyWave") == 0)
	{
		if(m_bTutorial)
			m_bTutorial = false;

		if(m_nPlayState == SPLIT)
		{
			if(!CGame::GetInstance()->GetMutliplayer())
			{
				if(m_pPlayer->GetPosition().z < 30.0f)
					CSpawner::GetInstance()->RunScript("BasicHeavyWave", false, NORMAL_WAVE);
				else
					CSpawner::GetInstance()->RunScript("BasicHeavyWave", true, NORMAL_WAVE);
			}
			else
			{
				CSpawner::GetInstance()->RunScript("BasicHeavyWave", true, NORMAL_WAVE);
				CSpawner::GetInstance()->RunScript("BasicHeavyWave", false, NORMAL_WAVE);
			}
		}
		else
		{
			CSpawner::GetInstance()->RunScript("BasicHeavyWave", false, NORMAL_WAVE);
		}
		return;
	}

	if(strcmp(buffer, "HeavyLock") == 0)
	{
		if(m_bTutorial)
			m_bTutorial = false;

		/*
		CODE FOR ANY SORT OF LOCK STUFF HERE
		*/
		m_fUnlock = 8.0;
		m_Cam->SetState(CCamera::LOCK);
		CSpawner::GetInstance()->SetTrickle(false);
		if(m_nPlayState == SPLIT)
		{
			if(!CGame::GetInstance()->GetMutliplayer())
			{
				if(m_pPlayer->GetPosition().z < 30.0f)
					CSpawner::GetInstance()->RunScript("BasicHeavyWave", false, LOCK_WAVE);
				else
					CSpawner::GetInstance()->RunScript("BasicHeavyWave", true, LOCK_WAVE);

				m_nEnemyLockCount = CSpawner::GetInstance()->GetNumberCreated();
			}
			else
			{
				CSpawner::GetInstance()->RunScript("BasicHeavyWave", true, LOCK_WAVE);
				m_nEnemyLockCount = CSpawner::GetInstance()->GetNumberCreated();
				CSpawner::GetInstance()->RunScript("BasicHeavyWave", false, LOCK_WAVE);
				m_nEnemyLockCount += CSpawner::GetInstance()->GetNumberCreated();
			}
		}
		else
		{
			CSpawner::GetInstance()->RunScript("BasicHeavyWave", false, LOCK_WAVE);
			m_nEnemyLockCount = CSpawner::GetInstance()->GetNumberCreated();
		}
		return;
	}

	if(strcmp(buffer, "LockClear") == 0)
	{
		/* Put code here to display the GO GO GO sign. */
		m_Cam->SetState(CCamera::FOLLOW);
		CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_DX_ANNOUNCER_GOGOGO, 200);
	}

	if(strcmp(buffer, "WinEvent") == 0)
	{
		m_Cam->ShakeCamera(0.0f);
		m_Cam->Update(0.015f);

		m_pPlayer->SetIsActive(false);
		if(CGame::GetInstance()->GetMutliplayer())
		{
			m_pPlayer2->SetIsActive(false);
		}
		m_bWin = true;
		CGame::GetInstance()->SetMultiplayer(m_startsAsMultiPlayer);
		bool check  = CGame::GetInstance()->GetMutliplayer();
		check = check;

		return;
	}

	if(strcmp(buffer, "LoseEvent") == 0)
	{
		m_Cam->ShakeCamera(0.0f);
		m_Cam->Update(0.015f);

		//CStateSystem::GetInstance()->AddState(CGameOverState::GetInstance());
		if(!CGame::GetInstance()->GetMutliplayer() &&
			m_pPlayer->GetNumLives() <= 0 && m_pPlayer->GetCurrentHealth() <= 0)
		{
			m_bGameOver = true;
			CGame::GetInstance()->SetMultiplayer(m_startsAsMultiPlayer);
			bool check  = CGame::GetInstance()->GetMutliplayer();
			check = check;
		}
		else if(m_pPlayer->GetNumLives() <= 0 && m_pPlayer->GetCurrentHealth() <= 0 &&
			m_pPlayer2->GetNumLives() <= 0 && m_pPlayer2->GetCurrentHealth() <= 0)
		{
			m_bGameOver = true;
			CGame::GetInstance()->SetMultiplayer(m_startsAsMultiPlayer);
			bool check  = CGame::GetInstance()->GetMutliplayer();
			check = check;
		}
		return;
	}

	if(strcmp(buffer, "IntroCompleted") == 0)
	{
		char* candy = "whatever";
		CEventSystem::GetInstance()->DispatchEvent(*m_eCameraFollow, NULL, 1.0f);
		m_bIntroDone = true;
		return;
	}

	if(strcmp(buffer, "SplitEvent") == 0 && GetPlayState() != SPLIT)
	{
		m_fCrateTimer = 0.0f;
		if(CGame::GetInstance()->GetMutliplayer())
		{
			m_fSplitUp = 2.0f;
			CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_DX_ANNOUNCER_SPLITUP, 200);
		}
	}

	if(strcmp(buffer, "MergeEvent") == 0)
	{
		m_fSpawnTimer = 0.0f;
		m_fCrateTimer = 0.0f;
		if(CGame::GetInstance()->GetMutliplayer())
		{
			m_fRegroup = 2.0f;
			CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_DX_ANNOUNCER_REGROUP, 200);
		}
	}

	if(strcmp(buffer, "ShootingGal") == 0)
	{
		CreateShootingGallery(4, CCamera::GetInstance()->GetViewPosition());
		char* candyBuff = "whatever";
		CEventSystem::GetInstance()->DispatchEvent(*m_eCameraLock, NULL, 1.0f);
		m_fUnlock = 8.0;
		if(CGame::GetInstance()->GetMutliplayer())
		{
			m_fShootEverything = 2.0f;
			CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_DX_ANNOUNCER_KILLEVERYTHING, 200);
		}
	}

	if(strcmp(buffer, "RainDebris") == 0)
	{
		m_bRainDebris = true;
		m_nDebrisOffset = 0;
		m_fDebrisTimer = 0.0f;
	}

	if(strcmp(buffer, "StopDebris") == 0)
	{
		m_bRainDebris = false;
		m_nDebrisOffset = 0;
		m_fDebrisTimer = 0.0f;
	}

	if(strcmp(buffer, "SpawnBoss") == 0)
	{
		CUltimus* theMOTHERFUCKINGboss = (CUltimus*)(CObjectFactory::GetInstance()->GetObject<CUltimus>(OBJ_ULTIMUS));

		theMOTHERFUCKINGboss->Init(D3DXVECTOR3(CCamera::GetInstance()->GetViewPosition().x+150, BOSS_YEAH, 0),false,600);
		//theMOTHERFUCKINGboss->SetRotationY(-D3DX_PI * 0.5f);
		CObjectManager::GetInstance()->AddObject(theMOTHERFUCKINGboss);

		CEvent* toDispatch = CEventSystem::GetInstance()->FindEvent("LockCamera");
		CEventSystem::GetInstance()->DispatchEvent(*toDispatch,NULL,1.0);
	}
}

void CGameState::CreatePlayer2()
{
	AK::MotionEngine::AddPlayerMotionDevice(1, AKCOMPANYID_AUDIOKINETIC, AKMOTIONDEVICEID_RUMBLE);
	AK::MotionEngine::SetPlayerListener(1, 0);
	if(CCharacterSelectionState::GetInstance()->GetSelection() == 0)
	{
		m_pPlayer2 = (CClaire*)(CObjectFactory::GetInstance()->GetObject<CClaire>(OBJ_CLAIRE));
		m_pPlayer2->SetType(OBJ_CLAIRE);
	}
	else
	{
		m_pPlayer2 = (CBarron*)(CObjectFactory::GetInstance()->GetObject<CBarron>(OBJ_BARRON));
		m_pPlayer2->SetType(OBJ_BARRON);
	}

	CCamera::GetInstance()->SetPlayerTwo(m_pPlayer2);
	m_pPlayer2->SetIsActive(true);
	if(m_nPlayState == SPLIT)
	{
		D3DXVECTOR3 temp;
		temp = m_pPlayer->GetPosition();
		temp.y+=100;
		temp.z+=60;
		m_pPlayer2->SetPosition(temp);
	}
	else
	{
		m_pPlayer2->SetPosition(m_pPlayer->GetPosition());
	}

	m_pPlayer2->SetVelocity(D3DXVECTOR3(0, 0, 0));
	m_pPlayer2->SetRotationY(-D3DX_PI*0.5);
	m_pPlayer2->SetHud(&hud2);
	switch(CGame::GetInstance()->GetDifficulty())
	{
	case 0:
		m_pPlayer2->SetNumLives(6);
		break;
	case 1:
		m_pPlayer2->SetNumLives(3);
		break;
	case 2:
		m_pPlayer2->SetNumLives(1);
		break;
	}
	m_pPlayer2->SetCurrentHealth(3);
	m_pPlayer2->GetHud()->SetHealth((int)m_pPlayer->GetCurrentHealth());
	m_pPlayer2->GetHud()->SetLives(m_pPlayer->GetNumLives());
	m_pPlayer2->SetPlayerNum(1);

	//////////////////////////////////////////////////////////////////////////
	// add 2nd gun
	//m_pPlayer2->AddShape(gun, &CRenderContextManager::GetInstance()->GetStaticLitContext(), gunMat, CRenderShape::PNTRenderFunc, false );
	//m_pPlayer2->GetShape(1).GetFrame().SetParent(&m_pPlayer2->GetFrame());
	//////////////////////////////////////////////////////////////////////////
	CGame::GetInstance()->SetMultiplayer(true);
	m_startsAsMultiPlayer = true;

	CObjectManager::GetInstance()->AddObject(m_pPlayer2);

	CEventSystem::GetInstance()->DispatchEvent(*m_eShowPlayerID, NULL, 1.0f);

	CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_DX_ANNOUNCER_PLAYERTWOENTERED, 200);
}

void CGameState::CreatePlayer()
{
	if(CCharacterSelectionState::GetInstance()->GetSelection() == 0)
	{
		m_pPlayer = (CBarron*)(CObjectFactory::GetInstance()->GetObject<CBarron>(OBJ_BARRON));
		m_pPlayer->SetType(OBJ_BARRON);
	}
	else
	{
		m_pPlayer = (CClaire*)(CObjectFactory::GetInstance()->GetObject<CClaire>(OBJ_CLAIRE));
		m_pPlayer->SetType(OBJ_CLAIRE);
		m_bIsBarron = false;
	}

	D3DXVECTOR3 pos;
	m_pPlayer->SetIsActive(true);

	D3DXMATRIX tempWorld = mxWorld;
	D3DXMATRIX rotMat;
	D3DXMATRIX scaleMat;

	D3DXMatrixIdentity(&rotMat);
	D3DXMatrixIdentity(&scaleMat);

	D3DXVECTOR3 temp;
	CLuaEngine::GetInstance()->GetData(temp,"playerStart");
	m_pPlayer->SetPosition(temp);
	m_pPlayer->SetVelocity(D3DXVECTOR3(0, 0, 0));
	m_pPlayer->SetRotationY(-D3DX_PI*0.5);
	m_pPlayer->SetHud(&hud);
	m_pPlayer->SetCurrentHealth(3);
	switch(CGame::GetInstance()->GetDifficulty())
	{
	case 0:
		m_pPlayer->SetNumLives(6);
		break;
	case 1:
		m_pPlayer->SetNumLives(3);
		break;
	case 2:
		m_pPlayer->SetNumLives(1);
		break;
	}

	m_pPlayer->GetHud()->SetHealth((int)m_pPlayer->GetCurrentHealth());
	m_pPlayer->GetHud()->SetLives(m_pPlayer->GetNumLives());
	m_pPlayer->SetPlayerNum(0);

	// Setting up Rumble Feedback Player 1 Only (for now)
	AK::MotionEngine::AddPlayerMotionDevice(0, AKCOMPANYID_AUDIOKINETIC, AKMOTIONDEVICEID_RUMBLE);
	AK::MotionEngine::SetPlayerListener(0, 0);
	AK::SoundEngine::SetListenerPipeline(0, true, true);

	//MY TIME WILL NEVER COME, MY TIME IS IMMORTAL - TJ
	//PS, narhaK is still a giant jerk face.

	bool test = CGame::GetInstance()->GetMutliplayer();
	//Player 2
	if(CGame::GetInstance()->GetMutliplayer())
	{
		AK::MotionEngine::AddPlayerMotionDevice(1, AKCOMPANYID_AUDIOKINETIC, AKMOTIONDEVICEID_RUMBLE);
		AK::MotionEngine::SetPlayerListener(1, 0);
		if(CCharacterSelectionState::GetInstance()->GetSelection() == 0)
		{
			m_pPlayer2 = (CClaire*)(CObjectFactory::GetInstance()->GetObject<CClaire>(OBJ_CLAIRE));
			m_pPlayer2->SetType(OBJ_CLAIRE);
		}
		else
		{
			m_pPlayer2 = (CBarron*)(CObjectFactory::GetInstance()->GetObject<CBarron>(OBJ_BARRON));
			m_pPlayer2->SetType(OBJ_BARRON);
		}

		CCamera::GetInstance()->SetPlayerTwo(m_pPlayer2);
		m_pPlayer2->SetIsActive(true);
		m_pPlayer2->SetPosition(temp);
		m_pPlayer2->SetVelocity(D3DXVECTOR3(0, 0, 0));
		m_pPlayer2->SetRotationY(-D3DX_PI*0.5);
		m_pPlayer2->SetHud(&hud2);
		switch(CGame::GetInstance()->GetDifficulty())
		{
		case 0:
			m_pPlayer2->SetNumLives(6);
			break;
		case 1:
			m_pPlayer2->SetNumLives(3);
			break;
		case 2:
			m_pPlayer2->SetNumLives(1);
			break;
		}
		m_pPlayer2->SetCurrentHealth(3);
		m_pPlayer2->GetHud()->SetHealth((int)m_pPlayer2->GetCurrentHealth());
		m_pPlayer2->GetHud()->SetLives(m_pPlayer2->GetNumLives());
		m_pPlayer2->SetPlayerNum(1);

		//////////////////////////////////////////////////////////////////////////
		// add 2nd gun
		//m_pPlayer2->AddShape(gun, &CRenderContextManager::GetInstance()->GetStaticLitContext(), gunMat, CRenderShape::PNTRenderFunc, false );
		//m_pPlayer2->GetShape(1).GetFrame().SetParent(&m_pPlayer2->GetFrame());
		//////////////////////////////////////////////////////////////////////////

		CObjectManager::GetInstance()->AddObject(m_pPlayer2);
	}
	else
	{
#ifdef TESTINGAI
		if(m_bIsBarron)
		{
			m_pPlayer2 = (CClaire*)(CObjectFactory::GetInstance()->GetObject<CClaire>(OBJ_CLAIRE));
			m_pPlayer2->SetType(OBJ_CLAIRE);
		}
		else
		{
			m_pPlayer2 = (CBarron*)(CObjectFactory::GetInstance()->GetObject<CBarron>(OBJ_BARRON));
			m_pPlayer2->SetType(OBJ_BARRON);
		}

		CCamera::GetInstance()->SetPlayerTwo(m_pPlayer2);
		m_pPlayer2->SetIsActive(true);
		m_pPlayer2->SetPosition(temp.x - 50.0f,temp.y,temp.z);
		m_pPlayer2->SetVelocity(D3DXVECTOR3(0, 0, 0));
		m_pPlayer2->SetRotationY(-D3DX_PI*0.5);
		m_pPlayer2->SetHud(&hud2);
		m_pPlayer2->SetCurrentHealth(3);
		m_pPlayer2->SetNumLives(3);
		m_pPlayer2->GetHud()->SetHealth(m_pPlayer->GetCurrentHealth());
		m_pPlayer2->GetHud()->SetLives(m_pPlayer->GetNumLives());
		m_pPlayer2->SetPlayerNum(1);
		m_pPlayer2->InitializeAI();
		switch(CGame::GetInstance()->GetDifficulty())
		{
		case 0:
			m_pPlayer2->SetCurrentHealth(6);
			break;
		case 1:
			m_pPlayer2->SetCurrentHealth(3);
			break;
		case 2:
			m_pPlayer2->SetCurrentHealth(2);
			break;
		}
		m_pPlayer2->SetCurrentTarget(m_pPlayer);

		CObjectManager::GetInstance()->AddObject(m_pPlayer2);

#endif
	}

	CObjectManager::GetInstance()->AddObject(m_pPlayer);
 	m_pBossMan = (CUltimus*)(CObjectFactory::GetInstance()->GetObject<CUltimus>(OBJ_ULTIMUS));
 	m_pBossMan->Init(D3DXVECTOR3(9770.0f,BOSS_YEAH,0.0f),false,200);
 	CObjectManager::GetInstance()->AddObject(m_pBossMan);
#ifndef TESTINGBOSS
	/*
	CStriker* m_Striker = (CStriker*)(CObjectFactory::GetInstance()->GetObject<CStriker>(OBJ_STRIKER));

	m_Striker->Init(D3DXVECTOR3(50, 0, 0));
	CObjectManager::GetInstance()->AddObject(m_Striker);*/

#endif

#ifdef TESTINGBOSS
	CUltimus* theMOTHERFUCKINGboss = (CUltimus*)(CObjectFactory::GetInstance()->GetObject<CUltimus>(OBJ_ULTIMUS));

	theMOTHERFUCKINGboss->Init(D3DXVECTOR3(-220, 0, 0),false,50);
	theMOTHERFUCKINGboss->SetRotationY(-D3DX_PI * 0.5f);
	CObjectManager::GetInstance()->AddObject(theMOTHERFUCKINGboss);
#endif
}

void CGameState::CreateTutorialEnemies(void)
{
#ifdef TESTINGBOSS
	return;
#endif

	CStriker* striker = (CStriker*)(CObjectFactory::GetInstance()->GetObject<CStriker>(OBJ_STRIKER));
	striker->Init(D3DXVECTOR3(m_Cam->GetViewPosition().x, m_pPlayer->GetPosition().y, 0.0f), true);

	CStriker* striker2 = (CStriker*)(CObjectFactory::GetInstance()->GetObject<CStriker>(OBJ_STRIKER));
	striker2->Init(D3DXVECTOR3(m_Cam->GetViewPosition().x + 10, m_pPlayer->GetPosition().y, 0.0f), true);

	CStriker* striker3 = (CStriker*)(CObjectFactory::GetInstance()->GetObject<CStriker>(OBJ_STRIKER));
	striker3->Init(D3DXVECTOR3(m_Cam->GetViewPosition().x + 20, m_pPlayer->GetPosition().y, 0.0f), true);

	CObjectManager::GetInstance()->AddObject(striker);
	CObjectManager::GetInstance()->AddObject(striker2);
	CObjectManager::GetInstance()->AddObject(striker3);
}

void CGameState::CreateShootingGallery(int numEnemiesToMake, D3DXVECTOR3 initialPos)
{
		m_fUnlock = 8.0;
	if(CGame::GetInstance()->GetMutliplayer())
	{
		if(m_nLevelArea == JUNGLE)
			m_nGalleryCount = 2;

		if(m_nGalleryCount == 1)
		{
			m_nShootingGalleryEnemiesAliveNear = MULT_FIRST_GAL_AMOUNT;
			m_nShootingGalleryEnemiesAliveFar = MULT_FIRST_GAL_AMOUNT;
		}
		else if(m_nGalleryCount == 2)
		{
			m_nShootingGalleryEnemiesAliveNear = MULT_SECOND_GAL_AMOUNT;
			m_nShootingGalleryEnemiesAliveFar = MULT_SECOND_GAL_AMOUNT;
		}

		m_bShootingGal = true;
		m_bAugmentOut = false;

		CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_DX_ANNOUNCER_KILLEVERYTHING, 200);

		char buffer[16];
		sprintf(buffer, "ShootingGal%i", m_nGalleryCount);
		CSpawner::GetInstance()->RunScript(buffer, true, GAL_WAVE);
		CSpawner::GetInstance()->RunScript(buffer, false, GAL_WAVE);
		CSpawner::GetInstance()->SetTrickle(false);
	}
	else
	{
		if(m_nLevelArea == JUNGLE)
			m_nGalleryCount = 2;

		if(m_nGalleryCount == 1)
		{
			m_nShootingGalleryEnemiesAliveNear = SOLO_FIRST_GAL_AMOUNT;
		}
		else if(m_nGalleryCount == 2)
		{
			m_nShootingGalleryEnemiesAliveNear = SOLO_SECOND_GAL_AMOUNT;
		}

		m_bShootingGal = true;
		m_bAugmentOut = false;

		CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_DX_ANNOUNCER_KILLEVERYTHING, 200);

		char buffer[16];
		sprintf(buffer, "SoloGal%i", m_nGalleryCount);

		if(m_pPlayer->GetPosition().z > 30.0f)
			CSpawner::GetInstance()->RunScript(buffer, true, GAL_WAVE);
		else
			CSpawner::GetInstance()->RunScript(buffer, false, GAL_WAVE);

		CSpawner::GetInstance()->SetTrickle(false);
	}
}

void CGameState::PlayPlayerSwitch()
{
	if( ( hud.GetScore() > hud2.GetScore() ) && (m_nPrevPlayer1Score < m_nPrevPlayer2Score) )
	{
		m_bAudioPlayPlayerScoreSwitch = true;
		m_bAudioPlayScore1 = true;
	}
	else if( ( hud2.GetScore() > hud.GetScore() ) && (m_nPrevPlayer2Score < m_nPrevPlayer1Score) )
	{
		m_bAudioPlayPlayerScoreSwitch = true;
		m_bAudioPlayScore2 = true;
	}

	m_nPrevPlayer1Score = hud.GetScore();
	m_nPrevPlayer2Score	= hud2.GetScore();
}