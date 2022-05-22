#include "CEndingState.h"
#include "CWinState.h"
#include "CStateMachine.h"
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

CEndingState::CEndingState(void)
{

}

CEndingState::~CEndingState(void)
{

}

void CEndingState::Enter(void)
{
	m_fTimer = 0.0f;
}

bool CEndingState::Input(void)
{
	if(CInput::GetInstance()->GetAction(ACT_ACCEPT,0)||CInput::GetInstance()->GetAction(ACT_ACCEPT,1))
	{
		CStateSystem::GetInstance()->ChangeBaseState(CWinState::GetInstance());
	}

	return true;
}

void CEndingState::Update(float fElapsedTime)
{
	m_fTimer += fElapsedTime;

	if(m_fTimer > 3.0)
	{
		m_nCurrentSlide++;	
	}

	if(m_nCurrentSlide > 5)
	{
		CStateSystem::GetInstance()->ChangeBaseState(CWinState::GetInstance());
	}
}

void CEndingState::Render(void)
{
	m_SlideShapes[m_nCurrentSlide].GetFrame().GetLocalMatrix() = m_mWorld;
}

bool CEndingState::Exit(void)
{
	return false;
}

CEndingState* CEndingState::GetInstance(void)
{
	static CEndingState instance;
	return &instance;
}