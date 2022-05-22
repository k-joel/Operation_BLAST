#ifndef CLaserBullet_h__
#define CLaserBullet_h__

#include "../CBase.h"
#include "../CPlayer.h"
#include "../../Mesh/animation.h"
#include "../../Mesh/CModel.h"

#include "../../fx/CParticleManager.h"

class CLaserBullet : public CBase
{
private:
	CBase* m_pOwner;

	int m_nFlatID;
	LPD3DXMESH m_BoxMesh;
	D3DXMATRIX m_BoxMat;

	float despawnTimer;

public:
	//Trilogy of Canasta
	CLaserBullet();
	CLaserBullet(const CLaserBullet& blt);
	~CLaserBullet();

	void Update(float fDt);
	void Render(void);
	
	void Initialize();

	//Mutator
	void SetOwner(CBase* owner) { m_pOwner = owner; }

	//Accessorizer
	CBase* GetOwner(void) { return m_pOwner; }

	bool CheckCollision(CBase* pBase);
	void CollisionReact(CBase* pBase);
};

#endif // CLaserBullet_h__