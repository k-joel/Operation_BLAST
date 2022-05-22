#ifndef _CGOAL_
#define _CGOAL_

#include "../Game Objects/CBase.h"
#include "../Game Objects/CPlayer.h"

class CGoalObject : public CBase
{
public:
	unsigned int m_goalID;
	unsigned int m_targetID;

	CGoalObject();
	~CGoalObject();

	void Update(float fElapsedTime);

	void Render();

	bool CheckCollision(CBase* pBase);

	void CollisionReact(CBase* pBase);
};
#endif //_CGOAL_