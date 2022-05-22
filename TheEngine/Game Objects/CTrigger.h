#ifndef _CTRIGGER_
#define _CTRIGGER_

#include "../Game Objects/CBase.h"

enum SpawnType {
	SPAWN_WAVE = 1,
	SPAWN_BOSS = 2,
};

enum WaveType {
	LIGHT_WAVE = 0,
	LIGHT_LOCK, 
	MEDIUM_WAVE,
	MEDIUM_LOCK,
	HEAVY_WAVE,
	HEAVY_LOCK
};

class CTriggerObject : public CBase
{
public:
	unsigned int m_spawnType;
	unsigned int m_numSpawn;

	CTriggerObject();
	~CTriggerObject();

	void Update(float fElapsedTime);
	void Render();
	bool CheckCollision(CBase* pBase);
	void CollisionReact(CBase* pBase);
	void Initialize();
};

class CJungleSpawn : public CBase
{
private:
	float m_fTimer; //Time until we actually spawn the enemy
	bool m_bActive; //Is this trigger active yet?
public:
	//Accessors
	void SetIsActive(bool active) { m_bActive = active; }
	void SetTimer(float timer) { m_fTimer = timer; }

	//Mutators
	float GetTimer(void) { return m_fTimer; }
	float GetIsActive(void) { return m_bActive; }

	void Update(float fElapsedTime);
	void Render();
	bool CheckCollision(CBase* pBase);
	void CollisionReact(CBase* pBase);
	void Initialize();
};

class CBeachSpawn : public CBase
{
private:
	float m_fTimer; //Time until we actually spawn the enemy
	bool m_bActive; //Is this trigger active yet?
public:
	//Accessors
	void SetIsActive(bool active) { m_bActive = active; }
	void SetTimer(float timer) { m_fTimer = timer; }

	//Mutators
	float GetTimer(void) { return m_fTimer; }
	float GetIsActive(void) { return m_bActive; }

	void Update(float fElapsedTime);
	void Render();
	bool CheckCollision(CBase* pBase);
	void CollisionReact(CBase* pBase);
	void Initialize();
};

#endif //_CTRIGGER_