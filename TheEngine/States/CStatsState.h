#include "IGameState.h"
#include <string>
using std::string;

class CTextureManager;
class CBitMapFont;
class CInput;

enum RenderStage { SCORE = 0,INITIALS };

class CStatsState : public IGameState
{
private:

	CTextureManager* m_CTM;
	CBitMapFont* m_CBF;
	CInput* m_CIP;

	int m_nScroll;
	int m_nSelection;
	float m_fBucket;
	float m_fTransTimer;

	bool blink;
	bool transition;
	bool m_bDone;

	int m_nBackgroundID;
	int m_nBulletCursorID;

	char szStats[10];

	//Variables for spacing of text
	float m_fScale;
	int m_nFontX;
	int m_nFontOffset;
	int m_nFontStart;
	int m_nArrowPosition;

	//Final Score
	int m_nFinalScoreP1;
	int m_nFinalScoreP2;

	//Score increment
	float m_fScoreBucket;
	int m_nRenderNext;
	int m_nP1Tally;
	int m_nP2Tally;
	
	int m_nRenderType;
	int m_nWhichPlayer;

	string Player1Initials;
	string Player2Initials;

	int m_nInitialSelection;
	int m_nRecursion;

	CStatsState(void);
	CStatsState(const CStatsState&);
	CStatsState& operator=(const CStatsState&);
	~CStatsState(void);
public:
	void Enter(void);
	bool Input(void);
	void Update(float fElpasedTime);
	void Render(void);
	bool Exit(void);

	void RenderPlayerScore();
	void RenderPlayerStats();
	void RenderPlayerInitials();
	void AddInitials(int player);
	void SavePlayer(int player, int ID);
	void SaveScore(int player);

	static CStatsState* GetInstance(void);
};