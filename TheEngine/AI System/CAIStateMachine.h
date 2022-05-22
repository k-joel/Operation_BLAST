#ifndef CAIStateMachine_h__
#define CAIStateMachine_h__

#include "../Game Objects/CBase.h"
#include "../Game Objects/CCharacter.h"
#include "../Game Objects/Enemy/CEnemy.h"

#include <map>
#include <vector>
#include <string>
using namespace std;

class CState;
class CAIStateMachine
{
	//AITYPE, STATE ID, and EVENT ID
	map< int, map<int, map<int,CState*> > > m_mTable;
	CState *m_pStateCur, *m_pStateNext;
	CCharacter* m_pNPC;
	// Containers for holding each Type of Enemies
	vector<CState*> m_states;
	list<CState*> m_lCoopStates;
	list<CState*> m_lAssaulterStates;
	list<CState*> m_lHeliosStates;
	list<CState*> m_lStrikerStates;

	void Transition(int nEventID);

public:

	CAIStateMachine();
	virtual ~CAIStateMachine();

	/******************************************************************************/
	//	Initialize	-	First call to set up the states for the respecting enemy
	//
	/******************************************************************************/
	void	Initialize(CCharacter *pNPC, list<CBase*> pTargets, bool bDummyMode = false);

	/******************************************************************************/
	//	SetStates	-	Adds the state to the container for the enemy to 
	//					respond to an event that has happened.
	//
	/******************************************************************************/
	void	SetStates(int nAIType,int nStateID,int nEventID, CState* cResultState);

	/******************************************************************************/
	//	Shutdown	-	Empty and cleans all the states when the enemy is not active					
	//
	/******************************************************************************/
	void	Shutdown();

	/******************************************************************************/
	//	Update	-	Updates the current state with the SelectState call					
	//
	/******************************************************************************/
	void	Update(float dt);

	/******************************************************************************/
	//	GetState	-	Returns the current state 					
	//
	/******************************************************************************/
	CState* GetState() {return m_pStateCur; }

	/******************************************************************************/
	//	SelectState	-	Goes through the containers and calls GetUtility 
	//					and selects the state with the highest Utility
	//
	/******************************************************************************/
	CState* SelectState();

};

#endif // CAIStateMachine_h__