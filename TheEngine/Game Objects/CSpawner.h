#ifndef _CSPAWNER_H
#define _CSPAWNER_H

#include "../LuaEngine/CLuaEngine.h"

enum waveEnum {TRICKLE = 0, LOCK_WAVE, GAL_WAVE, NORMAL_WAVE};
class CSpawner
{
private:
	lua_State* luaState;
	
	bool m_bTrickle; //Turns on and off trickle mode for the spawner
	bool m_bForestDrops; //Are enemies raining outta' the trees??

	float m_fTrickleTimer; //Timer to send new enemies in.
	float m_fTreeTimer; //Timer between enemies dropping out of the trees
	float m_fTreeLimit; //The limit for the timer before it triggers another tree duuuuuude

	int m_nNumberCreated; //How many enemies a script just made
	int m_nWaveType; //What the current wave type is
	int m_nTreeCount; //How many enemies have jumped from the tree tops
	int m_nTreeLimit; //How many max enemies can come from the trees

	CSpawner();
	~CSpawner();
	CSpawner(const CSpawner&);
	CSpawner& operator=(const CSpawner&);
public:

	//Accessors
	bool GetTrickle(void) { return m_bTrickle; }
	int GetNumberCreated(void) { return m_nNumberCreated; }
	int GetWaveType(void) { return m_nWaveType; }

	//Mutators
	void SetTrickle(bool trickle) { m_bTrickle = trickle; m_fTrickleTimer = 0.0f; }
	void SetNumberCreated(int created) { m_nNumberCreated = created; }
	void SetWaveType(int type) { m_nWaveType = type; }
	void SetTreeLimit(int limit) { m_nTreeLimit = limit;}
	void SetForestDrops(bool drop) { m_bForestDrops = drop; }

	static CSpawner* GetInstance(void);
	void RunScript(char* scriptName, bool backPlane, int waveType);
	void Refresh(void);
	static int CreateEnemy(lua_State* luaState);

	void Update(float fDT);
	void IncreaseCreated(void) { ++m_nNumberCreated; }
	void TurnOnForestEnemies(float timeBetweenSpawns, int numOfEnemies);
};

#endif