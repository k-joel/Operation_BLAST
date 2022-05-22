#pragma once

#include "IGameState.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <string>
#include <map>
using std::string;
using std::map;

enum eType { SND_SFX, SND_MUSIC };

//IMPORTANT//
//This is used to define the number of "steps" that the loading bar must lerp thru to get to 100%
// i.e. Value of 4 will increase the bar by 25% each time, 4 times.
#define NUM_ASSETS 12

struct HighScore
{
	string m_name;
	int m_score;

	string GetName() {return m_name;}
	int GetScore() {return m_score;}

	void SetName(string name) {m_name = name;}
	void SetScore(int score) {m_score = score;}

	HighScore()
	{
		m_name = "NMG";
		m_score = 0;
	}
};

class CTextureManager;
class CBitMapFont;
class CModelManager;
class CShaderManager;
class CLayoutImporter;
class CParticleManager;
class CLoadState : public IGameState
{
	friend class CLayoutImporter;
private:

	CTextureManager*	m_pTM;
	CBitMapFont*		m_pBF;
	CModelManager*		m_pMM;
	CShaderManager*		m_pSM;
	CParticleManager*	m_pPM;

	// the current loading stage
	int								m_nCurrentStage;
	RECT							m_rLoadbarRect;
	RECT							m_rLoadbarBGRect;

	// name of the current stage
	string							m_szCurAssetName;

	// maps to store the IDs
	map<string, int>				m_mImageIDMap;
	map<string, int>				m_mSFXIDMap;
	map<string, int>				m_mMusicIDMap;
	map<string, int>				m_mModelIDMap;

	//HighScore Table
	HighScore m_LoadScore[10];

	CLoadState(void);
	CLoadState(const CLoadState&);
	CLoadState& operator=(const CLoadState&);
	~CLoadState(void);
public:
	void Enter(void);
	bool Input(void);
	void Update(float fElapsedTime);
	void Render(void);
	bool Exit(void);

	int GetSoundID(string soundName) { return 0; } // need to figure out how sounds are loaded
	int GetImageID(string imageName) { return m_mImageIDMap[imageName]; }
	int GetModelID(string modelName) { return m_mModelIDMap[modelName]; }
	HighScore* GetHighScore(int ID) { return &m_LoadScore[ID]; }
	void LoadHighScore(string file);
	void WriteHighScore(string file);

	static CLoadState* GetInstance(void);
};