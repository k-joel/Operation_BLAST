#ifndef _MAIN_MENU_STATE_H
#define _MAIN_MENU_STATE_H

#include "IGameState.h"

#include "../Resources/Sound/Wwise_IDs.h"
#include "../Sound/WwiseManager.h"
#include "../Mesh/CModel.h"
#include "../Rendering/CRenderShape.h"
#include "../Game Objects/CGui.h"

class CBitMapFont;
class CInput;
class CTextureManager;
class CCamera;
class CDirect3D;

class CMainMenuState : public IGameState
{
private:

	// Pointers to instances of managers and singletons
	CBitMapFont* m_CBF;
	CInput* m_CIP;
	CTextureManager* m_CTM;
	CCamera* m_Cam;
	CDirect3D* m_pD3D;
	CGui REC;

	float m_fTotalTime;

	// For user choice
	int m_nSelection;
	//Background Image
	int m_nBackground;
	// Shader effect
	int m_nEffect;

	// Variables to allow for the typing font.
	float m_fDrawBucket;
	bool m_bDrawSelection[8];
	bool m_bDone;
	int m_nCharPosition;
	char buffer[64];
	bool options;

	//Variables for spacing of text
	float m_fScale;
	int m_nFontX;
	int m_nFontOffset;
	int m_nFontStart;
	int m_nArrowPosition;

	bool m_bRender;

	// Audio Variables
	static bool m_bPlayAudioMenu;

	// Model for the computer console and variables
	CRenderShape m_ConsoleShape;
	CModel* m_consoleScreen;
	D3DXMATRIX m_ConsoleWorld;

	//Scan line model 
	CRenderShape m_ScanlineShape;
	D3DXMATRIX m_mScaline;

	// Camera ModelView Matrix
	D3DXMATRIX m_mxViewProj;

	// Variables for confirmation of enter and exit
	bool m_bConf;
	int m_nConfirm;

	CMainMenuState(void);
	CMainMenuState(const CMainMenuState&);
	CMainMenuState& operator=(const CMainMenuState&);
	~CMainMenuState(void);

public:
	void Enter(void);
	bool Input(void);
	void Update(float fElpasedTime);
	void Render(void);
	bool Exit(void);
	bool Reset();
	void Draw(const char* message,int position,int nextBool, int currentBool, int x, int y, float scale);
	void MenuRender();
	bool GetOptions(){return options;}
	void SetOptions(bool o){options = o;}


	static CMainMenuState* GetInstance(void);
};

#endif