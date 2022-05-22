#include "IGameState.h"
#include "../Mesh/CModel.h"
#include "../Rendering/CRenderShape.h"

enum Hats{ARMY = 0, CLAIREHAT, BEAR, CAT, COW, FANCY, FRUIT, NAR, MAR, POKE, POT, SHARK, SOM, HATMAX};

class CCharacterSelectionState : public IGameState
{
private:
	CCharacterSelectionState(void);
	CCharacterSelectionState(const CCharacterSelectionState&);
	CCharacterSelectionState& operator=(const CCharacterSelectionState&);
	~CCharacterSelectionState(void);

	int m_nSelection;
	int m_nFinalSelection;
	int m_nConfirm;
	bool m_bConf;
	bool m_bHat;
	bool m_bHat2;
	bool m_bAudioPlayCharacter;

	// Model for the computer console and variables
	CRenderShape m_BarronShape;
	D3DXMATRIX m_mBarron;

	// Model for the computer console and variables
	CRenderShape m_ClaireShape;
	D3DXMATRIX m_mClaire;

	// Model for the HATS
	CRenderShape m_HatShape[14];
	D3DXMATRIX m_mHat;
	int m_nHatSelectionPlayer1;
	int m_nHatSelectionPlayer2;

	//Matrix for the winning animations
	D3DXMATRIX matIdentity;
	D3DXMATRIX matCIdentity;

	// Camera ModelView Matrix
	D3DXMATRIX m_mxViewProj;

	//Variables for text
	float m_fScale;
	int m_nFontX;
	int m_nOtherFontX;
	int m_nFontOffset;
	int m_nFontStart;
	int m_nArrowPosition;

public:
	void Enter(void);
	bool Input(void);
	void Update(float fElpasedTime);
	void Render(void);
	bool Exit(void);

	bool InputPlayer1(void);
	bool InputPlayer2(void);
	bool FinalInput(void);

	void RenderPlayer1Hats(void);
	void RenderPlayer2Hats(void);
	void RenderConfirm(void);

	int GetSelection(void) {return m_nFinalSelection;}
	int GetPlayer1Hat(void) {return m_nHatSelectionPlayer1;}
	int GetPlayer2Hat(void) {return m_nHatSelectionPlayer2;}

	static CCharacterSelectionState* GetInstance(void);
};