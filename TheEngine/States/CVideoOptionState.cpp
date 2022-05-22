#include "CVideoOptionState.h"
#include "CGameState.h"
#include "CMainMenuState.h"
#include "../Core/CConfigLoader.h"
#include "../Rendering/CTextureManager.h"
#include "../Camera/Camera.h"
#include "../Input/CInput.h"
#include "../States/CStateMachine.h"
#include "../Font/CBitMapFont.h"
#include "../Rendering/CDirect3D.h"
#include "../Rendering/CRenderContextManager.h"

const AkGameObjectID MENU = 200;

const int SELECTIONS = 4;

CVideoOptionState::CVideoOptionState(void)
{
	m_fScale = 1.0f;
	m_nFontX = 200;
	m_nArrowPosition = m_nFontX-50;
	m_nFontOffset = 42;
	m_nFontStart = 150;
}

CVideoOptionState::~CVideoOptionState(void)
{
}

void CVideoOptionState::Enter(void)
{
	m_TM = CTextureManager::GetInstance();
	m_IPM = CInput::GetInstance();
	m_SM = CStateSystem::GetInstance();
	m_BMF = CBitMapFont::GetInstance();

	m_nVideoModeSelection = -1;
	for(unsigned int i = 0; i < CDirect3D::GetInstance()->GetNumModes(); i++)
	{
		if(CDirect3D::GetInstance()->GetDisplayMode(i).Height == CConfigLoader::GetInstance()->GetResHeight() &&
			CDirect3D::GetInstance()->GetDisplayMode(i).Width == CConfigLoader::GetInstance()->GetResWidth() )
		{
			m_nVideoModeSelection = i;
			break;
		}
	}

	m_Buttons.Initialize(true);

	if(m_nVideoModeSelection == -1)
	{
		char szBuffer[256] = {0};
		sprintf_s(szBuffer, sizeof(szBuffer), "Incorrect Video Mode in config file.\nResetting mode to 1024x768");
		MessageBox(0, szBuffer, "Error", MB_OK);

		CConfigLoader::GetInstance()->SetResWidth(1024);
		CConfigLoader::GetInstance()->SetResHeight(768);
		CConfigLoader::GetInstance()->WriteConfig();
		CDirect3D::GetInstance()->ChangeDisplayParam(CConfigLoader::GetInstance()->GetResWidth(),CConfigLoader::GetInstance()->GetResHeight(),
			CConfigLoader::GetInstance()->GetAA(),CConfigLoader::GetInstance()->GetWindowed(),CConfigLoader::GetInstance()->GetVSync());
	}
}

bool CVideoOptionState::Input(void)
{
	KeyboardDevice* keyboard = (KeyboardDevice*)CInput::GetInstance()->GetDevice(DEV_KEYBOARD);
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
	if(CInput::GetInstance()->GetAction(ACT_MOVEDOWN))
	{
		m_nSelection ++;
		CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_MENU_DOWN, MENU);
	}

	if(CInput::GetInstance()->GetAction(ACT_MOVEUP))
	{
		m_nSelection --;
		CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_MENU_UP, MENU);
	}

	if(m_nSelection > SELECTIONS)
		m_nSelection = 0;

	if(m_nSelection < 0)
		m_nSelection = SELECTIONS;
	static float m_fPrevX = 0.0f;
	static float m_fCurrX = 0.0f;
	m_fPrevX = m_fCurrX;
	m_fCurrX = CInput::GetInstance()->GetActionAmount(ACT_MOVEX);
	if(m_fCurrX >= 32765 && m_fPrevX != m_fCurrX)
	{
		CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_MENU_DOWN, 200);
		switch(m_nSelection)
		{
		case 0:
			if(CConfigLoader::GetInstance()->GetGammaLevel() < 100)
				CConfigLoader::GetInstance()->SetGammaLevel(CConfigLoader::GetInstance()->GetGammaLevel()+5);
			CRenderContextManager::GetInstance()->SetGamma(CConfigLoader::GetInstance()->GetGammaLevel());
			break;
		case 1:
			CConfigLoader::GetInstance()->SetVSync(!CConfigLoader::GetInstance()->GetVSync());
			break;
		case 2:
			CConfigLoader::GetInstance()->SetWindowed(!CConfigLoader::GetInstance()->GetWindowed());
			break;
			/*case 3:
			if(m_nVideoModeSelection < int(CDirect3D::GetInstance()->GetNumModes()-1))
			m_nVideoModeSelection++;
			break;*/
		}
	}
	if(m_fCurrX <= -32765 && m_fPrevX != m_fCurrX)
	{
		CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_MENU_UP, 200);
		switch(m_nSelection)
		{
		case 0:
			if(CConfigLoader::GetInstance()->GetGammaLevel() > 0)
				CConfigLoader::GetInstance()->SetGammaLevel(CConfigLoader::GetInstance()->GetGammaLevel()-5);
			CRenderContextManager::GetInstance()->SetGamma(CConfigLoader::GetInstance()->GetGammaLevel());
			break;
		case 1:
			CConfigLoader::GetInstance()->SetVSync(!CConfigLoader::GetInstance()->GetVSync());
			break;
		case 2:
			CConfigLoader::GetInstance()->SetWindowed(!CConfigLoader::GetInstance()->GetWindowed());
			break;
			/*case 3:
			if(m_nVideoModeSelection > 0)
			m_nVideoModeSelection--;
			break;*/
		}
	}
	if(CInput::GetInstance()->GetAction(ACT_MOVELEFT))
	{
		CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_MENU_UP, 200);
		switch(m_nSelection)
		{
		case 0:
			if(CConfigLoader::GetInstance()->GetGammaLevel() > 5)
				CConfigLoader::GetInstance()->SetGammaLevel(CConfigLoader::GetInstance()->GetGammaLevel()-5);
			CRenderContextManager::GetInstance()->SetGamma(CConfigLoader::GetInstance()->GetGammaLevel());
			break;
		case 1:
			CConfigLoader::GetInstance()->SetVSync(!CConfigLoader::GetInstance()->GetVSync());
			break;
		case 2:
			CConfigLoader::GetInstance()->SetWindowed(!CConfigLoader::GetInstance()->GetWindowed());
			break;
			/*case 3:
			if(m_nVideoModeSelection > 0)
			m_nVideoModeSelection--;
			break;*/
		}
	}

	if(CInput::GetInstance()->GetAction(ACT_MOVERIGHT))
	{
		CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_MENU_DOWN, 200);
		switch(m_nSelection)
		{
		case 0:
			if(CConfigLoader::GetInstance()->GetGammaLevel() < 100)
				CConfigLoader::GetInstance()->SetGammaLevel(CConfigLoader::GetInstance()->GetGammaLevel()+5);
			CRenderContextManager::GetInstance()->SetGamma(CConfigLoader::GetInstance()->GetGammaLevel());
			break;
		case 1:
			CConfigLoader::GetInstance()->SetVSync(!CConfigLoader::GetInstance()->GetVSync());
			break;
		case 2:
			CConfigLoader::GetInstance()->SetWindowed(!CConfigLoader::GetInstance()->GetWindowed());
			break;
			/*case 3:
			if(m_nVideoModeSelection < int(CDirect3D::GetInstance()->GetNumModes()-1))
			m_nVideoModeSelection++;
			break;*/
		}
	}

	if(keyboard->KeyPressed(DIK_RETURN) || CInput::GetInstance()->GetAction(ACT_ACCEPT))
	{
		//CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_2D_MENUACCEPT, MENU_SELECT_SFX);
		switch(m_nSelection)
		{
		case 3:
			CConfigLoader::GetInstance()->SetGammaLevel(50);
			CRenderContextManager::GetInstance()->SetGamma(CConfigLoader::GetInstance()->GetGammaLevel());
			CConfigLoader::GetInstance()->SetVSync(false);
			CConfigLoader::GetInstance()->SetWindowed(true);
			break;
		case 4:
			{
				CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_MENU_GOBACK, MENU);
				CRenderContextManager::GetInstance()->SetGamma(CConfigLoader::GetInstance()->GetGammaLevel());
				CConfigLoader::GetInstance()->WriteConfig();
				CDirect3D::GetInstance()->ChangeDisplayParam(CConfigLoader::GetInstance()->GetResWidth(),CConfigLoader::GetInstance()->GetResHeight(),
					CConfigLoader::GetInstance()->GetAA(),CConfigLoader::GetInstance()->GetWindowed(),CConfigLoader::GetInstance()->GetVSync());
				CStateSystem::GetInstance()->RemoveTopState();
				break;
			}
		}
	}

	return true;
}

void CVideoOptionState::Update(float fElapsedTime)
{
	if(!CGameState::GetInstance()->GetPaused())
	{
		CMainMenuState::GetInstance()->Update(fElapsedTime);
	}
}

void CVideoOptionState::Render(void)
{
	D3DXMATRIX m_mxViewProj  = CCamera::GetInstance()->GetViewMatrix() * CCamera::GetInstance()->GetProjectionMatrix();
	RenderVideo();
	m_Buttons.Render(m_mxViewProj,true,true,true,false,((XboxGamePadDevice*)CInput::GetInstance()->GetDevice(DEV_GAMEPAD))->ControllerConnected(0));
	m_BMF->Print("=>",m_nArrowPosition,m_nFontStart+(m_nFontOffset*m_nSelection),false,false,m_fScale);
}

bool CVideoOptionState::Exit(void)
{
	//CConfigLoader::GetInstance()->SetResWidth(CDirect3D::GetInstance()->GetDisplayMode(m_nVideoModeSelection).Width);
	//CConfigLoader::GetInstance()->SetResHeight(CDirect3D::GetInstance()->GetDisplayMode(m_nVideoModeSelection).Height);
	
	return false;
}

CVideoOptionState* CVideoOptionState::GetInstance(void)
{
	static CVideoOptionState instance;
	return &instance;
}

void CVideoOptionState::RenderVideo(void)
{
	char buff[128];

	sprintf_s(buff, sizeof(buff), "Gamma Levels %i", CConfigLoader::GetInstance()->GetGammaLevel());
	m_BMF->Print(buff,m_nFontX,m_nFontStart,false,false,m_fScale);

	//sprintf_s(buff, sizeof(buff), "\\VSync\\%b", CConfigLoader::GetInstance()->GetVSync());
	if(CConfigLoader::GetInstance()->GetVSync())
		m_BMF->Print("VSync: Activated",m_nFontX,m_nFontStart+m_nFontOffset,false,false,m_fScale);
	else
		m_BMF->Print("VSync: DeActivated",m_nFontX,m_nFontStart+m_nFontOffset,false,false,m_fScale);

	//sprintf_s(buff, sizeof(buff), "\\Windowed\\%b", CConfigLoader::GetInstance()->GetWindowed());
	if(CConfigLoader::GetInstance()->GetWindowed())
		m_BMF->Print("Window Mode: Activated",m_nFontX,m_nFontStart+(m_nFontOffset*2),false,false,m_fScale);
	else
		m_BMF->Print("Window Mode: DeActivated",m_nFontX,m_nFontStart+(m_nFontOffset*2),false,false,m_fScale);

		m_BMF->Print("Reset to Default",m_nFontX,m_nFontStart+(m_nFontOffset*3),false,false,m_fScale);

	/*char videoBuff[128];
	sprintf_s(videoBuff, sizeof(videoBuff), "\\Video Mode\\%i x %i", CDirect3D::GetInstance()->GetDisplayMode(m_nVideoModeSelection).Width,
	CDirect3D::GetInstance()->GetDisplayMode(m_nVideoModeSelection).Height);
	m_BMF->Print(videoBuff,m_nFontX,m_nFontStart+(m_nFontOffset*3),false,false,m_fScale);*/

	m_BMF->Print("Exit",m_nFontX,m_nFontStart+(m_nFontOffset*4),false,false,m_fScale);
}