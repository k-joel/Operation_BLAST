#ifndef _GAME_STATE_H
#define _GAME_STATE_H

#include "IGameState.h"
class CInput;

#include "../Camera/Camera.h"
#include "../Rendering/CDirect3D.h"
#include "../Rendering/CTextureManager.h"
#include "../Collision/CollisionObject.h"
#include "../Game Objects/CPlayer.h"
#include "../Game Objects/CHUD.h"
#include "../Game Objects/Barron.h"
#include "../Game Objects/Enemy/CUltimus.h"
#include "../Game Objects/CClaire.h"
#include "../Event System/CEventSystem.h"
#include "../Event System/CEvent.h"
#include "../Event System/IListener.h"

#include "../Mesh/CModel.h"
#include "../Game Objects/CObjectManager.h"
#include "../Game Objects/Enemy/CStriker.h"
#include "../Game Objects/CSceneObject.h"

#include "../Game Objects/CMotion.h"
#include "../Game Objects/CGui.h"

//forward declarations here
class CTextureManager;
class CInput;
class CFlag;
enum game_state{SPLIT = 0, LOCKED};
enum level_state{BEACH = 0, JUNGLE, TRAIN_YARD, FACTORY};

class CGameState : public IGameState ,IListener
{
private:
	CFlag*						m_Flag;

	//system class pointers
	CInput*						m_pDI;
	CTextureManager*			m_pTM;
	CDirect3D*					m_pD3D;
	CObjectManager*				m_pOM;
	CCamera*					m_Cam;

	CHUD 						hud;
	CHUD 						hud2;

	CPlayer*					m_pPlayer;
	CPlayer*					m_pPlayer2;

	D3DXMATRIX 					mxWorld;
	D3DXMATRIX 					m_mxViewProj;

	//Add events here (need an event pointer for each event
	CEvent*						m_eCameraLock;
	CEvent*						m_eCameraFollow;
	CEvent*						m_eSpawnLightWave;
	CEvent*						m_eLightLock;
	CEvent*						m_eSpawnMediumWave;
	CEvent*						m_eMediumLock;
	CEvent*						m_eSpawnHeavyWave;
	CEvent*						m_eHeavyLock;
	CEvent*						m_eWinEvent;
	CEvent*						m_eLoseEvent;
	CEvent*						m_eTutorialCompleted;
	CEvent*						m_eSplitEvent;
	CEvent*						m_eMergeEvent;
	CEvent*						m_eShootingGal;
	CEvent*						m_eRainDebris;
	CEvent*						m_eStopDebris;
	CEvent*						m_eSpawnBoss;
	CEvent*						m_eLockClear;
	CEvent*						m_eShowPlayerID;

	int							m_nSkinFxID;
	int							m_nFlatID;
	int							m_nShootID;
	int							m_nPlayState; //This is the state of the game play.  Plane SPLIT, or LOCKED.
	int							m_nTriggerCount;
	int							m_nEnemyLockCount; //Used to keep track of how many enemies are in the current locked screen
	int							m_nPrevEnemyLockCount; //Previous frame's enemy count
	int 						m_nFade;
	int 						m_nEnemiesAlive;
	int 						m_nTutorialEnemiesAlive; //Enemies in the tutorial
	int 						m_nIntroEnemies; //Number of enemies to create in the tutorial
	int 						m_nShootingGalleryEnemiesAliveFar;//Number of enemies in a shooting gallery far plane
	int 						m_nShootingGalleryEnemiesAliveNear;//Number of enemies in a shooting gallery near plane
	int 						m_nDebrisOffset; //Offset of the debris
	int 						m_nDebrisAddAmount;
	int							m_nPrevPlayer1Score;
	int							m_nPrevPlayer2Score;
	int							m_nLevelArea; //Level area enum
	int							m_nGalleryCount; //Which gallery I'm on (starts at 1)

	//player feedback
	unsigned int 				m_nShootEverything;
	unsigned int 				m_nMoveForward;
	unsigned int 				m_nSplitUp;
	unsigned int 				m_nRegroup;
	unsigned int 				m_nWinner1;
	unsigned int 				m_nWinner2;

	bool 						m_bUltimateCheat; //Allows for unlimited ultimate bar
	bool 						m_bTutorial; //Are we in tutorial mode?
	bool 						m_bEnemiesPresent; //Are enemies now present in the tutorial?
	bool 						m_bGameOver;
	bool 						m_bWin;
	bool 						m_bIntroComplete;
	bool 						m_bRainDebris; //Should I be raining debris or not?
	bool 						m_bPaused;
	bool 						m_bIsBarron;
	bool 						m_bIntroDone;
	bool 						m_bWaveOneOut;
	bool 						m_bWaveTwoOut;
	bool 						m_bWaveThreeOut;
	bool 						m_bRender;
	bool 						m_startsAsMultiPlayer;
	bool 						m_bShootingGal; //Am I in a shooting gallery?
	bool						m_bTutorialEnemiesCleared; //Did the player finish killing the tutorial enemies?
	bool						m_bPlayer2Entered;//Only set if a second player enters. Prevents a reenter after they die.
	bool						m_bAudioPlayPlayerScoreSwitch;
	bool						m_bAudioPlayScore1;
	bool						m_bAudioPlayScore2;
	bool						m_bAugmentOut; //Has the augment been fired off yet?

	float 						m_fFadeTimer;
	float 						m_fFadeSet;
	float 						m_fSpawnTimer; //Spawn timer for the waves
	float 						m_fDebrisTimer; //Timer for raining debris for the boss
	float 						m_fShootEverything;
	float 						m_fMoveForward;
	float 						m_fSplitUp;
	float 						m_fRegroup;
	float 						m_fUnlock;
	float 						m_fWinner1;
	float 						m_fWinner2;
	float 						m_fCrateTimer;
	float						m_fTutorialEnemyTimer; //How long until the enemies appear in the tutorial
	float						m_fStartCountdown; //Countdown until the real enemies start coming
	float						m_fAugmentTimer; //Timer until the second part of a gallery wave comes in

	float						m_fGameTime; // total elapsed time while in game state

	CGui						m_PressStart;
	CUltimus*					m_pBossMan;
	CGameState(void);
	CGameState(const CGameState&);
	CGameState& operator=(const CGameState&);
	~CGameState(void);

	//Private function to make a shooting gallery.
	void CreateShootingGallery(int numEnemiesToMake, D3DXVECTOR3 initialPos);

	//Private function to create player2 if jumping in.
	void CreatePlayer2();

public:
	void Enter(void);
	bool Input(void);
	void Update(float fElpasedTime);
	void Render(void);
	bool Exit(void);

	static CGameState* GetInstance(void);

	//Accessors
	int GetEnemiesAlive(void) { return m_nEnemiesAlive; }
	int GetPlayState(void) { return m_nPlayState; }
	int GetTriggerCount(void) { return m_nTriggerCount; }
	bool GetShootingGallery(void) { return m_bShootingGal; }
	int GetShootingGalleryEnemiesAliveFar(void) { return m_nShootingGalleryEnemiesAliveFar; }
	int GetShootingGalleryEnemiesAliveNear(void) { return m_nShootingGalleryEnemiesAliveNear; }
	bool GetRainingDebris(void) { return m_bRainDebris; }
	bool GetPaused(void) {return m_bPaused; }
	CPlayer* GetPlayerOne(void) { return m_pPlayer; }
	CPlayer* GetPlayerTwo(void) { return m_pPlayer2; }
	int GetEnemyLockCount(void) { return m_nEnemyLockCount; }
	bool GetTutorial(void) { return m_bTutorial; }
	bool GetTutorialEnemiesPresent(void) { return m_bEnemiesPresent; }
	bool GetTutorialEnemiesCleared(void) { return m_bTutorialEnemiesCleared;}
	int GetTutorialEnemiesAlive(void) { return m_nTutorialEnemiesAlive; }
	int GetLevelArea(void) { return m_nLevelArea; }
	float GetGameTime(void)	{ return m_fGameTime; }

	//Mutators
	void SetEnemiesAlive(int alive) { m_nEnemiesAlive = alive;}
	void SetPlayState(int state) { m_nPlayState = state; }
	void SetTriggerCount(int count) { m_nTriggerCount = count; }
	void SetShootingGalleryEnemiesAliveFar(int num) {m_nShootingGalleryEnemiesAliveFar = num; }
	void SetShootingGalleryEnemiesAliveNear(int num) {m_nShootingGalleryEnemiesAliveNear = num; }
	void SetRainingDebris(bool rain) { m_bRainDebris = rain; }
	void SetIsPaused(bool Paused) {m_bPaused = Paused;}
	void SetEnemyLockCount(int count) { m_nEnemyLockCount = count; }
	void SetTutorial(bool chihuahua) { m_bTutorial = chihuahua; }
	void SetTutorialEnemiesPresent(bool christmas) { m_bEnemiesPresent = christmas; }
	void SetTutorialEnemiesCleared(bool crystal) { m_bTutorialEnemiesCleared = crystal; }
	void SetTutorialEnemiesAlive(int life) { m_nTutorialEnemiesAlive = life; }
	void SetLevelArea(int area) { m_nLevelArea = area; }

	//Functions to be overloaded for the event system
	void Register(void);
	void Deregister(void);
	void React(const tDispatch& toReactTo);

	CHUD* GetHUD(){return &hud;}
	CHUD* GetHUD2(){return &hud2;}

	void CreatePlayer();

	int GetSkinFxID() { return m_nSkinFxID; }
	int GetFlatID() { return m_nFlatID; }
	D3DXMATRIX GetViewProj() { return m_mxViewProj; }

	//Setting up the intro enemies.
	void CreateTutorialEnemies(void);

	//Raining debris!
	void RainDebris(void);

	// Checking which player has the higher score
	void PlayPlayerSwitch();
};

#endif