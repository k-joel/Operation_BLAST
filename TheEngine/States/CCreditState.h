#ifndef _CREDIT_STATE_H
#define _CREDIT_STATE_H

#include "IGameState.h"
#include <string>
using std::string;

class CBitMapFont;
class CInput;
class CTextureManager;

using std::ifstream;
#include <fstream>



class CCreditState : public IGameState
{
private:
	CCreditState(void);
	CCreditState(const CCreditState&);
	CCreditState& operator=(const CCreditState&);
	~CCreditState(void);

	float m_fScale;
	int m_nFontX;
	int m_nFontOffset;
	int m_nFontStart;
	int m_nArrowPosition;
	int m_nImage;

	//Pointers to singletons
	CBitMapFont* m_CBF;
	CInput* m_CIP;
	CTextureManager* m_CTM;

	// Camera ModelView Matrix
	D3DXMATRIX m_mxViewProj;

	//File members
	ifstream in;
	bool m_bOpen;
	float m_fCreditBucket;
	string name;
	string title;

public:
	void Enter(void);
	bool Input(void);
	void Update(float fElpasedTime);
	void Render(void);
	bool Exit(void);

	void LoadCredits();

	static CCreditState* GetInstance(void);
};

#endif 