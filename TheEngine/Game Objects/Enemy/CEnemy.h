#ifndef CEnemy_h__
#define CEnemy_h__

#include "../CCharacter.h"
#include "../../AI System/CAIStateMachine.h"
#include <vector>

#include "../../Resources/Sound/Wwise_IDs.h"
#include "../../Sound/WwiseManager.h"

using namespace std;

enum EnemyType { E_STRIKER = 0, E_ASSAULTER, E_HELIOS, E_ULTIMUS, E_MAXTYPE};
enum EnemyScore {STRIKER_SCORE = 100, ASSAULTER_SCORE = 200, HELIOS_SCORE = 50};
enum EnemySpawnMethod {DEFAULT_METHOD = 0, JUMP_IN_METHOD, DROP_DOWN_METHOD, TREE_METHOD, BOSS_METHOD, JETPACK_METHOD, TELEPORT_METHOD};
class CEnemy	: public CCharacter
{
private:
	//CCharacter* m_cCurTarget;
	AkSoundPosition pos;

	bool m_bIsLockEnemy;
	bool m_bIsGalEnemy;
	bool m_bThunderStruck; //Already got hit by the lightning gun?
	bool m_bShotgunHit; //Did I get hit already by the shotgun ult??!?!?!??!?!?!?!?!?!?!? NICK IS THE WORST.

	CEnemy* m_pNearest;

	static long long ID;

	int nAudioID;
	int m_nShadowEffect;
	int m_nSpawnMethod; //Enum for the way the enemy waltzes on the screen
	bool m_bArriving;

protected:
	bool m_bExplode;
	bool m_bJumping;
	bool m_bJumpPadTime;
	bool m_bFirstTime;
	int m_nEnemyType;
	float m_fRandVelocity;

	float m_fHitTimer;
	float m_fRunTimer;
	float m_bHit;

	int m_nHit;
	// checks for Audio
	//AkSoundPosition pos;
	bool m_bAudioPlayJump;
	bool m_bAudioPlayLand;
	bool m_bAudioPlayDie;
	bool m_bAudioPlayHurt;
	bool m_bAudioPlayShoot;
	bool m_bAudioPlayWalk;
	bool m_bAudioPlayFly;

	bool m_bBossAudioPlayCannon;
	bool m_bBossAudioPlayCharge2;
	bool m_bBossAudioPlayCharge1;
	bool m_bBossAudioPlayHit;
	bool m_bBossAudioPlayLaser2;
	bool m_bBossAudioPlayDebirs;
	bool m_bBossAudioPlayRocket;
	bool m_bBossAudioPlayLaser1;
	bool m_bBossAudioPlayStomp;
	bool m_bIsBossAudioPlaySpawn;
	bool m_bBossAudioPlayPhaseOver;

public:

	CEnemy();
	~CEnemy();

	void Shutdown();

	//void Initialize();

	// Accessors
	int GetAudioID(void)			{ return nAudioID; }
	int GetEnemyType(void)			{ return m_nEnemyType; }
	float GetRunTimer(void)			{ return m_fRunTimer; }

	bool GetPlayJump(void)			{ return m_bAudioPlayJump; }
	bool GetIsArriving(void)			{ return m_bArriving; }
	bool GetIsJumping(void)			{ return m_bJumping; }
	bool GetJumpPadTime(void)			{ return m_bJumpPadTime; }
	bool GetFirstTime(void)			{ return m_bFirstTime; }
	bool GetPlayLand(void)			{ return m_bAudioPlayLand; }
	bool GetPlayDie(void)			{ return m_bAudioPlayDie; }
	bool GetPlayHurt(void)			{ return m_bAudioPlayHurt; }
	bool GetPlayShoot(void)			{ return m_bAudioPlayShoot; }
	bool GetPlayWalk(void)			{ return m_bAudioPlayWalk; }
	bool GetPlayFly(void)			{ return m_bAudioPlayFly; }

	bool GetPlayBossStomp(void)		{ return m_bBossAudioPlayStomp; }
	bool GetPlayBossDie(void)		{ return m_bBossAudioPlayPhaseOver; }
	bool GetPlayBossCharge2(void)	{ return m_bBossAudioPlayCharge2; }
	bool GetPlayBossCharge1(void)	{ return m_bBossAudioPlayCharge1; }
	bool GetPlayBossRocket(void)	{ return m_bBossAudioPlayRocket; }
	bool GetPlayBossCannon(void)	{ return m_bBossAudioPlayCannon; }
	bool GetPlayBossHit(void)		{ return m_bBossAudioPlayHit; }
	bool GetPlayBossLaser2(void)	{ return m_bBossAudioPlayLaser2; }
	bool GetPlayBossLaser1(void)	{ return m_bBossAudioPlayLaser1; }
	bool GetIsPlayBossSpawn(void)	{ return m_bIsBossAudioPlaySpawn; }
	bool GetIsHitByShotgun(void)	{ return m_bShotgunHit; }

	bool GetIsLockEnemy(void)		{ return m_bIsLockEnemy; }
	bool GetIsGalEnemy(void)		{ return m_bIsGalEnemy; }
	CEnemy* GetNearestEnemy()		{ return m_pNearest; }
	bool GetIsThunderStruck(void)	{ return m_bThunderStruck; }
	int GetSpawnMethod(void)		{ return m_nSpawnMethod; }
	//CCharacter* GetCurrentTarget()	{return m_cCurTarget; }

	// Mutators
	void SetEnemyType(int nType)	{ m_nEnemyType = nType; }
	void SetPlayJump(bool bPlay)			{ m_bAudioPlayJump = bPlay; }
	void SetIsArriving(bool bIsArriving)			{ m_bArriving = bIsArriving; }
	void SetPlayLand(bool bPlay)			{ m_bAudioPlayLand = bPlay; }
	void SetPlayDie(bool bPlay)				{ m_bAudioPlayDie = bPlay; }
	void SetPlayHurt(bool bPlay)			{ m_bAudioPlayHurt = bPlay; }
	void SetPlayShoot(bool bPlay)			{ m_bAudioPlayShoot = bPlay; }
	void SetPlayWalk(bool bPlay)			{ m_bAudioPlayWalk = bPlay; }
	void SetPlayFly(bool bPlay)				{ m_bAudioPlayFly = bPlay; }
	void SetIsJumping(bool bJumping)		{ m_bJumping = bJumping; }
	void SetJumpPadTime(bool bJump)			{ m_bJumpPadTime = bJump; }
	void SetFirstTime(bool bFirstTime)		{ m_bFirstTime = bFirstTime; }
	void SetIsGalEnemy(bool bGal)			{ m_bIsGalEnemy = bGal; }
	void SetIsLockEnemy(bool bLock)			{ m_bIsLockEnemy = bLock; }
	void SetSpawnMethod(int method)			{ m_nSpawnMethod = method; }

	void SetPlayBossStomp(bool bPlay)		{ m_bBossAudioPlayStomp = bPlay; }
	void SetPlayBossDie(bool bPlay)			{ m_bBossAudioPlayPhaseOver = bPlay; }
	void SetPlayBossCannon(bool bPlay)		{ m_bBossAudioPlayCannon = bPlay; }
	void SetPlayBossRocket(bool bPlay)		{ m_bBossAudioPlayRocket = bPlay; }
	void SetPlayBossCharge2(bool bPlay)		{ m_bBossAudioPlayCharge2 = bPlay; }
	void SetPlayBossCharge1(bool bPlay)		{ m_bBossAudioPlayCharge1 = bPlay; }
	void SetPlayBossHit(bool bPlay)			{ m_bBossAudioPlayHit = bPlay; }
	void SetPlayBossLaser2(bool bPlay)		{ m_bBossAudioPlayLaser2 = bPlay; }
	void SetPlayBossLaser1(bool bPlay)		{ m_bBossAudioPlayLaser1 = bPlay; }
	void SetIsPlayBossSpawn(bool bIsPlay)	{ m_bIsBossAudioPlaySpawn = bIsPlay; }
	void SetIsHitByShotgunUlt(bool trip)	{ m_bShotgunHit = trip; }

	void PlaySpawnLoop(void);
	void StopSpawnLoop(void);


	void SetNearestEnemy(CEnemy* pEnemy)	{ m_pNearest = pEnemy; }
	void SetIsThunderStruck(bool struck)	{ m_bThunderStruck = struck; }
	void SetRunTimer(float fRunTimer)		{ m_fRunTimer = fRunTimer; }
	void Input();
	void Update(float fElapsedTime);
	void Render();
	void Init(D3DXVECTOR3 vPosition);
	virtual void AttackTarget();
	//void SelectTarget(vector<CCharacter*> vCharacters);
	//bool GetInDistance(float fRadius);
	void MoveTowardsTarget();

	void CollisionReact(CBase *pBase);
	void SceneCollisionReact(CBase* pBase);
	void FindNearestEnemy(void);
	void Initialize(void);
};

#endif // CEnemy_h__