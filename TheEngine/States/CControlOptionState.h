#include "IGameState.h"


class CControlOptionState : public IGameState
{
private:
	CControlOptionState(void);
	CControlOptionState(const CControlOptionState&);
	CControlOptionState& operator=(const CControlOptionState&);
	~CControlOptionState(void);

	// Camera ModelView Matrix
	D3DXMATRIX m_mxViewProj;
public:
	void Enter(void);
	bool Input(void);
	void Update(float fElpasedTime);
	void Render(void);
	bool Exit(void);

	static CControlOptionState* GetInstance(void);
};