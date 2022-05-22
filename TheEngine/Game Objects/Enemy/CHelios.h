#ifndef CHelios_h__
#define CHelios_h__

#include "CEnemy.h"
#include "../../AI System/CAIStateMachine.h"

class CHelios: public CEnemy
{
private:

	CAIStateMachine m_mStateMachine;
	bool m_bCounted; //This bool makes sure that this enemy is not updated for score multiple times.
	bool m_bFlyingAway; 
	bool m_bLeft;
public:

	CHelios();
	~CHelios();

	void SetIsFlyingAway(bool bIsFlyingAway)	{m_bFlyingAway = bIsFlyingAway; }
	void SetIsLeft(bool bLeft)					{m_bLeft = bLeft; }
	bool GetIsFlyingAway(void)	const			{return m_bFlyingAway; }
	bool GetIsLeft(void)		const			{return m_bLeft; }

	void Init(D3DXVECTOR3 vPosition,  bool bDummyMode = false, int nHealth = 5);
	void Input();
	void Update(float fElapsedTime);
	void Render();

	bool CheckCollision(CBase *pBase);
	void CollisionReact(CBase *pBase);

	void Shutdown(void);
};
#endif // CHelios_h__