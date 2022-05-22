#ifndef _CHIGHSCORESTATE_H_
#define _CHIGHSCORESTATE_H_


#include "IGameState.h"
#include "../Resources/Sound/Wwise_IDs.h"
#include "../Sound/WwiseManager.h"
#include "../Mesh/CModel.h"
#include "../Rendering/CRenderShape.h"

class CBitMapFont;
class CInput;
class CTextureManager;
class CDirect3D;

class CHighScoreState : public IGameState
{
private:
	CHighScoreState(void);
	CHighScoreState(const CHighScoreState&);
	CHighScoreState& operator=(const CHighScoreState&);
	~CHighScoreState(void);

	//HighScore m_Scores[10];
	int m_nOffset;
public:
	void Enter(void);
	bool Input(void);
	void Update(float fElpasedTime);
	void Render(void);
	bool Exit(void);

	static CHighScoreState* GetInstance(void);
};

#endif
