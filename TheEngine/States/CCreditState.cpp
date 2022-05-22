#include "CCreditState.h"
#include "CStateMachine.h"
#include "CMainMenuState.h"
#include "CGameState.h"
#include "CLoadState.h"
#include "../Font/CBitMapFont.h"
#include "../Input/CInput.h"
#include "../Rendering/CTextureManager.h"
#include "../Camera/Camera.h"
#include "../Resources/Sound/Wwise_IDs.h"
#include "../Sound/WwiseManager.h"

CCreditState::CCreditState(void)
{
	m_fScale = 0.85f;
	m_nFontX = 525;
	m_nArrowPosition = 120;
	m_nFontOffset = 42;
	m_nFontStart = 250;
	m_fCreditBucket = 0.0;
}

CCreditState::~CCreditState(void)
{

}

void CCreditState::Enter(void)
{
	name.clear();
	title.clear();
	m_bOpen = false;
	// Set up the font for the screen
	m_CBF = CBitMapFont::GetInstance();
	m_CTM = CTextureManager::GetInstance();
	m_CIP = CInput::GetInstance();
	in.open("credits.txt");
	if(in.is_open())
	{
		m_bOpen = true;
	}

	m_Buttons.Initialize(true);
	m_fCreditBucket = 3.0;
	
}

bool CCreditState::Input(void)
{
	KeyboardDevice* keyboard = (KeyboardDevice*)CInput::GetInstance()->GetDevice(DEV_KEYBOARD);

	if(m_CIP->GetAction(ACT_ACCEPT)||m_CIP->GetAction(ACT_ACCEPT,1))
	{
		CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_MENU_GOBACK, 200);
		CMainMenuState::GetInstance()->SetOptions(false);
		CStateSystem::GetInstance()->RemoveTopState();
	}

	return true;
}

void CCreditState::Update(float fElapsedTime)
{
	m_fCreditBucket+= fElapsedTime;

	if(!CGameState::GetInstance()->GetPaused())
	{
		CMainMenuState::GetInstance()->Update(fElapsedTime);
	}

	m_mxViewProj  = CCamera::GetInstance()->GetViewMatrix() * CCamera::GetInstance()->GetProjectionMatrix();

	if(m_fCreditBucket > 2.0)
	{
		LoadCredits();
		m_fCreditBucket = 0.0;
	}
}

void CCreditState::Render(void)
{
	CTextureManager::GetInstance()->Draw(CLoadState::GetInstance()->GetImageID("credit"),0,0,1.0f,0.75f);
	for(unsigned int i = 0; i < name.size(); i++)
	{
		m_CBF->Print(name.c_str(),m_nFontX,m_nFontStart,true,false,m_fScale);
		m_CBF->Print(title.c_str(),m_nFontX,m_nFontStart+100,true,false,m_fScale);
		switch(m_nImage)
		{
		case 0:
			break;
		case 1:
			CTextureManager::GetInstance()->Draw(CLoadState::GetInstance()->GetImageID("Lua"),m_nFontX-75,m_nFontStart+140);
			break;
		case 2:
			CTextureManager::GetInstance()->Draw(CLoadState::GetInstance()->GetImageID("Wwise"),m_nFontX-120,m_nFontStart+140);
			break;
		case 3:
			break;
		}
	}
	m_Buttons.Render(m_mxViewProj,false,false,true,false,((XboxGamePadDevice*)CInput::GetInstance()->GetDevice(DEV_GAMEPAD))->ControllerConnected(0));

}

bool CCreditState::Exit(void)
{
	if(m_bOpen)
	{
		in.clear();
		in.close();
	}
	return false;
}

CCreditState* CCreditState::GetInstance(void)
{
	static CCreditState instance;
	return &instance;
}

void CCreditState::LoadCredits()
{
	if(m_bOpen)
	{
		if(!in.eof())
		{
			char names[100];
			in.get(names,sizeof name, '\n');
			name = names;
			in.ignore(INT_MAX,'\n');
			
			char titles[100];
			in.get(titles,sizeof title, '\n');
			in.ignore(INT_MAX,'\n');
			title = titles;

			in >> m_nImage;
			in.ignore(INT_MAX,'\n');
		}
	}
}