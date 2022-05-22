#include "IGameState.h"

class CTextureManager;
class CStateSystem;
class CInput;
class CBitMapFont;

class CHowToState : public IGameState
{
private:
	CHowToState(void);
	CHowToState(const CHowToState&);
	CHowToState& operator=(const CHowToState&);
	~CHowToState(void);

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

	// Camera ModelView Matrix
	D3DXMATRIX m_mxViewProj;

	int m_nConfirm;

	bool m_bRender;
public:
	void Enter(void);
	bool Input(void);
	void Update(float fElpasedTime);
	void Render(void);
	bool Exit(void);

	static CHowToState* GetInstance(void);
};