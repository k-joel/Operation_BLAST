#include "CHowToState.h"
#include "CMainMenuState.h"
#include "CLoadState.h"
#include "CStateMachine.h"
#include "CGameState.h"
#include "../Rendering/CTextureManager.h"
#include "../Font/CBitMapFont.h"
#include "../Input/CInput.h"
#include "../Camera/Camera.h"

#include "../Resources/Sound/Wwise_IDs.h"
#include "../Sound/WwiseManager.h"

enum {COVER,SOBJECTIVE,MOBJECTIVE,ENEMIES,WEAPONS,SG,FT,HUD,GAMEPAD,KEYBOARD,HOWTOMAX};

CHowToState::CHowToState(void)
{
	m_nSelection = 0;
	m_fScale = 0.85f;
	m_nFontX = 150;
	m_nArrowPosition = 120;
	m_nFontOffset = 42;
	m_nFontStart = 150;
	m_nConfirm = 0;
}

CHowToState::~CHowToState(void)
{

}

void CHowToState::Enter(void)
{
	m_Buttons.Initialize(true);
}

bool CHowToState::Input(void)
{
	static float m_fPrevX = 0.0f;
	static float m_fCurrX = 0.0f;
	m_fPrevX = m_fCurrX;
	m_fCurrX = CInput::GetInstance()->GetActionAmount(ACT_MOVEX);
	if(m_fCurrX >= 32760 && m_fPrevX != m_fCurrX)
	{
		m_nSelection--;
		m_nConfirm --;
		CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_MENU_UP, 200);
	}
	if(m_fCurrX <= -32760 && m_fPrevX != m_fCurrX)
	{
		m_nSelection++;
		m_nConfirm ++;
		CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_MENU_DOWN, 200);
	}
	
	static float m_fPrevX2 = 0.0f;
	static float m_fCurrX2 = 0.0f;
	m_fPrevX2 = m_fCurrX2;
	m_fCurrX2 = CInput::GetInstance()->GetActionAmount(ACT_MOVEX,1);
	if(m_fCurrX2 >= 32760 && m_fPrevX2 != m_fCurrX2)
	{
		m_nSelection--;
		m_nConfirm --;
		CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_MENU_UP, 200);
	}
	if(m_fCurrX2 <= -32760 && m_fPrevX2 != m_fCurrX2)
	{
		m_nSelection++;
		m_nConfirm ++;
		CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_MENU_DOWN, 200);
	}
	
	if(CInput::GetInstance()->GetAction(ACT_MOVELEFT) || CInput::GetInstance()->GetAction(ACT_MOVELEFT,1))
	{
		m_nSelection--;
		m_nConfirm--;
		CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_MENU_UP, 200);
	}

	if(CInput::GetInstance()->GetAction(ACT_MOVERIGHT) || CInput::GetInstance()->GetAction(ACT_MOVERIGHT,1))
	{
		m_nSelection++;
		m_nConfirm++;
		CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_MENU_DOWN, 200);
	}

	if(m_nConfirm > 1)
	{
		m_nConfirm = 1;
	}

	if(m_nConfirm < 0)
	{
		m_nConfirm = 0;
	}

	if(m_nSelection < 0)
	{
		m_nSelection = HOWTOMAX-1;
	}
	
	if(m_nSelection > HOWTOMAX-1)
	{
		m_nSelection = 0;
	}

	if(CInput::GetInstance()->GetAction(ACT_ACCEPT,0) || CInput::GetInstance()->GetAction(ACT_ACCEPT,1))
	{
		CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_MENU_GOBACK, 200);
		CMainMenuState::GetInstance()->SetOptions(false);
		CStateSystem::GetInstance()->RemoveTopState();
	}
	return true;
}

void CHowToState::Update(float fElapsedTime)
{
	if(!CGameState::GetInstance()->GetPaused())
	{
		CMainMenuState::GetInstance()->Update(fElapsedTime);
	}

	m_mxViewProj  = CCamera::GetInstance()->GetViewMatrix() * CCamera::GetInstance()->GetProjectionMatrix();
}

void CHowToState::Render(void)
{
	//CBitMapFont::GetInstance()->Print("Welcome to the How to Play Tutorial",m_nFontX,m_nFontStart,false,false,m_fScale);

	switch(m_nSelection)
	{
	case COVER:
		CTextureManager::GetInstance()->Draw(CLoadState::GetInstance()->GetImageID("Cover"),m_nFontX,m_nFontStart,0.55f,0.40f);
		break;
	case SOBJECTIVE:
		CTextureManager::GetInstance()->Draw(CLoadState::GetInstance()->GetImageID("SObjective"),m_nFontX,m_nFontStart,0.55f,0.40f);
		break;
	case MOBJECTIVE:
		CTextureManager::GetInstance()->Draw(CLoadState::GetInstance()->GetImageID("MObjective"),m_nFontX,m_nFontStart,0.55f,0.40f);
		break;
	case ENEMIES:
		CTextureManager::GetInstance()->Draw(CLoadState::GetInstance()->GetImageID("Enemies"),m_nFontX,m_nFontStart,0.55f,0.40f);
		break;
	case WEAPONS:
		CTextureManager::GetInstance()->Draw(CLoadState::GetInstance()->GetImageID("Weapons"),m_nFontX,m_nFontStart,0.55f,0.40f);
		break;
	case SG:
		CTextureManager::GetInstance()->Draw(CLoadState::GetInstance()->GetImageID("SG"),m_nFontX,m_nFontStart,0.55f,0.40f);
		break;
	case FT:
		CTextureManager::GetInstance()->Draw(CLoadState::GetInstance()->GetImageID("FT"),m_nFontX,m_nFontStart,0.55f,0.40f);
		break;
	/*case LG:
		CTextureManager::GetInstance()->Draw(CLoadState::GetInstance()->GetImageID("LG"),m_nFontX,m_nFontStart,0.55f,0.40f);
		break;
	case RL:
		CTextureManager::GetInstance()->Draw(CLoadState::GetInstance()->GetImageID("RL"),m_nFontX,m_nFontStart,0.55f,0.40f);
		break;*/
	case HUD:
		CTextureManager::GetInstance()->Draw(CLoadState::GetInstance()->GetImageID("HUD"),m_nFontX,m_nFontStart,0.55f,0.40f);
		break;
	case GAMEPAD:
		CTextureManager::GetInstance()->Draw(CLoadState::GetInstance()->GetImageID("GamePad"),m_nFontX,m_nFontStart,0.55f,0.40f);
		break;
	case KEYBOARD:
		CTextureManager::GetInstance()->Draw(CLoadState::GetInstance()->GetImageID("KeyBoard"),m_nFontX,m_nFontStart,0.55f,0.40f);
		break;
	}
	
	m_Buttons.Render(m_mxViewProj,false,true,true,false,((XboxGamePadDevice*)CInput::GetInstance()->GetDevice(DEV_GAMEPAD))->ControllerConnected(0));

	CBitMapFont::GetInstance()->Print("Prev",m_nFontX,m_nFontStart+(m_nFontOffset*10),false,false);
	CBitMapFont::GetInstance()->Print("Next",m_nFontX+(m_nFontX*3),m_nFontStart+(m_nFontOffset*10),false,false);
	CBitMapFont::GetInstance()->Print("=>",m_nArrowPosition+(m_nFontX*3*m_nConfirm)-5,m_nFontStart+(m_nFontOffset*10),false,false,m_fScale);
}

bool CHowToState::Exit(void)
{
	CGameState::GetInstance()->SetIsPaused(false);
	return false;
}

CHowToState* CHowToState::GetInstance(void)
{
	static CHowToState instance;
	return &instance;
}