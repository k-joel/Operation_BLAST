#ifndef _CFLAMETHROWERBULLET_H
#define _CFLAMETHROWERBULLET_H

#include "../CPlayer.h"
#include "../../Rendering/CRenderShape.h"
#include "../../FX/CParticleEffect.h"

// Normal Fire
class CFlamethrowerBullet : public CBase
{
private:
	CBase* m_pOwner;

	//CRenderShape m_shape;

	int m_fxID;
	int m_nAimDir; //Direction I'm firing in.  Use the enums

	float m_fMaxLife;
	float m_fDamage;
	float m_fDespawnTimer; //Timer counting up to despawn

	CParticleEffect* m_pEffect;

	bool m_bIsLeft; //Am I facing left?

	D3DXMATRIX m_mxFakeBoneMat;

public:
	CFlamethrowerBullet();
	~CFlamethrowerBullet();

	void Update(float fDt);
	void Render(void);

	void SetFXID(unsigned int id) { m_fxID = id; }

	//Accessors
	CBase* GetOwner(void) { return m_pOwner; }
	float GetDamage(void) { return m_fDamage; }
	float GetMaxLife(void) { return m_fMaxLife; }
	int GetAimDir(void) { return m_nAimDir; }
	bool GetIsLeft(void) { return m_bIsLeft; }

	//Mutators
	void SetOwner(CBase* owner) { m_pOwner = owner; }
	void SetDamage(float damage) { m_fDamage = damage; }
	void SetMaxLife(float candy) { m_fMaxLife = candy; }
	void SetParticleEffect(CParticleEffect* fx) { m_pEffect = fx; }
	void SetAimDir(int dir) { m_nAimDir = dir; }
	void SetIsLeft(bool left) { m_bIsLeft = left; }

	bool CheckCollision(CBase *pBase);
	void CollisionReact(CBase *pBase);

	void Initialize();

	void Shutdown();
};

// Alternate Fire
class CFlamethrowerAlternate : public CBase
{
private:
	CBase* m_pOwner;

	float m_fDespawnTimer;
	float m_fMaxLife;
	float m_fDamage;
	float m_fNapalmDamage;
	float m_fNapalmDuration;
	float m_fNapalmFrequency;

public:
	CFlamethrowerAlternate();
	CFlamethrowerAlternate(const CFlamethrowerAlternate& blt);
	~CFlamethrowerAlternate();

	void Initialize();
	void Update(float fDt);
	void Render(void);

	//Accessors
	CBase* GetOwner(void) { return m_pOwner;}
	float GetDamage(void) { return m_fDamage; }
	float GetMaxLife(void) { return m_fMaxLife; }
	float GetNapalmLife(void) { return m_fNapalmDuration; }
	float GetNapalmDamage(void) { return m_fNapalmDamage; }

	//Mutators
	void SetOwner(CBase* owner) { m_pOwner = owner; }
	void SetDamage(float damage) { m_fDamage = damage; }
	void SetMaxLife(float life) { m_fMaxLife = life; }
	void SetNapalmLife(float life) { m_fNapalmDuration = life; }
	void SetNapalmDamage(float damage) { m_fNapalmDamage = damage; }
	void SetNapalmFrequency(float freq) { m_fNapalmFrequency = freq; }

	bool CheckCollision(CBase* pBase);
	void CollisionReact(CBase* pBase);
};

// Ultimate
class CFlamethrowerUltimate	:	public CBase
{
private:
	CBase* m_pOwner;
	int m_nBounceCount;
	float m_fLifeTime;

	CParticleEffect* m_pEffect;

	int nAudioID;

	D3DXVECTOR3 m_previousVel;
	D3DXVECTOR3 m_previousPos;

	void ChangeBounce(D3DXVECTOR3& currentVel);

	float m_fDamage;
public:
	// Trillogy
	CFlamethrowerUltimate();
	CFlamethrowerUltimate(const CFlamethrowerUltimate& blt);
	~CFlamethrowerUltimate();

	void Update(float fDt);
	void Render(void);

	// Mutator
	void SetOwner(CBase* owner) { m_pOwner = owner; }
	void SetBounceCount(int count) { m_nBounceCount = count; }
	void SetPreviousVel(D3DXVECTOR3 vel) { m_previousVel = vel; }
	void SetPreviousPos(D3DXVECTOR3 pos) { m_previousPos = pos; }
	void SetDamage(float damage) { m_fDamage = damage; }
	void SetAudioID(int nID)	{ nAudioID = nID; }
	void SetEffect(CParticleEffect* cats) { m_pEffect = cats; }

	// Accessors
	CBase* GetOwner(void) { return m_pOwner; }
	int GetBounceCount(void) { return m_nBounceCount; }
	D3DXVECTOR3 GetPreviousVel(void) { return m_previousVel; }
	D3DXVECTOR3 GetPreviousPos(void) { return m_previousPos; }
	float GetDamage(void) { return m_fDamage; }

	bool CheckCollision(CBase* pBase);
	void CollisionReact(CBase* pBase);
	void Shutdown(void);
};

//Flame puddle for the alternate
class CNapalm : public CBase
{
private:
	CBase* m_pOwner; //Owner of the puddle.

	float m_fDamage; //How much damage this thing deals.
	float m_fMaxLife; //How long to burn for
	float m_fLifeTimer; //Time I've lived so far
	float m_fSpawnTimer; //The timer to spawn emitters.
	float m_fSpawn; //The limit for when to spawn actual particles

	CParticleEffect* m_pEffect;

public:
	// Trilobite
	CNapalm();
	CNapalm(const CNapalm& blt);
	~CNapalm();

	void Update(float fDt);
	void Render(void);
	void Initialize(void);
	void Shutdown(void);

	//Accessors
	float GetDamage(void) { return m_fDamage; }
	float GetMaxLife(void) { return m_fMaxLife; }
	float GetSpawnTime(void) { return m_fSpawn; }
	CBase* GetOwner(void) { return m_pOwner; }

	//Mutators
	void SetDamage(float damage) { m_fDamage = damage; }
	void SetMaxLife(float life) { m_fMaxLife = life; }
	void SetSpawnTime(float time) { m_fSpawn = time; }
	void SetOwner(CBase* owner) { m_pOwner = owner; }
	void SetEffect(CParticleEffect* effect){ m_pEffect = effect;}

};

#endif