#ifndef CStriker_h__
#define CStriker_h__

#include "CEnemy.h"
#include "../../AI System/CAIStateMachine.h"
#include "../../Event System/IListener.h"
#include "../../Event System/CEvent.h"
#include "../../Event System/CEventSystem.h"

#include "../../Mesh/CModel.h"

class CStriker	: public CEnemy, IListener
{
	CEvent* m_eRun;

	//CModel model;
private:

	CAIStateMachine m_mStateMachine;
public:

	CStriker();
	~CStriker();

	void Init(D3DXVECTOR3 vPosition,  bool bDummyMode = false, int nHealth = 3);
	void Input();
	void Update(float fElapsedTime);
	void Render();

	bool CheckCollision(CBase *pBase);
	void CollisionReact(CBase *pBase);

	void Register();
	void Deregister();
	void React(const tDispatch& toReactTo);

	void Shutdown(void);
};
#endif // CStriker_h__