#include "CMainMenuState.h"
#include "../Core/CGame.h"
#include "CStateMachine.h"
#include "CGameState.h"
#include "COptionState.h"
#include "CCreditState.h"
#include "CLoadState.h"
#include "CCharacterSelectionState.h"
#include "CHighScoreState.h"
#include "CHowToState.h"
#include "CGameState.h"
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
#include "../Game Objects/CButtonGui.h"

#include "../Debug System/DebugSystem.h"

const AkGameObjectID MENU = 200;
const int FONTSELECTIONS = 8;
const int SELECTIONS = 6;

bool CMainMenuState::m_bPlayAudioMenu = false;

CMainMenuState::CMainMenuState(void)
{
	m_nSelection = 0;
	m_fScale = 1.0f;
	m_nFontX = 300;
	m_nArrowPosition = m_nFontX-50;
	m_nFontOffset = 42;
	m_nFontStart = 250;
	m_bConf = false;
	m_nConfirm = 0;
	//buffer = "Welcome  to  OPERATION B.L.A.S.T.";
}

CMainMenuState::~CMainMenuState(void)
{
}

void CMainMenuState::Enter(void)
{
	// Set up the font for the screen
	m_CBF = CBitMapFont::GetInstance();
	m_CTM = CTextureManager::GetInstance();

	// Setup Input for the winstate
	m_CIP = CInput::GetInstance();
	m_Cam = CCamera::GetInstance();
	m_pD3D = CDirect3D::GetInstance();

	D3DXMATRIX reset;

	D3DXMatrixIdentity(&reset);
	m_Cam->SetViewMatrix(&reset);
	// Setup the camera
	float w = (float)m_pD3D->GetPresentParams()->BackBufferWidth;
	float h = (float)m_pD3D->GetPresentParams()->BackBufferHeight;
	m_Cam->BuildPerspective(D3DX_PI * 0.25f, w/h, 1.0f, 5000.0f);
	m_Cam->SetViewPosition(-0.2f,10.0f,20.1f);
	m_Cam->SetBaseY(m_Cam->GetViewPosition().y);
	m_Cam->ViewRotateGlobalX(-.06f);

	D3DXMatrixTranslation(&m_ConsoleWorld,0,0,50);

	m_consoleScreen = CModelManager::GetInstance()->GetModel(CLoadState::GetInstance()->GetModelID("PauseMenuScreenShape"));
	m_ConsoleShape.Init(&m_consoleScreen->GetMesh(), NULL, &CRenderContextManager::GetInstance()->GetHUDContext(), m_ConsoleWorld, CRenderShape::PNTRenderFunc);
	

	/*CModel* model = CModelManager::GetInstance()->GetModel(CLoadState::GetInstance()->GetModelID("Scan"));
	static TUVScrollData data;
	data.fRate = 0.5;
	data.vDirection = D3DXVECTOR2(0.0f,1.0f);
	m_ScanlineShape.Init(&model->GetMesh(),NULL,&CRenderContextManager::GetInstance()->GetStaticUVScrollContext(),m_ConsoleWorld,CRenderShape::UVScrollPNTRenderFunc);
	m_ScanlineShape.SetData((void*)&data);*/

	this->m_Buttons.Initialize(true);

	CWwiseManager::GetInstance()->RegisterObject(MENU, "Menu");
	if(m_bPlayAudioMenu == false)
	{
		CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::STOP_ALL, MENU);
		CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_MENU_AMBIENCE_LOOPS, MENU);
		CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_MX_MENU_MUSIC, MENU);
		//if(m_bDone != true)
			CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_TYPING_LOOP, MENU);
		m_bPlayAudioMenu = true;
	}
	Reset();
	m_bRender = true;
	m_bConf = false;
	m_bDone = true;
	m_nConfirm = 0;
	options = false;

	m_fTotalTime = D3DX_PI/2.0f*3.0f*10.0f;

	REC.Initialize("REC");

	CCamera::GetInstance()->m_MC.GetWorldMatrix(500);
	CCamera::GetInstance()->m_MC.GetWorldMatrix(20.0f,0.0f,1000.0f);
}

bool CMainMenuState::Input(void)
{
	KeyboardDevice* keyboard = (KeyboardDevice*)CInput::GetInstance()->GetDevice(DEV_KEYBOARD);
	//keyboard->KeyPressed(DIK_W)

	if(m_bDone)
	{
		static float m_fPrevY = 0.0f;
		static float m_fCurrY = 0.0f;
		m_fPrevY = m_fCurrY;
		m_fCurrY = CInput::GetInstance()->GetActionAmount(ACT_MOVEY);
		float x = m_fCurrY;
		if(m_fCurrY >= 32760 && m_fPrevY != m_fCurrY)
		{
			m_nSelection --;
			CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_MENU_UP, MENU);
		}
		if(m_fCurrY <= -32760 && m_fPrevY != m_fCurrY)
		{
			m_nSelection ++;
			CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_MENU_UP, MENU);
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
				CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_MENU_UP, MENU);
			}
			if(m_fCurrX <= -32760 && m_fPrevX != m_fCurrX)
			{
				m_nConfirm ++;
				CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_MENU_UP, MENU);
			}

			if(CInput::GetInstance()->GetAction(ACT_MOVELEFT))
			{
				m_nConfirm--;
			}

			if(CInput::GetInstance()->GetAction(ACT_MOVERIGHT))
			{
				m_nConfirm++;
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
					return false;
					break;
				case 1:
					m_nConfirm = 0;
					m_bConf = false;
					return true;
					break;
				}
			}
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

		if(keyboard->KeyDown(DIK_NUMPAD8))
				m_Cam->ViewTranslateLocalZ(0.1f);

			if(keyboard->KeyDown(DIK_NUMPAD2))
				m_Cam->ViewTranslateLocalZ(-0.1f);

			if(keyboard->KeyDown(DIK_NUMPAD4))
				m_Cam->ViewRotateGlobalX(-.01f);

			if(keyboard->KeyDown(DIK_NUMPAD6))
				m_Cam->ViewRotateGlobalX(.01f);

			if(keyboard->KeyDown(DIK_NUMPAD7))
				m_Cam->ViewTranslateLocalX(-0.1f);

			if(keyboard->KeyDown(DIK_NUMPAD9))
				m_Cam->ViewTranslateLocalX(0.1f);

		if(m_nSelection > SELECTIONS)
			m_nSelection = 0;

		if(m_nSelection < 0)
			m_nSelection = SELECTIONS;

		if((keyboard->KeyPressed(DIK_RETURN) || CInput::GetInstance()->GetAction(ACT_ACCEPT)) && !m_bConf)
		{
			CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_MENU_SELECT, MENU);
			switch(m_nSelection)
				{
				case 0:
					CStateSystem::GetInstance()->AddState(CCharacterSelectionState::GetInstance());
					CGame::GetInstance()->SetMultiplayer(false);
					options = true;
					m_bRender = false;
					break;
				case 1:
					// Multiplayer
					{
						XboxGamePadDevice* gamepad = (XboxGamePadDevice*)CInput::GetInstance()->GetDevice(DEV_GAMEPAD);
						if(gamepad->ControllerConnected(1))
						{
							CGame::GetInstance()->SetMultiplayer(true);
							CStateSystem::GetInstance()->AddState(CCharacterSelectionState::GetInstance());
							options = true;
							m_bRender = false;
						}
					}
					break;
				case 2:
					//How To
					CStateSystem::GetInstance()->AddState(CHowToState::GetInstance());
					m_bRender = false;
					options = true;
					break;
				case 3:
					m_bRender = false;
					options = true;
					CStateSystem::GetInstance()->AddState(COptionState::GetInstance());
					break;
				case 4:
					//High Scores
					m_bRender = false;
					options = true;
					CStateSystem::GetInstance()->AddState(CHighScoreState::GetInstance());
					break;
				case 5:
					//Credits
					m_bRender = false;
					options = true;
					CStateSystem::GetInstance()->AddState(CCreditState::GetInstance());
					break;
				case 6:
					m_bConf = true;
					break;
				}
		}

		
	}

	if((keyboard->KeyPressed(DIK_RETURN) || CInput::GetInstance()->GetAction(ACT_ACCEPT)) && !m_bDone)
	{
		m_bDone = true;
	}

	return true;
}

void CMainMenuState::Update(float fElapsedTime)
{
	m_fTotalTime += fElapsedTime;

	CCamera::GetInstance()->m_MC.GetWorldMatrix(sin(m_fTotalTime*0.1f)*40.0f+60.0f,0.0f,3.0f);
	CCamera::GetInstance()->Update(fElapsedTime);

	m_mxViewProj  = CCamera::GetInstance()->GetViewMatrix() * CCamera::GetInstance()->GetProjectionMatrix();

	D3DXVECTOR3 location = CCamera::GetInstance()->GetViewPosition();
	D3DXMATRIX rotation;
	D3DXMatrixRotationX(&rotation,D3DXToRadian(8));
	D3DXMatrixTranslation(&m_ConsoleWorld,location.x+0.25f,location.y-13.75f,location.z+27.0f);
	D3DXMatrixMultiply(&m_ConsoleWorld,&rotation,&m_ConsoleWorld);

	/*D3DXMatrixRotationX(&rotation,D3DXToRadian(8));
	D3DXMatrixTranslation(&m_mScaline,location.x+0.25,location.y-12.75,location.z+28);
	D3DXMatrixMultiply(&m_mScaline,&rotation,&m_mScaline);*/

	m_fDrawBucket += fElapsedTime;
	if(m_fDrawBucket > 0.05)
	{
		m_fDrawBucket = 0.0f;
		++m_nCharPosition;
	}

	if(m_bDone == true)
	{
	}

	REC.UpdateBlink(fElapsedTime,0.5f);

	CGameState::GetInstance()->SetIsPaused(false);
	m_bRender = true;
	//CDebugSystem::GetInstance()->RegisterData("Camera Pos",m_Cam->GetViewPosition());
}

void CMainMenuState::Render(void)
{
	/*m_ScanlineShape.GetFrame().GetLocalMatrix() = m_mScaline;
	m_ScanlineShape.GetRenderContext()->AddRenderNode(&m_ScanlineShape);*/

	m_ConsoleShape.GetFrame().GetLocalMatrix() = m_ConsoleWorld * m_mxViewProj;
	m_ConsoleShape.GetRenderContext()->AddRenderNode(&m_ConsoleShape);

	if(m_bRender && !options)
	{
		if(!m_bConf)
		{
			MenuRender();
		}

		if(m_bDone && !m_bConf)
		{
			m_CBF->Print("=>",m_nArrowPosition,m_nFontStart+(m_nFontOffset*m_nSelection),false,false,m_fScale);
			this->m_Buttons.Render(m_mxViewProj,true,false,true,false,((XboxGamePadDevice*)CInput::GetInstance()->GetDevice(DEV_GAMEPAD))->ControllerConnected(0));
		}

		if(m_bConf)
		{
			this->m_Buttons.Render(m_mxViewProj,false,true,true,false,((XboxGamePadDevice*)CInput::GetInstance()->GetDevice(DEV_GAMEPAD))->ControllerConnected(0));
			m_CBF->Print("=>",m_nArrowPosition*(m_nConfirm+1),m_nFontStart+(m_nFontOffset*2),false,false,0.85f);
			m_CBF->Print("Do you wish to exit the program?",160,m_nFontStart+(m_nFontOffset),false,false,0.85f);
			m_CBF->Print("Yes",m_nFontX,m_nFontStart+(m_nFontOffset*2),false,false,0.85f);
			m_CBF->Print("No",m_nFontX+m_nFontX,m_nFontStart+(m_nFontOffset*2),false,false,0.85f);
		}
		
	}

	REC.SetPostion(CCamera::GetInstance()->GetViewPosition().x+3,CCamera::GetInstance()->GetViewPosition().y-8,CCamera::GetInstance()->GetViewPosition().z+26);
	REC.SetScale(0.5,0.5,1.0);
	REC.RenderSingleBlink(m_mxViewProj);
	
	CObjectManager::GetInstance()->RenderObjects();

	CRenderContextManager::GetInstance()->Render();

	
}

bool CMainMenuState::Exit(void)
{
	//CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::STOP_FX_MENU_AMBIENCE_LOOPS, MENU);
	//CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::STOP_MX_MUSICLOOP_05, MENU);
	//CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::STOP_ALL, AK_UnknownObject);
	m_bPlayAudioMenu = false;
	return false;
}

CMainMenuState* CMainMenuState::GetInstance(void)
{
	static CMainMenuState instance;
	return &instance;
}

bool CMainMenuState::Reset()
{
	for(unsigned int i = 0; i < FONTSELECTIONS; i++)
	{
		m_bDrawSelection[i] = false;
	}

	m_bDone = false;
	m_bDrawSelection[0] = true;
	m_fDrawBucket = 0.0f;
	m_nCharPosition = 0;

	return true;
}

void CMainMenuState::Draw(const char* message, int position,int nextBool,int currentBool,int x, int y, float scale)
{
	if(message[position+1])
	{
		char* temp = (char*)message;
		temp[position+1] = 0;
		m_CBF->Print(temp,x,y,false,true,scale);
	}
	else
	{
		if(nextBool != -1)
		{
			m_bDrawSelection[nextBool] = true;
		}
		m_bDrawSelection[currentBool] = false;
		m_nCharPosition = 0;
	}
}

void CMainMenuState::MenuRender()
{

	XboxGamePadDevice* gamepad = (XboxGamePadDevice*)CInput::GetInstance()->GetDevice(DEV_GAMEPAD);
	// Puts Message into Char buffer
	if(m_bDrawSelection[0] && !m_bDone)
	{
		memcpy_s(buffer,64,"Welcome to OPERATION B.L.A.S.T.",64);
	}

	// Types out the text
	if(m_bDrawSelection[0] && !m_bDone)
	{
		Draw(buffer,m_nCharPosition,1,0,200,110,m_fScale);
	}
	else
	{
	// Displays normal
		m_CBF->Print("OPERATION B.L.A.S.T.",240,110,false,true,m_fScale);
	}

	// Puts new message into char buffer
	if(m_bDrawSelection[1]&& !m_bDone)
	{
		memcpy_s(buffer,64,"Single Player",64);
	}

	// Types out next text
	if(m_bDrawSelection[1]&& !m_bDone)
	{
		Draw(buffer,m_nCharPosition,2,1,m_nFontX,m_nFontStart,m_fScale);
	}
	else if(!m_bDrawSelection[1] && !m_bDrawSelection[0] || m_bDone)
	{
		m_CBF->Print("Single Player",m_nFontX,m_nFontStart,false,false,m_fScale);
	}

	// Puts new message into char buffer
	if(m_bDrawSelection[2] && !m_bDone && gamepad->ControllerConnected(1))
	{
		memcpy_s(buffer,64,"Multiplayer",64);
	}
	else if(m_bDrawSelection[2] && !m_bDone && !gamepad->ControllerConnected(1))
	{
		memcpy_s(buffer,64,"\\!@#$! !@#$\\!@#$!@#$!@#",64);
	}

	// Types out next text
	if(m_bDrawSelection[2] && !m_bDone)
	{
		Draw(buffer,m_nCharPosition,3,2,m_nFontX,m_nFontStart+m_nFontOffset,m_fScale);
	}
	else if(!m_bDrawSelection[1] && !m_bDrawSelection[0] && !m_bDrawSelection[2] || m_bDone)
	{
		if(gamepad->ControllerConnected(1))
			m_CBF->Print("Multiplayer",m_nFontX,m_nFontStart+(m_nFontOffset*1),false,false,m_fScale);
		else
			m_CBF->Print("Multiplayer",m_nFontX,m_nFontStart+(m_nFontOffset*1),false,false,m_fScale,D3DXCOLOR(0.5,0.5,0.5,1.0));
	}

	// Puts new message into char buffer
	if(m_bDrawSelection[3]&& !m_bDone)
	{
		memcpy_s(buffer,64,"\\How To",64);
	}

	// Types out next text
	if(m_bDrawSelection[3]&& !m_bDone)
	{
		Draw(buffer,m_nCharPosition,4,3,m_nFontX,m_nFontStart+(m_nFontOffset*2),m_fScale);
	}
	else if(!m_bDrawSelection[1] && !m_bDrawSelection[0] && !m_bDrawSelection[2] && !m_bDrawSelection[3] || m_bDone)
	{
		m_CBF->Print("How To",m_nFontX,m_nFontStart+(m_nFontOffset*2),false,false,m_fScale);
	}

	if(m_bDrawSelection[4] && !m_bDone)
	{
		memcpy_s(buffer,64,"\\Options",64);
	}

	// Types out next text
	if(m_bDrawSelection[4]&& !m_bDone)
	{
		Draw(buffer,m_nCharPosition,5,4,m_nFontX,m_nFontStart+(m_nFontOffset*3),m_fScale);
	}
	else if(!m_bDrawSelection[1] && !m_bDrawSelection[0] && !m_bDrawSelection[2] && !m_bDrawSelection[3] && !m_bDrawSelection[4] || m_bDone)
	{
		m_CBF->Print("Options",m_nFontX,m_nFontStart+(m_nFontOffset*3),false,false,m_fScale);
	}

	if(m_bDrawSelection[5]&& !m_bDone)
	{
		memcpy_s(buffer,64,"High Scores",64);
	}

	// Types out next text
	if(m_bDrawSelection[5]&& !m_bDone)
	{
		Draw(buffer,m_nCharPosition,6,5,m_nFontX,m_nFontStart+(m_nFontOffset*4),m_fScale);
	}
	else if(!m_bDrawSelection[1] && !m_bDrawSelection[0] && !m_bDrawSelection[2] && !m_bDrawSelection[3] && !m_bDrawSelection[4] && !m_bDrawSelection[5] || m_bDone)
	{
		m_CBF->Print("High Scores",m_nFontX,m_nFontStart+(m_nFontOffset*4),false,false,m_fScale);
	}

	if(m_bDrawSelection[6]&& !m_bDone)
	{
		memcpy_s(buffer,64,"\\Credits",64);
	}

	// Types out next text
	if(m_bDrawSelection[6]&& !m_bDone)
	{
		Draw(buffer,m_nCharPosition,7,6,m_nFontX,m_nFontStart+(m_nFontOffset*5),m_fScale);
	}
	else if(!m_bDrawSelection[1] && !m_bDrawSelection[0] && !m_bDrawSelection[2] && !m_bDrawSelection[3] && !m_bDrawSelection[4] && !m_bDrawSelection[5]
			&& !m_bDrawSelection[6] || m_bDone)
	{
		m_CBF->Print("Credits",m_nFontX,m_nFontStart+(m_nFontOffset*5),false,false,m_fScale);
	}

	if(m_bDrawSelection[7]&& !m_bDone)
	{
		memcpy_s(buffer,64,"\\Exit",64);
	}

	// Types out next text
	if(m_bDrawSelection[7]&& !m_bDone)
	{
		Draw(buffer,m_nCharPosition,-1,7,m_nFontX,m_nFontStart+(m_nFontOffset*6),m_fScale);
	}
	else if(!m_bDrawSelection[1] && !m_bDrawSelection[0] && !m_bDrawSelection[2] && !m_bDrawSelection[3] && !m_bDrawSelection[4]&& !m_bDrawSelection[5]
			&& !m_bDrawSelection[6] && !m_bDrawSelection[7] || m_bDone)
	{
		m_bDone = true;
			CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::STOP_FX_TYPING_LOOP, MENU);
		m_CBF->Print("Exit",m_nFontX,m_nFontStart+(m_nFontOffset*6),false,false,m_fScale);
	}
}