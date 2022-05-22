#ifndef _CSTATEMACHINE_H_
#define _CSTATEMACHINE_H_

#include "../States/IGameState.h"

#include <vector>
using std::vector;
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Class Name: CStateSystem
//
// Author: Nicholas "Mark 3" Guzman
//
// Last Mod: NONE
//
// Description: Allow for stacked states and handles all their updates and renders
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class CStateSystem
{
private: 
	bool m_bRemoveFlag;
	int m_nRemoveNum;
	bool m_bUpdateUniqueOnlyFlag;

	vector<IGameState*> vStates;
	vector<IGameState*> vWaitingStates;

	CStateSystem(void);
	CStateSystem(const CStateSystem& sys);
	CStateSystem& operator=(const CStateSystem& sys);
	~CStateSystem(void){}

public:
///////////////////////////////////////////////////////////////////////
 // Function Name: GetInstance
 //
 // Author: Nicholas "Mark 3" Guzman
 //
 // In: NONE
 //     
 // Out: NONE
 //
 // Return: CStateSystem*
 //
 // Description: Returns an instance of the CStateSystem
 ///////////////////////////////////////////////////////////////////////
	static CStateSystem* GetInstance(void);

///////////////////////////////////////////////////////////////////////
 // Function Name: AddState
 //
 // Author: Nicholas "Mark 3" Guzman
 //
 // In: IGameState* pNewState
 //     
 // Out: NONE
 //
 // Return: NONE
 //
 // Description: Adds a state to the top of the CStateSystem
 ///////////////////////////////////////////////////////////////////////
	void AddState(IGameState* pNewState);

///////////////////////////////////////////////////////////////////////
 // Function Name: AddStateAtIndex
 //
 // Author: Nicholas "Mark 3" Guzman
 //
 // In: int
 //     
 // Out:
 //
 // Return:
 //
 // Description: 
 ///////////////////////////////////////////////////////////////////////
	void AddStateAtIndex(int nIndex, IGameState* pNewState);

	void ChangeBaseState(IGameState* pNewState);

	bool RemoveTopState(void);

	void RemoveState(IGameState* pState);

	void RemoveNumStates(int nIndex);

	bool RemoveAllStates();

	void Initialize(IGameState* pFirstState);

	bool Input(void);

	void Update(float fElapsedTime);

	void Render(void);

	void Shutdown();

	//////////////////////////////////////////////////////////////////////////////////////////
	// Accessors and Mutators //
	//////////////////////////////////////////////////////////////////////////////////////////
	void SetRemoveFlag( bool value ){ m_bRemoveFlag = value; }
	void SetUpdateUniqueFlag( bool value ){ m_bUpdateUniqueOnlyFlag = value; }
	void SetRemoveNum( int value ){ m_nRemoveNum = value; }
	//////////////////////////////////////////////////////////////////////////////////////////
	bool	RemoveFlagOn( void ) const { return m_bRemoveFlag; }
	bool	UpdateUniqueFlagOn(void ) const { return m_bUpdateUniqueOnlyFlag; }
	int		GetRemoveNum( void ) const { return m_nRemoveNum; }

};



#endif