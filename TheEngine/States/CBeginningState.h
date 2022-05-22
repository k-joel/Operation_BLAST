#include "IGameState.h"


class CBeginningState : public IGameState
{
private:
	CBeginningState(void);
	CBeginningState(const CBeginningState&);
	CBeginningState& operator=(const CBeginningState&);
	~CBeginningState(void);
public:
	void Enter(void);
	bool Input(void);
	void Update(float fElpasedTime);
	void Render(void);
	bool Exit(void);

	static CBeginningState* GetInstance(void);
};