#include "IGameState.h"
#include "../Mesh/CModel.h"
#include "../Rendering/CRenderShape.h"

class CEndingState : public IGameState
{
private:
	CEndingState(void);
	CEndingState(const CEndingState&);
	CEndingState& operator=(const CEndingState&);
	~CEndingState(void);

	CRenderShape m_SlideShapes[6];
	D3DXMATRIX m_mWorld;

	int m_nCurrentSlide;
	float m_fTimer;

public:
	void Enter(void);
	bool Input(void);
	void Update(float fElpasedTime);
	void Render(void);
	bool Exit(void);

	static CEndingState* GetInstance(void);
};