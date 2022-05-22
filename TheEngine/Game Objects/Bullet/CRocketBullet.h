#ifndef CRocketBullet_h__
#define CRocketBullet_h__

#include "../CBase.h"
#include "../CPlayer.h"
#include "../../Mesh/animation.h"
#include "../../Mesh/CModel.h"
#include "../../FX/CParticleManager.h"

class CRocketBullet : public CBase
{
private:
	CBase* m_pOwner;

	int m_nFlatID;

	float despawnTimer;
	float m_fMaxLife;
	float m_fDamage;

	int m_nParticleID;
	bool m_bCreated;

public:
	//Trilogy of Canasta
	CRocketBullet();
	CRocketBullet(const CRocketBullet& blt);
	~CRocketBullet();

	void Update(float fDt);
	void Render(void);

	//Mutator
	void SetOwner(CBase* owner) { m_pOwner = owner; }
	void SetMaxLife(float fLife)	{ m_fMaxLife = fLife; }
	void SetDamage(float damage)	{ m_fDamage = damage; }

	//Accessorizer
	CBase* GetOwner(void) { return m_pOwner; }
	float GetDamage(void) { return m_fDamage; }

	bool CheckCollision(CBase* pBase);
	void CollisionReact(CBase* pBase);
	void Shutdown();
};

class CRocketAlternate : public CBase
{
private:
	CBase* m_pOwner;

	float m_fDespawnTimer;
	float m_fMaxLife;
	float m_fDamage;

	int m_nParticleID;

	bool m_bCreated;
public:
	//Trilogy of Canasta
	CRocketAlternate();
	CRocketAlternate(const CRocketAlternate& blt);
	~CRocketAlternate();

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
	void Shutdown();
};

class CRocketUltimate : public CBase
{
private:
	CBase* m_pOwner;

	float m_fDespawnTimer;
	float m_fMaxLife;
	float m_fDamage;

	int m_nParticleID;
	bool m_bCreated;
public:
	//Trilogy of Canasta
	CRocketUltimate();
	CRocketUltimate(const CRocketUltimate& blt);
	~CRocketUltimate();

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
	void Shutdown();
};

#endif // CRocketBullet_h__