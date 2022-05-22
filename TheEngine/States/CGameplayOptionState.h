#include "IGameState.h"

class CTextureManager;
class CStateSystem;
class CInput;
class CBitMapFont;

class CGameplayOptionState : public IGameState
{
private:
	CGameplayOptionState(void);
	CGameplayOptionState(const CGameplayOptionState&);
	CGameplayOptionState& operator=(const CGameplayOptionState&);
	~CGameplayOptionState(void);

	CInput* m_IPM;
	CTextureManager* m_TM;
	CStateSystem* m_SM;
	CBitMapFont* m_BMF;


	int m_nSelection;
	int m_nFont;

	// Variables to allow for the typing font.
	float m_fScale;
	int m_nFontX;
	int m_nFontOffset;
	int m_nFontStart;
	int m_nArrowPosition;

	int m_nDiff;

public:
	void Enter(void);
	bool Input(void);
	void Update(float fElpasedTime);
	void Render(void);
	bool Exit(void);

	void SetDif(int diff) {m_nDiff = diff;}
	int GetDif(){return m_nDiff;}

	static CGameplayOptionState* GetInstance(void);
};