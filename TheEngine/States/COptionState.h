#include "IGameState.h"
#include "../Mesh/CModel.h"
#include "../Rendering/CRenderShape.h"

class CTextureManager;
class CStateSystem;
class CInput;
class CBitMapFont;

class COptionState : public IGameState
{
private:
	COptionState(void);
	COptionState(const COptionState&);
	COptionState& operator=(const COptionState&);
	~COptionState(void);

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

	bool m_bRender;

	// Models for the Input
	CRenderShape m_GamePadAcceptShape;
	CModel* m_GamePadAccept;
	D3DXMATRIX m_mGamePadAccept;

	CRenderShape m_GamePadUDShape;
	CModel* m_GamePadUD;
	D3DXMATRIX m_mGamePadUD;

	CRenderShape m_KeyAcceptShape;
	CModel* m_KeyAccept;
	D3DXMATRIX m_mKeyAccept;

	CRenderShape m_KeyUDShape;
	CModel* m_KeyUD;
	D3DXMATRIX m_mKeyUD;

	// Camera ModelView Matrix
	D3DXMATRIX m_mxViewProj;

public:
	void Enter(void);
	bool Input(void);
	void Update(float fElpasedTime);
	void Render(void);
	bool Exit(void);

	void RenderOptions(void);
	void RenderOptionsGame(void);

	static COptionState* GetInstance(void);
};