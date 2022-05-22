#ifndef _CMOTORCYCLE_
#define _CMOTORCYCLE_

#include "../Game Objects/CBase.h"
#include "../Game Objects/CPlayer.h"

class CMotorcycleObject : public CBase
{
public:
	CPlayer* pParent;
	bool m_resetRevSound;

	CMotorcycleObject();
	~CMotorcycleObject();

	void Update(float fElapsedTime);

	void Render();

	bool CheckCollision(CBase* pBase);

	void CollisionReact(CBase* pBase);
};
#endif //_CMOTORCYCLE_