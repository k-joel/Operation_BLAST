////////////////////////////////////////////////////
//File Name	:	"CGame.h"
//
//
//
//Purpose	:	To encapsulate all game related code
/////////////////////////////////////////////////////

// defines
#define MAX_CONSOLE_BUFFER_LINES    300     // console line count
#define MAX_CONSOLE_BUFFER_COLUMNS  80      // console line count

//Cross Compiler
#ifndef _CGAME_H_
#define _CGAME_H_
#include "../Sound/WwiseManager.h"

//SGD Singletions:
#include <Windows.h>

class CDirect3D;
class CTextureManager;
class CInput;
class CStateSystem;
class CShaderManager;
class CConfigLoader;
class CModelManager;
class CRenderContextManager;
class CObjectFactory;
class CObjectManager;
class CFXManager;
class CBVH;
class CParticleManager;
//Define class here

//Singleton
class CGame
{
private:

	CDirect3D			*m_pD3D;
	CTextureManager		*m_pTM;
	CInput				*m_pInput;
	CStateSystem		*m_pState;
	CWwiseManager		*m_pWw;
	CShaderManager		*m_pSM;
	CConfigLoader		*m_pCL;
	CModelManager		*m_pMM;
	CRenderContextManager *m_pRCM;
	CObjectFactory		*m_pOF;
	CObjectManager		*m_pOM;
	CFXManager			*m_pFX;
	CBVH				*m_pBVH;
	CParticleManager	*m_pPM;

	//timing vars
	DWORD					m_dwTimeStamp;
	float					m_fElapsedTime;
	DWORD					m_dwPreviousTimeStamp;
	SMALL_RECT				m_rConsoleWindow; //Size of the console window
	bool					m_bIsWindowed;
	bool					m_bIsPaused;
	bool					m_bMultiplayer;
	bool					m_bConsole; //If the console is out or not

	//Bools for the various cheats
	bool					m_bUnlimitedUlt; //Unlimited ultimate
	bool					m_bUnlimitedAmmo; //Unlimted ammo for all guns
	bool					m_bShotgun; //Gives all players the shotgun
	bool					m_bLightningGun; //Gives all players the lightning gun
	bool					m_bRocketLauncher; //Gives all players the launcher
	bool					m_bFlamethrower; //Gives all players the flamethrower
	bool					m_bGodLike; //Makes the players invulnerable

	// Sound Volumes
	int m_nMasterVolume;
	int m_nMusicVolume;
	int m_nSFXVolume;
	int m_nDialogVolume;

	//Difficulty
	int m_nDifficulty;

	//Constructor
	CGame(void);
	//Copy Constructor
	CGame(const CGame&);
	//Assignment operator
	CGame& operator=(const CGame&);
	//Destructor
	~CGame(void) {};

	//1.Input
	bool Input(void);

	//2.Update
	void Update(float fDt);

	//3.Draw
	void Render(void);

public:
	//Singleton Accessor
	static CGame *GetInstance(void);

	//3 steps a game goes thorugh in its lifetime:
	//1. Initialization
	void Initialization(HWND hWnd, HINSTANCE hInstance, const char* szSettingsFile);
	//2. Execution
	bool Main(void);
	//3. Clean up
	void Shutdown(void);

	//Game State Machine
	//void ChangeState(IGameState *newState);
	void SetMusicVolume(int nVol)					{ m_nMusicVolume = nVol; }
	void SetSFXVolume(int nVol)						{ m_nSFXVolume = nVol; }
	void SetMasterVolume(int nVol)					{ m_nMasterVolume = nVol; }
	void SetDialogVolume(int nVol)					{ m_nDialogVolume = nVol; }
	void SetMultiplayer(bool bMulti)				{ m_bMultiplayer = bMulti; }
	void SetPaused(bool bPause)						{ m_bIsPaused = bPause; }
	void SetDifficulty(int nDiff)					{m_nDifficulty = nDiff; }
	void SetConsoleWindowRect(SMALL_RECT rect)		{ m_rConsoleWindow = rect; }
	void SetUnlimitedUltCheat(bool ult)				{ m_bUnlimitedUlt = ult; }
	void SetUnlimtedAmmoCheat(bool ammo)			{ m_bUnlimitedAmmo = ammo; }
	void SetShotgunCheat(bool shotty)				{ m_bShotgun = shotty; }
	void SetLightningGunCheat(bool zap)				{ m_bLightningGun = zap; }
	void SetRocketLauncherCheat(bool lawnchair)		{ m_bRocketLauncher = lawnchair; }
	void SetFlamethrowerCheat(bool hotstuff)		{ m_bFlamethrower = hotstuff; }
	void SetGodLikeCheat(bool immortal)				{ m_bGodLike = immortal; }

	int GetMusicVolume(void)						{ return m_nMusicVolume; }
	int GetSFXVolume(void)							{ return m_nSFXVolume; }
	int GetMasterVolume(void)						{ return m_nMasterVolume; }
	int GetDialogVolume(void)						{ return m_nDialogVolume; }
	bool GetMutliplayer(void)						{ return m_bMultiplayer; }
	bool GetPaused(void)							{ return m_bIsPaused; }
	int GetDifficulty(void)							{ return m_nDifficulty;}
	bool GetConsole(void)							{ return m_bConsole; }
	bool GetUnlimitedUltCheat(void) 				{ return m_bUnlimitedUlt; }
	bool GetUnlimtedAmmoCheat(void) 				{ return m_bUnlimitedAmmo; }
	bool GetShotgunCheat(void)						{ return m_bShotgun; }
	bool GetLightningGunCheat(void)					{ return m_bLightningGun; }
	bool GetRocketLauncherCheat(void)				{ return m_bRocketLauncher; }
	bool GetFlamethrowerCheat(void)					{ return m_bFlamethrower; }
	bool GetGodLikeCheat(void)						{ return m_bGodLike; }
	float GetElapsedTime(void)						{ return m_fElapsedTime; }
};

#endif