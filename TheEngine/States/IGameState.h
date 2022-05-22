#ifndef _IGAMESTATE_H_
#define _IGAMESTATE_H_

#include "../Game Objects/CButtonGui.h"

class IGameState
{
private:
	bool m_bUnique;
	
public:
	CButtonGui m_Buttons;
	virtual ~IGameState(void) = 0 {}

	// Enters game state
	virtual void Enter(void) = 0;

	virtual bool Input(void) = 0;

	virtual void Update(float fElapsedTime) = 0;

	virtual void Render(void) = 0;

	virtual bool Exit(void) = 0;

	bool GetUnique()	{	return m_bUnique; }
	void SetUnique( bool b ) { m_bUnique = b; }
};



#endif