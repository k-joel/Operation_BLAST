#ifndef CSmartLaserBullet_h__
#define CSmartLaserBullet_h__

#include "../CBase.h"
#include "../CPlayer.h"
#include "../../Mesh/animation.h"
#include "../../Mesh/CModel.h"

class CSmartLaserBullet : public CBase
{
private:
	CBase* m_pOwner;

	int m_nFlatID;

	float despawnTimer;

public:
	//Trilogy of Canasta
	CSmartLaserBullet();
	CSmartLaserBullet(const CSmartLaserBullet& blt);
	~CSmartLaserBullet();

	void Update(float fDt);
	void Render(void);

	//Mutator
	void SetOwner(CBase* owner) { m_pOwner = owner; }

	//Accessorizer
	CBase* GetOwner(void) { return m_pOwner; }

	bool CheckCollision(CBase* pBase);
	void CollisionReact(CBase* pBase);
};

#endif // CSmartLaserBullet_h__