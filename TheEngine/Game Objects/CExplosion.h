#ifndef _CEXPLOSION_
#define _CEXPLOSION_

#include "../Game Objects/CBase.h"

class CExplosionObject : public CBase
{
	CBase* m_creator;
	float m_lifeTime;
public:

	void SetupExplosion(CBase* creator, float lifeTime);

	CExplosionObject();
	~CExplosionObject();

	void Update(float fElapsedTime);

	void Render();

	bool CheckCollision(CBase* pBase);

	void CollisionReact(CBase* pBase);
};
#endif //_CEXPLOSION_