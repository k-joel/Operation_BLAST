#include "IGameState.h"

class CTextureManager;
class CStateSystem;
class CInput;
class CBitMapFont;

class CVideoOptionState : public IGameState
{
private:
	CVideoOptionState(void);
	CVideoOptionState(const CVideoOptionState&);
	CVideoOptionState& operator=(const CVideoOptionState&);
	~CVideoOptionState(void);

	CInput* m_IPM;
	CTextureManager* m_TM;
	CStateSystem* m_SM;
	CBitMapFont* m_BMF;

	int m_nSelection;
	int m_nVideoModeSelection;

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

	void RenderVideo(void);

	static CVideoOptionState* GetInstance(void);
};