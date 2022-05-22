#pragma once
#include <map>
using namespace std;

#include "CEffect.h"

class CObjectFactory;
class CObjectManager;
class CModelManager;
class CLoadState;
class CFXManager
{
	friend class CObjectFactory;
private:

	map<int, CEffect*> m_mActiveFX;

	// pointers
	CObjectFactory* m_pOF;
	CObjectManager* m_pOM;
	CModelManager* m_pMM;
	CLoadState* m_pLS;

	// Singleton data
	CFXManager();
	~CFXManager();
	CFXManager(const CFXManager &) {}
	CFXManager &operator=(const CFXManager &) {}

	static CFXManager	m_Instance;			// An instance to this class.

	static int sm_ObjectID;

	void SetupEffect(int nType, CBase *pObj, D3DXMATRIX world);
public:

	static CFXManager *GetInstance(void);

	void InitializeFXManager();
	void ShutdownFXManager();

	int LoadEffect(int nType, D3DXMATRIX world, float fLife, bool bLooping, CFrame *pOwnerFrame = NULL,
		bool bLerpSphere=false, bool bLerpAABB=false);
	void Update(float fDT);
	void UnloadEffect(int nID);

	void KillEffect(int nID) { m_mActiveFX[nID]->SetIsActive(false); }
	CEffect* GetEffect(int nID) { return m_mActiveFX[nID]; }
};