#include "CIntroState.h"
#include "CStateMachine.h"
#include "CLoadState.h"
#include "../Input/CInput.h"
#include "../Rendering/CTextureManager.h"

CIntroState::CIntroState(void)
{

}

CIntroState::~CIntroState(void)
{

}

void CIntroState::Enter(void)
{
	m_nImages[0] = CTextureManager::GetInstance()->LoadTexture("Resources/Textures/GPGlogo.tga");
	m_nImages[1] = CTextureManager::GetInstance()->LoadTexture("Resources/Textures/threewaytiestuds.png");
	m_nImages[2] = CTextureManager::GetInstance()->LoadTexture("Resources/Textures/nslogo.tga");
	m_nImages[3] = CTextureManager::GetInstance()->LoadTexture("Resources/Textures/1.png");
	m_nImages[4] = CTextureManager::GetInstance()->LoadTexture("Resources/Textures/IntroSplashScreen.png");
	m_bDown = false;
	m_nCurrent = 0;
}

bool CIntroState::Input(void)
{

	return true;
}

void CIntroState::Update(float fElapsedTime)
{

	if(m_fFadeInTimer < 2.0 && m_bDown == false)
	{
		m_fFadeInTimer += fElapsedTime;
	}
	else if(m_fFadeInTimer > 0.0 && m_bDown == true)
	{
		m_fFadeInTimer -= fElapsedTime;
	}

	if(m_fFadeInTimer >= 2.0)
	{
		m_bDown = true;
	}

	if(m_fFadeInTimer <= 0 && m_bDown == true)
	{
		m_nCurrent++;
		m_bDown = false;
	}

	if(m_nCurrent == 5)
	{
		CStateSystem::GetInstance()->ChangeBaseState(CLoadState::GetInstance());
	}
}

void CIntroState::Render(void)
{
	switch(m_nCurrent)
	{
	case 0:
		CTextureManager::GetInstance()->Draw(m_nImages[m_nCurrent],0,0,0.5,0.75,NULL,0,0,0,D3DXCOLOR(1.0,1.0,1.0,m_fFadeInTimer));
		break;
	case 1:
		CTextureManager::GetInstance()->Draw(m_nImages[m_nCurrent],0,0,1,0.75,NULL,0,0,0,D3DXCOLOR(1.0,1.0,1.0,m_fFadeInTimer));
		break;
	case 2:
		CTextureManager::GetInstance()->Draw(m_nImages[m_nCurrent],0,0,1,1,NULL,0,0,0,D3DXCOLOR(1.0,1.0,1.0,m_fFadeInTimer));
		break;
	case 3:
		CTextureManager::GetInstance()->Draw(m_nImages[m_nCurrent],0,0,1,0.85f);
		break;
	case 4:
		CTextureManager::GetInstance()->Draw(m_nImages[m_nCurrent],0,0,1.0,0.75f,NULL,0,0,0,D3DXCOLOR(1.0,1.0,1.0,m_fFadeInTimer));
		break;
	}
}

bool CIntroState::Exit(void)
{
	return false;
}

CIntroState* CIntroState::GetInstance(void)
{
	static CIntroState instance;
	return &instance;
}