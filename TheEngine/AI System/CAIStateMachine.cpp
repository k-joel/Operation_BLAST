#include "CAIStateMachine.h"
#include "CState.h"
#include "../Game Objects/CCharacter.h"
#include "../Input/CInput.h"
#include <math.h>

CAIStateMachine::CAIStateMachine()
{
}
CAIStateMachine::~CAIStateMachine()
{

}
void	CAIStateMachine::Initialize(CCharacter *pNPC,  list<CBase*> pTargets, bool bDummyMode)
{
	m_pNPC = pNPC;
	Shutdown();

	switch(m_pNPC->GetType())
	{
	case OBJ_ASSAULTER:
		{
			CDeathState*			death					= new CDeathState(pNPC);
			CJumpState*				jump					= new CJumpState(pNPC,pTargets);
			CSelectState*			select					= new CSelectState(pNPC,pTargets);
			CRunState*				run						= new CRunState(pNPC,pTargets);
			CShootState*			shoot					= new CShootState(pNPC);
			CArrivalState*			arrival					= new CArrivalState(pNPC);

			m_lAssaulterStates.push_back(death);
			m_lAssaulterStates.push_back(jump);
			m_lAssaulterStates.push_back(select);
			m_lAssaulterStates.push_back(run);
			m_lAssaulterStates.push_back(shoot);
			m_lAssaulterStates.push_back(arrival);

			m_pStateCur		= arrival;
			m_pStateNext	= NULL;
			m_pStateCur->Enter();
		}
		break;
	case  OBJ_STRIKER:
		{
			CDeathState*			death					= new CDeathState(pNPC);
			CJumpState*				jump					= new CJumpState(pNPC,pTargets);
			CSelectState*			select					= new CSelectState(pNPC,pTargets);
			CRunState*				run						= new CRunState(pNPC,pTargets);
			CArrivalState*			arrival					= new CArrivalState(pNPC);

			m_lStrikerStates.push_back(death);
			m_lStrikerStates.push_back(jump);
			m_lStrikerStates.push_back(select);
			m_lStrikerStates.push_back(run);
			m_lStrikerStates.push_back(arrival);

			m_pStateCur		= arrival;
			m_pStateNext	= NULL;
			m_pStateCur->Enter();
		}
		break;
	case OBJ_HELIOS:
		{
			CDeathState*			death					= new CDeathState(pNPC);
			CShootState*			shoot					= new CShootState(pNPC);
			CFlyState*				fly						= new CFlyState(pNPC,pTargets);

			m_lHeliosStates.push_back(death);
			m_lHeliosStates.push_back(fly);
			m_lHeliosStates.push_back(shoot);
			//m_lStrikerStates.push_back(arrival);

			m_pStateCur		= fly;
			m_pStateNext	= NULL;
			m_pStateCur->Enter();
		}
		break;
	case OBJ_ULTIMUS:
		{

			CDeathState*			death					= new CDeathState(pNPC);
			CSpawnState*			spawner					= new CSpawnState(pNPC,pTargets);
			CRocketState*			rockets					= new CRocketState(pNPC,pTargets);
			CLargeLaserState*		largelaser				= new CLargeLaserState(pNPC);
			CSmallLaserState*		smalllaser				= new CSmallLaserState(pNPC);

			m_states.push_back(death);
			m_states.push_back(spawner);
			m_states.push_back(rockets);
			m_states.push_back(largelaser);
			m_states.push_back(smalllaser);
			SetStates(OBJ_ULTIMUS,ESPAWN,E_GOTOLLASERATTACK,			largelaser);
			SetStates(OBJ_ULTIMUS,ELARGELASER,E_GOTOSLASERATTACK,		smalllaser);
			SetStates(OBJ_ULTIMUS,ESMALLLASER,E_GOTOROCKETATTACK,		rockets);
			SetStates(OBJ_ULTIMUS,EROCKET,E_GOTOSPAWN,					spawner);
			SetStates(OBJ_ULTIMUS,ESPAWN,E_GOTODEATH,			death);
			SetStates(OBJ_ULTIMUS,ELARGELASER,E_GOTODEATH,		death);
			SetStates(OBJ_ULTIMUS,ESMALLLASER,E_GOTODEATH,		death);
			SetStates(OBJ_ULTIMUS,EROCKET,E_GOTODEATH,			death);

			m_pStateCur		= spawner;
			m_pStateNext	= NULL;
			m_pStateCur->Enter();
		}
		break;
	}
}

void	CAIStateMachine::Shutdown()
{
	for(unsigned int i(0); i < m_states.size();i++)
		delete m_states[i];
	m_states.clear();

	for(list<CState*>::iterator i = m_lCoopStates.begin(); i  != m_lCoopStates.end(); )
	{
		delete (*i);
		i = m_lCoopStates.erase(i);
	}
	m_lCoopStates.clear();

	for(list<CState*>::iterator i = m_lAssaulterStates.begin(); i  != m_lAssaulterStates.end(); )
	{
		delete (*i);
		i = m_lAssaulterStates.erase(i);
	}
	m_lAssaulterStates.clear();

	for(list<CState*>::iterator i = m_lStrikerStates.begin(); i  != m_lStrikerStates.end(); )
	{
		delete (*i);
		i = m_lStrikerStates.erase(i);
	}
	m_lStrikerStates.clear();

	for(list<CState*>::iterator i = m_lHeliosStates.begin(); i  != m_lHeliosStates.end(); )
	{
		delete (*i);
		i = m_lHeliosStates.erase(i);
	}
	m_lCoopStates.clear();
}
void	CAIStateMachine::Transition(int nEventID)
{
	if (m_mTable[m_pNPC->GetType()][m_pStateCur->GetId()][nEventID])
	{
		m_pStateNext = m_mTable[m_pNPC->GetType()][m_pStateCur->GetId()][nEventID];
		m_pStateNext->Enter();
		m_pStateCur->Exit();
		m_pStateCur = m_pStateNext;
	}
}
void	CAIStateMachine::Update(float dt)
{
	if(m_pNPC->GetType() != OBJ_ULTIMUS)
	{
		m_pStateNext = SelectState();
		if(m_pStateNext && m_pStateNext != m_pStateCur)
		{
			m_pStateNext->Enter();
			m_pStateCur->Exit();
			m_pStateCur = m_pStateNext;
		}
		else
			m_pStateCur->Update(dt);
	}
	else
	{
		int nResult = m_pStateCur->Update(dt);
		if( nResult != E_STAYINSTATE)
			Transition(nResult);
	}


}
void	CAIStateMachine::SetStates(int nAIType,int nStateID,int nEventID, CState* cResultState)
{
	//If you are this AITYPE in this STATEID and this EVENT triggers then move to this STATE
	m_mTable[nAIType][nStateID][nEventID] = cResultState;
}
CState*	CAIStateMachine::SelectState()
{			
	CState* pBest = NULL;
	float fBestUtil = 0.0f;

	switch(m_pNPC->GetType())
	{
	case OBJ_BARRON:
	case OBJ_CLAIRE:
		{
			for(list<CState*>::iterator itr = m_lCoopStates.begin(); itr != m_lCoopStates.end(); ++itr)
			{
				float fUtil = (*itr)->GetUtility();
				if(fUtil > fBestUtil)
				{
					pBest = *itr;
					fBestUtil = fUtil;
				}
			}
			return pBest;
		}
		break;
	case OBJ_ASSAULTER:
		{
			for(list<CState*>::iterator itr = m_lAssaulterStates.begin(); itr != m_lAssaulterStates.end(); ++itr)
			{
				float fUtil = (*itr)->GetUtility();
				if(fUtil > fBestUtil)
				{
					pBest = *itr;
					fBestUtil = fUtil;
				}
			}
			return pBest;
		}
		break;
	case OBJ_STRIKER:
		{
			for(list<CState*>::iterator itr = m_lStrikerStates.begin(); itr != m_lStrikerStates.end(); ++itr)
			{
				float fUtil = (*itr)->GetUtility();
				if(fUtil > fBestUtil)
				{
					pBest = *itr;
					fBestUtil = fUtil;
				}
			}
			return pBest;
		}
		break;
	case OBJ_HELIOS:
		{
			for(list<CState*>::iterator itr = m_lHeliosStates.begin(); itr != m_lHeliosStates.end(); ++itr)
			{
				float fUtil = (*itr)->GetUtility();
				if(fUtil > fBestUtil)
				{
					pBest = *itr;
					fBestUtil = fUtil;
				}
			}
			return pBest;
		}
		break;
	}
	return pBest;

}

