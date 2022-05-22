#include "CSoundOptionState.h"
#include "../Rendering/CTextureManager.h"
#include "../Input/CInput.h"
#include "../States/CStateMachine.h"
#include "../Font/CBitMapFont.h"
#include "CGameplayOptionState.h"
#include "CVideoOptionState.h"
#include "CControlOptionState.h"
#include "CLoadState.h"
#include "CMainMenuState.h"
#include "../Core/CGame.h"
#include "../Core/CConfigLoader.h"
#include "../Resources/Sound/Wwise_IDs.h"
#include "../Sound/WwiseManager.h"
#include "CGameState.h"

using namespace std;

//const AkGameObjectID MUSIC_TEST = 100;
//const AkGameObjectID SFX_TEST = 200;
//const AkGameObjectID MASTER_TEST = 300;
const AkGameObjectID MENU = 200;

CSoundOptionState::CSoundOptionState(void)
{
	// Set the volume of sounds
	//CWwiseManager::GetInstance()->SetMXVolume((float)CGame::GetInstance()->GetMusicVolume());
	//CWwiseManager::GetInstance()->SetFXVolume((float)CGame::GetInstance()->GetSFXVolume());
	//CWwiseManager::GetInstance()->SetMasterVolume((float)CGame::GetInstance()->GetMasterVolume());

	m_fScale = 1.0f;
	m_nFontX = 200;
	m_nArrowPosition = m_nFontX-50;
	m_nFontOffset = 42;
	m_nFontStart = 150;
	m_nPreviousSFXVolume = 0;
	m_nPreviousDXVolume = 0;
}

CSoundOptionState::~CSoundOptionState(void)
{
}

void CSoundOptionState::Enter(void)
{
	m_TM = CTextureManager::GetInstance();
	m_IPM = CInput::GetInstance();
	m_SM = CStateSystem::GetInstance();
	m_BMF = CBitMapFont::GetInstance();

	//Setting Up Variables
	m_nSelection = 0;
	m_fScale = 1.0f;
	m_nFontX = 300;
	m_nArrowPosition = m_nFontX-50;
	m_nFontOffset = 42;
	m_nFontStart = 150;


	m_Buttons.Initialize(true);

	// Sound Variables
	m_nPreviousSFXVolume = CGame::GetInstance()->GetSFXVolume();
	m_nPreviousDXVolume = CGame::GetInstance()->GetDialogVolume();
	/*CWwiseManager::GetInstance()->RegisterObject(MUSIC_TEST, "Music Test");
	CWwiseManager::GetInstance()->RegisterObject(SFX_TEST, "SFX Test");
	CWwiseManager::GetInstance()->RegisterObject(MASTER_TEST, "Master Test");*/

	//CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_MX_MUSICLOOP_02, MENU);
}

bool CSoundOptionState::Input(void)
{
	KeyboardDevice* keyboard = (KeyboardDevice*)CInput::GetInstance()->GetDevice(DEV_KEYBOARD);

	//if(keyboard->KeyPressed(DIK_S))

	//Player 1
	static float m_fPrevY = 0.0f;
	static float m_fCurrY = 0.0f;
	m_fPrevY = m_fCurrY;
	m_fCurrY = CInput::GetInstance()->GetActionAmount(ACT_MOVEY);

	//Player 2
	static float m_fPrevY2 = 0.0f;
	static float m_fCurrY2 = 0.0f;
	m_fPrevY2 = m_fCurrY2;
	m_fCurrY2 = CInput::GetInstance()->GetActionAmount(ACT_MOVEY,1);


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

	if(CInput::GetInstance()->GetAction(ACT_MOVEDOWN)||CInput::GetInstance()->GetAction(ACT_MOVEDOWN,1))
	{
		m_nSelection++;
		CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_MENU_DOWN, MENU);
	}

	//if(keyboard->KeyPressed(DIK_W))
	if(CInput::GetInstance()->GetAction(ACT_MOVEUP)||CInput::GetInstance()->GetAction(ACT_MOVEUP,1))
	{
		m_nSelection--;
		CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_MENU_UP, MENU);
	}

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

	//if(keyboard->KeyDown(DIK_A))
	//Player 1
	static float m_fPrevX = 0.0f;
	static float m_fCurrX = 0.0f;
	m_fPrevX = m_fCurrX;
	m_fCurrX = CInput::GetInstance()->GetActionAmount(ACT_MOVEX);

	//Player 2
	static float m_fPrevX2 = 0.0f;
	static float m_fCurrX2 = 0.0f;
	m_fPrevX2 = m_fCurrX2;
	m_fCurrX2 = CInput::GetInstance()->GetActionAmount(ACT_MOVEX,1);

	//Player 1
	if(m_fCurrX >= 32765 && m_fPrevX != m_fCurrX)
	{
		switch(m_nSelection)
		{
		case 0:
			{
				// Master
				if(CGame::GetInstance()->GetMasterVolume() < 100)
				{
					CGame::GetInstance()->SetMasterVolume(CGame::GetInstance()->GetMasterVolume() +5);
					CConfigLoader::GetInstance()->SetMasterVolume(CGame::GetInstance()->GetMasterVolume());
					CWwiseManager::GetInstance()->SetMasterVolume((float)CGame::GetInstance()->GetMasterVolume());
				}
				break;
			}
		case 1:
			{
				// Music
				if(CGame::GetInstance()->GetMusicVolume() < 100)
				{
					CGame::GetInstance()->SetMusicVolume(CGame::GetInstance()->GetMusicVolume() +5);
					CConfigLoader::GetInstance()->SetMusicVolume(CGame::GetInstance()->GetMusicVolume());
					CWwiseManager::GetInstance()->SetMXVolume((float)CGame::GetInstance()->GetMusicVolume());
				}
				break;
			}
		case 2:
			{
				// SFX
				if(CGame::GetInstance()->GetSFXVolume() < 105)
				{
					m_nPreviousSFXVolume = CGame::GetInstance()->GetSFXVolume();
					if(CGame::GetInstance()->GetSFXVolume() < 100)
						CGame::GetInstance()->SetSFXVolume(CGame::GetInstance()->GetSFXVolume() +5);
					CConfigLoader::GetInstance()->SetSFXVolume(CGame::GetInstance()->GetSFXVolume());
					CWwiseManager::GetInstance()->SetFXVolume((float)CGame::GetInstance()->GetSFXVolume());
				}
				break;
			}
		case 3:
			{
				// Dialoge
				if(CGame::GetInstance()->GetDialogVolume() < 105)
				{
					m_nPreviousDXVolume = CGame::GetInstance()->GetDialogVolume();
					if(CGame::GetInstance()->GetDialogVolume() < 100)
						CGame::GetInstance()->SetDialogVolume(CGame::GetInstance()->GetDialogVolume() +5);
					CConfigLoader::GetInstance()->SetDialogueVolume(CGame::GetInstance()->GetDialogVolume());
					CWwiseManager::GetInstance()->SetDXVolume((float)CGame::GetInstance()->GetDialogVolume());
				}
			}
		}
	}

	//Player 1
	if(m_fCurrX <= -32765 && m_fPrevX != m_fCurrX)
	{
		switch(m_nSelection)
		{
		case 0:
			{
				// Master
				if(CGame::GetInstance()->GetMasterVolume() > 0)
				{
					CGame::GetInstance()->SetMasterVolume(CGame::GetInstance()->GetMasterVolume() -5);
					CConfigLoader::GetInstance()->SetMasterVolume(CGame::GetInstance()->GetMasterVolume());
					CWwiseManager::GetInstance()->SetMasterVolume((float)CGame::GetInstance()->GetMasterVolume());
				}
				break;
			}
		case 1:
			{
				// Music
				if(CGame::GetInstance()->GetMusicVolume() > 0)
				{
					CGame::GetInstance()->SetMusicVolume(CGame::GetInstance()->GetMusicVolume() -5);
					CConfigLoader::GetInstance()->SetMusicVolume(CGame::GetInstance()->GetMusicVolume());
					CWwiseManager::GetInstance()->SetMXVolume((float)CGame::GetInstance()->GetMusicVolume());
				}
				break;
			}
		case 2:
			{
				// SFX
				if(CGame::GetInstance()->GetSFXVolume() > 0)
				{
					m_nPreviousSFXVolume = CGame::GetInstance()->GetSFXVolume();
					CGame::GetInstance()->SetSFXVolume(CGame::GetInstance()->GetSFXVolume() -5);
					CConfigLoader::GetInstance()->SetSFXVolume(CGame::GetInstance()->GetSFXVolume());
					CWwiseManager::GetInstance()->SetFXVolume((float)CGame::GetInstance()->GetSFXVolume());
				}
				break;
			}
		case 3:
			{
				// Dialogue
				if(CGame::GetInstance()->GetDialogVolume() > 0)
				{
					m_nPreviousDXVolume = CGame::GetInstance()->GetDialogVolume();
					CGame::GetInstance()->SetDialogVolume(CGame::GetInstance()->GetDialogVolume() -5);
					CConfigLoader::GetInstance()->SetDialogueVolume(CGame::GetInstance()->GetDialogVolume());
					CWwiseManager::GetInstance()->SetDXVolume((float)CGame::GetInstance()->GetDialogVolume());
				}
				break;
			}
		}
	}

	//Player 2
	if(m_fCurrX2 >= 32765 && m_fPrevX2 != m_fCurrX2)
	{
		switch(m_nSelection)
		{
		case 0:
			{
				// Master
				if(CGame::GetInstance()->GetMasterVolume() < 100)
				{
					CGame::GetInstance()->SetMasterVolume(CGame::GetInstance()->GetMasterVolume() +5);
					CConfigLoader::GetInstance()->SetMasterVolume(CGame::GetInstance()->GetMasterVolume());
					CWwiseManager::GetInstance()->SetMasterVolume((float)CGame::GetInstance()->GetMasterVolume());
				}
				break;
			}
		case 1:
			{
				// Music
				if(CGame::GetInstance()->GetMusicVolume() < 100)
				{
					CGame::GetInstance()->SetMusicVolume(CGame::GetInstance()->GetMusicVolume() +5);
					CConfigLoader::GetInstance()->SetMusicVolume(CGame::GetInstance()->GetMusicVolume());
					CWwiseManager::GetInstance()->SetMXVolume((float)CGame::GetInstance()->GetMusicVolume());
				}
				break;
			}
		case 2:
			{
				// SFX
				if(CGame::GetInstance()->GetSFXVolume() < 105)
				{
					m_nPreviousSFXVolume = CGame::GetInstance()->GetSFXVolume();
					if(CGame::GetInstance()->GetSFXVolume() < 100)
						CGame::GetInstance()->SetSFXVolume(CGame::GetInstance()->GetSFXVolume() +5);
					CConfigLoader::GetInstance()->SetSFXVolume(CGame::GetInstance()->GetSFXVolume());
					CWwiseManager::GetInstance()->SetFXVolume((float)CGame::GetInstance()->GetSFXVolume());
				}
				break;
			}
		case 3:
			{
				// Dialoge
				if(CGame::GetInstance()->GetDialogVolume() < 105)
				{
					m_nPreviousDXVolume = CGame::GetInstance()->GetDialogVolume();
					if(CGame::GetInstance()->GetDialogVolume() < 100)
						CGame::GetInstance()->SetDialogVolume(CGame::GetInstance()->GetDialogVolume() +5);
					CConfigLoader::GetInstance()->SetDialogueVolume(CGame::GetInstance()->GetDialogVolume());
					CWwiseManager::GetInstance()->SetDXVolume((float)CGame::GetInstance()->GetDialogVolume());
				}
			}
		}
	}

	//Player 2
	if(m_fCurrX2 <= -32765 && m_fPrevX2 != m_fCurrX2)
	{
		switch(m_nSelection)
		{
		case 0:
			{
				// Master
				if(CGame::GetInstance()->GetMasterVolume() > 0)
				{
					CGame::GetInstance()->SetMasterVolume(CGame::GetInstance()->GetMasterVolume() -5);
					CConfigLoader::GetInstance()->SetMasterVolume(CGame::GetInstance()->GetMasterVolume());
					CWwiseManager::GetInstance()->SetMasterVolume((float)CGame::GetInstance()->GetMasterVolume());
				}
				break;
			}
		case 1:
			{
				// Music
				if(CGame::GetInstance()->GetMusicVolume() > 0)
				{
					CGame::GetInstance()->SetMusicVolume(CGame::GetInstance()->GetMusicVolume() -5);
					CConfigLoader::GetInstance()->SetMusicVolume(CGame::GetInstance()->GetMusicVolume());
					CWwiseManager::GetInstance()->SetMXVolume((float)CGame::GetInstance()->GetMusicVolume());
				}
				break;
			}
		case 2:
			{
				// SFX
				if(CGame::GetInstance()->GetSFXVolume() > 0)
				{
					m_nPreviousSFXVolume = CGame::GetInstance()->GetSFXVolume();
					CGame::GetInstance()->SetSFXVolume(CGame::GetInstance()->GetSFXVolume() -5);
					CConfigLoader::GetInstance()->SetSFXVolume(CGame::GetInstance()->GetSFXVolume());
					CWwiseManager::GetInstance()->SetFXVolume((float)CGame::GetInstance()->GetSFXVolume());
				}
				break;
			}
		case 3:
			{
				// Dialogue
				if(CGame::GetInstance()->GetDialogVolume() > 0)
				{
					m_nPreviousDXVolume = CGame::GetInstance()->GetDialogVolume();
					CGame::GetInstance()->SetDialogVolume(CGame::GetInstance()->GetDialogVolume() -5);
					CConfigLoader::GetInstance()->SetDialogueVolume(CGame::GetInstance()->GetDialogVolume());
					CWwiseManager::GetInstance()->SetDXVolume((float)CGame::GetInstance()->GetDialogVolume());
				}
				break;
			}
		}
	}

	if(CInput::GetInstance()->GetAction(ACT_MOVELEFT)||CInput::GetInstance()->GetAction(ACT_MOVELEFT,1))
	{
		switch(m_nSelection)
		{
		case 0:
			{
				// Master
				if(CGame::GetInstance()->GetMasterVolume() > 0)
				{
					CGame::GetInstance()->SetMasterVolume(CGame::GetInstance()->GetMasterVolume() -5);
					CConfigLoader::GetInstance()->SetMasterVolume(CGame::GetInstance()->GetMasterVolume());
					CWwiseManager::GetInstance()->SetMasterVolume((float)CGame::GetInstance()->GetMasterVolume());
				}
				break;
			}
		case 1:
			{
				// Music
				if(CGame::GetInstance()->GetMusicVolume() > 0)
				{
					CGame::GetInstance()->SetMusicVolume(CGame::GetInstance()->GetMusicVolume() -5);
					CConfigLoader::GetInstance()->SetMusicVolume(CGame::GetInstance()->GetMusicVolume());
					CWwiseManager::GetInstance()->SetMXVolume((float)CGame::GetInstance()->GetMusicVolume());
				}
				break;
			}
		case 2:
			{
				// SFX
				if(CGame::GetInstance()->GetSFXVolume() > 0)
				{
					m_nPreviousSFXVolume = CGame::GetInstance()->GetSFXVolume();
					CGame::GetInstance()->SetSFXVolume(CGame::GetInstance()->GetSFXVolume() -5);
					CConfigLoader::GetInstance()->SetSFXVolume(CGame::GetInstance()->GetSFXVolume());
					CWwiseManager::GetInstance()->SetFXVolume((float)CGame::GetInstance()->GetSFXVolume());
				}
				break;
			}
		case 3:
			{
				// Dialogue
				if(CGame::GetInstance()->GetDialogVolume() > 0)
				{
					m_nPreviousDXVolume = CGame::GetInstance()->GetDialogVolume();
					CGame::GetInstance()->SetDialogVolume(CGame::GetInstance()->GetDialogVolume() -5);
					CConfigLoader::GetInstance()->SetDialogueVolume(CGame::GetInstance()->GetDialogVolume());
					CWwiseManager::GetInstance()->SetDXVolume((float)CGame::GetInstance()->GetDialogVolume());
				}
				break;
			}
		}
	}

	//if(keyboard->KeyDown(DIK_D))
	if(CInput::GetInstance()->GetAction(ACT_MOVERIGHT)||CInput::GetInstance()->GetAction(ACT_MOVERIGHT,1))
	{
		switch(m_nSelection)
		{
		case 0:
			{
				// Master
				if(CGame::GetInstance()->GetMasterVolume() < 100)
				{
					CGame::GetInstance()->SetMasterVolume(CGame::GetInstance()->GetMasterVolume() +5);
					CConfigLoader::GetInstance()->SetMasterVolume(CGame::GetInstance()->GetMasterVolume());
					CWwiseManager::GetInstance()->SetMasterVolume((float)CGame::GetInstance()->GetMasterVolume());
				}
				break;
			}
		case 1:
			{
				// Music
				if(CGame::GetInstance()->GetMusicVolume() < 100)
				{
					CGame::GetInstance()->SetMusicVolume(CGame::GetInstance()->GetMusicVolume() +5);
					CConfigLoader::GetInstance()->SetMusicVolume(CGame::GetInstance()->GetMusicVolume());
					CWwiseManager::GetInstance()->SetMXVolume((float)CGame::GetInstance()->GetMusicVolume());
				}
				break;
			}
		case 2:
			{
				// SFX
				if(CGame::GetInstance()->GetSFXVolume() < 105)
				{
					m_nPreviousSFXVolume = CGame::GetInstance()->GetSFXVolume();
					if(CGame::GetInstance()->GetSFXVolume() < 100)
						CGame::GetInstance()->SetSFXVolume(CGame::GetInstance()->GetSFXVolume() +5);
					CConfigLoader::GetInstance()->SetSFXVolume(CGame::GetInstance()->GetSFXVolume());
					CWwiseManager::GetInstance()->SetFXVolume((float)CGame::GetInstance()->GetSFXVolume());
				}
				break;
			}
		case 3:
			{
				// Dialoge
				if(CGame::GetInstance()->GetDialogVolume() < 105)
				{
					m_nPreviousDXVolume = CGame::GetInstance()->GetDialogVolume();
					if(CGame::GetInstance()->GetDialogVolume() < 100)
						CGame::GetInstance()->SetDialogVolume(CGame::GetInstance()->GetDialogVolume() +5);
					CConfigLoader::GetInstance()->SetDialogueVolume(CGame::GetInstance()->GetDialogVolume());
					CWwiseManager::GetInstance()->SetDXVolume((float)CGame::GetInstance()->GetDialogVolume());
				}
			}
		}
	}

	if(m_nSelection > 4)
		m_nSelection = 0;

	if(m_nSelection < 0)
		m_nSelection = 4;

	if(keyboard->KeyPressed(DIK_RETURN) || CInput::GetInstance()->GetAction(ACT_ACCEPT)||CInput::GetInstance()->GetAction(ACT_ACCEPT,1))
	{
		if(m_nSelection == 4)
		{
			CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_MENU_GOBACK, MENU);
			m_SM->RemoveTopState();
		}
	}

	if(m_nSelection == 2)
	{
		if( ( keyboard->KeyRelease(DIK_A) || keyboard->KeyRelease(DIK_D) || CInput::GetInstance()->GetAction(ACT_RELEASELEFT) || CInput::GetInstance()->GetAction(ACT_RELEASERIGHT) || CInput::GetInstance()->GetAction(ACT_RELEASELEFT,1)||CInput::GetInstance()->GetAction(ACT_RELEASERIGHT,1) ) 
			&& (CGame::GetInstance()->GetSFXVolume() != m_nPreviousSFXVolume) )
			CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_MACHINEGUN, MENU);
	}

	if(m_nSelection == 3)
	{
		if( (keyboard->KeyRelease(DIK_A) || keyboard->KeyRelease(DIK_D) || CInput::GetInstance()->GetAction(ACT_RELEASELEFT) || CInput::GetInstance()->GetAction(ACT_RELEASERIGHT) || CInput::GetInstance()->GetAction(ACT_RELEASELEFT,1)||CInput::GetInstance()->GetAction(ACT_RELEASERIGHT,1) ) 
			&& (CGame::GetInstance()->GetDialogVolume() != m_nPreviousDXVolume) )
			CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_DX_BARRON_LAND, MENU);
	}

	return true;
}

void CSoundOptionState::Update(float fElapsedTime)
{
	if(!CGameState::GetInstance()->GetPaused())
	{
		CMainMenuState::GetInstance()->Update(fElapsedTime);
	}
}

void CSoundOptionState::Render(void)
{
	//CTextureManager::GetInstance()->Draw(m_nBackground,0,0,0.5,0.75);

	D3DXMATRIX m_mxViewProj  = CCamera::GetInstance()->GetViewMatrix() * CCamera::GetInstance()->GetProjectionMatrix();

	char buff[128];

	m_Buttons.Render(m_mxViewProj,true,true,true,false,((XboxGamePadDevice*)CInput::GetInstance()->GetDevice(DEV_GAMEPAD))->ControllerConnected(0));

	sprintf_s(buff, sizeof(buff), "Master: %i", CGame::GetInstance()->GetMasterVolume());
		m_BMF->Print(buff,m_nFontX,m_nFontStart,false,false,m_fScale);

	sprintf_s(buff, sizeof(buff), "Music: %i", CGame::GetInstance()->GetMusicVolume());
		m_BMF->Print(buff,m_nFontX,m_nFontStart+m_nFontOffset,false,false,m_fScale);

	sprintf_s(buff, sizeof(buff), "SFX: %i", CGame::GetInstance()->GetSFXVolume());
		m_BMF->Print(buff,m_nFontX, m_nFontStart+(m_nFontOffset*2),false,false,m_fScale);

	sprintf_s(buff, sizeof(buff), "Dialog: %i", CGame::GetInstance()->GetDialogVolume());
		m_BMF->Print(buff,m_nFontX, m_nFontStart+(m_nFontOffset*3),false,false,m_fScale);

		m_BMF->Print("Exit",m_nFontX,m_nFontStart+(m_nFontOffset*4),false,false,m_fScale);

		m_BMF->Print("=>",m_nArrowPosition,m_nFontStart+(m_nFontOffset*m_nSelection),false,false,m_fScale);
		//I heard that Kahran once punched an infant in the face.
}

bool CSoundOptionState::Exit(void)
{
	//CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::STOP_MX_MUSICLOOP_02, MENU);
	CConfigLoader::GetInstance()->WriteConfig();
	return false;
}

CSoundOptionState* CSoundOptionState::GetInstance(void)
{
	static CSoundOptionState instance;
	return &instance;
}