#include "COptionState.h"
#include "CGameState.h"
#include "CSoundOptionState.h"
#include "CVideoOptionState.h"
#include "CGameplayOptionState.h"
#include "CControlOptionState.h"
#include "CMainMenuState.h"
#include "CStateMachine.h"
#include "CLoadState.h"
#include "../Core/CGame.h"
#include "../Rendering/CTextureManager.h"
#include "../Font/CBitMapFont.h"
#include "../Input/CInput.h"
#include "../Camera/Camera.h"
#include "../Mesh/modelImporter.h"
#include "../Rendering/CShaderManager.h"
#include "../Rendering/CTextureManager.h"
#include "../Rendering/CRenderer.h"
#include "../Rendering/CDirect3D.h"
#include "../Rendering/CRenderContextManager.h"
#include "../Mesh/CModelManager.h"

const AkGameObjectID MENU = 200;

COptionState::COptionState(void)
{
	m_fScale = 1.0f;
	m_nFontX = 300;
	m_nArrowPosition = m_nFontX-50;
	m_nFontOffset = 42;
	m_nFontStart = 150;
}

COptionState::~COptionState(void)
{
}

void COptionState::Enter(void)
{
	m_TM = CTextureManager::GetInstance();
	m_IPM = CInput::GetInstance();
	m_SM = CStateSystem::GetInstance();
	m_BMF = CBitMapFont::GetInstance();

	//Setting Up Variables
	m_nSelection = 0;
	m_bRender = true;

	m_GamePadAccept = CModelManager::GetInstance()->GetModel(CLoadState::GetInstance()->GetModelID("GamePadAccept"));
	m_GamePadAcceptShape.Init(&m_GamePadAccept->GetMesh(),NULL,&CRenderContextManager::GetInstance()->GetHUDContext(),m_mGamePadAccept,CRenderShape::PNTRenderFunc);

	m_GamePadUD = CModelManager::GetInstance()->GetModel(CLoadState::GetInstance()->GetModelID("GamePadUD"));
	m_GamePadUDShape.Init(&m_GamePadUD->GetMesh(),NULL,&CRenderContextManager::GetInstance()->GetHUDContext(),m_mGamePadUD,CRenderShape::PNTRenderFunc);

	m_KeyAccept = CModelManager::GetInstance()->GetModel(CLoadState::GetInstance()->GetModelID("KeyAccept"));
	m_KeyAcceptShape.Init(&m_KeyAccept->GetMesh(),NULL,&CRenderContextManager::GetInstance()->GetHUDContext(),m_mKeyAccept,CRenderShape::PNTRenderFunc);

	m_KeyUD = CModelManager::GetInstance()->GetModel(CLoadState::GetInstance()->GetModelID("KeyUD"));
	m_KeyUDShape.Init(&m_KeyUD->GetMesh(),NULL,&CRenderContextManager::GetInstance()->GetHUDContext(),m_mKeyUD,CRenderShape::PNTRenderFunc);

	m_Buttons.Initialize(true);
}

bool COptionState::Input(void)
{
	//KeyboardDevice* keyboard = (KeyboardDevice*)CInput::GetInstance()->GetDevice(DEV_KEYBOARD);
	static float m_fPrevY = 0.0f;
	static float m_fCurrY = 0.0f;
	m_fPrevY = m_fCurrY;
	m_fCurrY = CInput::GetInstance()->GetActionAmount(ACT_MOVEY);

	static float m_fPrevY2 = 0.0f;
	static float m_fCurrY2 = 0.0f;
	m_fPrevY2 = m_fCurrY2;
	m_fCurrY2 = CInput::GetInstance()->GetActionAmount(ACT_MOVEY,1);

	//Player 1 Movement
	if(m_fCurrY >= 32765 && m_fPrevY != m_fCurrY)
	{
		m_nSelection --;
		CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_MENU_UP, MENU);
	}
	if(m_fCurrY <= -32765 && m_fPrevY != m_fCurrY)
	{
		m_nSelection ++;
		CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_MENU_DOWN, MENU);
	}
	if(m_IPM->GetAction(ACT_MOVEDOWN) || m_IPM->GetAction(ACT_MOVEDOWN,1))
	{
		m_nSelection++;
		CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_MENU_DOWN, MENU);
	}

	if(m_IPM->GetAction(ACT_MOVEUP) || m_IPM->GetAction(ACT_MOVEUP,1))
	{
		m_nSelection--;
		CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_MENU_UP, MENU);
	}

	//Player 2 Movement
	if(m_fCurrY2 >= 32765 && m_fPrevY2 != m_fCurrY2)
	{
		m_nSelection --;
		CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_MENU_UP, MENU);
	}
	if(m_fCurrY2 <= -32765 && m_fPrevY2 != m_fCurrY2)
	{
		m_nSelection ++;
		CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_MENU_DOWN, MENU);
	}

	if(!CGameState::GetInstance()->GetPaused())
	{
		if(m_nSelection > 3)
			m_nSelection = 0;

		if(m_nSelection < 0)
			m_nSelection = 3;
	}
	else
	{
		if(m_nSelection > 2)
			m_nSelection = 0;

		if(m_nSelection < 0)
			m_nSelection = 2;
	}

	if(m_IPM->GetAction(ACT_ACCEPT) || m_IPM->GetAction(ACT_ACCEPT,1))
	{
		if(!CGameState::GetInstance()->GetPaused())
		{
			switch(m_nSelection)
			{
			case 0:
				m_bRender = false;
				CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_MENU_SELECT, MENU);
				m_SM->AddState(CGameplayOptionState::GetInstance());
				break;
			case 1:
				m_bRender = false;
				CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_MENU_SELECT, MENU);
				m_SM->AddState(CVideoOptionState::GetInstance());
				break;
			case 2:
				m_bRender = false;
				CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_MENU_SELECT, MENU);
				m_SM->AddState(CSoundOptionState::GetInstance());
				break;
			case 3:
				CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_MENU_GOBACK, MENU);
				CMainMenuState::GetInstance()->SetOptions(false);
				m_SM->RemoveTopState();
				break;
			}
		}
		else
		{
			switch(m_nSelection)
			{
			case 0:
				m_bRender = false;
				CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_MENU_SELECT, MENU);
				m_SM->AddState(CVideoOptionState::GetInstance());
				break;
			case 1:
				m_bRender = false;
				CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_MENU_SELECT, MENU);
				m_SM->AddState(CSoundOptionState::GetInstance());
				break;
			case 2:
				CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_MENU_GOBACK, MENU);
				CMainMenuState::GetInstance()->SetOptions(false);
				m_SM->RemoveTopState();
				break;
			}
		}
	}

	return true;
}

void COptionState::Update(float fElapsedTime)
{
	if(!CGameState::GetInstance()->GetPaused())
	{
		CMainMenuState::GetInstance()->Update(fElapsedTime);
	}

	m_mxViewProj  = CCamera::GetInstance()->GetViewMatrix() * CCamera::GetInstance()->GetProjectionMatrix();

	m_bRender = true;
}

void COptionState::Render(void)
{
	if(m_bRender)
	{
		if(((XboxGamePadDevice*)CInput::GetInstance()->GetDevice(DEV_GAMEPAD))->ControllerConnected(0))
		{
			m_Buttons.Render(m_mxViewProj,true,false,true,false,true);
		}
		else
		{
			m_Buttons.Render(m_mxViewProj,true,false,true,false,false);
		}

		if(!CGameState::GetInstance()->GetPaused())
		{
			RenderOptions();
		}
		else
		{	
			RenderOptionsGame();		
		}

		m_BMF->Print("=>",m_nArrowPosition,m_nFontStart+((m_nFontOffset)*m_nSelection),false,false,m_fScale);
	}
}

bool COptionState::Exit(void)
{
	return false;
}

COptionState* COptionState::GetInstance(void)
{
	static COptionState instance;
	return &instance;
}

void COptionState::RenderOptions(void)
{
		m_BMF->Print("Options",350,110,false,false);

		m_BMF->Print("Gameplay",m_nFontX,m_nFontStart,false,false,m_fScale);
		
		m_BMF->Print("Video",m_nFontX,m_nFontStart+m_nFontOffset,false,false,m_fScale);

		m_BMF->Print("Sound",m_nFontX,m_nFontStart+(m_nFontOffset*2),false,false,m_fScale);

		m_BMF->Print("Back",m_nFontX,m_nFontStart+(m_nFontOffset*3),false,false,m_fScale);
}

void COptionState::RenderOptionsGame(void)
{
		m_BMF->Print("Options",350,110,false,false);
		
		m_BMF->Print("Video",m_nFontX,m_nFontStart,false,false,m_fScale);

		m_BMF->Print("Sound",m_nFontX,m_nFontStart+(m_nFontOffset),false,false,m_fScale);

		m_BMF->Print("Back",m_nFontX,m_nFontStart+(m_nFontOffset*2),false,false,m_fScale);
}