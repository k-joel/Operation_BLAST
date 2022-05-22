#include "IGameState.h"
#include "../Mesh/CModel.h"
#include "../Rendering/CRenderShape.h"
#include "../Game Objects/CBase.h"

class CTextureManager;
class CBitMapFont;
class CInput;


class CGameOverState : public IGameState
{
private:

	CTextureManager* m_CTM;
	CBitMapFont* m_CBF;
	CInput* m_CIP;
	
	int test;
	float m_fBucket;
	float m_fTransTimer;
	
	bool blink;
	bool transition;
	bool m_bRender;
	bool m_bDone;

	// Model for the computer console and variables
	CRenderShape m_ConsoleShape;
	CModel* m_consoleScreen;
	D3DXMATRIX m_ConsoleWorld;

	CRenderShape m_BarronShape;
	CRenderShape m_ClaireShape;
	// Camera ModelView Matrix
	D3DXMATRIX m_mxViewProj;

	
	D3DXMATRIX matIdentity;
	D3DXMATRIX matCIdentity;

	CRenderShape m_GameOverShape;
	D3DXMATRIX matGameOver;

	//Variables for spacing of text
	float m_fScale;
	int m_nFontX;
	int m_nFontOffset;
	int m_nFontStart;
	int m_nArrowPosition;

	int m_nBackgroundID;
	int m_nBulletCursorID;

	int m_nSelection;

	CGameOverState(void);
	CGameOverState(const CGameOverState&);
	CGameOverState& operator=(const CGameOverState&);
	~CGameOverState(void);
public:
	void Enter(void);
	bool Input(void);
	void Update(float fElpasedTime);
	void Render(void);
	bool Exit(void);

	static CGameOverState* GetInstance(void);
};