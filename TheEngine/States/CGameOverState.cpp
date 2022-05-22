#include "CGameOverState.h"
#include "CStateMachine.h"
#include "CMainMenuState.h"
#include "CStatsState.h"
#include "CLoadState.h"
#include "../Core/CGame.h"
#include "CCharacterSelectionState.h"
#include "../Rendering/CTextureManager.h"
#include "../Font/CBitMapFont.h"
#include "../Input/CInput.h"
#include "../Rendering/CRenderer.h"
#include "../Rendering/CRenderContextManager.h"
#include "../Mesh/CModelManager.h"
#include "../Camera/Camera.h"

#include "../Resources/Sound/Wwise_IDs.h"
#include "../Sound/WwiseManager.h"
#include "../Rendering/CRenderShape.h"
#include "../Rendering/CRenderContextManager.h"
#include "../Rendering/CRenderShape.h"
#include "../Mesh/CModelManager.h"

CGameOverState::CGameOverState(void)
{
	m_nSelection = 0;
	m_fScale = 1.0;
	m_nFontX = 150;
	m_nArrowPosition = 120;
	m_nFontOffset = 42;
	m_nFontStart = 150;

	/*m_Barron.InitShape(CModelManager::GetInstance()->GetModel(CLoadState::GetInstance()->GetModelID("barron_geoShape")),
	&CRenderContextManager::GetInstance()->GetAnimatedLitContext(), matIdentity, CRenderShape::PNTBBRenderFunc, true);
	m_Barron.GetShape(0).GetAnimCtrl().SetAnimation("lose");
	CObjectManager::getinstance*/


}

CGameOverState::~CGameOverState(void)
{
}

void CGameOverState::Enter(void)
{
	m_CBF = CBitMapFont::GetInstance();
	m_CTM = CTextureManager::GetInstance();
	m_CIP = CInput::GetInstance();

	m_CBF = CBitMapFont::GetInstance();



	m_fBucket = 0.0;
	m_fTransTimer = 0.0;
	test = 0;

	m_nSelection = 0;

	blink = false;
	transition = false;
	m_bRender = true;
	m_bDone = false;

	// Setting HUD position
	D3DXVECTOR3 location = CCamera::GetInstance()->GetViewPosition();
	D3DXMATRIX rotation;
	D3DXMatrixRotationX(&rotation,D3DXToRadian(13));
	D3DXMatrixTranslation(&m_ConsoleWorld,location.x,location.y-13,location.z+26);
	D3DXMatrixMultiply(&m_ConsoleWorld,&rotation,&m_ConsoleWorld);

	m_consoleScreen = CModelManager::GetInstance()->GetModel(CLoadState::GetInstance()->GetModelID("PauseMenuScreenShape"));
	m_ConsoleShape.Init(&m_consoleScreen->GetMesh(), NULL, &CRenderContextManager::GetInstance()->GetHUDContext(), m_ConsoleWorld, CRenderShape::PNTRenderFunc);

	// Setting Barron position
	D3DXMatrixIdentity(&matIdentity);
	D3DXMatrixIdentity(&matCIdentity);
	CModel *model = CModelManager::GetInstance()->GetModel(CLoadState::GetInstance()->GetModelID("barron_geoShape"));
	m_BarronShape.Init(&model->GetMesh(), model,&CRenderContextManager::GetInstance()->GetAnimatedLitContext(), matIdentity, CRenderShape::PNTBBRenderFunc);
	m_BarronShape.GetAnimCtrl().SetAnimation("lose");

	// Setting Claire position
	CModel *clmodel = CModelManager::GetInstance()->GetModel(CLoadState::GetInstance()->GetModelID("Claire_geoShape"));
	m_ClaireShape.Init(&clmodel->GetMesh(), clmodel,&CRenderContextManager::GetInstance()->GetAnimatedLitContext(), matCIdentity, CRenderShape::PNTBBRenderFunc);
	m_ClaireShape.GetAnimCtrl().SetAnimation("lose");

	model = CModelManager::GetInstance()->GetModel(CLoadState::GetInstance()->GetModelID("GameOverShape"));
	m_GameOverShape.Init(&model->GetMesh(),model,&CRenderContextManager::GetInstance()->GetAnimatedLitContext(),matGameOver,CRenderShape::PNTBBRenderFunc);
	m_GameOverShape.GetAnimCtrl().SetAnimation("gameOver");

	D3DXMatrixIdentity(&rotation);
	D3DXMatrixRotationY(&rotation,D3DXToRadian(-75));
	D3DXMatrixTranslation(&matIdentity,CCamera::GetInstance()->GetViewPosition().x+5,CCamera::GetInstance()->GetViewPosition().y-20,CCamera::GetInstance()->GetViewPosition().z+150);
	D3DXMatrixMultiply(&matIdentity,&rotation,&matIdentity);

	D3DXMatrixIdentity(&rotation);
	D3DXMatrixRotationY(&rotation,D3DXToRadian(-90));
	D3DXMatrixTranslation(&matCIdentity,CCamera::GetInstance()->GetViewPosition().x-50,CCamera::GetInstance()->GetViewPosition().y-20,CCamera::GetInstance()->GetViewPosition().z+150);
	D3DXMatrixMultiply(&matCIdentity,&rotation,&matCIdentity);

	D3DXMatrixTranslation(&matGameOver,CCamera::GetInstance()->GetViewPosition().x-5,CCamera::GetInstance()->GetViewPosition().y-5,CCamera::GetInstance()->GetViewPosition().z+40);

	m_Buttons.Initialize(true);

	CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_MX_N2_GAMEOVER, 200);	// AudioID 200 is MENU
}

bool CGameOverState::Input(void)
{
	KeyboardDevice* keyboard = (KeyboardDevice*)CInput::GetInstance()->GetDevice(DEV_KEYBOARD);

	static float m_fPrevX = 0.0f;
	static float m_fCurrX = 0.0f;
	m_fPrevX = m_fCurrX;
	m_fCurrX = CInput::GetInstance()->GetActionAmount(ACT_MOVEX);
	if(m_fCurrX >= 32760 && m_fPrevX != m_fCurrX)
	{
		m_nSelection ++;
		CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_MENU_DOWN, 200);
	}
	if(m_fCurrX <= -32760 && m_fPrevX != m_fCurrX)
	{
		m_nSelection --;
		CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_MENU_UP, 200);
	}

	static float m_fPrevX2 = 0.0f;
	static float m_fCurrX2 = 0.0f;
	m_fPrevX2 = m_fCurrX2;
	m_fCurrX2 = CInput::GetInstance()->GetActionAmount(ACT_MOVEX,1);
	if(m_fCurrX2 >= 32760 && m_fPrevX2 != m_fCurrX2)
	{
		m_nSelection ++;
		CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_MENU_DOWN, 200);
	}
	if(m_fCurrX2 <= -32760 && m_fPrevX2 != m_fCurrX2)
	{
		m_nSelection --;
		CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_MENU_UP, 200);
	}


	if(m_CIP->GetAction(ACT_MOVELEFT)||m_CIP->GetAction(ACT_MOVELEFT,1))
	{
		--m_nSelection;
		CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_MENU_UP, 200); // AudioID 200 is MENU
	}

	if(m_CIP->GetAction(ACT_MOVERIGHT)||m_CIP->GetAction(ACT_MOVERIGHT,1))
	{
		++m_nSelection;
		CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_MENU_DOWN, 200); // AudioID 200 is MENU
	}

	if(m_nSelection < 0)
		m_nSelection = 0;

	if(m_nSelection > 1)
		m_nSelection = 1;

	if(m_CIP->GetAction(ACT_ACCEPT)||m_CIP->GetAction(ACT_ACCEPT,1))
	{

		switch(m_nSelection)
		{
		case 0:
			m_bRender = false;
			CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_MENU_SELECT, 200);	// AudioID 200 is MENU
			m_GameOverShape.GetAnimCtrl().SetAnimation("gameOverIdle");
			m_BarronShape.GetAnimCtrl().SetAnimation("loseIdle");
			m_ClaireShape.GetAnimCtrl().SetAnimation("loseIdle");
			m_bDone = true;
			break;
		case 1:
			m_bRender = false;
			CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_MENU_GOBACK, 200);	// AudioID 200 is MENU
			CStateSystem::GetInstance()->ChangeBaseState(CMainMenuState::GetInstance());
			break;
		}
	}

	if(keyboard->KeyDown(DIK_NUMPAD8))
		CCamera::GetInstance()->ViewTranslateLocalZ(0.1f);

	if(keyboard->KeyDown(DIK_NUMPAD2))
		CCamera::GetInstance()->ViewTranslateLocalZ(-0.1f);

	if(keyboard->KeyDown(DIK_NUMPAD4))
		CCamera::GetInstance()->ViewRotateGlobalX(-.01f);

	if(keyboard->KeyDown(DIK_NUMPAD6))
		CCamera::GetInstance()->ViewRotateGlobalX(.01f);

	if(keyboard->KeyDown(DIK_NUMPAD7))
		CCamera::GetInstance()->ViewTranslateLocalX(-0.1f);

	if(keyboard->KeyDown(DIK_NUMPAD9))
		CCamera::GetInstance()->ViewTranslateLocalX(0.1f);


	return true;
}

void CGameOverState::Update(float fElapsedTime)
{
	m_fBucket += fElapsedTime;
	m_fTransTimer += fElapsedTime;

	if(m_fBucket > 1.0 && blink == false)
	{
		blink = true;
		m_fBucket = 0.0;
	}
	else if(m_fBucket > 1.0 && blink == true)
	{
		blink = false;
		m_fBucket = 0.0;
	}
	//m_BarronShape.GetFrame().GetLocalMatrix() = 
	//if(m_BarronShape.GetAnimCtrl().getLoopCount() <= 2)
	if(m_GameOverShape.GetAnimCtrl().getLoopCount() > 0)
	{
		m_GameOverShape.GetAnimCtrl().SetAnimation("gameOverIdle");
		m_GameOverShape.GetAnimCtrl().update(fElapsedTime);
	}

	if(m_BarronShape.GetAnimCtrl().getLoopCount() > 0)
	{
		m_BarronShape.GetAnimCtrl().SetAnimation("loseIdle");
		m_BarronShape.GetAnimCtrl().update(fElapsedTime);
	}

	if(m_ClaireShape.GetAnimCtrl().getLoopCount() > 0)
	{
		m_ClaireShape.GetAnimCtrl().SetAnimation("loseIdle");
		m_ClaireShape.GetAnimCtrl().update(fElapsedTime);
	}
	
	m_BarronShape.GetAnimCtrl().update(fElapsedTime);
	m_ClaireShape.GetAnimCtrl().update(fElapsedTime);
	m_GameOverShape.GetAnimCtrl().update(fElapsedTime);
	//m_Barron.GetShape(0).GetAnimCtrl().update(fElapsedTime);
	


	if(m_bDone)
	{
		CStateSystem::GetInstance()->AddState(CStatsState::GetInstance());
	}

}

void CGameOverState::Render(void)
{
	//m_CTM->Draw(m_nBackgroundID,0,0,0.25f,0.20f,0,0.0f,0.0f,0.0f,D3DXCOLOR(155,155,155,100));
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

		if(CCharacterSelectionState::GetInstance()->GetSelection() == 0 && !CGame::GetInstance()->GetMutliplayer())
		{
			m_BarronShape.GetFrame().GetLocalMatrix() = matIdentity;
			m_BarronShape.GetRenderContext()->AddRenderNode(&m_BarronShape);
		}
		else if(CCharacterSelectionState::GetInstance()->GetSelection() == 1 && !CGame::GetInstance()->GetMutliplayer())
		{
			m_ClaireShape.GetFrame().GetLocalMatrix() = matCIdentity;
			m_ClaireShape.GetRenderContext()->AddRenderNode(&m_ClaireShape);
		}
		else if(CGame::GetInstance()->GetMutliplayer())
		{
			m_BarronShape.GetFrame().GetLocalMatrix() = matIdentity;
			m_BarronShape.GetRenderContext()->AddRenderNode(&m_BarronShape);

			m_ClaireShape.GetFrame().GetLocalMatrix() = matCIdentity;
			m_ClaireShape.GetRenderContext()->AddRenderNode(&m_ClaireShape);
		}
		m_GameOverShape.GetFrame().GetLocalMatrix() = matGameOver;
		m_GameOverShape.GetRenderContext()->AddRenderNode(&m_GameOverShape);
}

bool CGameOverState::Exit(void)
{
	CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::STOP_MX_N2_GAMEOVER, 200);	// AudioID 200 is MENU
	return false;
}

CGameOverState* CGameOverState::GetInstance(void)
{
	static CGameOverState instance;
	return &instance;
}