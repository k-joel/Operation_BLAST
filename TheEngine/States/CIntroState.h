#include "IGameState.h"


class CIntroState : public IGameState
{
private:
	CIntroState(void);
	CIntroState(const CIntroState&);
	CIntroState& operator=(const CIntroState&);
	~CIntroState(void);

	int m_nImages[5];
	int m_nCurrent;
	float m_fFadeInTimer;
	bool m_bDown;
public:
	void Enter(void);
	bool Input(void);
	void Update(float fElpasedTime);
	void Render(void);
	bool Exit(void);

	static CIntroState* GetInstance(void);
};