#include "IGameState.h"
#include "../Mesh/CModel.h"
#include "../Rendering/CRenderShape.h"
#include "../Game Objects/CBase.h"

class CBitMapFont;
class CInput;
class CTextureManager;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Class Name: CWinState
//
// Author: Nicholas "Mark 3" Guzman
//
// Last Mod: NONE
//
// Description: The winning screen
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class CWinState : public IGameState
{
private:

	CBitMapFont* m_CBF;
	CInput* m_CIP;
	CTextureManager* m_CTM;

	int m_nScroll;
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

	// For rendering Winning Condition
	CRenderShape m_BarronShape;
	CRenderShape m_ClaireShape;

	//Matrix for the winning animations
	D3DXMATRIX matIdentity;
	D3DXMATRIX matCIdentity;

	//For Victory Animation
	CRenderShape m_VictoryShape;
	D3DXMATRIX matVictory;

	//For Player1 Animation
	CRenderShape m_Player1Shape;
	D3DXMATRIX matPlayer1;

	//For Player2 Animation
	CRenderShape m_Player2Shape;
	D3DXMATRIX matPlayer2;

	// Camera ModelView Matrix
	D3DXMATRIX m_mxViewProj;

	int m_nBackgroundID;
	int m_nBulletCursorID;

	//Variables for spacing of text
	float m_fScale;
	int m_nFontX;
	int m_nFontOffset;
	int m_nFontStart;
	int m_nArrowPosition;

	int m_nSelection;

	CWinState(void);
	CWinState(const CWinState&);
	CWinState& operator=(const CWinState&);
	~CWinState(void);
public:
///////////////////////////////////////////////////////////////////////
 // Function Name: Enter
 //
 // Author: Nicholas "Mark 3" Guzman
 //
 // In: NONE
 //     
 // Out: NONE
 //
 // Return: NONE
 //
 // Description: Enters the state and intializes anything needed for the state
 ///////////////////////////////////////////////////////////////////////
	void Enter(void);

///////////////////////////////////////////////////////////////////////
 // Function Name: Input
 //
 // Author: Nicholas "Mark 3" Guzman
 //
 // In: NONE
 //     
 // Out: NONE
 //
 // Return: NONE
 //
 // Description: Handles Input for the state 
 ///////////////////////////////////////////////////////////////////////
	bool Input(void);

///////////////////////////////////////////////////////////////////////
 // Function Name: Update
 //
 // Author: Nicholas "Mark 3" Guzman
 //
 // In: float fElapsedTime
 //     
 // Out: NONE
 //
 // Return: NONE
 //
 // Description: Handles the update for the state 
 ///////////////////////////////////////////////////////////////////////
	void Update(float fElpasedTime);

///////////////////////////////////////////////////////////////////////
 // Function Name: Render
 //
 // Author: Nicholas "Mark 3" Guzman
 //
 // In: NONE
 //     
 // Out: NONE
 //
 // Return: NONE
 //
 // Description: Handles the rendering of the state
 ///////////////////////////////////////////////////////////////////////
	void Render(void);

///////////////////////////////////////////////////////////////////////
 // Function Name: Exit
 //
 // Author: Nicholas "Mark 3" Guzman
 //
 // In: NONE
 //     
 // Out: NONE
 //
 // Return: bool
 //
 // Description: Unloads all the objects of the state 
 ///////////////////////////////////////////////////////////////////////
	bool Exit(void);

	static CWinState* GetInstance(void);
};