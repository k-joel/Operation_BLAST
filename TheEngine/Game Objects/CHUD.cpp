#include "CHUD.h"
#include "../Rendering/CTextureManager.h"
#include "../Font/CBitMapFont.h"
#include "../AI System/CAIStateMachine.h"
#include "../AI System/CState.h"
#include "../Mesh/modelImporter.h"
#include "../Rendering/CRenderer.h"
#include "../Rendering/CShaderManager.h"
#include "../Rendering/CRenderContextManager.h"
#include "../States/CGameState.h"
#include "../States/CLoadState.h"
#include "../Input/CInput.h"
#include "../Mesh/CModelManager.h"
#include "../Core/CGame.h"
#include "../FX/CParticleManager.h"
#include "../Camera/Camera.h"

#include "../Sound/WwiseManager.h"
#include "../Mesh/CModel.h"

#include "../Rendering/CDirect3D.h"

#include "CCharacter.h"

#include "CCharacter.h"
#include <vector>
#include <stdlib.h>
using namespace std;

CHUD::CHUD()
{
	m_nScore = 0;
	m_nHealth = 0;
	m_nLives = 0;
	m_fMaxMultiBar = 0;
	m_fCurMultiBar = 0;
	m_fBucket = 0.0f;
	m_fMultiplyer = 1.0f;
	m_nFlatMultiplyer = 1;
	m_nPlayerType = PLAYER1;
	m_fMultiplyerScale = 0.5f;
	m_nWeaponType = HMACHINE_GUN;
	m_fUltScale = 0.0f;
	m_nAmmo = 0;
	m_nPlayKillCount = 0;
	m_bAudioPlayKillSpree = false;
	m_bAudioPlayKillPlayer1 = false;
	m_bAudioPlayKillPlayer2 = false;

	for(unsigned int i = 0; i < 10; i++)
	{
		szScore[i] = '0';

		szScoreDial[i] = '0';
	}

	created = false;
}

CHUD::~CHUD()
{
}

void CHUD::Initialize()
{
	// Reset All Parameters for the HUD

	for(unsigned int i = 0; i < 10; i++)
	{
		D3DXMatrixIdentity(&m_Dials[i]);
		m_nDialValues[i] = 0;
	}

	for(unsigned int i = 0; i < 3; i++)
	{
		D3DXMatrixIdentity(&m_Heart[i]);
		//D3DXMatrixIdentity(&m_mHUDSlider[i]);
	}

	
	D3DXMatrixIdentity(&m_Lives);
	
	for(unsigned int i = 0; i < 4; i++)
	{
		D3DXMatrixIdentity(&m_Multi[i]);
	}
	D3DXMatrixIdentity(&m_MultiView);
	D3DXMatrixIdentity(&m_MultiRotation);
	D3DXMatrixIdentity(&m_MultiScale);
	

	D3DXMatrixIdentity(&m_mWeaponCard);
	D3DXMatrixIdentity(&m_mHUDUltBar);
	D3DXMatrixIdentity(&m_mHUDUltCase);
	D3DXMatrixIdentity(&m_Frame);
	D3DXMatrixIdentity(&m_mHUDPortrait);
	D3DXMatrixIdentity(&m_mUltParticle);

	/*D3DXMatrixIdentity(&m_mHUDMultiplier[0]);
	D3DXMatrixIdentity(&m_mHUDMultiplier[1]);*/

	if(m_nPlayerType == PLAYER1)
	{
		m_nPlayerPositions = 1;
	}
	else
	{
		m_nPlayerPositions = -1;
	}

	D3DXMatrixIdentity(&m_mHUDPortraitDeath);
	D3DXMatrixIdentity(&m_mHUDWeaponCardDeath);

	// Reset the view, rotation, scale
	D3DXMatrixIdentity(&m_View);
	D3DXMatrixIdentity(&m_Rotation);
	D3DXMatrixIdentity(&m_Scale);
	//D3DXMatrixOrthoLH(&m_View,1,1,-1,-1);
	//D3DXMatrixTranslation(&m_View,0,0,-20);

	 m_nScore = 0;
	 m_nHealth = 0;
	 m_nLives = 0;
	 m_fMaxMultiBar = 100;
	 m_fCurMultiBar = 100;
	 m_fMultiplyer = 1.0f;

	 m_BFC = CBitMapFont::GetInstance();
	 m_TMC = CTextureManager::GetInstance();

	 D3DXMATRIX set;
	 D3DXMatrixIdentity(&set);

	if(m_nPlayerType == PLAYER1)
	{
		//m_consoleScreen = CModelManager::GetInstance()->GetModel(CLoadState::GetInstance()->GetModelID("PauseMenuScreenShape"));
		//m_ConsoleShape.Init(&m_consoleScreen->GetMesh(), NULL, &CRenderContextManager::GetInstance()->GetHUDContext(), m_ConsoleWorld, CRenderShape::PNTRenderFunc);
		
		//Score
		m_HUDDial = CModelManager::GetInstance()->GetModel(CLoadState::GetInstance()->GetModelID("Dial_scoreShape"));
		for(unsigned int i = 0; i < 10; i++)
		{
			m_HUDDialShape[i].Init(&m_HUDDial->GetMesh(),NULL,&CRenderContextManager::GetInstance()->GetHUDContext(),set,CRenderShape::PNTRenderFunc);
		}
	
		//Health
		m_HUDHeart = CModelManager::GetInstance()->GetModel(CLoadState::GetInstance()->GetModelID("Health_heartsShape"));
		for(unsigned int i = 0; i < 3; i++)
		{
			m_HUDHeartShape[i].Init(&m_HUDHeart->GetMesh(),NULL,&CRenderContextManager::GetInstance()->GetHUDContext(),set,CRenderShape::PNTRenderFunc);
		}

		//Hud Frame
		m_HUDFrame = CModelManager::GetInstance()->GetModel(CLoadState::GetInstance()->GetModelID("HUD_mainP1Shape"));
		m_HUDFrameShape.Init(&m_HUDFrame->GetMesh(),NULL,&CRenderContextManager::GetInstance()->GetHUDContext(),set,CRenderShape::PNTRenderFunc);

		//Health Slider 
		//m_HUDHealthSlider = CModelManager::GetInstance()->GetModel(CLoadState::GetInstance()->GetModelID("P1_HealthSliderShape"));
		//for(unsigned int i = 0; i < 3; i++)
		//{
		//	m_HUDHealthSliderShape[i].Init(&m_HUDHealthSlider->GetMesh(),NULL,&CRenderContextManager::GetInstance()->GetHUDContext(),set,CRenderShape::PNTRenderFunc);
		//}

		//Lives
		m_HUDLife[0] = CModelManager::GetInstance()->GetModel(CLoadState::GetInstance()->GetModelID("Lives_0xShape"));
		m_HUDLife[1] = CModelManager::GetInstance()->GetModel(CLoadState::GetInstance()->GetModelID("Lives_1xShape"));
		m_HUDLife[2] = CModelManager::GetInstance()->GetModel(CLoadState::GetInstance()->GetModelID("Lives_2xShape"));
		m_HUDLife[3] = CModelManager::GetInstance()->GetModel(CLoadState::GetInstance()->GetModelID("Lives_3xShape"));

		for(unsigned int i = 0; i < 4; i++)
		{
			m_HUDLifeShape[i].Init(&m_HUDLife[i]->GetMesh(),NULL,&CRenderContextManager::GetInstance()->GetHUDContext(),set,CRenderShape::PNTRenderFunc);
		}

		//Multiplier Wheel
		m_HUDMulti = CModelManager::GetInstance()->GetModel(CLoadState::GetInstance()->GetModelID("Dial_MultiShape"));
		m_HUDMultiShape.Init(&m_HUDMulti->GetMesh(),NULL,&CRenderContextManager::GetInstance()->GetHUDContext(),set,CRenderShape::PNTRenderFunc);

		/*m_HUDMultiplierBack = CModelManager::GetInstance()->GetModel(CLoadState::GetInstance()->GetModelID("P1_MultiplierBackPlaneShape"));
		m_HUDMultiplierBackShape.Init(&m_HUDMultiplierBack->GetMesh(),NULL,&CRenderContextManager::GetInstance()->GetHUDContext(),set,CRenderShape::PNTRenderFunc);

		m_HUDMultiplierFront = CModelManager::GetInstance()->GetModel(CLoadState::GetInstance()->GetModelID("P1_MultiplierFrontPlaneShape"));
		m_HUDMultiplierFrontShape.Init(&m_HUDMultiplierFront->GetMesh(),NULL,&CRenderContextManager::GetInstance()->GetHUDContext(),set,CRenderShape::PNTRenderFunc);*/

		//Weapon Cards
		m_HUDWeaponCard[HMACHINE_GUN] = CModelManager::GetInstance()->GetModel(CLoadState::GetInstance()->GetModelID("Weapon_machinegunShape"));
		m_HUDWeaponCardShape[HMACHINE_GUN].Init(&m_HUDWeaponCard[HMACHINE_GUN]->GetMesh(),NULL,&CRenderContextManager::GetInstance()->GetHUDContext(),set,CRenderShape::PNTRenderFunc);

		m_HUDWeaponCard[HSHOTGUN] = CModelManager::GetInstance()->GetModel(CLoadState::GetInstance()->GetModelID("Weapon_shotgunShape"));
		m_HUDWeaponCardShape[HSHOTGUN].Init(&m_HUDWeaponCard[HSHOTGUN]->GetMesh(),NULL,&CRenderContextManager::GetInstance()->GetHUDContext(),set,CRenderShape::PNTRenderFunc);
		
		m_HUDWeaponCard[HFLAMETHROWER] = CModelManager::GetInstance()->GetModel(CLoadState::GetInstance()->GetModelID("Weapon_flamethrowerShape"));
		m_HUDWeaponCardShape[HFLAMETHROWER].Init(&m_HUDWeaponCard[HFLAMETHROWER]->GetMesh(),NULL,&CRenderContextManager::GetInstance()->GetHUDContext(),set,CRenderShape::PNTRenderFunc);

		m_HUDWeaponCard[HLIGHTNING_GUN] = CModelManager::GetInstance()->GetModel(CLoadState::GetInstance()->GetModelID("Weapon_lightningShape"));
		m_HUDWeaponCardShape[HLIGHTNING_GUN].Init(&m_HUDWeaponCard[HLIGHTNING_GUN]->GetMesh(),NULL,&CRenderContextManager::GetInstance()->GetHUDContext(),set,CRenderShape::PNTRenderFunc);

		m_HUDWeaponCard[HROCKET_LAUNCHER] = CModelManager::GetInstance()->GetModel(CLoadState::GetInstance()->GetModelID("Weapon_RocketlauncherShape"));
		m_HUDWeaponCardShape[HROCKET_LAUNCHER].Init(&m_HUDWeaponCard[HROCKET_LAUNCHER]->GetMesh(),NULL,&CRenderContextManager::GetInstance()->GetHUDContext(),set,CRenderShape::PNTRenderFunc);

		//Weapon Case
		m_UltimateCase = CModelManager::GetInstance()->GetModel(CLoadState::GetInstance()->GetModelID("HUD_specialShape"));
		m_UltimateCaseShape.Init(&m_UltimateCase->GetMesh(),NULL,&CRenderContextManager::GetInstance()->GetHUDContext(),set,CRenderShape::PNTRenderFunc);

		//Portrait
		if(m_nPortraitType == 0)
		{
			m_HUDPortrait = CModelManager::GetInstance()->GetModel(CLoadState::GetInstance()->GetModelID("Portrait_barronShape"));
			m_HUDPortraitShape.Init(&m_HUDPortrait->GetMesh(),NULL,&CRenderContextManager::GetInstance()->GetHUDContext(),set,CRenderShape::PNTRenderFunc);
		}
		else
		{
			m_HUDPortrait = CModelManager::GetInstance()->GetModel(CLoadState::GetInstance()->GetModelID("Portrait_claireShape"));
			m_HUDPortraitShape.Init(&m_HUDPortrait->GetMesh(),NULL,&CRenderContextManager::GetInstance()->GetHUDContext(),set,CRenderShape::PNTRenderFunc);
		}
		
		CModel* model;
		model = CModelManager::GetInstance()->GetModel(CLoadState::GetInstance()->GetModelID("P1Ultimate"));
		UltimateBarShape.Init(&model->GetMesh(),NULL,&CRenderContextManager::GetInstance()->GetHUDContext(),set,CRenderShape::PNTRenderFunc);


		/*m_UltimateBar = CModelManager::GetInstance()->GetModel(CLoadState::GetInstance()->GetModelID("P1_UltimateBarIndicatorShape"));
		m_UltimateBarShape.Init(&m_UltimateBar->GetMesh(),NULL,&CRenderContextManager::GetInstance()->GetHUDContext(),set,CRenderShape::PNTRenderFunc);*/

	}
	else if(m_nPlayerType == PLAYER2)
	{
		//m_consoleScreen = CModelManager::GetInstance()->GetModel(CLoadState::GetInstance()->GetModelID("PauseMenuScreenShape"));
		//m_ConsoleShape.Init(&m_consoleScreen->GetMesh(), NULL, &CRenderContextManager::GetInstance()->GetHUDContext(), m_ConsoleWorld, CRenderShape::PNTRenderFunc);
		
		//Score
		m_HUDDial = CModelManager::GetInstance()->GetModel(CLoadState::GetInstance()->GetModelID("Dial_scoreShape"));
		for(unsigned int i = 0; i < 10; i++)
		{
			m_HUDDialShape[i].Init(&m_HUDDial->GetMesh(),NULL,&CRenderContextManager::GetInstance()->GetHUDContext(),set,CRenderShape::PNTRenderFunc);
		}
	
		//Health
		m_HUDHeart = CModelManager::GetInstance()->GetModel(CLoadState::GetInstance()->GetModelID("Health_heartP2Shape"));
		for(unsigned int i = 0; i < 3; i++)
		{
			m_HUDHeartShape[i].Init(&m_HUDHeart->GetMesh(),NULL,&CRenderContextManager::GetInstance()->GetHUDContext(),set,CRenderShape::PNTRenderFunc);
		}

		//Hud Frame
		m_HUDFrame = CModelManager::GetInstance()->GetModel(CLoadState::GetInstance()->GetModelID("HUD_mainP2Shape"));
		m_HUDFrameShape.Init(&m_HUDFrame->GetMesh(),NULL,&CRenderContextManager::GetInstance()->GetHUDContext(),set,CRenderShape::PNTRenderFunc);

		//Health Slider 
		//m_HUDHealthSlider = CModelManager::GetInstance()->GetModel(CLoadState::GetInstance()->GetModelID("P1_HealthSliderShape"));
		//for(unsigned int i = 0; i < 3; i++)
		//{
		//	m_HUDHealthSliderShape[i].Init(&m_HUDHealthSlider->GetMesh(),NULL,&CRenderContextManager::GetInstance()->GetHUDContext(),set,CRenderShape::PNTRenderFunc);
		//}

		//Lives
		m_HUDLife[0] = CModelManager::GetInstance()->GetModel(CLoadState::GetInstance()->GetModelID("Lives_0xShape"));
		m_HUDLife[1] = CModelManager::GetInstance()->GetModel(CLoadState::GetInstance()->GetModelID("Lives_1xShape"));
		m_HUDLife[2] = CModelManager::GetInstance()->GetModel(CLoadState::GetInstance()->GetModelID("Lives_2xShape"));
		m_HUDLife[3] = CModelManager::GetInstance()->GetModel(CLoadState::GetInstance()->GetModelID("Lives_3xShape"));

		//Multiplier Wheel
		m_HUDMulti = CModelManager::GetInstance()->GetModel(CLoadState::GetInstance()->GetModelID("Dial_MultiShape"));
		m_HUDMultiShape.Init(&m_HUDMulti->GetMesh(),NULL,&CRenderContextManager::GetInstance()->GetHUDContext(),set,CRenderShape::PNTRenderFunc);

		for(unsigned int i = 0; i < 4; i++)
		{
			m_HUDLifeShape[i].Init(&m_HUDLife[i]->GetMesh(),NULL,&CRenderContextManager::GetInstance()->GetHUDContext(),set,CRenderShape::PNTRenderFunc);
		}


		/*m_HUDMultiplierBack = CModelManager::GetInstance()->GetModel(CLoadState::GetInstance()->GetModelID("P1_MultiplierBackPlaneShape"));
		m_HUDMultiplierBackShape.Init(&m_HUDMultiplierBack->GetMesh(),NULL,&CRenderContextManager::GetInstance()->GetHUDContext(),set,CRenderShape::PNTRenderFunc);

		m_HUDMultiplierFront = CModelManager::GetInstance()->GetModel(CLoadState::GetInstance()->GetModelID("P1_MultiplierFrontPlaneShape"));
		m_HUDMultiplierFrontShape.Init(&m_HUDMultiplierFront->GetMesh(),NULL,&CRenderContextManager::GetInstance()->GetHUDContext(),set,CRenderShape::PNTRenderFunc);*/

		//Weapon Cards
		m_HUDWeaponCard[HMACHINE_GUN] = CModelManager::GetInstance()->GetModel(CLoadState::GetInstance()->GetModelID("Weapon_machinegunP2Shape"));
		m_HUDWeaponCardShape[HMACHINE_GUN].Init(&m_HUDWeaponCard[HMACHINE_GUN]->GetMesh(),NULL,&CRenderContextManager::GetInstance()->GetHUDContext(),set,CRenderShape::PNTRenderFunc);

		m_HUDWeaponCard[HSHOTGUN] = CModelManager::GetInstance()->GetModel(CLoadState::GetInstance()->GetModelID("Weapon_shotgunP2Shape"));
		m_HUDWeaponCardShape[HSHOTGUN].Init(&m_HUDWeaponCard[HSHOTGUN]->GetMesh(),NULL,&CRenderContextManager::GetInstance()->GetHUDContext(),set,CRenderShape::PNTRenderFunc);
		
		m_HUDWeaponCard[HFLAMETHROWER] = CModelManager::GetInstance()->GetModel(CLoadState::GetInstance()->GetModelID("Weapon_flamethrowerP2Shape"));
		m_HUDWeaponCardShape[HFLAMETHROWER].Init(&m_HUDWeaponCard[HFLAMETHROWER]->GetMesh(),NULL,&CRenderContextManager::GetInstance()->GetHUDContext(),set,CRenderShape::PNTRenderFunc);

		m_HUDWeaponCard[HLIGHTNING_GUN] = CModelManager::GetInstance()->GetModel(CLoadState::GetInstance()->GetModelID("Weapon_lightningP2Shape"));
		m_HUDWeaponCardShape[HLIGHTNING_GUN].Init(&m_HUDWeaponCard[HLIGHTNING_GUN]->GetMesh(),NULL,&CRenderContextManager::GetInstance()->GetHUDContext(),set,CRenderShape::PNTRenderFunc);

		m_HUDWeaponCard[HROCKET_LAUNCHER] = CModelManager::GetInstance()->GetModel(CLoadState::GetInstance()->GetModelID("Weapon_rocketlauncherP2Shape"));
		m_HUDWeaponCardShape[HROCKET_LAUNCHER].Init(&m_HUDWeaponCard[HROCKET_LAUNCHER]->GetMesh(),NULL,&CRenderContextManager::GetInstance()->GetHUDContext(),set,CRenderShape::PNTRenderFunc);

		//Weapon Case
		m_UltimateCase = CModelManager::GetInstance()->GetModel(CLoadState::GetInstance()->GetModelID("HUD_specialP2Shape"));
		m_UltimateCaseShape.Init(&m_UltimateCase->GetMesh(),NULL,&CRenderContextManager::GetInstance()->GetHUDContext(),set,CRenderShape::PNTRenderFunc);

		//Portrait
		if(m_nPortraitType == 0)
		{
			m_HUDPortrait = CModelManager::GetInstance()->GetModel(CLoadState::GetInstance()->GetModelID("Portrait_barronShape"));
			m_HUDPortraitShape.Init(&m_HUDPortrait->GetMesh(),NULL,&CRenderContextManager::GetInstance()->GetHUDContext(),set,CRenderShape::PNTRenderFunc);
		}
		else
		{
			m_HUDPortrait = CModelManager::GetInstance()->GetModel(CLoadState::GetInstance()->GetModelID("Portrait_claireShape"));
			m_HUDPortraitShape.Init(&m_HUDPortrait->GetMesh(),NULL,&CRenderContextManager::GetInstance()->GetHUDContext(),set,CRenderShape::PNTRenderFunc);
		}

		CModel* model;
		model = CModelManager::GetInstance()->GetModel(CLoadState::GetInstance()->GetModelID("P2Ultimate"));
		UltimateBarShape.Init(&model->GetMesh(),NULL,&CRenderContextManager::GetInstance()->GetHUDContext(),set,CRenderShape::PNTRenderFunc);


		/*m_UltimateBar = CModelManager::GetInstance()->GetModel(CLoadState::GetInstance()->GetModelID("P1_UltimateBarIndicatorShape"));
		m_UltimateBarShape.Init(&m_UltimateBar->GetMesh(),NULL,&CRenderContextManager::GetInstance()->GetHUDContext(),set,CRenderShape::PNTRenderFunc);*/

	}
	
	m_HUDPortraitDeath = CModelManager::GetInstance()->GetModel(CLoadState::GetInstance()->GetModelID("Portrait_deathShape"));
	m_HUDPortraitDeathShape.Init(&m_HUDPortraitDeath->GetMesh(),NULL,&CRenderContextManager::GetInstance()->GetHUDContext(),set,CRenderShape::PNTRenderFunc);

	m_HUDWeaponCardDeath = CModelManager::GetInstance()->GetModel(CLoadState::GetInstance()->GetModelID("Weapon_deathShape"));
	m_HUDWeaponCardDeathShape.Init(&m_HUDWeaponCardDeath->GetMesh(),NULL,&CRenderContextManager::GetInstance()->GetHUDContext(),set,CRenderShape::PNTRenderFunc);

	

	//-----------------------------------------
	//
	// Heart Locations on the Screen
	//
	//-----------------------------------------


	if(m_nPlayerType == PLAYER1)
	{
		D3DXMatrixTranslation(&m_Heart[0],-460.0f*m_nPlayerPositions,240.0f,90.0f);
		D3DXMatrixTranslation(&m_Heart[1],-430.0f*(float)m_nPlayerPositions,240.0f,90.0f);
		D3DXMatrixTranslation(&m_Heart[2],-400.0f*(float)m_nPlayerPositions,240.0f,90.0f);
	}
	else
	{
		D3DXMatrixTranslation(&m_Heart[0],820.0f,240.0f,90.0f);
		D3DXMatrixTranslation(&m_Heart[1],790.0f,240.0f,90.0f);
		D3DXMatrixTranslation(&m_Heart[2],760.0f,240.0f,90.0f);
	}

	//------------------------------------------
	//
	// Portrait Location on the Screen
	//
	//------------------------------------------

	if(m_nPlayerType == PLAYER1 && m_nPortraitType == 0)
	{
		D3DXMatrixTranslation(&m_mHUDPortrait,-460.0f*m_nPlayerPositions,240,100);
	}
	else if(m_nPlayerType == PLAYER1 && m_nPortraitType == 1)
	{
		D3DXMatrixTranslation(&m_mHUDPortrait,-370.0f*m_nPlayerPositions,240,100);
		
	}

	if(m_nPlayerType == PLAYER2 && m_nPortraitType == 0)
	{
		D3DXMatrixTranslation(&m_mHUDPortrait,-370.0f*m_nPlayerPositions,240,100);
	}
	else if(m_nPlayerType == PLAYER2 && m_nPortraitType == 1)
	{
		D3DXMatrixTranslation(&m_mHUDPortrait,-460.0f*m_nPlayerPositions,240,100);
		
	}

	D3DXMatrixIdentity(&m_Scale);
	//D3DXMatrixScaling(&m_Scale,1.5,1.5,1.0);

	//-----------------------------------------------
	//
	// Frame Location on the Screen
	//
	//-----------------------------------------------

	D3DXMatrixIdentity(&m_Scale);

	D3DXMatrixTranslation(&m_Frame,-460.0f*m_nPlayerPositions,240,100);
	//D3DXMatrixScaling(&m_Scale,1.0f,1.0f,1.0f);
	//D3DXMatrixMultiply(&m_Frame,&m_Scale,&m_Frame);

	D3DXMatrixIdentity(&m_Scale);
	//D3DXMatrixScaling(&m_Scale,0.60f,0.50f,0.5f);

	//-----------------------------------------------
	//
	//	Dial Locations on the Screen
	//
	//-----------------------------------------------

	if(m_nPlayerType == PLAYER1)
	{
		D3DXMatrixTranslation(&m_Dials[9],-345*(float)m_nPlayerPositions,317,150);
		D3DXMatrixTranslation(&m_Dials[8],-325*(float)m_nPlayerPositions,317,150);
		D3DXMatrixTranslation(&m_Dials[7],-305*(float)m_nPlayerPositions,317,150);
		D3DXMatrixTranslation(&m_Dials[6],-285*(float)m_nPlayerPositions,317,150);
		D3DXMatrixTranslation(&m_Dials[5],-265*(float)m_nPlayerPositions,317,150);
		D3DXMatrixTranslation(&m_Dials[4],-245*(float)m_nPlayerPositions,317,150);
		D3DXMatrixTranslation(&m_Dials[3],-225*(float)m_nPlayerPositions,317,150);
		D3DXMatrixTranslation(&m_Dials[2],-205*(float)m_nPlayerPositions,317,150);
		D3DXMatrixTranslation(&m_Dials[1],-185*(float)m_nPlayerPositions,317,150);
		D3DXMatrixTranslation(&m_Dials[0],-165*(float)m_nPlayerPositions,317,150);
	}
	else
	{
		D3DXMatrixTranslation(&m_Dials[0],-345*(float)m_nPlayerPositions,317,150);
		D3DXMatrixTranslation(&m_Dials[1],-325*(float)m_nPlayerPositions,317,150);
		D3DXMatrixTranslation(&m_Dials[2],-305*(float)m_nPlayerPositions,317,150);
		D3DXMatrixTranslation(&m_Dials[3],-285*(float)m_nPlayerPositions,317,150);
		D3DXMatrixTranslation(&m_Dials[4],-265*(float)m_nPlayerPositions,317,150);
		D3DXMatrixTranslation(&m_Dials[5],-245*(float)m_nPlayerPositions,317,150);
		D3DXMatrixTranslation(&m_Dials[6],-225*(float)m_nPlayerPositions,317,150);
		D3DXMatrixTranslation(&m_Dials[7],-205*(float)m_nPlayerPositions,317,150);
		D3DXMatrixTranslation(&m_Dials[8],-185*(float)m_nPlayerPositions,317,150);
		D3DXMatrixTranslation(&m_Dials[9],-165*(float)m_nPlayerPositions,317,150);
	}
	
	//------------------------------------------------
	//
	// Dial Location for Multiplyer
	//
	//------------------------------------------------
	if(m_nPlayerType == PLAYER1)
	{
	
		D3DXMatrixTranslation(&m_Multi[0],-135.0f*m_nPlayerPositions,317,150);
		
		D3DXMatrixIdentity(&m_MultiRotation);
		D3DXMatrixTranslation(&m_Multi[1],-135.0f*m_nPlayerPositions,317,150);
		D3DXMatrixRotationX(&m_MultiRotation,D3DXToRadian(90));
		D3DXMatrixMultiply(&m_Multi[1],&m_MultiRotation,&m_Multi[1]);

		D3DXMatrixIdentity(&m_MultiRotation);
		D3DXMatrixTranslation(&m_Multi[2],-135.0f*m_nPlayerPositions,317,150);
		D3DXMatrixRotationX(&m_MultiRotation,D3DXToRadian(180));
		D3DXMatrixMultiply(&m_Multi[2],&m_MultiRotation,&m_Multi[2]);

		D3DXMatrixIdentity(&m_MultiRotation);
		D3DXMatrixTranslation(&m_Multi[3],-135.0f*m_nPlayerPositions,317,150);
		D3DXMatrixRotationX(&m_MultiRotation,D3DXToRadian(270));
		D3DXMatrixMultiply(&m_Multi[3],&m_MultiRotation,&m_Multi[3]);
	}
	else
	{
		D3DXMatrixTranslation(&m_Multi[0],132,317,150);
		
		D3DXMatrixIdentity(&m_MultiRotation);
		D3DXMatrixTranslation(&m_Multi[1],132,317,150);
		D3DXMatrixRotationX(&m_MultiRotation,D3DXToRadian(90));
		D3DXMatrixMultiply(&m_Multi[1],&m_MultiRotation,&m_Multi[1]);

		D3DXMatrixIdentity(&m_MultiRotation);
		D3DXMatrixTranslation(&m_Multi[2],132,317,150);
		D3DXMatrixRotationX(&m_MultiRotation,D3DXToRadian(180));
		D3DXMatrixMultiply(&m_Multi[2],&m_MultiRotation,&m_Multi[2]);

		D3DXMatrixIdentity(&m_MultiRotation);
		D3DXMatrixTranslation(&m_Multi[3],132,317,150);
		D3DXMatrixRotationX(&m_MultiRotation,D3DXToRadian(270));
		D3DXMatrixMultiply(&m_Multi[3],&m_MultiRotation,&m_Multi[3]);
	}

	//------------------------------------------------
	//
	// Death Cards 
	//
	//------------------------------------------------
	D3DXMatrixTranslation(&m_mHUDPortraitDeath,-415.0f*m_nPlayerPositions,318,100);
	D3DXMatrixTranslation(&m_mHUDWeaponCardDeath,-415.0*m_nPlayerPositions,-318,70); //GOOD

	//------------------------------------------------
	//
	// Life Plane
	//
	//-------------------------------------------------
	D3DXMatrixTranslation(&m_Lives,-460.0f*m_nPlayerPositions,240,115);

	//--------------------------------------------------
	//
	// Weapon Cards GOOD
	//
	//--------------------------------------------------

	if(m_nPlayerType == PLAYER1)
	D3DXMatrixTranslation(&m_mWeaponCard,-440.0f*m_nPlayerPositions,-220,70);
	else if(m_nPlayerType == PLAYER2)
	D3DXMatrixTranslation(&m_mWeaponCard,800,-220,70);

	//---------------------------------------------------
	//
	// Ultimate Bar GOOD
	//
	//---------------------------------------------------
	
	D3DXMatrixTranslation(&m_mHUDUltCase,-440.0f*(float)m_nPlayerPositions,-220,100);
	//D3DXMATRIX s;
	//D3DXMatrixScaling(&s,m_fUltScale,1.0,1.0);
	D3DXMatrixTranslation(&m_mUlt,-440.0f*(float)m_nPlayerPositions,-360,50);

	if(m_nPlayerType == PLAYER1)
	{
		 D3DXMatrixTranslation(&m_mUltParticle,-440.0f*(float)m_nPlayerPositions,-360,50);
	}
	else if(m_nPlayerType == PLAYER2)
	{
		 D3DXMatrixTranslation(&m_mUltParticle,-600.0f*(float)m_nPlayerPositions,-360,50);
	}


	//D3DXMatrixMultiply(&m_mUlt,&s,&m_mUlt);

	created = false;

	m_Cam = CCamera::GetInstance();

	m_fUltScale = 0.0f;
	m_nPlayKillCount = 0;
	m_bAudioPlayKillSpree = false;
	m_bAudioPlayKillPlayer1 = false;
	m_bAudioPlayKillPlayer2 = false;
}

void CHUD::AddScore(int PointValue)
{
	m_nScore += (PointValue * m_nFlatMultiplyer);
	//CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_2D_COINPICKUP, 200);
	if(m_nPlayerType == PLAYER1 && m_nPortraitType == 0)
	{
		CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_DX_BARRON_POSITIVE_FEEDBACK, 4);
	}
	else if(m_nPlayerType == PLAYER1 && m_nPortraitType == 1)
	{
		if( rand() % 2 == 0)
			CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_DX_CLAIRE_POSITIVE_FEEDBACK, 5);
	}

	if(m_nPlayerType == PLAYER2 && m_nPortraitType == 0)
	{
		CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_DX_BARRON_POSITIVE_FEEDBACK, 4);
	}
	else if(m_nPlayerType == PLAYER2 && m_nPortraitType == 1)
	{
		if( rand() % 2 == 0)
		CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_DX_CLAIRE_POSITIVE_FEEDBACK, 5);
	}
}

void CHUD::SubtractScore(int PointValue)
{
	m_nScore -= PointValue;
	if(m_nScore < 0)
		m_nScore = 0;
}

void CHUD::AddHealth(int Health)
{
	m_nHealth += Health;
}

void CHUD::SubtractHealth(int Health)
{
	m_nHealth -= Health;
	//CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_HEALTH_LOSS, 200);
	if(m_nHealth < 0)
		m_nHealth = 0;

	if(m_nHealth == 0)
	{
		if(m_nLives > 0)
		{
			SubtractLife(1);
		}
	}
}

void CHUD::Update(float fElaspedTime)
{
	m_fBucket += fElaspedTime;

	if(m_fBucket > 1.0)
	{
		m_fBucket = 0.0;
	}

	m_fMultiplyerScale = m_fMultiplyer - (float)m_nFlatMultiplyer;

	//D3DXMATRIX mScale;
	//D3DXMatrixScaling(&mScale,1.0,scale,1.0);
	//m_mHUDMultiplier[1] = mScale * m_mHUDMultiplier[1];
	
	float w = (float)CDirect3D::GetInstance()->GetPresentParams()->BackBufferWidth;
	float h = (float)CDirect3D::GetInstance()->GetPresentParams()->BackBufferHeight;
	D3DXMATRIX OrthoProjection;
	D3DXMatrixOrthoLH(&OrthoProjection, w, h , 1.0f, 500.0f);

	SetFinalMultiplyer();

	if(m_bAudioPlayKillSpree == true)
	{
		CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_DX_ANNOUNCER_KILLINGSPREE, 200);
		m_bAudioPlayKillSpree = false;
	}

	if(m_bAudioPlayKillPlayer1 == true && m_nPlayKillCount <= 0)
	{
		CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_DX_ANNOUNCER_KILLED_PLAYERONE, 200);
		//m_bAudioPlayKillPlayer1 = false;
		m_nPlayKillCount++;
	}

	if(m_bAudioPlayKillPlayer2 == true && m_nPlayKillCount <= 0)
	{
		CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_DX_ANNOUNCER_KILLED_PLAYERTWO, 200);
		AK::MotionEngine::RemovePlayerMotionDevice(1, AKCOMPANYID_AUDIOKINETIC, AKMOTIONDEVICEID_RUMBLE);
		//m_bAudioPlayKillPlayer2 = false;
		m_nPlayKillCount++;
	}

	if(m_fUltScale >= 1.0 && !created)
	{
		D3DXMATRIX world;
		D3DXMatrixIdentity(&world);
		D3DXMatrixTranslation(&world,CCamera::GetInstance()->GetViewPosition().x,CCamera::GetInstance()->GetViewPosition().y,20);
		D3DXMATRIX temp;
		D3DXMatrixIdentity(&temp);
		temp = world;
		D3DXMatrixMultiply(&world,&OrthoProjection,&temp);
		fire = CParticleManager::GetInstance()->SpawnParticle( "HUD_fire",world);
		created = true;
	}

	if(created)
	{
		D3DXMATRIX world;
		D3DXMatrixIdentity(&world);
		D3DXMatrixTranslation(&world,CCamera::GetInstance()->GetViewPosition().x-100,CCamera::GetInstance()->GetViewPosition().y-100,0);
		D3DXMATRIX temp;
		D3DXMatrixIdentity(&temp);
		if(m_nPlayerType == PLAYER1)
		{
			D3DXMatrixTranslation(&temp,-440.0f*(float)m_nPlayerPositions,-360,10);
		}
		else
		{
			D3DXMatrixTranslation(&temp,-310.0f*(float)m_nPlayerPositions,-360,10);
			
		}
		D3DXMatrixMultiply(&world,&OrthoProjection,&temp);
		fire->SetWorld(temp * OrthoProjection);
	}

	if(m_fUltScale < 1.0 && created)
	{
		fire->SetIsActive(false);
		created = false;
	}

	//D3DXMatrixRotationX(&m_Rotation,1*fElaspedTime);
}

void CHUD::Render()
{
	// Render for the hud mocup
	//sprintf_s(szScore, sizeof(szScore),"%i",m_nScore);
	//sprintf_s(szMultiplier, sizeof(szScore),"x%i",m_nFlatMultiplyer);
	//if(m_nPlayerType == PLAYER1)
	//	m_BFC->Print(szMultiplier,446,65,false,false,0.85f);
	//else
	//	m_BFC->Print(szMultiplier,555,65,false,false,0.85f);
	////m_BFC->Print(szScore,50,0,false,false,1.0);

	if(m_nPlayerType == PLAYER1)
	{
		if(m_nLives >= 0 && m_nHealth > 0)
		{
			if(m_nWeaponType == HMACHINE_GUN)
			{
				if(m_fUltScale >= 1.0)
				{
					m_BFC->Print("MAX",70,680,false,false,1.0f,D3DXCOLOR(0.5,0,0,255));
				}
				else
				{
					m_BFC->Print("MAX",70,680,false,false,1.0f);
				}
			}
			else
			{
				if(m_fUltScale >= 1.0)
				{
					sprintf_s(szScore, sizeof(szScore),"x%i",m_nAmmo);
					m_BFC->Print(szScore,70,680,false,false,1.0f,D3DXCOLOR(0.5,0,0,255));
				}
				else
				{
					sprintf_s(szScore, sizeof(szScore),"x%i",m_nAmmo);
					m_BFC->Print(szScore,70,680,false,false,1.0f);
				}
			}
		}
	}
	else
	{
		if(m_nLives >= 0 && m_nHealth > 0)
		{
			if(m_nWeaponType == HMACHINE_GUN)
			{
				if(m_fUltScale >= 1.0)
				{
					m_BFC->Print("MAX",890,680,false,false,1.0f,D3DXCOLOR(0.5,0,0,255));
				}
				else
				{
					m_BFC->Print("MAX",890,680,false,false,1.0f);
				}
			}
			else
			{
				if(m_fUltScale >= 1.0)
				{
					sprintf_s(szScore, sizeof(szScore),"x%i",m_nAmmo);
					m_BFC->Print(szScore,890,680,false,false,1.0f,D3DXCOLOR(0.5,0,0,255));
				}
				else
				{
					sprintf_s(szScore, sizeof(szScore),"x%i",m_nAmmo);
					m_BFC->Print(szScore,890,680,false,false,1.0f);
				}
			}
		}
	}



	float w = (float)CDirect3D::GetInstance()->GetPresentParams()->BackBufferWidth;
	float h = (float)CDirect3D::GetInstance()->GetPresentParams()->BackBufferHeight;
	D3DXMATRIX OrthoProjection;
	D3DXMatrixOrthoLH(&OrthoProjection, w, h , 1.0f, 500.0f);

	
		//Render Hearts
		for(int renderHeart = 0; renderHeart < m_nHealth; renderHeart++)
		{
			if(renderHeart < 3)
			{
				m_HUDHeartShape[renderHeart].GetFrame().GetLocalMatrix() = m_Heart[renderHeart] * OrthoProjection;
				m_HUDHeartShape[renderHeart].GetRenderContext()->AddRenderNode(&m_HUDHeartShape[renderHeart]);
			}
		}
	

	//Render Dials
	for(int i(0); i < 10; ++i)
	{
		m_HUDDialShape[i].GetFrame().GetLocalMatrix() = m_Dials[i] * OrthoProjection;
		m_HUDDialShape[i].GetRenderContext()->AddRenderNode(&m_HUDDialShape[i]);
	}

	//Render Portrait
	if(m_nLives >= 0 && m_nHealth > 0)
	{
		m_HUDPortraitShape.GetFrame().GetLocalMatrix() = m_mHUDPortrait * OrthoProjection;
		m_HUDPortraitShape.GetRenderContext()->AddRenderNode(&m_HUDPortraitShape);
	}
	else
	{
		m_HUDPortraitDeathShape.GetFrame().GetLocalMatrix() = m_mHUDPortraitDeath * OrthoProjection;
		m_HUDPortraitDeathShape.GetRenderContext()->AddRenderNode(&m_HUDPortraitDeathShape);
		if(m_nPlayerType == PLAYER1)
		{
			if(m_bAudioPlayKillPlayer1 == false)
				m_bAudioPlayKillPlayer1 = true;
		}
		else if(m_nPlayerType == PLAYER2)
		{
			if(m_bAudioPlayKillPlayer2 == false)
				m_bAudioPlayKillPlayer2 = true;
		}
	}

	//Render HUD Body
	m_HUDFrameShape.GetFrame().GetLocalMatrix() = m_Frame * OrthoProjection;
	m_HUDFrameShape.GetRenderContext()->AddRenderNode(&m_HUDFrameShape);

	//Render Life Dial
	switch(m_nLives)
	{
	case 0:
		m_HUDLifeShape[0].GetFrame().GetLocalMatrix() = m_Lives * OrthoProjection;
		m_HUDLifeShape[0].GetRenderContext()->AddRenderNode(&m_HUDLifeShape[0]);
		break;
	case 1:
		m_HUDLifeShape[1].GetFrame().GetLocalMatrix() = m_Lives * OrthoProjection;
		m_HUDLifeShape[1].GetRenderContext()->AddRenderNode(&m_HUDLifeShape[1] );
		break;
	case 2:
		m_HUDLifeShape[2].GetFrame().GetLocalMatrix() = m_Lives * OrthoProjection;
		m_HUDLifeShape[2].GetRenderContext()->AddRenderNode(&m_HUDLifeShape[2] );
		break;
	case 3:
		m_HUDLifeShape[3].GetFrame().GetLocalMatrix() = m_Lives * OrthoProjection;
		m_HUDLifeShape[3].GetRenderContext()->AddRenderNode(&m_HUDLifeShape[3] );
		break;
	case 4:
		m_HUDLifeShape[3].GetFrame().GetLocalMatrix() = m_Lives * OrthoProjection;
		m_HUDLifeShape[3].GetRenderContext()->AddRenderNode(&m_HUDLifeShape[3] );
		break;
	case 5:
		m_HUDLifeShape[3].GetFrame().GetLocalMatrix() = m_Lives * OrthoProjection;
		m_HUDLifeShape[3].GetRenderContext()->AddRenderNode(&m_HUDLifeShape[3] );
		break;
	case 6:
		m_HUDLifeShape[3].GetFrame().GetLocalMatrix() = m_Lives * OrthoProjection;
		m_HUDLifeShape[3].GetRenderContext()->AddRenderNode(&m_HUDLifeShape[3] );
		break;
	}

	switch(m_nFlatMultiplyer)
	{
	case 1:
		m_HUDMultiShape.GetFrame().GetLocalMatrix() = m_Multi[0] * OrthoProjection;
		m_HUDMultiShape.GetRenderContext()->AddRenderNode(&m_HUDMultiShape);
		break;
	case 2:
		m_HUDMultiShape.GetFrame().GetLocalMatrix() = m_Multi[1] * OrthoProjection;
		m_HUDMultiShape.GetRenderContext()->AddRenderNode(&m_HUDMultiShape);
		break;
	case 3:
		m_HUDMultiShape.GetFrame().GetLocalMatrix() = m_Multi[2] * OrthoProjection;
		m_HUDMultiShape.GetRenderContext()->AddRenderNode(&m_HUDMultiShape);
		break;
	case 4:
		m_HUDMultiShape.GetFrame().GetLocalMatrix() = m_Multi[3] * OrthoProjection;
		m_HUDMultiShape.GetRenderContext()->AddRenderNode(&m_HUDMultiShape);
		break;
	}
	

	////Render Multiplier
	//m_HUDMultiplierBackShape.GetFrame().GetLocalMatrix() = m_mHUDMultiplier[0] * OrthoProjection;
	//m_HUDMultiplierBackShape.GetRenderContext()->AddRenderNode(&m_HUDMultiplierBackShape);

	D3DXMATRIX scale;
	//D3DXMatrixScaling(&scale,1.0,m_fMultiplyerScale,1.0);

	//m_HUDMultiplierFrontShape.GetFrame().GetLocalMatrix() = scale * m_mHUDMultiplier[1]*OrthoProjection;
	//m_HUDMultiplierFrontShape.GetRenderContext()->AddRenderNode(&m_HUDMultiplierFrontShape);

	//Weapon Type Render
	if(m_nLives >= 0 && m_nHealth > 0)
	{
		switch(m_nWeaponType)
		{
		case HMACHINE_GUN:
			{
				m_HUDWeaponCardShape[HMACHINE_GUN].GetFrame().GetLocalMatrix() = m_mWeaponCard * OrthoProjection;
				m_HUDWeaponCardShape[HMACHINE_GUN].GetRenderContext()->AddRenderNode(&m_HUDWeaponCardShape[HMACHINE_GUN]);
				break;
			}
		case HSHOTGUN:
			{
				m_HUDWeaponCardShape[HSHOTGUN].GetFrame().GetLocalMatrix() = m_mWeaponCard * OrthoProjection;
				m_HUDWeaponCardShape[HSHOTGUN].GetRenderContext()->AddRenderNode(&m_HUDWeaponCardShape[HSHOTGUN]);
				break;
			}
		case HFLAMETHROWER:
			{
				m_HUDWeaponCardShape[HFLAMETHROWER].GetFrame().GetLocalMatrix() = m_mWeaponCard * OrthoProjection;
				m_HUDWeaponCardShape[HFLAMETHROWER].GetRenderContext()->AddRenderNode(&m_HUDWeaponCardShape[HFLAMETHROWER]);
				break;
			}
		case HLIGHTNING_GUN:
			{
				m_HUDWeaponCardShape[HLIGHTNING_GUN].GetFrame().GetLocalMatrix() = m_mWeaponCard * OrthoProjection;
				m_HUDWeaponCardShape[HLIGHTNING_GUN].GetRenderContext()->AddRenderNode(&m_HUDWeaponCardShape[HLIGHTNING_GUN]);
				break;
			}
		case HROCKET_LAUNCHER:
			{
				m_HUDWeaponCardShape[HROCKET_LAUNCHER].GetFrame().GetLocalMatrix() = m_mWeaponCard * OrthoProjection;
				m_HUDWeaponCardShape[HROCKET_LAUNCHER].GetRenderContext()->AddRenderNode(&m_HUDWeaponCardShape[HROCKET_LAUNCHER]);
				break;
			}
		}
	}

	if(m_nLives == 0 && m_nHealth == 0)
	{
		m_HUDWeaponCardDeathShape.GetFrame().GetLocalMatrix() = m_mHUDWeaponCardDeath * OrthoProjection;
		m_HUDWeaponCardDeathShape.GetRenderContext()->AddRenderNode(&m_HUDWeaponCardDeathShape);
	}

	// Ultimate Bar Render
	m_UltimateCaseShape.GetFrame().GetLocalMatrix() = m_mHUDUltCase * OrthoProjection;
	m_UltimateCaseShape.GetRenderContext()->AddRenderNode(&m_UltimateCaseShape);
	
	D3DXMatrixIdentity(&scale);
	D3DXMatrixScaling(&scale,m_fUltScale,1.0,1.0);
	
	
	UltimateBarShape.GetFrame().GetLocalMatrix() = scale * m_mUlt * OrthoProjection;
	UltimateBarShape.GetRenderContext()->AddRenderNode(&UltimateBarShape);	
	

	//CRenderContextManager::GetInstance()->Render();

}

void CHUD::Shutdown()
{
}

void CHUD::IncreaseMultiplier()
{
	if(m_fMultiplyer < 4.5)
	{
		m_fMultiplyer += 0.20f;
	}
}

void CHUD::DecreaseMultiplier()
{
	if(m_fMultiplyer > 1.0)
	{
		m_fMultiplyer -= 0.10f;
	}

	if(m_fMultiplyer < 1.0)
	{
		m_fMultiplyer = 1.0f;
	}
}

void CHUD::ResetMultiplyer()
{
	m_fMultiplyer = 1.0f;
}

void CHUD::SetFinalMultiplyer()
{
	m_nFlatMultiplyer = (int)m_fMultiplyer;

	if( (m_nFlatMultiplyer == 4) && (m_nPrevMult == 3) )
	{
		if(m_bAudioPlayKillSpree == false)
			m_bAudioPlayKillSpree = true;
	}

	m_nPrevMult = m_nFlatMultiplyer;
	
}

void CHUD::AddLife(int Life)
{
	m_nLives += Life;
}

void CHUD::SubtractLife(int Life)
{
	m_nLives -= Life;
	m_nHealth = 3;

	if(m_nLives < 0)
	{
		m_nLives = 0;
		m_nHealth = 0;
	}

}

void CHUD::UpdateDials(int score)
{
	int previous = m_nScore;
	AddScore(score);

	m_nFinalScore = m_nScore-previous;

	_itoa_s(m_nFinalScore,szScoreDial,10,10);
	m_nStrLen = strlen(szScoreDial);

	for(int index = 0; index < m_nStrLen; index++)
	{
		if(szScoreDial[index] != '0')
		{
			m_nSpinValue = szScoreDial[index] - 48;
			SpinDial((m_nStrLen-1)-index,m_nSpinValue);
		}
	}
}

void CHUD::SpinDial(int DialIndex, int SpinValue)
{
	D3DXMatrixIdentity(&m_Rotation);
	m_Rotation = m_Dials[DialIndex];
	m_fRotation = (float)(36 * SpinValue);
	D3DXMatrixRotationX(&m_Rotation,D3DXToRadian(m_fRotation));
	D3DXMatrixMultiply(&m_Dials[DialIndex],&m_Rotation,&m_Dials[DialIndex]);

	m_nDialValues[DialIndex] += SpinValue;

	for(int i = DialIndex; i < 10; i++)
	{
		if(m_nDialValues[i] > 9)
		{
			int leftovers = m_nDialValues[i]%10;
			m_nDialValues[i] = leftovers;
			m_nDialValues[i+1]++;
			D3DXMatrixRotationX(&m_Rotation,D3DXToRadian(36));
			D3DXMatrixMultiply(&m_Dials[i+1],&m_Rotation,&m_Dials[i+1]);
		}
	}
}

void CHUD::RotateMultiplyer(int rotate)
{
	/*D3DXMatrixIdentity(&m_MultiRotation);
	D3DXMatrixRotationX(&m_MultiRotation,D3DXToRadian(36*rotate));
	D3DXMatrixMultiply(&m_Multi,&m_MultiRotation,&m_Multi);*/
}