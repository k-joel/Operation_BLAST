#ifndef CFizzle_h__
#define CFizzle_h__

#include "../CCharacter.h"
#include "../../Rendering/CRenderShape.h"
#include "../../Collision/CCapsule.h"

class CFizzle : public CBase
{
private:
	CCharacter* m_pOwner;

	CRenderShape m_shape;
	CCapsule m_Capsule;

	//CBase* m_pExtraPiece;

public:
	CFizzle();
	~CFizzle();

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

#endif // CFizzle_h__


