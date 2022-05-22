#ifndef _CHUD_H_
#define _CHUD_H_

#include <D3DX9Math.h>
#include "../Mesh/CModel.h"
#include "../Camera/Camera.h"
#include "../Rendering/CRenderShape.h"
#include "../FX/CParticleEffect.h"

class CBitMapFont;
class CTextureManager;

enum HUDID {PLAYER1 = 0, PLAYER2 };
enum HUD_WEAPON_TYPE{HMACHINE_GUN = 0, HSHOTGUN , HFLAMETHROWER, HLIGHTNING_GUN, HROCKET_LAUNCHER};


class CHUD
{
private:
	
	CCamera* m_Cam;
	
	// Hud Display variables
	int m_nScore;
	int m_nHealth;
	int m_nLives;
	float m_fMultiplyer;
	int m_nFlatMultiplyer;
	
	// Spinning Dial Variables
	int m_nSpinValue;
	int m_nStrLen;
	int m_nStartDialPos;
	int m_nPreviousScore;
	int m_nFinalScore;
	float m_fRotation;

	// Model for Dials
	CRenderShape m_HUDDialShape[10];
	CModel* m_HUDDial;
	D3DXMATRIX m_Dials[10];
	int m_nDialValues[10];
	D3DXMATRIX m_Rotation;
	D3DXMATRIX m_Scale;
	D3DXMATRIX m_View;
	int m_nEffect;

	// Model for Multiplier Dial
	CRenderShape m_HUDMultiShape;
	CModel* m_HUDMulti;
	D3DXMATRIX m_Multi[4];
	D3DXMATRIX m_MultiRotation;
	D3DXMATRIX m_MultiScale;
	D3DXMATRIX m_MultiView;

	// Model for body;
	CRenderShape m_HUDFrameShape;
	CModel* m_HUDFrame;
	D3DXMATRIX m_Frame;

	// Model for HeartGrenade
	CRenderShape m_HUDHeartShape[3];
	CModel* m_HUDHeart;
	D3DXMATRIX m_Heart[3];

	// Model for Lives
	CRenderShape m_HUDLifeShape[4];
	CModel* m_HUDLife[4];
	D3DXMATRIX m_Lives;

	//Model for weapon Card
	CRenderShape m_HUDWeaponCardShape[5];
	CModel* m_HUDWeaponCard[5];
	D3DXMATRIX m_mWeaponCard;
	int m_nWeaponType;

	//Model for Ultimate Case
	CRenderShape m_UltimateCaseShape;
	CModel* m_UltimateCase;
	D3DXMATRIX m_mHUDUltCase;

	//Model for Ultimate Bar
	CRenderShape m_UltimateBarShape;
	CModel* m_UltimateBar;
	D3DXMATRIX m_mHUDUltBar;
	float m_fUltScale;

	//Model for Portrait
	CRenderShape m_HUDPortraitShape;
	CModel* m_HUDPortrait;
	D3DXMATRIX m_mHUDPortrait;
	int m_nPortraitType;

	//Model for DEATH
	CRenderShape m_HUDPortraitDeathShape;
	CModel* m_HUDPortraitDeath;
	D3DXMATRIX m_mHUDPortraitDeath;

	//Model for Death
	CRenderShape m_HUDWeaponCardDeathShape;
	CModel* m_HUDWeaponCardDeath;
	D3DXMATRIX m_mHUDWeaponCardDeath;

	//Model for Ultimatebar
	CRenderShape UltimateBarShape;
	D3DXMATRIX m_mUlt;

	//Matrix for Ultimate Particle effect
	D3DXMATRIX m_mUltParticle;

	float m_fMultiplyerScale;

	int m_fMaxMultiBar;
	int m_fCurMultiBar;

	// Time Bucket
	float m_fBucket;

	// Instances
	CBitMapFont* m_BFC;
	CTextureManager* m_TMC;

	// String Arrays
	char szScore[10];
	char szMultiplier[4];
	char szLives[10];
	char szScoreDial[10];

	
	CParticleEffect* fire;
	bool created;

	// Hud ID type to determine if you render for Player 1 or Player 2
	int m_nPlayerType;
	int m_nPlayerPositions;

	// Ammo Count
	int m_nAmmo;

	// Audio
	int m_nPrevMult;
	int m_nPlayKillCount;
	bool m_bAudioPlayKillSpree;
	bool m_bAudioPlayKillPlayer1;
	bool m_bAudioPlayKillPlayer2;

public:
	
	CHUD();
	~CHUD();

	void Initialize();
	
	// Mutators
	void AddScore(int PointValue);
	void SubtractScore(int PointValue);
	void AddHealth(int Health);
	void SubtractHealth(int Health);
	void AddLife(int Life);
	void SubtractLife(int Life);
	void SetHealth(int Health) { m_nHealth = Health;}
	void SetLives(int Lives) { m_nLives = Lives; }
	void SetType(int Type) {m_nPlayerType = Type; }
	void SetWeaponType(int Type) {m_nWeaponType = Type; }
	void SetUltScale(float Scale) {m_fUltScale = Scale; }
	void SetAmmo(int Ammo) {m_nAmmo = Ammo; }
	void SetPortraitType(int PT) {m_nPortraitType = PT;}

	void IncreaseMultiplier();
	void DecreaseMultiplier();
	void SetFinalMultiplyer();
	void ResetMultiplyer();
	void RotateMultiplyer(int rotate);

	// Accesors
	int GetScore() {return m_nScore;}
	int GetMult() {return m_nFlatMultiplyer;}
	int GetHealth() { return m_nHealth; }
	int GetLife() { return m_nLives; }
	int GetPlayerType() { return m_nPlayerType; }
	int GetAmmo() { return m_nAmmo; }
	int GetHudPortrate() { return m_nPortraitType; }

	void UpdateDials(int score);
	void SpinDial(int DialIndex, int SpinValue);

	void Update(float fElaspedTime);
	void Render();
	
	void Shutdown();
};



#endif