#include "IGameState.h"

class CTextureManager;
class CStateSystem;
class CInput;
class CBitMapFont;

class CSoundOptionState : public IGameState
{
private:
	CSoundOptionState(void);
	CSoundOptionState(const CSoundOptionState&);
	CSoundOptionState& operator=(const CSoundOptionState&);
	~CSoundOptionState(void);

	CInput* m_IPM;
	CTextureManager* m_TM;
	CStateSystem* m_SM;
	CBitMapFont* m_BMF;

	int m_nPreviousSFXVolume;
	int m_nPreviousDXVolume;

	int m_nBackground;
	int m_nSelection;

	float m_fScale;
	int m_nFontX;
	int m_nFontOffset;
	int m_nFontStart;
	int m_nArrowPosition;

public:
	void Enter(void);
	bool Input(void);
	void Update(float fElpasedTime);
	void Render(void);
	bool Exit(void);

	static CSoundOptionState* GetInstance(void);
};