#include "CCharacterSelectionState.h"

#include "CStateMachine.h"
#include "CMainMenuState.h"
#include "CGameState.h"
#include "COptionState.h"
#include "CLoadState.h"
#include "../Core/CGame.h"
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

CCharacterSelectionState::CCharacterSelectionState(void)
{
	m_bConf = false;
	m_bAudioPlayCharacter = false;
	m_nSelection = 0;
	m_nFinalSelection = 0;
	m_nConfirm = 0;
	m_fScale = 1.0f;
	m_nFontX = 300;
	m_nOtherFontX = 200;
	m_nArrowPosition = m_nFontX-50;
	m_nFontOffset = 42;
	m_nFontStart = 150;
}

CCharacterSelectionState::~CCharacterSelectionState(void)
{

}

void CCharacterSelectionState::Enter(void)
{
	m_bConf = false;
	m_bHat = false;
	m_bHat2 = false;
	m_bAudioPlayCharacter = false;
	m_nSelection = 0; 
	m_nFinalSelection = 0;
	m_nHatSelectionPlayer1 = 0;
	m_nHatSelectionPlayer2 = 0;

	CModel* model;
	CModel* clmodel;

	D3DXMatrixIdentity(&matIdentity);
	D3DXMatrixIdentity(&matCIdentity);
	D3DXMatrixIdentity(&m_mHat);

	m_Buttons.Initialize(true);

	// Setting Barron position
	model = CModelManager::GetInstance()->GetModel(CLoadState::GetInstance()->GetModelID("barron_geoShape"));
	m_BarronShape.Init(&model->GetMesh(), model,&CRenderContextManager::GetInstance()->GetAnimatedLitContext(), matIdentity, CRenderShape::PNTBBRenderFunc);

		// Setting Claire position
	clmodel = CModelManager::GetInstance()->GetModel(CLoadState::GetInstance()->GetModelID("Claire_geoShape"));
	m_ClaireShape.Init(&clmodel->GetMesh(), clmodel,&CRenderContextManager::GetInstance()->GetAnimatedLitContext(), matCIdentity, CRenderShape::PNTBBRenderFunc);

	// Setting Animations
	m_BarronShape.GetAnimCtrl().SetAnimation("victory");
	m_ClaireShape.GetAnimCtrl().SetAnimation("victory");

	// Setting Proper Rotations
	CCamera* mCam = CCamera::GetInstance();
	D3DXMATRIX rotation;
	D3DXMatrixIdentity(&rotation);
	D3DXMatrixRotationY(&rotation,D3DXToRadian(-75));
	D3DXMatrixTranslation(&matIdentity,CCamera::GetInstance()->GetViewPosition().x,CCamera::GetInstance()->GetViewPosition().y-25,CCamera::GetInstance()->GetViewPosition().z+100);
	D3DXMatrixMultiply(&matIdentity,&rotation,&matIdentity);

	D3DXMatrixIdentity(&rotation);
	D3DXMatrixRotationY(&rotation,D3DXToRadian(-90));
	D3DXMatrixTranslation(&matCIdentity,CCamera::GetInstance()->GetViewPosition().x,CCamera::GetInstance()->GetViewPosition().y-25,CCamera::GetInstance()->GetViewPosition().z+100);
	D3DXMatrixMultiply(&matCIdentity,&rotation,&matCIdentity);

	D3DXMatrixTranslation(&m_mHat,CCamera::GetInstance()->GetViewPosition().x-10,CCamera::GetInstance()->GetViewPosition().y+10,CCamera::GetInstance()->GetViewPosition().z+100);
	// Loading Hats
	model = CModelManager::GetInstance()->GetModel(CLoadState::GetInstance()->GetModelID("armyhatShape"));
	m_HatShape[ARMY].Init(&model->GetMesh(),model,&CRenderContextManager::GetInstance()->GetStaticLitContext(),m_mHat,CRenderShape::PNTRenderFunc);

	model = CModelManager::GetInstance()->GetModel(CLoadState::GetInstance()->GetModelID("Claire_hatShape"));
	m_HatShape[CLAIREHAT].Init(&model->GetMesh(),model,&CRenderContextManager::GetInstance()->GetStaticLitContext(),m_mHat,CRenderShape::PNTRenderFunc);

	model = CModelManager::GetInstance()->GetModel(CLoadState::GetInstance()->GetModelID("bearhatShape"));
	m_HatShape[BEAR].Init(&model->GetMesh(),model,&CRenderContextManager::GetInstance()->GetStaticLitContext(),m_mHat,CRenderShape::PNTRenderFunc);

	model = CModelManager::GetInstance()->GetModel(CLoadState::GetInstance()->GetModelID("cathatShape"));
	m_HatShape[CAT].Init(&model->GetMesh(),model,&CRenderContextManager::GetInstance()->GetStaticLitContext(),m_mHat,CRenderShape::PNTRenderFunc);

	model = CModelManager::GetInstance()->GetModel(CLoadState::GetInstance()->GetModelID("cowboyhatShape"));
	m_HatShape[COW].Init(&model->GetMesh(),model,&CRenderContextManager::GetInstance()->GetStaticLitContext(),m_mHat,CRenderShape::PNTRenderFunc);

	model = CModelManager::GetInstance()->GetModel(CLoadState::GetInstance()->GetModelID("fancyhatShape"));
	m_HatShape[FANCY].Init(&model->GetMesh(),model,&CRenderContextManager::GetInstance()->GetStaticLitContext(),m_mHat,CRenderShape::PNTRenderFunc);

	model = CModelManager::GetInstance()->GetModel(CLoadState::GetInstance()->GetModelID("fruithatShape"));
	m_HatShape[FRUIT].Init(&model->GetMesh(),model,&CRenderContextManager::GetInstance()->GetStaticLitContext(),m_mHat,CRenderShape::PNTRenderFunc);

	model = CModelManager::GetInstance()->GetModel(CLoadState::GetInstance()->GetModelID("narwhalhatShape"));
	m_HatShape[NAR].Init(&model->GetMesh(),model,&CRenderContextManager::GetInstance()->GetStaticLitContext(),m_mHat,CRenderShape::PNTRenderFunc);

	model = CModelManager::GetInstance()->GetModel(CLoadState::GetInstance()->GetModelID("marchinghatShape"));
	m_HatShape[MAR].Init(&model->GetMesh(),model,&CRenderContextManager::GetInstance()->GetStaticLitContext(),m_mHat,CRenderShape::PNTRenderFunc);

	model = CModelManager::GetInstance()->GetModel(CLoadState::GetInstance()->GetModelID("pokehatShape"));
	m_HatShape[POKE].Init(&model->GetMesh(),model,&CRenderContextManager::GetInstance()->GetStaticLitContext(),m_mHat,CRenderShape::PNTRenderFunc);

	model = CModelManager::GetInstance()->GetModel(CLoadState::GetInstance()->GetModelID("pothatShape"));
	m_HatShape[POT].Init(&model->GetMesh(),model,&CRenderContextManager::GetInstance()->GetStaticLitContext(),m_mHat,CRenderShape::PNTRenderFunc);

	model = CModelManager::GetInstance()->GetModel(CLoadState::GetInstance()->GetModelID("sharkhatShape"));
	m_HatShape[SHARK].Init(&model->GetMesh(),model,&CRenderContextManager::GetInstance()->GetStaticLitContext(),m_mHat,CRenderShape::PNTRenderFunc);

	model = CModelManager::GetInstance()->GetModel(CLoadState::GetInstance()->GetModelID("sombrerohatShape"));
	m_HatShape[SOM].Init(&model->GetMesh(),model,&CRenderContextManager::GetInstance()->GetStaticLitContext(),m_mHat,CRenderShape::PNTRenderFunc);



	m_bAudioPlayCharacter = true;

}

bool CCharacterSelectionState::Input(void)
{
	KeyboardDevice* keyboard = (KeyboardDevice*)CInput::GetInstance()->GetDevice(DEV_KEYBOARD);
	
	
	if(m_bConf)
	{
		FinalInput();
	}

	if(m_bHat2 && !m_bConf)
	{
		InputPlayer2();
	}

	if(!m_bHat2 && !m_bConf)
	{
		InputPlayer1();
	}

	

		if(m_nHatSelectionPlayer1 >= HATMAX)
		{
			m_nHatSelectionPlayer1 = 0;
		}
		
		if(m_nHatSelectionPlayer1 < 0)
		{
			m_nHatSelectionPlayer1 = HATMAX-1;
		}

		if(m_nHatSelectionPlayer2 >= HATMAX)
		{
			m_nHatSelectionPlayer2 = 0;
		}
			
		if(m_nHatSelectionPlayer2 < 0)
		{
			m_nHatSelectionPlayer2 = HATMAX-1;
		}

		if(m_nSelection > 1)
		{
			m_nSelection = 0;
		}

		if( m_nSelection < 0)
		{
			m_nSelection = 1;
		}

		if((CInput::GetInstance()->GetAction(ACT_BACK) || CInput::GetInstance()->GetAction(ACT_MENUBACK)) && !m_bHat)
		{
			CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_MENU_GOBACK, 200);
			CMainMenuState::GetInstance()->SetOptions(false);
			CStateSystem::GetInstance()->RemoveTopState();
		}

		if((CInput::GetInstance()->GetAction(ACT_BACK) || CInput::GetInstance()->GetAction(ACT_MENUBACK)) && (m_bConf))
		{
			CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_MENU_GOBACK, 200);
			m_bConf = false;
			m_bHat = false;
			m_bHat2 = false;
			m_nHatSelectionPlayer1 = 0;
			m_nHatSelectionPlayer2 = 0;
		}

		if((CInput::GetInstance()->GetAction(ACT_BACK) || CInput::GetInstance()->GetAction(ACT_MENUBACK)) && (m_bHat) && (!m_bHat2))
		{
			m_bHat = false;
		}

		if((CInput::GetInstance()->GetAction(ACT_BACK,1) || CInput::GetInstance()->GetAction(ACT_MENUBACK,1)) && (m_bHat2))
		{
			m_bHat = true;
			m_bHat2 = false;
		}

	return true;
}

bool CCharacterSelectionState::InputPlayer1(void)
{
	KeyboardDevice* keyboard = (KeyboardDevice*)CInput::GetInstance()->GetDevice(DEV_KEYBOARD);

	//Player 1 Selection
	if(!m_bConf && !m_bHat)
	{

		static float m_fPrevY = 0.0f;
		static float m_fCurrY = 0.0f;
		m_fPrevY = m_fCurrY;
		m_fCurrY = CInput::GetInstance()->GetActionAmount(ACT_MOVEY);
		if(m_fCurrY >= 32760 && m_fPrevY != m_fCurrY)
		{
			m_nSelection --;
			CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_MENU_UP, 200);
			
			m_BarronShape.GetAnimCtrl().SetAnimation("victory");
			m_ClaireShape.GetAnimCtrl().SetAnimation("victory");
			m_bAudioPlayCharacter = true;
		}
		if(m_fCurrY <= -32760 && m_fPrevY != m_fCurrY)
		{
			m_nSelection ++;
			CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_MENU_DOWN, 200);
			m_BarronShape.GetAnimCtrl().SetAnimation("victory");
			m_ClaireShape.GetAnimCtrl().SetAnimation("victory");
			m_bAudioPlayCharacter = true;
		}
		if(CInput::GetInstance()->GetAction(ACT_MOVEDOWN))
		{
			m_nSelection ++;
			CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_MENU_DOWN, 200);
			m_BarronShape.GetAnimCtrl().SetAnimation("victory");
			m_ClaireShape.GetAnimCtrl().SetAnimation("victory");
			m_bAudioPlayCharacter = true;
		}

		if(CInput::GetInstance()->GetAction(ACT_MOVEUP))
		{
			m_nSelection --;
			CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_MENU_UP, 200);
			m_BarronShape.GetAnimCtrl().SetAnimation("victory");
			m_ClaireShape.GetAnimCtrl().SetAnimation("victory");
			m_bAudioPlayCharacter = true;
		}
	}

	//Input for Player1 Selection
	if(m_bHat && !m_bConf)
	{		
		static float m_fPrevX = 0.0f;
		static float m_fCurrX = 0.0f;
		m_fPrevX = m_fCurrX;
		m_fCurrX = CInput::GetInstance()->GetActionAmount(ACT_MOVEX);
		if(m_nSelection == 0)
		{
	
			if(m_fCurrX >= 32760 && m_fPrevX != m_fCurrX)
			{
				m_nHatSelectionPlayer1 --;
				CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_MENU_UP, 200);
			}
			if(m_fCurrX <= -32760 && m_fPrevX != m_fCurrX)
			{
				m_nHatSelectionPlayer1 ++;
				CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_MENU_DOWN, 200);
			}
			if(CInput::GetInstance()->GetAction(ACT_MOVELEFT))
			{
				m_nHatSelectionPlayer1--;
				CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_MENU_UP, 200);

			}

			if(CInput::GetInstance()->GetAction(ACT_MOVERIGHT))
			{ 
				m_nHatSelectionPlayer1++;
				CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_MENU_DOWN, 200);

			}
		}
		else
		{
			if(m_fCurrX >= 32760 && m_fPrevX != m_fCurrX)
			{
				m_nHatSelectionPlayer2--;
				CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_MENU_UP, 200);
			}
			if(m_fCurrX <= -32760 && m_fPrevX != m_fCurrX)
			{
				m_nHatSelectionPlayer2++;
				CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_MENU_DOWN, 200);
			}
			if(CInput::GetInstance()->GetAction(ACT_MOVELEFT))
			{
				m_nHatSelectionPlayer2--;
				CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_MENU_UP, 200);

			}

			if(CInput::GetInstance()->GetAction(ACT_MOVERIGHT))
			{ 
				m_nHatSelectionPlayer2++;
				CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_MENU_DOWN, 200);

			}
		}
	}

	if(keyboard->KeyPressed(DIK_RETURN) || CInput::GetInstance()->GetAction(ACT_ACCEPT))
		{
			switch(m_nSelection)
			{
			case 0:
				m_nFinalSelection = 0;
				CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_MENU_SELECT, 200);
				
				if(m_bHat && !CGame::GetInstance()->GetMutliplayer())
				{
					m_bConf = true;
				}
				else if(m_bHat && CGame::GetInstance()->GetMutliplayer())
				{
					m_bHat2 = true;
				}
				m_bHat = true;
				break;
			case 1:
				m_nFinalSelection = 1;
				CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_MENU_SELECT, 200);
				
				if(m_bHat && !CGame::GetInstance()->GetMutliplayer())
				{
					m_bConf = true;
				}
				else if(m_bHat && CGame::GetInstance()->GetMutliplayer())
				{
					m_bHat2 = true;
				}
				m_bHat = true;
				break;
			}
		}

	return true;
}

bool CCharacterSelectionState::InputPlayer2(void)
{
	// If Multiplayer mode is active
		static float m_fPrevX = 0.0f;
		static float m_fCurrX = 0.0f;
		m_fPrevX = m_fCurrX;
		m_fCurrX = CInput::GetInstance()->GetActionAmount(ACT_MOVEX,1);

		if(m_nSelection == 0)
		{
			if(m_fCurrX >= 32760 && m_fPrevX != m_fCurrX)
			{
				m_nHatSelectionPlayer2 --;
				CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_MENU_UP, 200);
			}
			if(m_fCurrX <= -32760 && m_fPrevX != m_fCurrX)
			{
				m_nHatSelectionPlayer2 ++;
				CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_MENU_DOWN, 200);
			}

			if(CInput::GetInstance()->GetAction(ACT_MOVELEFT,1))
			{
				m_nHatSelectionPlayer2--;
				CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_MENU_UP, 200);
			}

			if(CInput::GetInstance()->GetAction(ACT_MOVERIGHT,1))
			{
				m_nHatSelectionPlayer2++;
				CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_MENU_DOWN, 200);
			}
		}
		else
		{
			if(m_fCurrX >= 32760 && m_fPrevX != m_fCurrX)
			{
				m_nHatSelectionPlayer1 --;
				CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_MENU_UP, 200);
			}
			if(m_fCurrX <= -32760 && m_fPrevX != m_fCurrX)
			{
				m_nHatSelectionPlayer1 ++;
				CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_MENU_DOWN, 200);
			}
			
			if(CInput::GetInstance()->GetAction(ACT_MOVELEFT,1))
			{
				m_nHatSelectionPlayer1--;
				CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_MENU_UP, 200);
			}

			if(CInput::GetInstance()->GetAction(ACT_MOVERIGHT,1))
			{
				m_nHatSelectionPlayer1++;
				CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_MENU_DOWN, 200);
			}
		}

	if(CInput::GetInstance()->GetAction(ACT_ACCEPT,1))
		{
			switch(m_nSelection)
			{
			case 0:
				CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_MENU_SELECT, 200);
				m_bConf = true;
				break;
			case 1:
				CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_MENU_SELECT, 200);
				m_bConf = true;
				break;
			}
		}

	return true;
}

bool CCharacterSelectionState::FinalInput(void)
{
	KeyboardDevice* keyboard = (KeyboardDevice*)CInput::GetInstance()->GetDevice(DEV_KEYBOARD);

	if(CInput::GetInstance()->GetAction(ACT_MOVELEFT))
	{
		--m_nConfirm;
	}

	if(CInput::GetInstance()->GetAction(ACT_MOVERIGHT))
	{
		++m_nConfirm;
	}

	if(m_nConfirm > 1)
	{
		m_nConfirm = 0;
	}

	if(m_nConfirm < 0)
	{
		m_nConfirm = 1;
	}

		if((keyboard->KeyPressed(DIK_RETURN) || CInput::GetInstance()->GetAction(ACT_ACCEPT)) && m_bConf)
		{
			if(m_nConfirm == 0)
			{
				CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_MENU_SELECT, 200);
				CStateSystem::GetInstance()->ChangeBaseState(CGameState::GetInstance());
			}
			else
			{
				m_bConf = false;
				m_bHat = false;
				m_bHat2 = false;
				m_nHatSelectionPlayer1 = 0;
				m_nHatSelectionPlayer2 = 0;
				CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_MENU_SELECT, 200);
			}
		}
	
	return true;
}

void CCharacterSelectionState::Update(float fElapsedTime)
{

	if(m_bAudioPlayCharacter == true)
	{
		m_bAudioPlayCharacter = false;
		if( (m_nSelection % 2) == 0)
		{
			CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_DX_BARRON_FLAMETHROWERULT, 200);
		}
		else if( (m_nSelection % 2) == 1 || m_nSelection == -1)
		{
			CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_DX_CLAIRE_FLAMETHROWERULT, 200);
		}
	}

	m_BarronShape.GetAnimCtrl().update(fElapsedTime);
	m_ClaireShape.GetAnimCtrl().update(fElapsedTime);

	if(m_BarronShape.GetAnimCtrl().getLoopCount() > 0)
	{
		m_BarronShape.GetAnimCtrl().SetAnimation("victoryIdle");
		m_BarronShape.GetAnimCtrl().update(fElapsedTime);
	}

	if(m_ClaireShape.GetAnimCtrl().getLoopCount() > 0)
	{
		m_ClaireShape.GetAnimCtrl().SetAnimation("victoryIdle");
		m_ClaireShape.GetAnimCtrl().update(fElapsedTime);
	}

	/*if(!CGameState::GetInstance()->GetPaused())
	{
		CMainMenuState::GetInstance()->Update(fElapsedTime);
	}*/


	CCamera::GetInstance()->Update(fElapsedTime);
}

void CCharacterSelectionState::Render(void)
{
	D3DXMATRIX m_mxViewProj  = CCamera::GetInstance()->GetViewMatrix() * CCamera::GetInstance()->GetProjectionMatrix();

	if(!m_bConf)
	{
			RenderPlayer1Hats();

		if(m_bHat2)
		{
			RenderPlayer2Hats();
		}
	}

	if(m_bConf)
	{
		RenderConfirm();
	}

	m_Buttons.Render(m_mxViewProj,true,true,true,true,false);

}

void CCharacterSelectionState::RenderPlayer1Hats(void)
{
	// Render if in Multiplayer or Single Player
	if(CGame::GetInstance()->GetMutliplayer())
	{
		if(!m_bHat)
		{
			CBitMapFont::GetInstance()->Print("Multiplayer File",m_nOtherFontX,m_nFontStart,false,false,1.5);
		}
		else
		{
			CBitMapFont::GetInstance()->Print("Hats",m_nFontX,m_nFontStart,false,false,1.5);
		}
	}
	else
	{
		if(!m_bHat)
		{
			CBitMapFont::GetInstance()->Print("Single Player",m_nOtherFontX,m_nFontStart,false,false,1.5);
		}
		else
		{
			CBitMapFont::GetInstance()->Print("Hats",m_nFontX,m_nFontStart,false,false,1.5);
		}
	}

	if(!m_bHat)
	{
		CBitMapFont::GetInstance()->Print("Barron",m_nFontX,m_nFontStart+(m_nFontOffset*2),false,false,m_fScale);
		CBitMapFont::GetInstance()->Print("Claire",m_nFontX,m_nFontStart+(m_nFontOffset*3),false,false,m_fScale);
		CBitMapFont::GetInstance()->Print("=>",m_nArrowPosition,m_nFontStart+((m_nFontOffset*(2+m_nSelection))),false,false,m_fScale);
		m_Buttons.Render(m_mxViewProj,true,false,true,true,((XboxGamePadDevice*)CInput::GetInstance()->GetDevice(DEV_GAMEPAD))->ControllerConnected(0));
	}

	// Render Character Selection 
	if(!m_bHat)
	{
		switch(m_nSelection)
		{
		case 0:
			m_BarronShape.GetFrame().GetLocalMatrix() = matIdentity;
			m_BarronShape.GetRenderContext()->AddRenderNode(&m_BarronShape);
			break;
		case 1:
			m_ClaireShape.GetFrame().GetLocalMatrix() = matCIdentity;
			m_ClaireShape.GetRenderContext()->AddRenderNode(&m_ClaireShape);
			break;
		}
	}

	if(m_bHat && !m_bHat2)
	{
		CBitMapFont::GetInstance()->Print("Player 1",m_nOtherFontX,m_nFontStart+m_nFontOffset,false,false,1.5);
		if(m_nSelection == 0)
		{
			switch(m_nHatSelectionPlayer1)
			{
			case 0:
				CBitMapFont::GetInstance()->Print("NONE",360,350);
				m_BarronShape.GetFrame().GetLocalMatrix() = matIdentity;
				m_BarronShape.GetRenderContext()->AddRenderNode(&m_BarronShape);
				break;
			case 1:
				m_HatShape[ARMY].GetFrame().GetLocalMatrix() = *m_BarronShape.GetAnimCtrl().getMatrixByID(2) * m_BarronShape.GetFrame().GetWorldMatrix();
				m_HatShape[ARMY].GetRenderContext()->AddRenderNode(&m_HatShape[ARMY]);
				m_BarronShape.GetFrame().GetLocalMatrix() = matIdentity;
				m_BarronShape.GetRenderContext()->AddRenderNode(&m_BarronShape);
				break;
			case 2:
				m_HatShape[BEAR].GetFrame().GetLocalMatrix() = *m_BarronShape.GetAnimCtrl().getMatrixByID(2) * m_BarronShape.GetFrame().GetWorldMatrix();
				m_HatShape[BEAR].GetRenderContext()->AddRenderNode(&m_HatShape[BEAR]);
				m_BarronShape.GetFrame().GetLocalMatrix() = matIdentity;
				m_BarronShape.GetRenderContext()->AddRenderNode(&m_BarronShape);
				break;
			case 3:
				m_HatShape[CAT].GetFrame().GetLocalMatrix() = *m_BarronShape.GetAnimCtrl().getMatrixByID(2) * m_BarronShape.GetFrame().GetWorldMatrix();
				m_HatShape[CAT].GetRenderContext()->AddRenderNode(&m_HatShape[CAT]);
				m_BarronShape.GetFrame().GetLocalMatrix() = matIdentity;
				m_BarronShape.GetRenderContext()->AddRenderNode(&m_BarronShape);
				break;
			case 4:
				m_HatShape[COW].GetFrame().GetLocalMatrix() = *m_BarronShape.GetAnimCtrl().getMatrixByID(2) * m_BarronShape.GetFrame().GetWorldMatrix();
				m_HatShape[COW].GetRenderContext()->AddRenderNode(&m_HatShape[COW]);
				m_BarronShape.GetFrame().GetLocalMatrix() = matIdentity;
				m_BarronShape.GetRenderContext()->AddRenderNode(&m_BarronShape);
				break;
			case 5:
				m_HatShape[FANCY].GetFrame().GetLocalMatrix() = *m_BarronShape.GetAnimCtrl().getMatrixByID(2) * m_BarronShape.GetFrame().GetWorldMatrix();
				m_HatShape[FANCY].GetRenderContext()->AddRenderNode(&m_HatShape[FANCY]);
				m_BarronShape.GetFrame().GetLocalMatrix() = matIdentity;
				m_BarronShape.GetRenderContext()->AddRenderNode(&m_BarronShape);
				break;
			case 6:
				m_HatShape[FRUIT].GetFrame().GetLocalMatrix() = *m_BarronShape.GetAnimCtrl().getMatrixByID(2) * m_BarronShape.GetFrame().GetWorldMatrix();
				m_HatShape[FRUIT].GetRenderContext()->AddRenderNode(&m_HatShape[FRUIT]);
				m_BarronShape.GetFrame().GetLocalMatrix() = matIdentity;
				m_BarronShape.GetRenderContext()->AddRenderNode(&m_BarronShape);
				break;
			case 7:
				m_HatShape[NAR].GetFrame().GetLocalMatrix() = *m_BarronShape.GetAnimCtrl().getMatrixByID(2) * m_BarronShape.GetFrame().GetWorldMatrix();
				m_HatShape[NAR].GetRenderContext()->AddRenderNode(&m_HatShape[NAR]);
				m_BarronShape.GetFrame().GetLocalMatrix() = matIdentity;
				m_BarronShape.GetRenderContext()->AddRenderNode(&m_BarronShape);
				break;
			case 8:
				m_HatShape[MAR].GetFrame().GetLocalMatrix() = *m_BarronShape.GetAnimCtrl().getMatrixByID(2) * m_BarronShape.GetFrame().GetWorldMatrix();
				m_HatShape[MAR].GetRenderContext()->AddRenderNode(&m_HatShape[MAR]);
				m_BarronShape.GetFrame().GetLocalMatrix() = matIdentity;
				m_BarronShape.GetRenderContext()->AddRenderNode(&m_BarronShape);
				break;
			case 9:
				m_HatShape[POKE].GetFrame().GetLocalMatrix() = *m_BarronShape.GetAnimCtrl().getMatrixByID(2) * m_BarronShape.GetFrame().GetWorldMatrix();
				m_HatShape[POKE].GetRenderContext()->AddRenderNode(&m_HatShape[POKE]);
				m_BarronShape.GetFrame().GetLocalMatrix() = matIdentity;
				m_BarronShape.GetRenderContext()->AddRenderNode(&m_BarronShape);
				break;
			case 10:
				m_HatShape[POT].GetFrame().GetLocalMatrix() = *m_BarronShape.GetAnimCtrl().getMatrixByID(2) * m_BarronShape.GetFrame().GetWorldMatrix();
				m_HatShape[POT].GetRenderContext()->AddRenderNode(&m_HatShape[POT]);
				m_BarronShape.GetFrame().GetLocalMatrix() = matIdentity;
				m_BarronShape.GetRenderContext()->AddRenderNode(&m_BarronShape);
				break;
			case 11:
				m_HatShape[SHARK].GetFrame().GetLocalMatrix() = *m_BarronShape.GetAnimCtrl().getMatrixByID(2) * m_BarronShape.GetFrame().GetWorldMatrix();
				m_HatShape[SHARK].GetRenderContext()->AddRenderNode(&m_HatShape[SHARK]);
				m_BarronShape.GetFrame().GetLocalMatrix() = matIdentity;
				m_BarronShape.GetRenderContext()->AddRenderNode(&m_BarronShape);
				break;
			case 12:
				m_HatShape[SOM].GetFrame().GetLocalMatrix() = *m_BarronShape.GetAnimCtrl().getMatrixByID(2) * m_BarronShape.GetFrame().GetWorldMatrix();
				m_HatShape[SOM].GetRenderContext()->AddRenderNode(&m_HatShape[SOM]);
				m_BarronShape.GetFrame().GetLocalMatrix() = matIdentity;
				m_BarronShape.GetRenderContext()->AddRenderNode(&m_BarronShape);
				break;
			}
		}
		else
		{
			switch(m_nHatSelectionPlayer2)
			{
			case 0:
				CBitMapFont::GetInstance()->Print("NONE",360,350);
				m_ClaireShape.GetFrame().GetLocalMatrix() = matCIdentity;
				m_ClaireShape.GetRenderContext()->AddRenderNode(&m_ClaireShape);
				break;
			case 1:
				m_HatShape[CLAIREHAT].GetFrame().GetLocalMatrix() = *m_ClaireShape.GetAnimCtrl().getMatrixByID(2) * m_ClaireShape.GetFrame().GetWorldMatrix();
				m_HatShape[CLAIREHAT].GetRenderContext()->AddRenderNode(&m_HatShape[CLAIREHAT]);
				m_ClaireShape.GetFrame().GetLocalMatrix() = matCIdentity;
				m_ClaireShape.GetRenderContext()->AddRenderNode(&m_ClaireShape);
				break;
			case 2:
				m_HatShape[BEAR].GetFrame().GetLocalMatrix() = *m_ClaireShape.GetAnimCtrl().getMatrixByID(2) * m_ClaireShape.GetFrame().GetWorldMatrix();
				m_HatShape[BEAR].GetRenderContext()->AddRenderNode(&m_HatShape[BEAR]);
				m_ClaireShape.GetFrame().GetLocalMatrix() = matCIdentity;
				m_ClaireShape.GetRenderContext()->AddRenderNode(&m_ClaireShape);
				break;
			case 3:
				m_HatShape[CAT].GetFrame().GetLocalMatrix() = *m_ClaireShape.GetAnimCtrl().getMatrixByID(2) * m_ClaireShape.GetFrame().GetWorldMatrix();
				m_HatShape[CAT].GetRenderContext()->AddRenderNode(&m_HatShape[CAT]);
				m_ClaireShape.GetFrame().GetLocalMatrix() = matCIdentity;
				m_ClaireShape.GetRenderContext()->AddRenderNode(&m_ClaireShape);
				break;
			case 4:
				m_HatShape[COW].GetFrame().GetLocalMatrix() = *m_ClaireShape.GetAnimCtrl().getMatrixByID(2) * m_ClaireShape.GetFrame().GetWorldMatrix();
				m_HatShape[COW].GetRenderContext()->AddRenderNode(&m_HatShape[COW]);
				m_ClaireShape.GetFrame().GetLocalMatrix() = matCIdentity;
				m_ClaireShape.GetRenderContext()->AddRenderNode(&m_ClaireShape);
				break;
			case 5:
				m_HatShape[FANCY].GetFrame().GetLocalMatrix() = *m_ClaireShape.GetAnimCtrl().getMatrixByID(2) * m_ClaireShape.GetFrame().GetWorldMatrix();
				m_HatShape[FANCY].GetRenderContext()->AddRenderNode(&m_HatShape[FANCY]);
				m_ClaireShape.GetFrame().GetLocalMatrix() = matCIdentity;
				m_ClaireShape.GetRenderContext()->AddRenderNode(&m_ClaireShape);
				break;
			case 6:
				m_HatShape[FRUIT].GetFrame().GetLocalMatrix() = *m_ClaireShape.GetAnimCtrl().getMatrixByID(2) * m_ClaireShape.GetFrame().GetWorldMatrix();
				m_HatShape[FRUIT].GetRenderContext()->AddRenderNode(&m_HatShape[FRUIT]);
				m_ClaireShape.GetFrame().GetLocalMatrix() = matCIdentity;
				m_ClaireShape.GetRenderContext()->AddRenderNode(&m_ClaireShape);
				break;
			case 7:
				m_HatShape[NAR].GetFrame().GetLocalMatrix() = *m_ClaireShape.GetAnimCtrl().getMatrixByID(2) * m_ClaireShape.GetFrame().GetWorldMatrix();
				m_HatShape[NAR].GetRenderContext()->AddRenderNode(&m_HatShape[NAR]);
				m_ClaireShape.GetFrame().GetLocalMatrix() = matCIdentity;
				m_ClaireShape.GetRenderContext()->AddRenderNode(&m_ClaireShape);
				break;
			case 8:
				m_HatShape[MAR].GetFrame().GetLocalMatrix() = *m_ClaireShape.GetAnimCtrl().getMatrixByID(2) * m_ClaireShape.GetFrame().GetWorldMatrix();
				m_HatShape[MAR].GetRenderContext()->AddRenderNode(&m_HatShape[MAR]);
				m_ClaireShape.GetFrame().GetLocalMatrix() = matCIdentity;
				m_ClaireShape.GetRenderContext()->AddRenderNode(&m_ClaireShape);
				break;
			case 9:
				m_HatShape[POKE].GetFrame().GetLocalMatrix() = *m_ClaireShape.GetAnimCtrl().getMatrixByID(2) * m_ClaireShape.GetFrame().GetWorldMatrix();
				m_HatShape[POKE].GetRenderContext()->AddRenderNode(&m_HatShape[POKE]);
				m_ClaireShape.GetFrame().GetLocalMatrix() = matCIdentity;
				m_ClaireShape.GetRenderContext()->AddRenderNode(&m_ClaireShape);
				break;
			case 10:
				m_HatShape[POT].GetFrame().GetLocalMatrix() = *m_ClaireShape.GetAnimCtrl().getMatrixByID(2) * m_ClaireShape.GetFrame().GetWorldMatrix();
				m_HatShape[POT].GetRenderContext()->AddRenderNode(&m_HatShape[POT]);
				m_ClaireShape.GetFrame().GetLocalMatrix() = matCIdentity;
				m_ClaireShape.GetRenderContext()->AddRenderNode(&m_ClaireShape);
				break;
			case 11:
				m_HatShape[SHARK].GetFrame().GetLocalMatrix() = *m_ClaireShape.GetAnimCtrl().getMatrixByID(2) * m_ClaireShape.GetFrame().GetWorldMatrix();
				m_HatShape[SHARK].GetRenderContext()->AddRenderNode(&m_HatShape[SHARK]);
				m_ClaireShape.GetFrame().GetLocalMatrix() = matCIdentity;
				m_ClaireShape.GetRenderContext()->AddRenderNode(&m_ClaireShape);
				break;
			case 12:
				m_HatShape[SOM].GetFrame().GetLocalMatrix() = *m_ClaireShape.GetAnimCtrl().getMatrixByID(2) * m_ClaireShape.GetFrame().GetWorldMatrix();
				m_HatShape[SOM].GetRenderContext()->AddRenderNode(&m_HatShape[SOM]);
				m_ClaireShape.GetFrame().GetLocalMatrix() = matCIdentity;
				m_ClaireShape.GetRenderContext()->AddRenderNode(&m_ClaireShape);
				break;
			}
		}
	}
}

void CCharacterSelectionState::RenderPlayer2Hats(void)
{
	// Render if in Multiplayer or Single Player
	
	CBitMapFont::GetInstance()->Print("Hats",m_nFontX,m_nFontStart,false,false,1.5);
		
	if(m_bHat && m_bHat2)
	{
		CBitMapFont::GetInstance()->Print("Player 2",m_nOtherFontX,m_nFontStart+m_nFontOffset,false,false,1.5);
		m_Buttons.Render(m_mxViewProj,false,true,true,true,((XboxGamePadDevice*)CInput::GetInstance()->GetDevice(DEV_GAMEPAD))->ControllerConnected(0));
		if(m_nSelection == 0)
		{
			switch(m_nHatSelectionPlayer2)
			{
			case 0:
				CBitMapFont::GetInstance()->Print("NONE",360,350);
				m_ClaireShape.GetFrame().GetLocalMatrix() = matCIdentity;
				m_ClaireShape.GetRenderContext()->AddRenderNode(&m_ClaireShape);
				break;
			case 1:
				m_HatShape[CLAIREHAT].GetFrame().GetLocalMatrix() = *m_ClaireShape.GetAnimCtrl().getMatrixByID(2) * m_ClaireShape.GetFrame().GetWorldMatrix();
				m_HatShape[CLAIREHAT].GetRenderContext()->AddRenderNode(&m_HatShape[CLAIREHAT]);
				m_ClaireShape.GetFrame().GetLocalMatrix() = matCIdentity;
				m_ClaireShape.GetRenderContext()->AddRenderNode(&m_ClaireShape);
				break;
			case 2:
				m_HatShape[BEAR].GetFrame().GetLocalMatrix() = *m_ClaireShape.GetAnimCtrl().getMatrixByID(2) * m_ClaireShape.GetFrame().GetWorldMatrix();
				m_HatShape[BEAR].GetRenderContext()->AddRenderNode(&m_HatShape[BEAR]);
				m_ClaireShape.GetFrame().GetLocalMatrix() = matCIdentity;
				m_ClaireShape.GetRenderContext()->AddRenderNode(&m_ClaireShape);
				break;
			case 3:
				m_HatShape[CAT].GetFrame().GetLocalMatrix() =*m_ClaireShape.GetAnimCtrl().getMatrixByID(2) * m_ClaireShape.GetFrame().GetWorldMatrix();
				m_HatShape[CAT].GetRenderContext()->AddRenderNode(&m_HatShape[CAT]);
				m_ClaireShape.GetFrame().GetLocalMatrix() = matCIdentity;
				m_ClaireShape.GetRenderContext()->AddRenderNode(&m_ClaireShape);
				break;
			case 4:
				m_HatShape[COW].GetFrame().GetLocalMatrix() = *m_ClaireShape.GetAnimCtrl().getMatrixByID(2) * m_ClaireShape.GetFrame().GetWorldMatrix();
				m_HatShape[COW].GetRenderContext()->AddRenderNode(&m_HatShape[COW]);
				m_ClaireShape.GetFrame().GetLocalMatrix() = matCIdentity;
				m_ClaireShape.GetRenderContext()->AddRenderNode(&m_ClaireShape);
				break;
			case 5:
				m_HatShape[FANCY].GetFrame().GetLocalMatrix() = *m_ClaireShape.GetAnimCtrl().getMatrixByID(2) * m_ClaireShape.GetFrame().GetWorldMatrix();
				m_HatShape[FANCY].GetRenderContext()->AddRenderNode(&m_HatShape[FANCY]);
				m_ClaireShape.GetFrame().GetLocalMatrix() = matCIdentity;
				m_ClaireShape.GetRenderContext()->AddRenderNode(&m_ClaireShape);
				break;
			case 6:
				m_HatShape[FRUIT].GetFrame().GetLocalMatrix() = *m_ClaireShape.GetAnimCtrl().getMatrixByID(2) * m_ClaireShape.GetFrame().GetWorldMatrix();
				m_HatShape[FRUIT].GetRenderContext()->AddRenderNode(&m_HatShape[FRUIT]);
				m_ClaireShape.GetFrame().GetLocalMatrix() = matCIdentity;
				m_ClaireShape.GetRenderContext()->AddRenderNode(&m_ClaireShape);
				break;
			case 7:
				m_HatShape[NAR].GetFrame().GetLocalMatrix() = *m_ClaireShape.GetAnimCtrl().getMatrixByID(2) * m_ClaireShape.GetFrame().GetWorldMatrix();
				m_HatShape[NAR].GetRenderContext()->AddRenderNode(&m_HatShape[NAR]);
				m_ClaireShape.GetFrame().GetLocalMatrix() = matCIdentity;
				m_ClaireShape.GetRenderContext()->AddRenderNode(&m_ClaireShape);
				break;
			case 8:
				m_HatShape[MAR].GetFrame().GetLocalMatrix() = *m_ClaireShape.GetAnimCtrl().getMatrixByID(2) * m_ClaireShape.GetFrame().GetWorldMatrix();
				m_HatShape[MAR].GetRenderContext()->AddRenderNode(&m_HatShape[MAR]);
				m_ClaireShape.GetFrame().GetLocalMatrix() = matCIdentity;
				m_ClaireShape.GetRenderContext()->AddRenderNode(&m_ClaireShape);
				break;
			case 9:
				m_HatShape[POKE].GetFrame().GetLocalMatrix() = *m_ClaireShape.GetAnimCtrl().getMatrixByID(2) * m_ClaireShape.GetFrame().GetWorldMatrix();
				m_HatShape[POKE].GetRenderContext()->AddRenderNode(&m_HatShape[POKE]);
				m_ClaireShape.GetFrame().GetLocalMatrix() = matCIdentity;
				m_ClaireShape.GetRenderContext()->AddRenderNode(&m_ClaireShape);
				break;
			case 10:
				m_HatShape[POT].GetFrame().GetLocalMatrix() = *m_ClaireShape.GetAnimCtrl().getMatrixByID(2) * m_ClaireShape.GetFrame().GetWorldMatrix();
				m_HatShape[POT].GetRenderContext()->AddRenderNode(&m_HatShape[POT]);
				m_ClaireShape.GetFrame().GetLocalMatrix() = matCIdentity;
				m_ClaireShape.GetRenderContext()->AddRenderNode(&m_ClaireShape);
				break;
			case 11:
				m_HatShape[SHARK].GetFrame().GetLocalMatrix() = *m_ClaireShape.GetAnimCtrl().getMatrixByID(2) * m_ClaireShape.GetFrame().GetWorldMatrix();
				m_HatShape[SHARK].GetRenderContext()->AddRenderNode(&m_HatShape[SHARK]);
				m_ClaireShape.GetFrame().GetLocalMatrix() = matCIdentity;
				m_ClaireShape.GetRenderContext()->AddRenderNode(&m_ClaireShape);
				break;
			case 12:
				m_HatShape[SOM].GetFrame().GetLocalMatrix() = *m_ClaireShape.GetAnimCtrl().getMatrixByID(2) * m_ClaireShape.GetFrame().GetWorldMatrix();
				m_HatShape[SOM].GetRenderContext()->AddRenderNode(&m_HatShape[SOM]);
				m_ClaireShape.GetFrame().GetLocalMatrix() = matCIdentity;
				m_ClaireShape.GetRenderContext()->AddRenderNode(&m_ClaireShape);
				break;
			}
		}
		else
		{
			switch(m_nHatSelectionPlayer1)
			{
			case 0:
				CBitMapFont::GetInstance()->Print("NONE",360,350);
				m_BarronShape.GetFrame().GetLocalMatrix() = matIdentity;
				m_BarronShape.GetRenderContext()->AddRenderNode(&m_BarronShape);
				break;
			case 1:
				m_HatShape[ARMY].GetFrame().GetLocalMatrix() = *m_BarronShape.GetAnimCtrl().getMatrixByID(2) * m_BarronShape.GetFrame().GetWorldMatrix();
				m_HatShape[ARMY].GetRenderContext()->AddRenderNode(&m_HatShape[ARMY]);
				m_BarronShape.GetFrame().GetLocalMatrix() = matIdentity;
				m_BarronShape.GetRenderContext()->AddRenderNode(&m_BarronShape);
				break;
			case 2:
				m_HatShape[BEAR].GetFrame().GetLocalMatrix() = *m_BarronShape.GetAnimCtrl().getMatrixByID(2) * m_BarronShape.GetFrame().GetWorldMatrix();
				m_HatShape[BEAR].GetRenderContext()->AddRenderNode(&m_HatShape[BEAR]);
				m_BarronShape.GetFrame().GetLocalMatrix() = matIdentity;
				m_BarronShape.GetRenderContext()->AddRenderNode(&m_BarronShape);
				break;
			case 3:
				m_HatShape[CAT].GetFrame().GetLocalMatrix() = *m_BarronShape.GetAnimCtrl().getMatrixByID(2) * m_BarronShape.GetFrame().GetWorldMatrix();
				m_HatShape[CAT].GetRenderContext()->AddRenderNode(&m_HatShape[CAT]);
				m_BarronShape.GetFrame().GetLocalMatrix() = matIdentity;
				m_BarronShape.GetRenderContext()->AddRenderNode(&m_BarronShape);
				break;
			case 4:
				m_HatShape[COW].GetFrame().GetLocalMatrix() = *m_BarronShape.GetAnimCtrl().getMatrixByID(2) * m_BarronShape.GetFrame().GetWorldMatrix();
				m_HatShape[COW].GetRenderContext()->AddRenderNode(&m_HatShape[COW]);
				m_BarronShape.GetFrame().GetLocalMatrix() = matIdentity;
				m_BarronShape.GetRenderContext()->AddRenderNode(&m_BarronShape);
				break;
			case 5:
				m_HatShape[FANCY].GetFrame().GetLocalMatrix() = *m_BarronShape.GetAnimCtrl().getMatrixByID(2) * m_BarronShape.GetFrame().GetWorldMatrix();
				m_HatShape[FANCY].GetRenderContext()->AddRenderNode(&m_HatShape[FANCY]);
				m_BarronShape.GetFrame().GetLocalMatrix() = matIdentity;
				m_BarronShape.GetRenderContext()->AddRenderNode(&m_BarronShape);
				break;
			case 6:
				m_HatShape[FRUIT].GetFrame().GetLocalMatrix() = *m_BarronShape.GetAnimCtrl().getMatrixByID(2) * m_BarronShape.GetFrame().GetWorldMatrix();
				m_HatShape[FRUIT].GetRenderContext()->AddRenderNode(&m_HatShape[FRUIT]);
				m_BarronShape.GetFrame().GetLocalMatrix() = matIdentity;
				m_BarronShape.GetRenderContext()->AddRenderNode(&m_BarronShape);
				break;
			case 7:
				m_HatShape[NAR].GetFrame().GetLocalMatrix() = *m_BarronShape.GetAnimCtrl().getMatrixByID(2) * m_BarronShape.GetFrame().GetWorldMatrix();
				m_HatShape[NAR].GetRenderContext()->AddRenderNode(&m_HatShape[NAR]);
				m_BarronShape.GetFrame().GetLocalMatrix() = matIdentity;
				m_BarronShape.GetRenderContext()->AddRenderNode(&m_BarronShape);
				break;
			case 8:
				m_HatShape[MAR].GetFrame().GetLocalMatrix() = *m_BarronShape.GetAnimCtrl().getMatrixByID(2) * m_BarronShape.GetFrame().GetWorldMatrix();
				m_HatShape[MAR].GetRenderContext()->AddRenderNode(&m_HatShape[MAR]);
				m_BarronShape.GetFrame().GetLocalMatrix() = matIdentity;
				m_BarronShape.GetRenderContext()->AddRenderNode(&m_BarronShape);
				break;
			case 9:
				m_HatShape[POKE].GetFrame().GetLocalMatrix() = *m_BarronShape.GetAnimCtrl().getMatrixByID(2) * m_BarronShape.GetFrame().GetWorldMatrix();
				m_HatShape[POKE].GetRenderContext()->AddRenderNode(&m_HatShape[POKE]);
				m_BarronShape.GetFrame().GetLocalMatrix() = matIdentity;
				m_BarronShape.GetRenderContext()->AddRenderNode(&m_BarronShape);
				break;
			case 10:
				m_HatShape[POT].GetFrame().GetLocalMatrix() = *m_BarronShape.GetAnimCtrl().getMatrixByID(2) * m_BarronShape.GetFrame().GetWorldMatrix();
				m_HatShape[POT].GetRenderContext()->AddRenderNode(&m_HatShape[POT]);
				m_BarronShape.GetFrame().GetLocalMatrix() = matIdentity;
				m_BarronShape.GetRenderContext()->AddRenderNode(&m_BarronShape);
				break;
			case 11:
				m_HatShape[SHARK].GetFrame().GetLocalMatrix() = *m_BarronShape.GetAnimCtrl().getMatrixByID(2) * m_BarronShape.GetFrame().GetWorldMatrix();
				m_HatShape[SHARK].GetRenderContext()->AddRenderNode(&m_HatShape[SHARK]);
				m_BarronShape.GetFrame().GetLocalMatrix() = matIdentity;
				m_BarronShape.GetRenderContext()->AddRenderNode(&m_BarronShape);
				break;
			case 12:
				m_HatShape[SOM].GetFrame().GetLocalMatrix() = *m_BarronShape.GetAnimCtrl().getMatrixByID(2) * m_BarronShape.GetFrame().GetWorldMatrix();
				m_HatShape[SOM].GetRenderContext()->AddRenderNode(&m_HatShape[SOM]);
				m_BarronShape.GetFrame().GetLocalMatrix() = matIdentity;
				m_BarronShape.GetRenderContext()->AddRenderNode(&m_BarronShape);
				break;
			}
		}
	}
}

void CCharacterSelectionState::RenderConfirm(void)
{
	if(m_bConf)
	{
		CBitMapFont::GetInstance()->Print("=>",m_nArrowPosition*(m_nConfirm+1),(m_nFontStart+100)+(m_nFontOffset*2),false,false,0.85);
		CBitMapFont::GetInstance()->Print("Do you wish to continue?",200,(m_nFontStart+100)+(m_nFontOffset),false,false,0.85);
		CBitMapFont::GetInstance()->Print("Yes",m_nFontX,(m_nFontStart+100)+(m_nFontOffset*2),false,false,0.85);
		CBitMapFont::GetInstance()->Print("No",m_nFontX+(m_nArrowPosition),(m_nFontStart+100)+(m_nFontOffset*2),false,false,0.85);
		m_Buttons.Render(m_mxViewProj,false,false,true,true,((XboxGamePadDevice*)CInput::GetInstance()->GetDevice(DEV_GAMEPAD))->ControllerConnected(0));
	}
}


bool CCharacterSelectionState::Exit(void)
{
	return false;
}

CCharacterSelectionState* CCharacterSelectionState::GetInstance(void)
{
	static CCharacterSelectionState instance;
	return &instance;
}