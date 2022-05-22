#include "CWinState.h"
#include "CStateMachine.h"
#include "CStatsState.h"
#include "CGameState.h"
#include "CMainMenuState.h"
#include "CCharacterSelectionState.h"
#include "../Rendering/CTextureManager.h"
#include "../Font/CBitMapFont.h"
#include "../Input/CInput.h"
#include "../Rendering/CRenderer.h"
#include "../Rendering/CRenderContextManager.h"
#include "../Mesh/CModelManager.h"
#include "../Camera/Camera.h"
#include "../Core/CGame.h"
#include "CLoadState.h"

#include "../Resources/Sound/Wwise_IDs.h"
#include "../Sound/WwiseManager.h"


CWinState::CWinState(void)
{
	m_nSelection = 0;
	m_fScale = 1.0f;
	m_nFontX = 150;
	m_nArrowPosition = 120;
	m_nFontOffset = 42;
	m_nFontStart = 150;
}

CWinState::~CWinState(void)
{
}

void CWinState::Enter(void)
{
	// Set up the font for the screen
	m_CBF = CBitMapFont::GetInstance();

	// Setup Input for the winstate
	m_CIP = CInput::GetInstance();
	m_CTM = CTextureManager::GetInstance();

	m_nScroll = 0;
	m_fBucket = 0.0;
	m_fTransTimer = 0.0;
	blink = false;
	transition = false;
	m_bRender = true;
	m_bDone = false;
	m_nSelection = 0;

	D3DXMatrixIdentity(&m_ConsoleWorld);
	D3DXMatrixIdentity(&matIdentity);
	D3DXMatrixIdentity(&matCIdentity);
	D3DXMatrixIdentity(&matVictory);

	// Rendering of the console
	D3DXVECTOR3 location = CCamera::GetInstance()->GetViewPosition();
	D3DXMATRIX rotation;
	D3DXMatrixRotationX(&rotation,D3DXToRadian(13));
	D3DXMatrixTranslation(&m_ConsoleWorld,location.x,location.y-13,location.z+26);
	D3DXMatrixMultiply(&m_ConsoleWorld,&rotation,&m_ConsoleWorld);

	m_consoleScreen = CModelManager::GetInstance()->GetModel(CLoadState::GetInstance()->GetModelID("PauseMenuScreenShape"));
	m_ConsoleShape.Init(&m_consoleScreen->GetMesh(), NULL, &CRenderContextManager::GetInstance()->GetHUDContext(), m_ConsoleWorld, CRenderShape::PNTRenderFunc);

	CModel* model;
	CModel* clmodel;
		//Player 1
	if(CCharacterSelectionState::GetInstance()->GetSelection() == 0)
	{
		model = CModelManager::GetInstance()->GetModel(CLoadState::GetInstance()->GetModelID("barron_geoShape"));
		m_BarronShape.Init(&model->GetMesh(), model,&CRenderContextManager::GetInstance()->GetAnimatedLitContext(), matIdentity, CRenderShape::PNTBBRenderFunc);

		// Setting Claire position
		clmodel = CModelManager::GetInstance()->GetModel(CLoadState::GetInstance()->GetModelID("Claire_geoShape"));
		m_ClaireShape.Init(&clmodel->GetMesh(), clmodel,&CRenderContextManager::GetInstance()->GetAnimatedLitContext(), matCIdentity, CRenderShape::PNTBBRenderFunc);

	}
	else if(CCharacterSelectionState::GetInstance()->GetSelection() == 1)
	{
		model = CModelManager::GetInstance()->GetModel(CLoadState::GetInstance()->GetModelID("Claire_geoShape"));
		m_BarronShape.Init(&model->GetMesh(), model,&CRenderContextManager::GetInstance()->GetAnimatedLitContext(), matIdentity, CRenderShape::PNTBBRenderFunc);

		// Setting Claire position
		clmodel = CModelManager::GetInstance()->GetModel(CLoadState::GetInstance()->GetModelID("barron_geoShape"));
		m_ClaireShape.Init(&clmodel->GetMesh(), clmodel,&CRenderContextManager::GetInstance()->GetAnimatedLitContext(), matCIdentity, CRenderShape::PNTBBRenderFunc);
	}
	
	// Setting Win or Lose Animation
	if(CGame::GetInstance()->GetMutliplayer())
	{
		if(CGameState::GetInstance()->GetHUD()->GetScore() > CGameState::GetInstance()->GetHUD2()->GetScore())
		{
			m_BarronShape.GetAnimCtrl().SetAnimation("victory");
			m_ClaireShape.GetAnimCtrl().SetAnimation("lose");
		}
		else
		{
			m_BarronShape.GetAnimCtrl().SetAnimation("lose");
			m_ClaireShape.GetAnimCtrl().SetAnimation("victory");
		}
	}
	else
	{
		m_BarronShape.GetAnimCtrl().SetAnimation("victory");
	}

	model = CModelManager::GetInstance()->GetModel(CLoadState::GetInstance()->GetModelID("VictoryShape"));
	m_VictoryShape.Init(&model->GetMesh(),model,&CRenderContextManager::GetInstance()->GetAnimatedLitContext(),matVictory,CRenderShape::PNTBBRenderFunc);
	m_VictoryShape.GetAnimCtrl().SetAnimation("victory");

	model = CModelManager::GetInstance()->GetModel(CLoadState::GetInstance()->GetModelID("Player_1Shape"));
	m_Player1Shape.Init(&model->GetMesh(),model,&CRenderContextManager::GetInstance()->GetAnimatedLitContext(),matPlayer1,CRenderShape::PNTBBRenderFunc);
	m_Player1Shape.GetAnimCtrl().SetAnimation("player");

	model = CModelManager::GetInstance()->GetModel(CLoadState::GetInstance()->GetModelID("Player_2Shape"));
	m_Player2Shape.Init(&model->GetMesh(),model,&CRenderContextManager::GetInstance()->GetAnimatedLitContext(),matPlayer2,CRenderShape::PNTBBRenderFunc);
	m_Player2Shape.GetAnimCtrl().SetAnimation("player");



	D3DXMatrixIdentity(&rotation);
	D3DXMatrixRotationY(&rotation,D3DXToRadian(-75));
	if(CGame::GetInstance()->GetMutliplayer())
	{
		if(CGameState::GetInstance()->GetHUD()->GetScore() > CGameState::GetInstance()->GetHUD2()->GetScore())
		{
			D3DXMatrixTranslation(&matIdentity,CCamera::GetInstance()->GetViewPosition().x-25,CCamera::GetInstance()->GetViewPosition().y-20,CCamera::GetInstance()->GetViewPosition().z+80);
		}
		else
		{
			D3DXMatrixTranslation(&matIdentity,CCamera::GetInstance()->GetViewPosition().x-25,CCamera::GetInstance()->GetViewPosition().y-20,CCamera::GetInstance()->GetViewPosition().z+150);
		}
	}
	else
	{
		D3DXMatrixTranslation(&matIdentity,CCamera::GetInstance()->GetViewPosition().x-25,CCamera::GetInstance()->GetViewPosition().y-20,CCamera::GetInstance()->GetViewPosition().z+80);
	}
	D3DXMatrixMultiply(&matIdentity,&rotation,&matIdentity);

	D3DXMatrixIdentity(&rotation);
	D3DXMatrixRotationY(&rotation,D3DXToRadian(-90));
	if(CGameState::GetInstance()->GetHUD()->GetScore() > CGameState::GetInstance()->GetHUD2()->GetScore())
	{
		D3DXMatrixTranslation(&matCIdentity,CCamera::GetInstance()->GetViewPosition().x+5,CCamera::GetInstance()->GetViewPosition().y-20,CCamera::GetInstance()->GetViewPosition().z+150);
	}
	else
	{
		D3DXMatrixTranslation(&matCIdentity,CCamera::GetInstance()->GetViewPosition().x+5,CCamera::GetInstance()->GetViewPosition().y-20,CCamera::GetInstance()->GetViewPosition().z+80);
	}
	D3DXMatrixMultiply(&matCIdentity,&rotation,&matCIdentity);

	D3DXMatrixTranslation(&matVictory,CCamera::GetInstance()->GetViewPosition().x-5,CCamera::GetInstance()->GetViewPosition().y-5,CCamera::GetInstance()->GetViewPosition().z+40);
	D3DXMatrixTranslation(&matPlayer1,CCamera::GetInstance()->GetViewPosition().x-5,CCamera::GetInstance()->GetViewPosition().y-10,CCamera::GetInstance()->GetViewPosition().z+40);
	D3DXMatrixTranslation(&matPlayer2,CCamera::GetInstance()->GetViewPosition().x-5,CCamera::GetInstance()->GetViewPosition().y-10,CCamera::GetInstance()->GetViewPosition().z+40);

	m_Buttons.Initialize(true);

	CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_MX_FANFARE_MUSIC, 200);	// AudioID 200 is MENU

}

bool CWinState::Input(void)
{
	//KeyboardDevice* keyboard = (KeyboardDevice*)CInput::GetInstance()->GetDevice(DEV_KEYBOARD);

	if(!transition)
	{				
		static float m_fPrevX = 0.0f;
		static float m_fCurrX = 0.0f;
		m_fPrevX = m_fCurrX;
		m_fCurrX = CInput::GetInstance()->GetActionAmount(ACT_MOVEX,1);
		if(m_fCurrX >= 32765 && m_fPrevX != m_fCurrX)
		{
			m_nSelection++;
			CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_MENU_DOWN, 200);
		}
		if(m_fCurrX <= -32765 && m_fPrevX != m_fCurrX)
		{
			m_nSelection--;
			CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_MENU_UP, 200);		
		}
		if(m_CIP->GetAction(ACT_MOVELEFT))
		{
			--m_nSelection;
			CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_MENU_DOWN, 200); // AudioID 200 is MENU
		}

		if(m_CIP->GetAction(ACT_MOVERIGHT))
		{
			++m_nSelection;
			CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_MENU_UP, 200); // AudioID 200 is MENU
		}

		if(m_nSelection < 0)
			m_nSelection = 0;

		if(m_nSelection > 1)
			m_nSelection = 1;

		if(m_CIP->GetAction(ACT_ACCEPT))
		{
			CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_MENU_SELECT, 200);	// AudioID 200 is MENU
			switch(m_nSelection)
			{
			case 0:
				m_bRender = false;
				m_bDone = true;
				if(CGame::GetInstance()->GetMutliplayer())
				{
					if(CGameState::GetInstance()->GetHUD()->GetScore() > CGameState::GetInstance()->GetHUD2()->GetScore())
					{
							m_BarronShape.GetAnimCtrl().SetAnimation("victoryIdle");
							m_ClaireShape.GetAnimCtrl().SetAnimation("loseIdle");	
					}
					else
					{
							m_BarronShape.GetAnimCtrl().SetAnimation("loseIdle");
							m_ClaireShape.GetAnimCtrl().SetAnimation("victoryIdle");		
					}
				}
				else
				{
						m_BarronShape.GetAnimCtrl().SetAnimation("victoryIdle");
				}
					m_VictoryShape.GetAnimCtrl().SetAnimation("victoryIdle");
					m_Player1Shape.GetAnimCtrl().SetAnimation("playerIdle");
					m_Player2Shape.GetAnimCtrl().SetAnimation("playerIdle");
				break;
			case 1:
				m_bRender = false;
			CStateSystem::GetInstance()->ChangeBaseState(CMainMenuState::GetInstance());
				break;
			}
		}
	}

	return true;
}

void CWinState::Update(float fElapsedTime)
{
	m_fBucket += fElapsedTime;
	m_fTransTimer += fElapsedTime;

	if(m_fBucket > 1.0 && !blink)
	{
		blink = true;
		m_Player1Shape.GetAnimCtrl().SetAnimation("player");
		m_Player2Shape.GetAnimCtrl().SetAnimation("player");
		m_Player1Shape.GetAnimCtrl().update(fElapsedTime);
		m_Player2Shape.GetAnimCtrl().update(fElapsedTime);
	}
		
		if(m_VictoryShape.GetAnimCtrl().getLoopCount() > 0)
		{
			m_VictoryShape.GetAnimCtrl().SetAnimation("victoryIdle");
			m_VictoryShape.GetAnimCtrl().update(fElapsedTime);
		}

		if(m_Player1Shape.GetAnimCtrl().getLoopCount() > 0)
		{
			m_Player1Shape.GetAnimCtrl().SetAnimation("playerIdle");
			m_Player1Shape.GetAnimCtrl().update(fElapsedTime);
		}

		if(m_Player2Shape.GetAnimCtrl().getLoopCount() > 0)
		{
			m_Player2Shape.GetAnimCtrl().SetAnimation("playerIdle");
			m_Player2Shape.GetAnimCtrl().update(fElapsedTime);
		}

		if(CGame::GetInstance()->GetMutliplayer())
		{
			if(CGameState::GetInstance()->GetHUD()->GetScore() > CGameState::GetInstance()->GetHUD2()->GetScore())
			{
				if(m_BarronShape.GetAnimCtrl().getLoopCount() > 0)
				{
					m_BarronShape.GetAnimCtrl().SetAnimation("victoryIdle");
					m_BarronShape.GetAnimCtrl().update(fElapsedTime);
				}

				if(m_ClaireShape.GetAnimCtrl().getLoopCount() > 0)
				{
					m_ClaireShape.GetAnimCtrl().SetAnimation("loseIdle");
					m_ClaireShape.GetAnimCtrl().update(fElapsedTime);
				}
			}
			else
			{
				if(m_BarronShape.GetAnimCtrl().getLoopCount() > 0)
				{
					m_BarronShape.GetAnimCtrl().SetAnimation("loseIdle");
					m_BarronShape.GetAnimCtrl().update(fElapsedTime);
				}

				if(m_ClaireShape.GetAnimCtrl().getLoopCount() > 0)
				{
					m_ClaireShape.GetAnimCtrl().SetAnimation("victoryIdle");
					m_ClaireShape.GetAnimCtrl().update(fElapsedTime);
				}
			}
		}
		else
		{
			if(m_BarronShape.GetAnimCtrl().getLoopCount() > 0)
			{
				m_BarronShape.GetAnimCtrl().SetAnimation("victoryIdle");
				m_BarronShape.GetAnimCtrl().update(fElapsedTime);
			}
		}

		m_BarronShape.GetAnimCtrl().update(fElapsedTime);
		m_VictoryShape.GetAnimCtrl().update(fElapsedTime);
		if(CGame::GetInstance()->GetMutliplayer())
		m_ClaireShape.GetAnimCtrl().update(fElapsedTime);

		if(blink)
		{
			m_Player1Shape.GetAnimCtrl().update(fElapsedTime);
			m_Player2Shape.GetAnimCtrl().update(fElapsedTime);
		}


		if(m_bDone)
		{
			CStateSystem::GetInstance()->AddState(CStatsState::GetInstance());
		}
}

void CWinState::Render(void)
{
	m_mxViewProj  = CCamera::GetInstance()->GetViewMatrix() * CCamera::GetInstance()->GetProjectionMatrix();

	m_ConsoleShape.GetFrame().GetLocalMatrix() = m_ConsoleWorld * m_mxViewProj;
	m_ConsoleShape.GetRenderContext()->AddRenderNode(&m_ConsoleShape);

	if(m_bRender)
	{
		
			if(m_nSelection == 0)
			m_CBF->Print("Stats",200,560,false,true,m_fScale);
			else
			m_CBF->Print("Stats",200,560,false,false,m_fScale);

			if(m_nSelection == 1)
			m_CBF->Print("Quit",500,560,false,true,m_fScale);
			else
			m_CBF->Print("Quit",500,560,false,false,m_fScale);

			if(m_nSelection == 0)
			m_CBF->Print("=>",150,560,false,false,m_fScale);
			else 
			m_CBF->Print("=>",450,560,false,false,m_fScale);

		m_Buttons.Render(m_mxViewProj,false,true,true,false,((XboxGamePadDevice*)CInput::GetInstance()->GetDevice(DEV_GAMEPAD))->ControllerConnected(0));
	}

		m_BarronShape.GetFrame().GetLocalMatrix() = matIdentity;
		m_BarronShape.GetRenderContext()->AddRenderNode(&m_BarronShape);

		m_VictoryShape.GetFrame().GetLocalMatrix()= matVictory;
		m_VictoryShape.GetRenderContext()->AddRenderNode(&m_VictoryShape);

		if(CGame::GetInstance()->GetMutliplayer())
			{
				if(CGameState::GetInstance()->GetHUD()->GetScore() > CGameState::GetInstance()->GetHUD2()->GetScore())
				{
					if(blink)
					{
						m_Player1Shape.GetFrame().GetLocalMatrix() = matPlayer1;
						m_Player1Shape.GetRenderContext()->AddRenderNode(&m_Player1Shape);
					}
				}
				else
				{
					if(blink)
					{
						m_Player2Shape.GetFrame().GetLocalMatrix() = matPlayer2;
						m_Player2Shape.GetRenderContext()->AddRenderNode(&m_Player2Shape);
					}
				}
			}
			else
			{
				if(blink)
				{
					m_Player1Shape.GetFrame().GetLocalMatrix() = matPlayer1;
					m_Player1Shape.GetRenderContext()->AddRenderNode(&m_Player1Shape);
				}
			}

		if(CGame::GetInstance()->GetMutliplayer())
		{
			m_ClaireShape.GetFrame().GetLocalMatrix() = matCIdentity;
			m_ClaireShape.GetRenderContext()->AddRenderNode(&m_ClaireShape);
		}

	
}

bool CWinState::Exit(void)
{
	CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::STOP_MX_FANFARE_MUSIC, 200);	// AudioID 200 is MENU
	return false;
}

CWinState* CWinState::GetInstance(void)
{
	static CWinState instance;
	return &instance;
}