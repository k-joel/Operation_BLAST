#include "CPauseState.h"

#include "CStateMachine.h"
#include "CMainMenuState.h"
#include "CGameState.h"
#include "COptionState.h"
#include "CLoadState.h"
#include "../Mesh/modelImporter.h"
#include "../Rendering/CShaderManager.h"
#include "../Rendering/CTextureManager.h"
#include "../Rendering/CRenderer.h"
#include "../Rendering/CRenderContextManager.h"
#include "../Mesh/CModelManager.h"
#include "../Font/CBitMapFont.h"
#include "../Input/CInput.h"
#include "../Camera/Camera.h"

#include "../Resources/Sound/Wwise_IDs.h"
#include "../Sound/WwiseManager.h"

#include "../Event System/CEventSystem.h"

const int FONTSELECTIONS = 4;
const int SELECTIONS = 2;

CPauseState::CPauseState(void)
{
	m_nSelection = 0;
	m_fScale = 1.0f;
	m_nFontX = 300;
	m_nArrowPosition = m_nFontX-50;
	m_nFontOffset = 42;
	m_nFontStart = 300;
	m_nConsoleOffset = 0;
	m_bEnterDone = false;
	m_fMovementBucket = 0.0f;

	m_CBF = CBitMapFont::GetInstance();
}

CPauseState::~CPauseState(void)
{
}

void CPauseState::Enter(void)
{
	D3DXMatrixIdentity(&m_ConsoleWorld);

	D3DXVECTOR3 location = CCamera::GetInstance()->GetViewPosition();
	D3DXMATRIX rotation;
	D3DXMatrixRotationX(&rotation,D3DXToRadian(13));
	D3DXMatrixTranslation(&m_ConsoleWorld,location.x,location.y-13,location.z);
	D3DXMatrixMultiply(&m_ConsoleWorld,&rotation,&m_ConsoleWorld);

	m_consoleScreen = CModelManager::GetInstance()->GetModel(CLoadState::GetInstance()->GetModelID("PauseMenuScreenShape"));
	m_ConsoleShape.Init(&m_consoleScreen->GetMesh(), NULL, &CRenderContextManager::GetInstance()->GetHUDContext(), m_ConsoleWorld, CRenderShape::PNTRenderFunc);

	m_Buttons.Initialize(true);
	
	//Reseting Variables
	m_nSelection = 0;
	m_fMovementBucket = 0.0f;
	m_nConsoleOffset = 0;
	m_bEnterDone = false;
	m_bExitDone = false;
	m_bReverse = false;
	m_bRender = true;
	m_bConf = false;

	CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PAUSE_ALL, 200);
}

bool CPauseState::Input(void)
{
	KeyboardDevice* keyboard = (KeyboardDevice*)CInput::GetInstance()->GetDevice(DEV_KEYBOARD);

	if(m_bEnterDone)
	{
		static float m_fPrevY = 0.0f;
		static float m_fCurrY = 0.0f;
		m_fPrevY = m_fCurrY;
		m_fCurrY = CInput::GetInstance()->GetActionAmount(ACT_MOVEY);

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

		if(CInput::GetInstance()->GetAction(ACT_MOVEDOWN) || CInput::GetInstance()->GetAction(ACT_MOVEDOWN, 1))
		{
			m_nSelection ++;
			CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_MENU_UP, 200); // AudioID 200 is MENU
		}

		if(CInput::GetInstance()->GetAction(ACT_MOVEUP) || CInput::GetInstance()->GetAction(ACT_MOVEUP, 1))
		{
			m_nSelection --;
			CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_MENU_DOWN, 200); // AudioID 200 is MENU
		}

		if(m_nSelection > SELECTIONS)
		{
			m_nSelection = 0;
		}

		if( m_nSelection < 0)
		{
			m_nSelection = SELECTIONS;
		}

		if((keyboard->KeyPressed(DIK_RETURN) || CInput::GetInstance()->GetAction(ACT_ACCEPT) || CInput::GetInstance()->GetAction(ACT_ACCEPT, 1)) && !m_bConf)
		{
			//CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_MENU_SELECT, 200);	// AudioID 200 is MENU
			switch(m_nSelection)
			{
			case 0:
				m_bReverse = true;
				m_bEnterDone = false;
				CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_MENU_GOBACK, 200);
				//CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::RESUME_ALL, 200);
				break;
			case 1:
				m_bRender = false;
				CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_MENU_SELECT, 200);
				CStateSystem::GetInstance()->AddState(COptionState::GetInstance());
				break;
			case 2:
				CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_MENU_GOBACK, 200);
				//CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::STOP_ALL, 200);
				m_bConf = true;
				return true;
				break;
			}
		}

		if(m_bConf)
		{

			static float m_fPrevX = 0.0f;
			static float m_fCurrX = 0.0f;
			m_fPrevX = m_fCurrX;
			m_fCurrX = CInput::GetInstance()->GetActionAmount(ACT_MOVEX);
			if(m_fCurrX >= 32760 && m_fPrevX != m_fCurrX)
			{
				m_nConfirm --;
				CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_MENU_UP, 200);
			}
			if(m_fCurrX <= -32760 && m_fPrevX != m_fCurrX)
			{
				m_nConfirm ++;
				CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_MENU_UP, 200);
			}

			if(CInput::GetInstance()->GetAction(ACT_MOVELEFT))
			{
				m_nConfirm--;
				CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_MENU_UP, 200);

			}

			if(CInput::GetInstance()->GetAction(ACT_MOVERIGHT))
			{
				m_nConfirm++;
				CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_MENU_UP, 200);
			}

			if(m_nConfirm < 0)
			{
				m_nConfirm = 1;
			}

			if(m_nConfirm > 1)
			{
				m_nConfirm = 0;
			}

			if(CInput::GetInstance()->GetAction(ACT_ACCEPT))
			{
				switch(m_nConfirm)
				{
				case 0:
					CStateSystem::GetInstance()->ChangeBaseState(CMainMenuState::GetInstance());
					break;
				case 1:
					m_nConfirm = 0;
					m_bConf = false;
					return true;
					break;
				}
			}
		}
	}
	return true;
}

void CPauseState::Update(float fElapsedTime)
{
	m_mxViewProj  = CCamera::GetInstance()->GetViewMatrix() * CCamera::GetInstance()->GetProjectionMatrix();

	if(!m_bEnterDone && !m_bReverse)
	{
		m_fMovementBucket += fElapsedTime;

		if(m_fMovementBucket > 0.05)
		{
			m_fMovementBucket = 0.0f;
			if(m_nConsoleOffset < 29)
			{
				D3DXVECTOR3 location = CCamera::GetInstance()->GetViewPosition();
				D3DXMATRIX rotation;
				D3DXMatrixRotationX(&rotation,D3DXToRadian(8));
				D3DXMatrixTranslation(&m_ConsoleWorld,location.x+0.25f,location.y-13.75f,location.z+m_nConsoleOffset);
				D3DXMatrixMultiply(&m_ConsoleWorld,&rotation,&m_ConsoleWorld);

				//D3DXMatrixTranslation(&m_ConsoleWorld,location.x,location.y-17,location.z+m_nConsoleOffset);
				++m_nConsoleOffset;
			}
			else
			{
				m_nConsoleOffset = 0;
				m_bEnterDone = true;
			}
		}
	}

	if(m_bReverse)
	{
		m_fMovementBucket += fElapsedTime;

		if(m_fMovementBucket > 0.05)
		{
			m_fMovementBucket = 0.0f;
			if(m_nConsoleOffset < 29)
			{
				++m_nConsoleOffset;
				D3DXVECTOR3 location = CCamera::GetInstance()->GetViewPosition();
				D3DXMATRIX rotation;
				D3DXMatrixIdentity(&rotation);
				D3DXMatrixRotationX(&rotation,D3DXToRadian(8));
				D3DXMatrixTranslation(&m_ConsoleWorld,location.x+0.25f,location.y-13.75f,(location.z+26)-m_nConsoleOffset);
				D3DXMatrixMultiply(&m_ConsoleWorld,&rotation,&m_ConsoleWorld);

				//D3DXMatrixTranslation(&m_ConsoleWorld,location.x,location.y-17,location.z+m_nConsoleOffset);
			}
			else
			{
				m_bExitDone = true;
				m_bReverse = false;
				CGameState::GetInstance()->SetIsPaused(false);
				//CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::RESUME_ALL, 200);
				CStateSystem::GetInstance()->RemoveTopState();
			}
		}
	}
	m_bRender = true;
}

void CPauseState::Render(void)
{

	if(m_bEnterDone && m_bRender && !m_bConf)
	{

		RenderMenu();
		m_Buttons.Render(m_mxViewProj,true,false,true,false,((XboxGamePadDevice*)CInput::GetInstance()->GetDevice(DEV_GAMEPAD))->ControllerConnected(0));
		m_CBF->Print("=>",m_nArrowPosition,m_nFontStart+(m_nFontOffset*m_nSelection),false,false,m_fScale);
	}

	
	if(m_bConf)
	{
		m_Buttons.Render(m_mxViewProj,false,true,true,false,((XboxGamePadDevice*)CInput::GetInstance()->GetDevice(DEV_GAMEPAD))->ControllerConnected(0));
		m_CBF->Print("=>",m_nArrowPosition*(m_nConfirm+1),m_nFontStart+(m_nFontOffset*2),false,false,0.85f);
		m_CBF->Print("Do you wish to go to Main Menu?",160,m_nFontStart+(m_nFontOffset),false,false,0.85f);
		m_CBF->Print("Yes",m_nFontX,m_nFontStart+(m_nFontOffset*2),false,false,0.85f);
		m_CBF->Print("No",m_nFontX+m_nFontX,m_nFontStart+(m_nFontOffset*2),false,false,0.85f);
	}

	m_ConsoleShape.GetFrame().GetLocalMatrix() = m_ConsoleWorld * m_mxViewProj;
	m_ConsoleShape.GetRenderContext()->AddRenderNode(&m_ConsoleShape);

	CRenderContextManager::GetInstance()->Render();
}

bool CPauseState::Exit(void)
{
	CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::RESUME_ALL, 200);
	CEventSystem::GetInstance()->DispatchEvent(*CEventSystem::GetInstance()->FindEvent("ShowPlayerID"), NULL, 1.0f);
	return false;
}

CPauseState* CPauseState::GetInstance(void)
{
	static CPauseState instance;
	return &instance;
}

void CPauseState::RenderMenu(void)
{
	m_CBF->Print("PAUSE",m_nFontX,110,false,true,m_fScale);

	m_CBF->Print("Resume",m_nFontX,m_nFontStart,false,false,m_fScale);

	m_CBF->Print("Options",m_nFontX,m_nFontStart+(m_nFontOffset),false,false,m_fScale);

	m_CBF->Print("Main Menu",m_nFontX,m_nFontStart+(m_nFontOffset*2),false,false,m_fScale);
}