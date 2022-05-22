
#ifndef CChargedLaser_h__
#define CChargedLaser_h__

#include "../CCharacter.h"
#include "../../Rendering/CRenderShape.h"
#include "../../Collision/CCapsule.h"

class CChargedLaser : public CBase
{
private:
	CCharacter* m_pOwner;

	CRenderShape m_shape;
	CCapsule m_Capsule;

	//CBase* m_pExtraPiece;

public:
	CChargedLaser();
	~CChargedLaser();

	void Update(float fDt);
	void Render(void);
	void Initialize();

	//Accessors
	CCharacter* GetOwner(void) { return m_pOwner; }

	//Mutators
	void SetOwner(CCharacter* owner) { m_pOwner = owner; }

	bool CheckCollision(CBase *pBase);
	void CollisionReact(CBase *pBase);
};


#endif // CChargedLaser_h__
