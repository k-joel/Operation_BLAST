#ifndef _PAUSE_STATE_H
#define _PAUSE_STATE_H

#include "IGameState.h"
#include "../Resources/Sound/Wwise_IDs.h"
#include "../Sound/WwiseManager.h"
#include "../Mesh/CModel.h"
#include "../Rendering/CRenderShape.h"

class CBitMapFont;
class CInput;
class CTextureManager;
class CDirect3D;

class CPauseState : public IGameState
{
private:

	// Pointers to instances of managers and singletons
	CBitMapFont* m_CBF;
	CInput* m_CIP;
	CTextureManager* m_CTM;

	// For user choice
	int m_nSelection;

	// Variable for Console movement
	int m_nConsoleOffset;
	bool m_bEnterDone;
	bool m_bExitDone;
	bool m_bReverse;
	float m_fMovementBucket;

	// Model for the computer console and variables
	CRenderShape m_ConsoleShape;
	CModel* m_consoleScreen;
	D3DXMATRIX m_ConsoleWorld;

	// Variables to allow for the typing font.
	float m_fDrawBucket;
	bool m_bDrawSelection[8];
	bool m_bDone;
	int m_nCharPosition;
	char buffer[64];
	float m_fScale;
	int m_nFontX;
	int m_nFontOffset;
	int m_nFontStart;
	int m_nArrowPosition;

	bool m_bRender;

	// Camera ModelView Matrix
	D3DXMATRIX m_mxViewProj;
	
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

	// Variables for confirmation of enter and exit
	bool m_bConf;
	int m_nConfirm;


	CPauseState(void);
	CPauseState(const CPauseState&);
	CPauseState& operator=(const CPauseState&);
	~CPauseState(void);

public:
	void Enter(void);
	bool Input(void);
	void Update(float fElpasedTime);
	void Render(void);
	bool Exit(void);

	void RenderMenu(void);

	static CPauseState* GetInstance(void);
};

#endif