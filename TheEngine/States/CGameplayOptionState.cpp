#include "CGameplayOptionState.h"
#include "../Rendering/CTextureManager.h"
#include "../Font/CBitMapFont.h"
#include "../Input/CInput.h"
#include "../Input/CInput.h"
#include "CStateMachine.h"
#include "CMainMenuState.h"
#include "../Core/CGame.h"
#include "../Core/CConfigLoader.h"
#include "../Camera/Camera.h"
#include "CGameState.h"

CGameplayOptionState::CGameplayOptionState(void)
{
	m_fScale = 1.0f;
	m_nFontX = 200;
	m_nArrowPosition = m_nFontX-50;
	m_nFontOffset = 42;
	m_nFontStart = 150;
	m_nDiff = 1;
}

CGameplayOptionState::~CGameplayOptionState(void)
{

}

void CGameplayOptionState::Enter(void)
{
	m_TM = CTextureManager::GetInstance();
	m_IPM = CInput::GetInstance();
	m_SM = CStateSystem::GetInstance();
	m_BMF = CBitMapFont::GetInstance();

	m_nDiff = CGame::GetInstance()->GetDifficulty();
	m_Buttons.Initialize(true);
	//Setting Up Variables
	m_nSelection = 0;
}

bool CGameplayOptionState::Input(void)
{
	if(CInput::GetInstance()->GetAction(ACT_ACCEPT)||CInput::GetInstance()->GetAction(ACT_ACCEPT,1))
	{
		switch(m_nSelection)
		{
		case 0:
			break;
		case 1:
			CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_MENU_GOBACK, 200);	// AudioID 200 is MENU
			CStateSystem::GetInstance()->RemoveTopState();
			break;
		}
	}
	
	static float m_fPrevY = 0.0f;
	static float m_fCurrY = 0.0f;
	m_fPrevY = m_fCurrY;
	m_fCurrY = CInput::GetInstance()->GetActionAmount(ACT_MOVEY);
	if(m_fCurrY >= 32765 && m_fPrevY != m_fCurrY)
	{
		m_nSelection --;
		CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_MENU_UP, 200);
	}
	if(m_fCurrY <= -32765 && m_fPrevY != m_fCurrY)
	{
		m_nSelection ++;
		CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_MENU_DOWN, 200);
	}

	static float m_fPrevY2 = 0.0f;
	static float m_fCurrY2 = 0.0f;
	m_fPrevY2 = m_fCurrY2;
	m_fCurrY2 = CInput::GetInstance()->GetActionAmount(ACT_MOVEY,1);
	if(m_fCurrY2 >= 32765 && m_fPrevY2 != m_fCurrY2)
	{
		m_nSelection --;
		CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_MENU_UP, 200);
	}
	if(m_fCurrY2 <= -32765 && m_fPrevY2 != m_fCurrY2)
	{
		m_nSelection ++;
		CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_MENU_DOWN, 200);
	}


	if(CInput::GetInstance()->GetAction(ACT_MOVEDOWN)||CInput::GetInstance()->GetAction(ACT_MOVEDOWN,1))
	{
		m_nSelection++;
		CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_MENU_DOWN, 200);
	}

	if(CInput::GetInstance()->GetAction(ACT_MOVEUP)||CInput::GetInstance()->GetAction(ACT_MOVEUP,1))
	{
		m_nSelection--;
		CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_MENU_UP, 200);
	}

	if(m_nSelection > 1)
		m_nSelection =0;
	if(m_nSelection < 0)
		m_nSelection = 1;
	
	static float m_fPrevX = 0.0f;
	static float m_fCurrX = 0.0f;
	m_fPrevX = m_fCurrX;
	m_fCurrX = CInput::GetInstance()->GetActionAmount(ACT_MOVEX);
	if(m_fCurrX >= 32765 && m_fPrevX != m_fCurrX)
	{
		switch(m_nSelection)
		{
		case 0:
			m_nDiff++;
			CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_MENU_DOWN, 200);
			break;
		case 1:
			break;
		}
	}
	if(m_fCurrX <= -32765 && m_fPrevX != m_fCurrX)
	{
		switch(m_nSelection)
		{
		case 0:
			m_nDiff--;
			CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_MENU_UP, 200);
			break;
		case 1:
			break;
		}
	}

	static float m_fPrevX2 = 0.0f;
	static float m_fCurrX2 = 0.0f;
	m_fPrevX2 = m_fCurrX2;
	m_fCurrX2 = CInput::GetInstance()->GetActionAmount(ACT_MOVEX,1);
	if(m_fCurrX2 >= 32765 && m_fPrevX2 != m_fCurrX2)
	{
		switch(m_nSelection)
		{
		case 0:
			m_nDiff++;
			CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_MENU_DOWN, 200);
			break;
		case 1:
			break;
		}
	}
	if(m_fCurrX2 <= -32765 && m_fPrevX2 != m_fCurrX2)
	{
		switch(m_nSelection)
		{
		case 0:
			m_nDiff--;
			CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_MENU_UP, 200);
			break;
		case 1:
			break;
		}
	}

	if(CInput::GetInstance()->GetAction(ACT_MOVERIGHT)||CInput::GetInstance()->GetAction(ACT_MOVERIGHT,1))
	{
		switch(m_nSelection)
		{
		case 0:
		m_nDiff++;
		CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_MENU_DOWN, 200);
			break;
		case 1:
			break;
		}
	}

	if(CInput::GetInstance()->GetAction(ACT_MOVELEFT)||CInput::GetInstance()->GetAction(ACT_MOVELEFT,1))
	{
		switch(m_nSelection)
		{
		case 0:
		m_nDiff--;
		CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_MENU_UP, 200);
			break;
		case 1:
			break;
		}
	}

	if(m_nDiff > 2)
		m_nDiff = 0;
	
	if(m_nDiff < 0)
		m_nDiff = 2;

	return true;
}

void CGameplayOptionState::Update(float fElapsedTime)
{
	if(!CGameState::GetInstance()->GetPaused())
	{
		CMainMenuState::GetInstance()->Update(fElapsedTime);
	}
}

void CGameplayOptionState::Render(void)
{
	D3DXMATRIX m_mxViewProj  = CCamera::GetInstance()->GetViewMatrix() * CCamera::GetInstance()->GetProjectionMatrix();
	//CBitMapFont::GetInstance()->Print("Please press A to exit.",300,110);
	m_BMF->Print("=>",m_nArrowPosition,m_nFontStart+((m_nFontOffset)*m_nSelection),false,false,m_fScale);
	m_Buttons.Render(m_mxViewProj,true,true,true,false,((XboxGamePadDevice*)CInput::GetInstance()->GetDevice(DEV_GAMEPAD))->ControllerConnected(0));
	
	switch(m_nDiff)
	{
	case 0:
		m_BMF->Print("Difficulty: Easy",m_nFontX,m_nFontStart,false,false,m_fScale);
			break;
	case 1:
		m_BMF->Print("Difficulty: Normal",m_nFontX,m_nFontStart,false,false,m_fScale);
		break;
	case 2:
		m_BMF->Print("Difficulty: Hard",m_nFontX,m_nFontStart,false,false,m_fScale);
		break;
	}

	m_BMF->Print("Back",m_nFontX,m_nFontOffset+m_nFontStart,false,false,m_fScale);

}

bool CGameplayOptionState::Exit(void)
{
	CGame::GetInstance()->SetDifficulty(m_nDiff);
	CConfigLoader::GetInstance()->SetDifficulty(CGame::GetInstance()->GetDifficulty());
	CConfigLoader::GetInstance()->WriteConfig();
	return false;
}

CGameplayOptionState* CGameplayOptionState::GetInstance(void)
{
	static CGameplayOptionState instance;
	return &instance;
}