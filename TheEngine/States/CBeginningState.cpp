#include "CBeginningState.h"

CBeginningState::CBeginningState(void)
{

}

CBeginningState::~CBeginningState(void)
{

}

void CBeginningState::Enter(void)
{

}

bool CBeginningState::Input(void)
{
	return true;
}

void CBeginningState::Update(float fElapsedTime)
{

}

void CBeginningState::Render(void)
{

}

bool CBeginningState::Exit(void)
{
	return false;
}

CBeginningState* CBeginningState::GetInstance(void)
{
	static CBeginningState instance;
	return &instance;
}