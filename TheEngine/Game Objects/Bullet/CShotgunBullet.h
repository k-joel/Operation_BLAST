#ifndef _CSHOTGUNBULLET_H
#define _CSHOTGUNBULLET_H

#include "../CBase.h"
#include "../CPlayer.h"
#include "../../Mesh/animation.h"
#include "../../Mesh/CModel.h"
#include "../../FX/CParticleManager.h"
#include "../../LuaEngine/CLuaEngine.h"

class CShotgunBullet : public CBase
{
private:
	CBase* m_pOwner;

	int m_nFlatID;

	float m_fMaxLife;
	float m_fDespawnTimer;
	float m_fDamage;

public:
	//Trilogy of Canasta
	CShotgunBullet();
	CShotgunBullet(const CShotgunBullet& blt);
	~CShotgunBullet();

	void Update(float fDt);
	void Render(void);

	//Mutator
	void SetOwner(CBase* owner) { m_pOwner = owner; }
	void SetDamage(float damage) {m_fDamage = damage; }
	void SetMaxLife(float life) {m_fMaxLife = life;}

	//Accessorizer
	CBase* GetOwner(void) { return m_pOwner; }
	float GetDamage(void) { return m_fDamage; }
	float GetMaxLife(void) { return m_fMaxLife; }

	bool CheckCollision(CBase* pBase);
	void CollisionReact(CBase* pBase);
};

class CShotgunAlternate : public CBase
{
private:
	CBase* m_pOwner;
	CParticleEffect* m_pBoreEffect;

	float m_fDespawnTimer;
	float m_fMaxLife;
	float m_fDamage;
	float m_fBoreTimer; //How long the bullet will remaing "boring" through the enemies.
	float m_fRotation;

	bool m_bBore; //Am I boring through an enemy?
	bool m_bPrevBore;

	D3DXVECTOR3 m_vBoreVel; //Velocity while boring through enemies
	D3DXVECTOR3 m_vNormalVel; //Normal velocity

public:
	//Trilogy of Canasta
	CShotgunAlternate();
	CShotgunAlternate(const CShotgunAlternate& blt);
	~CShotgunAlternate();

	void Update(float fDt);
	void Render(void);

	//Mutator
	void SetOwner(CBase* owner) { m_pOwner = owner; }
	void SetDamage(float damage) {m_fDamage = damage; }
	void SetMaxLife(float life) { m_fMaxLife = life; }
	void SetBore(bool bore) { m_bBore = bore; m_vVelocity = m_vBoreVel; m_fBoreTimer = 0.5f;}
	void SetBoreTimer(float time) { m_fBoreTimer = time; }
	void SetNormalVel(D3DXVECTOR3 vel) { m_vNormalVel = vel; }
	void SetBoreVel(D3DXVECTOR3 vel) { m_vBoreVel = vel; }
	void SetBoreEffect(CParticleEffect* effect) {m_pBoreEffect = effect;}
	void SetRotation(float fRot) {m_fRotation = fRot;}
	void Shutdown();

	//Accessorizer
	CBase* GetOwner(void) { return m_pOwner; }
	float GetDamage(void) { return m_fDamage; }
	float GetMaxLife(void) { return m_fMaxLife; }
	bool GetBore(void) { return m_bBore; }
	float GetBoreTimer(void) { return m_fBoreTimer; }
	float GetRotation(void) { return m_fRotation; }

	bool CheckCollision(CBase* pBase);
	void CollisionReact(CBase* pBase);
};

class CShotgunUltimate : public CBase
{
private:
	CBase* m_pOwner;
	lua_State* luaState;

	int m_nBounceCount; //Timer to make the shot bounce again.  Remove this as soon as real bounce becomes available.

	D3DXVECTOR3 m_previousVel; //Previous Velocity before I hit the ground.
	D3DXVECTOR3 m_previousPos; //Previous position before I hit the ground.

	void ChangeBounce(D3DXVECTOR3& currentVel);

	CParticleEffect* m_pEffect;

	float m_fDamage;
public:
	float m_fSpawnMore;

	//Trilogy of Canasta
	CShotgunUltimate();
	CShotgunUltimate(const CShotgunUltimate& blt);
	~CShotgunUltimate();

	void Update(float fDt);
	void Render(void);

	 void Initialize();

	//Mutator
	void SetOwner(CBase* owner) { m_pOwner = owner; }
	void SetBounceCount(int count) { m_nBounceCount = count; }
	void SetPreviousVel(D3DXVECTOR3 vel) { m_previousVel = vel; }
	void SetPreviousPos(D3DXVECTOR3 pos) { m_previousPos = pos; }
	void SetDamage(float damage) { m_fDamage = damage; }
	void SetEffect(CParticleEffect* worthless) { m_pEffect = worthless; }

	//Accessorizer
	CBase* GetOwner(void) { return m_pOwner; }
	int GetBounceCount(void) { return m_nBounceCount; }
	D3DXVECTOR3 GetPreviousVel(void) { return m_previousVel; }
	D3DXVECTOR3 GetPreviousPos(void) { return m_previousPos; }
	float GetDamage(void) { return m_fDamage; }

	bool CheckCollision(CBase* pBase);
	void CollisionReact(CBase* pBase);
	void Shutdown();
};

#endif