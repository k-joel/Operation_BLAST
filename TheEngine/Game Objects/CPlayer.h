
#pragma once

#include "CCharacter.h"

//Event system includes!  Take THAT, Alex!
#include "../Event System/CEvent.h"
#include "../Event System/CEventSystem.h"
#include "../Event System/IListener.h"
#include "../Input/CInput.h"

#include "../Mesh/CModel.h"

#include "Weapon States/IBaseWeaponState.h"
#include "../AI System/CAIStateMachine.h"

#include "../LuaEngine/CLuaEngine.h"

#include "../FX/CParticleEffect.h"

class CHUD;

enum state { IDLE = 0, WALK, SHOOT };
//enum aim_dir{STRAIGHT_UP = 0, ANGLE_UP, STRAIGHT_FORWARD, ANGLE_DOWN, STRAIGHT_DOWN,
//enum player_aim_dir {AIM_FORWARD = 0, AIM_UP, AIM_DOWN, AIM_ANGLE_UP, AIM_ANGLE_DOWN };
enum player_aim_dir {AIM_UP = 0, AIM_ANGLE_UP, AIM_FORWARD, AIM_ANGLE_DOWN, AIM_DOWN };


class CPlayer	: public CCharacter, IListener
{
private:
	lua_State* luaState;

	bool m_bRespawn;
	bool m_bIsJumping;
	bool m_bIsDownJumping;
	bool m_bInvincible;
	bool m_bReverse;
	bool m_bSlow;
	bool m_bAimSet; //Bool to make sure the aim has already been set.
	bool m_bLightningStorm;
	bool m_bGrounded;
	bool readyToFire; //Ready to fire the normal?
	bool m_bAltReady; //Ready to fire the alternate?
	bool m_bUltReady; //Ready to fire the ultimate?
	bool m_bFlameOn; //Am I currently shooting the flamethrower?
	bool m_bRocketDrop; //Am I going to drop the rocket
	bool m_bFiring;
	bool m_bHit;
	bool m_bAI;
	bool m_bChargePartOut; //Is the charge particle out?
	bool m_bAudioFlameNorm;

	float m_fGroundTimer; //How long to remain grounded
	float m_fMult; //The multiplier!  OMG!
	float m_fMultTimer; //Timer to keep track of when to decrease the multiplier
	float m_fRespawnTimer;
	float m_fReverseTimer; //Timer to stop people from being silly billies.  AKA, shotgun ult.
	float m_fSlowTimer;
	float m_fStormDamage; //Damage of the bolts
	float m_fDelay; //Delay of the bolts
	float m_fDelayDecay; //Decay rate of the delay (makes bolts fire faster)
	float m_fBoltSpeed; //How fast the bolts fly down on the screen
	float m_fBoltTimer; //Timer for the bolts to go off
	float m_fRocketTimer; // 3 Seconds until BOOM!
	float fireTimer; //Timer for the normal shot.
	float m_fAltTimer; //Timer for the alternate shot.
	float m_fUltTimer; //Timer for the ult shot.  WILL NOT BE USED IN LATER BUILDS.
	float m_fUltGauge; //The ultimate gauge.  When it's full (100), the player can use their ult.
	float m_fInvincibleTimer;
	float m_fBlink;
	float m_fHitTimer;
	float m_fLifeTime;
	float m_fShowID;
	float m_fDustTimer; //Timer for how often we kick up dust particles
	float m_fUltChargeUp;

	int m_nIDEffect;
	int m_nShadowEffect;
	int m_nHatStandIn;
	int m_nScore;  //The player score!  Wah-hey!
	int state;
	int m_nCurrentAnimation;
	int m_nPreviousAnimation;
	int m_nPlayerNum;
	int m_nAimDir;
	int m_nHat;
	int m_nBoltCount; //How many bolts have fired off
	int m_nMaxBolts; //Maximum number of bolts to fire off
	int m_nAmmo; //Ammo for the current weapon.  Machine gun contains -100, which means unlimited.  Be sure to handle that.
	int m_nBlood;

	CEvent* m_eLoseEvent; //Event to fire off if the player is out of lives
	CEvent* m_eWinEvent; //Event to fire off if the player wins?
	CEvent* m_eSplitEvent; //Event that fires off when the planes split
	CEvent* m_eMergeEvent; //Event that fires off when the planes merge
	CEvent* m_eShowPlayerID;

	CHUD* m_myHUD;

	CInput* m_pInput;
	
	CParticleEffect* m_pPilotLightParticle;
	CParticleEffect* m_pDustEffect;
	CParticleEffect* m_pDashEffect;

	D3DXMATRIX m_gunWorldPos;

	CBase* m_goal;

	IBaseWeaponState* m_pWeaponState; //The current weapon state of the player.  Used to do all SORTS a' cool things!

	CAIStateMachine m_AIsm;

protected:
	bool m_bAudioPlayHurt;
	bool m_bAudioPlayDie;
	bool m_bAudioPlayJump;
	bool m_bAudioPlayLand;
	bool m_bAudioPlayWalk;
	bool m_bAudioPlayUlt;
	bool m_bAudioPlayPickUp;
	bool m_bAudioIsFlamePlay;
	bool m_bAudioPlayDash;

	bool m_bAudioPlayTransMGun;
	bool m_bAudioPlayTransSGun;
	bool m_bAudioPlayTransFGun;
	bool m_bAudioPlayTransLGun;
	bool m_bAudioPlayTransRGun;

	int m_nUltHitCount;

	AkSoundPosition pos;

	bool m_bIsDashing;
	int m_nDashCount;
	float m_fCount;
	D3DXVECTOR3 m_vStartDashPos;
	D3DXVECTOR3 m_vEndDashPos;

	bool m_bIsProne;

public:

	CPlayer();
	~CPlayer();

	void SwitchWeapon(IBaseWeaponState* state);
	void Initialize();
	void Shutdown();

	//Accesor
	int GetScore(void) { return m_nScore; }
	bool GetRespawning(void) {return m_bRespawn; }
	float GetMult(void) { return m_fMult; }
	float GetMultTimer(void) { return m_fMultTimer; }
	bool GetIsJumping(void)		{ return m_bIsJumping; }
	float GetUltGauge(void) { return m_fUltGauge; }
	int GetAmmo(void) { return m_nAmmo; }
	int GetState(void) { return state; }
	int GetAimDir(void) { return m_nAimDir; }
	bool GetIsFiring(void) { return m_bFiring; }
	bool GetReadytoFire(void) { return readyToFire; }
	bool GetFlameOn(void) { return m_bFlameOn; }
	int GetPlayerNumber(void) { return m_nPlayerNum; }
	lua_State* GetLuaState(void) { return luaState; }
	bool GetLightningStorm(void) { return m_bLightningStorm; }

	//Mutator
	void SetScore(int score) { m_nScore = score; }
	void SetPlayerNum(int  nPlayer) { m_nPlayerNum = nPlayer; }
	void SetMult(float mult) { m_fMult = mult; }
	void SetMultTimer(float time) { m_fMultTimer = time; }
	void SetIsJumping(bool bIsJumping)	{ m_bIsJumping = bIsJumping; }
	void SetFlameOn(bool flame) { m_bFlameOn = flame; }
	void SetReadytoFire(bool bReady) { readyToFire = bReady; }
	void UpdateAnimation(float fElapsedTime);
	void SetAI(bool bAI) {m_bAI = bAI; }
	void SetUltGauge(float gauge) { m_fUltGauge = gauge; }
	void SetLightningStorm(bool storm) { m_bLightningStorm = storm; }
	void SetStormDamage(float damage) { m_fStormDamage = damage; }
	void SetStormDelay(float delay) { m_fDelay = delay; }
	void SetStormDelayDecay(float decay) { m_fDelayDecay = decay; }
	void SetStormMaxBolts(int max) { m_nMaxBolts = max; }
	void SetStormBoltSpeed(float speed) {m_fBoltSpeed = speed; }
	void SetIsGrounded(bool ground) { m_bGrounded = ground; }

	///////////////////////////////////////////////////////////////////////
	//
	// Function Name: AddScore
	//
	// Author: TJ "I Scored" Farnie
	//
	// In: score - The points to add to the player's score
	//
	// Out: None
	//
	// Return: None
	//
	// Description: Adds the points times the multiplier to the score.  
	//				Any sort of silly quick math is handled in here.
	///////////////////////////////////////////////////////////////////////
	void AddScore(int score);

	///////////////////////////////////////////////////////////////////////
	//
	// Function Name: GetInstance
	//
	// Author: TJ "Multi Man" Farnie
	//
	// In: increase - Amount to increase the multiplier by
	//
	// Out: None
	//
	// Return: None
	//
	// Description: Increases the multiplier amount.  
	///////////////////////////////////////////////////////////////////////
	void IncreaseMultiplier(float increase);

	///////////////////////////////////////////////////////////////////////
	//
	// Function Name: Jump
	//
	// Author: Alex Hernandez
	//
	// In: None
	//
	// Out: None
	//
	// Return: None
	//
	// Description: Causes the player to jump
	///////////////////////////////////////////////////////////////////////
	void Jump(void);

	///////////////////////////////////////////////////////////////////////
	//
	// Function Name: Shoot
	//
	// Author: Alex Hernandez
	//
	// In: float fRotY, fRotX
	//		D3DXVECTOR3 vel
	//
	// Out: None
	//
	// Return: None
	//
	// Description: Causes the player to shoot
	///////////////////////////////////////////////////////////////////////
	void Shoot(int nAngle, int nTypeFire);

	void Respawn(void);


	void Input(int nPlayer = 0);
	void Update(float fElapsedTime);
	void Render();

	bool CheckCollision(CBase *pBase);
	void CollisionReact(CBase *pBase); 
	void SetHud(CHUD* hud){m_myHUD = hud;}
	CHUD* GetHud() {return m_myHUD; }

	//Event system functions
	void Register();
	void Deregister();
	void React(const tDispatch& toReactTo);

	bool CheckSceneCollision(CBase* pBase);
	void SceneCollisionReact(CBase* pBase);
	void InitializeAI();
};
