#include "CControlOptionState.h"
#include "../Rendering/CTextureManager.h"
#include "../Font/CBitMapFont.h"
#include "../Input/CInput.h"
#include "../Input/CInput.h"
#include "CStateMachine.h"
#include "../Core/CGame.h"
#include "../Resources/Sound/Wwise_IDs.h"
#include "../Sound/WwiseManager.h"
#include "../Camera/Camera.h"

CControlOptionState::CControlOptionState(void)
{

}

CControlOptionState::~CControlOptionState(void)
{

}

void CControlOptionState::Enter(void)
{
	m_Buttons.Initialize(CGame::GetInstance()->GetPaused());
}

bool CControlOptionState::Input(void)
{
	if(CInput::GetInstance()->GetAction(ACT_ACCEPT)||CInput::GetInstance()->GetAction(ACT_ACCEPT,1))
	{
		CWwiseManager::GetInstance()->PostEvent(AK::EVENTS::PLAY_FX_MENU_GOBACK, 200);
		CStateSystem::GetInstance()->RemoveTopState();
	}

	return true;
}

void CControlOptionState::Update(float fElapsedTime)
{
	m_mxViewProj  = CCamera::GetInstance()->GetViewMatrix() * CCamera::GetInstance()->GetProjectionMatrix();
}

void CControlOptionState::Render(void)
{
	CBitMapFont::GetInstance()->Print("Please press Accept to exit.",200,110);
	m_Buttons.Render(m_mxViewProj,false,false,true,false,((XboxGamePadDevice*)CInput::GetInstance()->GetDevice(DEV_GAMEPAD))->ControllerConnected(0));
}

bool CControlOptionState::Exit(void)
{
	return false;
}

CControlOptionState* CControlOptionState::GetInstance(void)
{
	static CControlOptionState instance;
	return &instance;
}