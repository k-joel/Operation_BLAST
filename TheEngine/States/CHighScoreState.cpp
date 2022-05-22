#include "CHighScoreState.h"
#include "CGameState.h"
#include "CStateMachine.h"
#include "CMainMenuState.h"
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

CHighScoreState::CHighScoreState(void)
{

}

CHighScoreState::~CHighScoreState(void)
{

}

void CHighScoreState::Enter(void)
{
	m_nOffset = 42;
	m_Buttons.Initialize(true);
}

bool CHighScoreState::Input(void)
{
	if(CInput::GetInstance()->GetAction(ACT_ACCEPT,0)||CInput::GetInstance()->GetAction(ACT_ACCEPT,1))
	{
		CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_MENU_GOBACK, 200);
		CMainMenuState::GetInstance()->SetOptions(false);
		CStateSystem::GetInstance()->RemoveTopState();
	}
	return true;
}

void CHighScoreState::Update(float fElapsedTime)
{
	if(!CGameState::GetInstance()->GetPaused())
	{
		CMainMenuState::GetInstance()->Update(fElapsedTime);
	}
}

void CHighScoreState::Render(void)
{
	D3DXMATRIX m_mxViewProj  = CCamera::GetInstance()->GetViewMatrix() * CCamera::GetInstance()->GetProjectionMatrix();
	CBitMapFont::GetInstance()->Print("RANKINGS!",320,170-(m_nOffset),false,false,1.25);
	for(unsigned int ScoreIndex = 0; ScoreIndex < 10; ScoreIndex++)
	{
		CBitMapFont::GetInstance()->Print(CLoadState::GetInstance()->GetHighScore(ScoreIndex)->GetName().c_str(),320,170+(m_nOffset*ScoreIndex),false,false,0.85f);
		char buffer[10];
		sprintf_s(buffer,sizeof(buffer),"%i",CLoadState::GetInstance()->GetHighScore(ScoreIndex)->GetScore());
		CBitMapFont::GetInstance()->Print(buffer,500,170+(m_nOffset*ScoreIndex),false,false,0.85f);
	}
	m_Buttons.Render(m_mxViewProj,false,false,true,false,((XboxGamePadDevice*)CInput::GetInstance()->GetDevice(DEV_GAMEPAD))->ControllerConnected(0));
}

bool CHighScoreState::Exit(void)
{
	return false;
}

CHighScoreState* CHighScoreState::GetInstance(void)
{
	static CHighScoreState instance;
	return &instance;
}