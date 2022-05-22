#ifndef _CMACHINEGUNBULLET_H
#define _CMACHINEGUNBULLET_H

#include "../CBase.h"
#include "../CPlayer.h"
#include "../../Mesh/animation.h"
#include "../../Mesh/CModel.h"

class CMachineGunBullet : public CBase
{
private:
	CBase* m_pOwner;

	int m_nFlatID;

	float m_fDespawnTimer;
	float m_fMaxLife;
	float m_fDamage;
public:
	//Trilogy of Canasta
	CMachineGunBullet();
	CMachineGunBullet(const CMachineGunBullet& blt);
	~CMachineGunBullet();

	void Update(float fDt);
	void Render(void);

	//Mutator
	void SetOwner(CBase* owner) { m_pOwner = owner; }
	void SetDamage(float damage) {m_fDamage = damage; }
	void SetMaxLife(float life) { m_fMaxLife = life; }

	//Accessorizer
	CBase* GetOwner(void) { return m_pOwner; }
	float GetDamage(void) { return m_fDamage; }
	float GetMaxLife(void) { return m_fMaxLife; }

	bool CheckCollision(CBase* pBase);
	void CollisionReact(CBase* pBase);
};

#endif