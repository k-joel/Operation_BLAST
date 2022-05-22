#ifndef CLIGHTNINGBULLET_H
#define CLIGHTNINGBULLET_H

#include "../CBase.h"

enum lightningAltState { BASE_BOLT = 0, FIRST_SPLIT_BOLT, FINAL_SPLIT_BOLT };

class CLightningBullet : public CBase
{
private:
	CBase* m_pOwner;

	CBase* m_pNextEnemy; //Next enemy to jump to
	CBase* m_pPreviousEnemy; //Previously hit enemy
	CBase* m_pCurrentEnemy; //Current target I'm on

	float m_fDespawnTimer;
	float m_fMaxLife;
	float m_fDamage;
	float m_fShockTimer; //How long to sit at the enemy and shock it
	float m_fArcDistance; //How far the projectile is willing to arc
	int m_nMaxJumps;
	int m_nJumpCount;

	bool m_bHit; //If I hit an enemy

public:
	CLightningBullet();
	CLightningBullet(const CLightningBullet& blt);
	~CLightningBullet();

	void Initialize();
	void Update(float fDt);
	void Render(void);

	//Accessors
	CBase* GetOwner(void) { return m_pOwner;}
	float GetDamage(void) { return m_fDamage; }
	float GetMaxLife(void) { return m_fMaxLife; }
	float GetShockTimer(void) { return m_fShockTimer; }
	CBase* GetPreviousEnemy(void) { return m_pPreviousEnemy; }
	CBase* GetCurrentEnemy(void) { return m_pCurrentEnemy; }
	CBase* GetNextEnemy(void)	{ return m_pNextEnemy; }
	int GetMaxJumps(void) { return m_nMaxJumps; }
	int GetJumpCount(void) { return m_nJumpCount; }
	float GetArcDistance(void) { return m_fArcDistance; }
	bool GetHitTarget(void) { return m_bHit; }

	//Mutators
	void SetOwner(CBase* owner) { m_pOwner = owner; }
	void SetDamage(float damage) { m_fDamage = damage; }
	void SetMaxLife(float life) { m_fMaxLife = life; }
	void SetNextEnemy(CBase* next)			{ m_pNextEnemy = next; }
	void SetShockTimer(float timer) { m_fShockTimer = timer; }
	void SetPreviousHit(CBase* previous) { m_pPreviousEnemy = previous; }
	void SetCurrentEnemy(CBase* current) { m_pPreviousEnemy = m_pCurrentEnemy; m_pCurrentEnemy = current; }
	void SetMaxJumps(int jumps) { m_nMaxJumps = jumps; }
	void SetHitTarget(bool hit) { m_bHit = hit; }

	bool CheckCollision(CBase* pBase);
	void CollisionReact(CBase* pBase);

};

class CLightningAlternate : public CBase
{
private:
	CBase* m_pOwner;

	float m_fDespawnTimer;
	float m_fMaxLife;
	float m_fDamage;
	int m_nSplitState; //The state of the bolt (initial, first split, final)
	bool m_bSplit; //HEY HEY did I split?

	CBase* m_pPreviousEnemy;
	CBase* m_pCurrentEnemy;

public:
	void Initialize();
	CLightningAlternate();
	CLightningAlternate(const CLightningAlternate& blt);
	~CLightningAlternate();

	void Update(float fDt);
	void Render(void);

	//Accessors
	CBase* GetOwner(void) { return m_pOwner;}
	float GetDamage(void) { return m_fDamage; }
	float GetMaxLife(void) { return m_fMaxLife; }
	int GetSplitState(void) { return m_nSplitState; }
	CBase* GetPreviousEnemy(void) { return m_pPreviousEnemy; }
	CBase* GetCurrentEnemy(void) { return m_pCurrentEnemy; }
	bool GetIsSplitting(void) { return m_bSplit; }

	//Mutators
	void SetOwner(CBase* owner) { m_pOwner = owner; }
	void SetDamage(float damage) { m_fDamage = damage; }
	void SetMaxLife(float life) { m_fMaxLife = life; }
	void SetSplitState(int state) { m_nSplitState = state; }
	void SetCurrentEnemy(CBase* enemy) {m_pPreviousEnemy = m_pCurrentEnemy; m_pCurrentEnemy = enemy; }
	void SetPreviousEnemy(CBase* enemy) { m_pPreviousEnemy = enemy; }
	void SetIsSplitting(bool split) { m_bSplit = split; }

	bool CheckCollision(CBase* pBase);
	void CollisionReact(CBase* pBase);

};

class CLightningUltimate : public CBase
{
private:
	CBase* m_pOwner;

	float m_fDespawnTimer;
	float m_fMaxLife;
	float m_fDamage;

public:
	CLightningUltimate();
	CLightningUltimate(const CLightningUltimate& blt);
	~CLightningUltimate();

	void Update(float fDt);
	void Render(void);

	//Accessors
	CBase* GetOwner(void) { return m_pOwner;}
	float GetDamage(void) { return m_fDamage; }
	float GetMaxLife(void) { return m_fMaxLife; }

	//Mutators
	void SetOwner(CBase* owner) { m_pOwner = owner; }
	void SetDamage(float damage) { m_fDamage = damage; }
	void SetMaxLife(float life) { m_fMaxLife = life; }

	bool CheckCollision(CBase* pBase);
	void CollisionReact(CBase* pBase);

};


#endif