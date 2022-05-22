#include "CStateMachine.h"
#include <stdlib.h>

CStateSystem::CStateSystem()
{
	m_bRemoveFlag = false;
	m_bUpdateUniqueOnlyFlag = false;
	m_nRemoveNum = -1;
}

CStateSystem* CStateSystem::GetInstance()
{
	static CStateSystem pInstance;

	return &pInstance;
}

void CStateSystem::Initialize(IGameState* pNewState)
{
	AddState(pNewState);
}

void CStateSystem::Shutdown()
{
	RemoveAllStates();
	AddState(NULL);
}

void CStateSystem::AddState(IGameState* pNewState)
{
	if(pNewState)
	{
		pNewState->Enter();
		vStates.push_back(pNewState);
	}
}

void CStateSystem::AddStateAtIndex(int nIndex, IGameState *pNewState) 
{
	if ( nIndex > 0 && nIndex < (int) vStates.size() )
	{
		if(pNewState)
		{
			pNewState->Enter( );
			vStates.insert(vStates.begin( ) + nIndex, pNewState );
		}
	}
}

void CStateSystem::RemoveNumStates(int nIndex)
{
	if(nIndex > 0 && nIndex <= (int) vStates.size())
	{
		while(nIndex)
		{
			vStates.back()->Exit();
			vStates.pop_back();
			--nIndex;
		}
	}

	while( vWaitingStates.size() > 0)
	{
		vWaitingStates.front()->Enter();
		vStates.push_back(vWaitingStates.front());
		vWaitingStates.erase( vWaitingStates.begin() );
	}

	SetUpdateUniqueFlag(false);
}

void CStateSystem::ChangeBaseState(IGameState* pNewState)
{
	RemoveAllStates();
	AddState(pNewState);
}

void CStateSystem::RemoveState(IGameState *pState)
{
	for ( unsigned int i = 0; i < vStates.size(); ++i )
	{
		if ( vStates[i] == pState )
		{
			vStates.erase( vStates.begin( ) + i );
			break;
		}
	}
}

bool CStateSystem::RemoveTopState()
{
	bool value = false;

	if(vStates.size())
	{
		if(vStates.back())
			value = vStates.back()->Exit();

		vStates.pop_back();
	}

	while ( vWaitingStates.size( ) > 0 )
	{
		vWaitingStates.front()->Enter( );
		vStates.push_back(vWaitingStates.front());
		vWaitingStates.erase( vWaitingStates.begin() );
	}
	return value;
}

bool CStateSystem::RemoveAllStates()
{
	bool value = false;

	//	Loop through all of the states
	while(vStates.size())
	{
		//	Call the states exit
		if(vStates.back())
			value = vStates.back()->Exit();

		//	Pop it off of the stack
		vStates.pop_back();
	}

	return value;
}

bool CStateSystem::Input(void)
{
	if(vStates.size())
	{
		int i = vStates.size() - 1;

		while( i >= 0 && vStates[i]->GetUnique())
		{
			if( !vStates[i]->Input() )
				return false;
			i--;
		}

		if ( !UpdateUniqueFlagOn( ) )
		{
			if( i >= 0 )
			{
				if( !vStates[i]->Input() )
					return false;
			}
		}
	}

	return true;
}

void CStateSystem::Update(float fElapsedTime)
{
	if(vStates.size())
	{
		int i = vStates.size() - 1;
		while(i >= 0 && vStates[i]->GetUnique())
		{
			vStates[i]->Update(fElapsedTime);
			--i;
		}

		if ( !UpdateUniqueFlagOn( ) )
		{
			if( i >= 0 )
				vStates[i]->Update(fElapsedTime);
		}
	}

	if ( vStates.size() )
	{
		if ( m_bRemoveFlag )
		{
			RemoveNumStates(m_nRemoveNum);
			
			// Set values to default
			m_bRemoveFlag = false;
			m_nRemoveNum = -1;
		}
	}

	if ( vWaitingStates.empty( ) )
		SetUpdateUniqueFlag(false);
}

void CStateSystem::Render(void)
{
	for(unsigned int i = 0; i < vStates.size(); i++)
	{
		if(vStates[i])
		{
			vStates[i]->Render();
		}
	}
}