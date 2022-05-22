#ifndef CState_h__
#define CState_h__

#include "../Game Objects/CCharacter.h"
#include "../Game Objects/CPlayer.h"
#include "../Game Objects/Enemy/CEnemy.h"
#include "../Game Objects/Enemy/CUltimus.h"

#include "../Event System/CEventSystem.h"
#include "../Event System/CEvent.h"
#include "../Collision/CCollisionTests.h"
#include "../Camera/Camera.h"
#include "../Game Objects/Bullet/CChargedLaser.h"
#include "../fx/CParticleEffect.h"
#include "../fx/CParticleManager.h"
#include <vector>
using namespace std;

class CCharacter;

enum StateID {	EIDLE, EFLY, ERUN, EWALK, EJUMP, EDEATH, ESELECT, ENAVIGATE, ESHOOT, EARRIVAL,//Enemies
ESPAWN,ELARGELASER,ESMALLLASER,EROCKET,	//Boss
EDUMMY													//Testing
};
enum EventID {	E_STAYINSTATE, E_GOTOSELECT, E_GOTOFLY, E_GOTORUN, E_GOTOWALK, E_GOTOJUMP, E_GOTOSHOOT, E_GOTODEATH, E_GOTOENDPHASE,
E_GOTOLLASERATTACK,E_GOTOSLASERATTACK,E_GOTOROCKETATTACK, E_GOTOSPAWN,								// Boss Stuff
};
class CState
{
protected:
	CCharacter* m_pNPC;
	list<CBase*> m_pTargets;
	int m_nStateID;

public:
	CState()
	{
	}

	virtual ~CState()
	{
	}

	/******************************************************************************/
	//	Enter	-	Enters into the state
	//
	//	Note:	This is a pure virtual function and must be overridden by its children
	/******************************************************************************/
	virtual void	Enter() = 0;

	/******************************************************************************/
	//	Update	-	Performs updates for the state
	//
	//	Return:	TRUE if successful
	//	Note:	This is a pure virtual function and must be overridden by its children
	/******************************************************************************/
	virtual int	Update(float fElapsedTime) = 0;

	virtual float	GetUtility(void ) = 0;
	/******************************************************************************/
	//	Exit	-	Exits out of the state
	//
	//	Note:	This is a pure virtual function and must be overridden by its children
	/******************************************************************************/
	virtual void	Exit() = 0;

	int GetId() { return m_nStateID; }
};
class CNavigateState : public CState
{
public:

	CNavigateState(CCharacter * pNPC)
	{
		// Remember the associated actor
		m_nStateID	= ENAVIGATE;
		m_pNPC = pNPC;
	}

	~CNavigateState(){}

	/******************************************************************************/
	//	Enter	-	Enters into the state
	/******************************************************************************/
	void	Enter();

	/******************************************************************************/
	//	Update	-	Performs updates for the state
	//
	//	Return:	TRUE if successful
	/******************************************************************************/
	int	Update(float fElapsedTime);

	float	GetUtility(void )
	{
		return 0.0f;
	}
	/******************************************************************************/
	//	Exit	-	Exits out of the state
	/******************************************************************************/
	void	Exit();
};
class CSelectState : public CState
{
public:

	CSelectState(CCharacter * pNPC, list<CBase*> pTargets)
	{
		// Remember the associated actor
		m_nStateID	= ESELECT;
		m_pNPC = pNPC;
		m_pTargets = pTargets;
	}

	~CSelectState(){}

	/******************************************************************************/
	//	Enter	-	Enters into the state
	/******************************************************************************/
	void	Enter();

	/******************************************************************************/
	//	Update	-	Performs updates for the state
	//
	//	Return:	TRUE if successful
	/******************************************************************************/
	int	Update(float fElapsedTime);

	float	GetUtility(void );
	/******************************************************************************/
	//	Exit	-	Exits out of the state
	/******************************************************************************/
	void	Exit();
};

class CRunState : public CState
{
	bool m_bTheWaitingBool;
	bool m_bFailSafe;
public:

	CRunState(CCharacter * pNPC,  list<CBase*> pTargets)
	{
		// Remember the associated actor
		m_nStateID	= ERUN;
		m_pNPC		= pNPC;
		m_pTargets	= pTargets;
		m_bTheWaitingBool = false;
		m_bFailSafe		 = false;
	}

	~CRunState(){}

	/******************************************************************************/
	//	Enter	-	Enters into the state
	/******************************************************************************/
	void	Enter();

	/******************************************************************************/
	//	Update	-	Performs updates for the state
	//
	//	Return:	TRUE if successful
	/******************************************************************************/
	int	Update(float fElapsedTime);

	float	GetUtility(void );
	/******************************************************************************/
	//	Exit	-	Exits out of the state
	/******************************************************************************/
	void	Exit();
};

class CDeathState : public CState
{
	float m_fSpawnTimer;
public:

	CDeathState(CCharacter * pNPC)
	{
		// Remember the associated actor
		m_nStateID	= EDEATH;
		m_pNPC = pNPC;
	}

	~CDeathState(){}

	/******************************************************************************/
	//	Enter	-	Enters into the state
	/******************************************************************************/
	void	Enter();

	/******************************************************************************/
	//	Update	-	Performs updates for the state
	//
	//	Return:	TRUE if successful
	/******************************************************************************/
	int	Update(float fElapsedTime);

	float	GetUtility(void );

	/******************************************************************************/
	//	Exit	-	Exits out of the state
	/******************************************************************************/
	void	Exit();
};

class CJumpState : public CState
{
public:

	CJumpState(CCharacter * pNPC,  list<CBase*> pTargets)
	{
		// Remember the associated actor
		m_nStateID	= EJUMP;
		m_pNPC = pNPC;
		m_pTargets = pTargets;
	}

	~CJumpState(){}

	/******************************************************************************/
	//	Enter	-	Enters into the state
	/******************************************************************************/
	void	Enter();

	/******************************************************************************/
	//	Update	-	Performs updates for the state
	//
	//	Return:	TRUE if successful
	/******************************************************************************/
	int	Update(float fElapsedTime);

	float	GetUtility(void );

	/******************************************************************************/
	//	Exit	-	Exits out of the state
	/******************************************************************************/
	void	Exit();
};

class CIdleState : public CState
{
public:

	CIdleState(CCharacter * pNPC)
	{
		// Remember the associated actor
		m_nStateID	= EIDLE;
		m_pNPC = pNPC;
	}

	~CIdleState(){}

	/******************************************************************************/
	//	Enter	-	Enters into the state
	/******************************************************************************/
	void	Enter();

	/******************************************************************************/
	//	Update	-	Performs updates for the state
	//
	//	Return:	TRUE if successful
	/******************************************************************************/
	int	Update(float fElapsedTime);

	float	GetUtility(void )
	{
		return 0.0f;
	}
	/******************************************************************************/
	//	Exit	-	Exits out of the state
	/******************************************************************************/
	void	Exit();
};
class CArrivalState : public CState
{
private:
	D3DXMATRIX m_mJetpack; //Matrix for the jetpack

	CParticleEffect* m_pJetpack; //Jetpack effect

	D3DXVECTOR3 m_vJumpTarget; //Where the jump should land
	D3DXVECTOR3 m_vJumpDir; //Direction I'll be jumping
	D3DXVECTOR3 m_vTreePos; //Position to keep the monsters at while they hide in the trees.  THE TREEEEEES!!!
	D3DXVECTOR3 m_vDropPos; //Position to drop the enemy on to.
	D3DXVECTOR3 m_vBossWalkTarget; //Position to make the boss enemies walk to before the head to the Z plane

	bool m_bBossTargetArrived; //Have I arrived at the boss target?
	bool m_bLanded; //Has the jetpacker landed yet?

	float m_fDropTimer; //Timer until the monster air drops
	float m_fTreeTimer; //Timer until the monster pops outta' the trees
	float m_fLandingTimer; //How long have I been "landing"?

	
	

public:

	//Accessors
	CParticleEffect* GetJetpackEffect(void) { return m_pJetpack; }
	bool GetHasLanded(void) {return m_bLanded; }

	//Mutators

	CArrivalState(CCharacter * pNPC)
	{
		// Remember the associated actor
		m_nStateID	= EARRIVAL;
		m_pNPC = pNPC;
	}

	~CArrivalState(){}

	/******************************************************************************/
	//	Enter	-	Enters into the state
	/******************************************************************************/
	void	Enter();

	/******************************************************************************/
	//	Update	-	Performs updates for the state
	//
	//	Return:	TRUE if successful
	/******************************************************************************/
	int	Update(float fElapsedTime);

	float	GetUtility(void )
	{
		return 0.0f;
	}
	/******************************************************************************/
	//	Exit	-	Exits out of the state
	/******************************************************************************/
	void	Exit();
};

class CDummyState : public CState
{
public:

	CDummyState(CCharacter * pNPC)
	{
		// Remember the associated actor
		m_nStateID	= EDUMMY;
		m_pNPC = pNPC;
	}

	~CDummyState(){}

	/******************************************************************************/
	//	Enter	-	Enters into the state
	/******************************************************************************/
	void	Enter()
	{
	}

	/******************************************************************************/
	//	Update	-	Performs updates for the state
	//
	//	Return:	TRUE if successful
	/******************************************************************************/
	int	Update(float fElapsedTime)
	{
		if(m_pNPC->GetIsDead() || m_pNPC->GetPosition().y == -200.0f)
			return E_GOTODEATH;

		return E_STAYINSTATE;
	}

	float	GetUtility(void )
	{
		return 0.0f;
	}
	/******************************************************************************/
	//	Exit	-	Exits out of the state
	/******************************************************************************/
	void	Exit()
	{
	}
};
class CShootState : public CState
{
	int	nNum;
	int nAnimLoop;
	CParticleEffect* pEffect;
	bool IbeChargin;

public:
	CShootState(CCharacter * pNPC)
	{
		// Remember the associated actor
		pEffect = NULL;
		m_nStateID	= ESHOOT;
		m_pNPC = pNPC;
		nNum = 0;
		nAnimLoop = 0;
		IbeChargin = false;
	}

	~CShootState(){}

	/******************************************************************************/
	//	Enter	-	Enters into the state
	/******************************************************************************/
	void	Enter();
	/******************************************************************************/
	//	Update	-	Performs updates for the state
	//
	//	Return:	TRUE if successful
	/******************************************************************************/
	int	Update(float fElapsedTime);

	float	GetUtility(void );
	/******************************************************************************/
	//	Exit	-	Exits out of the state
	/******************************************************************************/
	void	Exit();
};

class CFlyState : public CState
{
	D3DXVECTOR3 m_vNormToPlayer;
	D3DXVECTOR3 m_vToPlayer;
public:
	CFlyState(CCharacter * pNPC,  list<CBase*> pTargets)
	{
		m_nStateID	= EFLY;
		m_pNPC		= pNPC;
		m_pTargets	= pTargets;
		m_vNormToPlayer = m_vToPlayer = D3DXVECTOR3(0,0,0);
	}

	~CFlyState()
	{
	}

	/******************************************************************************/
	//	Enter	-	Enters into the state
	//
	//	Note:	This is a pure virtual function and must be overridden by its children
	/******************************************************************************/
	void	Enter();

	/******************************************************************************/
	//	Update	-	Performs updates for the state
	//
	//	Return:	TRUE if successful
	//	Note:	This is a pure virtual function and must be overridden by its children
	/******************************************************************************/
	int	Update(float fElapsedTime);

	float	GetUtility(void );
	/******************************************************************************/
	//	Exit	-	Exits out of the state
	//
	//	Note:	This is a pure virtual function and must be overridden by its children
	/******************************************************************************/
	void	Exit();
};

// Boss Shit
class CSpawnState : public CState
{
	bool m_bFirstTime;
public:
	CSpawnState(CCharacter * pNPC, list<CBase*> pTargets )
	{
		m_nStateID	= ESPAWN;
		m_pNPC		= pNPC;
		m_bFirstTime = true;
		m_pTargets = pTargets;
	}

	~CSpawnState()
	{
	}

	/******************************************************************************/
	//	Enter	-	Enters into the state
	//
	//	Note:	This is a pure virtual function and must be overridden by its children
	/******************************************************************************/
	void	Enter();

	/******************************************************************************/
	//	Update	-	Performs updates for the state
	//
	//	Return:	TRUE if successful
	//	Note:	This is a pure virtual function and must be overridden by its children
	/******************************************************************************/
	int	Update(float fElapsedTime);

	float	GetUtility(void )
	{
		return 0;
	}
	/******************************************************************************/
	//	Exit	-	Exits out of the state
	//
	//	Note:	This is a pure virtual function and must be overridden by its children
	/******************************************************************************/
	void	Exit();
};

class CLargeLaserState : public CState
{
	bool m_bFirstTime;
	bool m_bChargeOne;
	bool m_bChargeBlast;
	D3DXVECTOR3 LargeLaserPos;
	D3DXMATRIX LargeLaserMat;

public:
	CLargeLaserState(CCharacter * pNPC )
	{
		m_nStateID	= ELARGELASER;
		m_pNPC		= pNPC;
		m_bFirstTime = true;
		m_bChargeOne = false;
		m_bChargeBlast = false;
		D3DXMatrixIdentity(&LargeLaserMat);
		LargeLaserPos = D3DXVECTOR3(0,0,0);
	}

	~CLargeLaserState()
	{
	}

	/******************************************************************************/
	//	Enter	-	Enters into the state
	//
	//	Note:	This is a pure virtual function and must be overridden by its children
	/******************************************************************************/
	void	Enter();

	/******************************************************************************/
	//	Update	-	Performs updates for the state
	//
	//	Return:	TRUE if successful
	//	Note:	This is a pure virtual function and must be overridden by its children
	/******************************************************************************/
	int	Update(float fElapsedTime);

	float	GetUtility(void )
	{
		return 0;
	}
	/******************************************************************************/
	//	Exit	-	Exits out of the state
	//
	//	Note:	This is a pure virtual function and must be overridden by its children
	/******************************************************************************/
	void	Exit();
};

class CSmallLaserState : public CState
{
	bool m_bFirstTime;
	bool m_bChargeOne;
	bool m_bChargeBlast;
	D3DXVECTOR3 SmallLaserPos;
	D3DXMATRIX SmallLaserMat;

public:
	CSmallLaserState(CCharacter * pNPC )
	{
		m_nStateID	= ESMALLLASER;
		m_pNPC		= pNPC;
		m_bFirstTime = true;
		m_bChargeOne = false;
		m_bChargeBlast = false;
		D3DXMatrixIdentity(&SmallLaserMat);
		SmallLaserPos = D3DXVECTOR3(0,0,0);
	}

	~CSmallLaserState()
	{
	}

	/******************************************************************************/
	//	Enter	-	Enters into the state
	//
	//	Note:	This is a pure virtual function and must be overridden by its children
	/******************************************************************************/
	void	Enter();

	/******************************************************************************/
	//	Update	-	Performs updates for the state
	//
	//	Return:	TRUE if successful
	//	Note:	This is a pure virtual function and must be overridden by its children
	/******************************************************************************/
	int	Update(float fElapsedTime);

	float	GetUtility(void )
	{
		return 0;
	}
	/******************************************************************************/
	//	Exit	-	Exits out of the state
	//
	//	Note:	This is a pure virtual function and must be overridden by its children
	/******************************************************************************/
	void	Exit();
};

class CRocketState : public CState
{
	bool m_bFirstTime;
	bool m_bShadowEffect;
	bool m_bShootingBlanks;
	bool m_bShootingReals;
	//CFlag *flag;
	D3DXVECTOR3 realvF;

	D3DXVECTOR3 CannonLaserPos;
	D3DXMATRIX CannonLaserMat;


public:
	CRocketState(CCharacter * pNPC, list<CBase*> pTargets)
	{
		m_nStateID	= EROCKET;
		m_pNPC		= pNPC;
		m_pTargets	= pTargets;
		m_bFirstTime = true;
		m_bShadowEffect = false;
		m_bShootingBlanks = false;
		m_bShootingReals = false;
		realvF = D3DXVECTOR3(0,0,0);


	}

	~CRocketState()
	{
	}

	/******************************************************************************/
	//	Enter	-	Enters into the state
	//
	//	Note:	This is a pure virtual function and must be overridden by its children
	/******************************************************************************/
	void	Enter();

	/******************************************************************************/
	//	Update	-	Performs updates for the state
	//
	//	Return:	TRUE if successful
	//	Note:	This is a pure virtual function and must be overridden by its children
	/******************************************************************************/
	int	Update(float fElapsedTime);

	float	GetUtility(void )
	{
		return 0;
	}
	/******************************************************************************/
	//	Exit	-	Exits out of the state
	//
	//	Note:	This is a pure virtual function and must be overridden by its children
	/******************************************************************************/
	void	Exit();
};


class CCoopSelect : public CState
{
public:
	CCoopSelect(CCharacter * pNPC, list<CBase*> pTargets)
	{
		m_nStateID		= ESELECT;
		m_pNPC			= pNPC;
		m_pTargets		= pTargets;
	}

	~CCoopSelect()
	{
	}

	/******************************************************************************/
	//	Enter	-	Enters into the state
	//
	//	Note:	This is a pure virtual function and must be overridden by its children
	/******************************************************************************/
	void	Enter();

	/******************************************************************************/
	//	Update	-	Performs updates for the state
	//
	//	Return:	TRUE if successful
	//	Note:	This is a pure virtual function and must be overridden by its children
	/******************************************************************************/
	int	Update(float fElapsedTime);

	float	GetUtility(void );
	/******************************************************************************/
	//	Exit	-	Exits out of the state
	//
	//	Note:	This is a pure virtual function and must be overridden by its children
	/******************************************************************************/
	void	Exit();
};

class CCoopRun : public CState
{
public:
	CCoopRun(CCharacter * pNPC, list<CBase*> pTargets)
	{
		m_nStateID		= ERUN;
		m_pNPC			= pNPC;
		m_pTargets		= pTargets;
	}

	~CCoopRun()
	{
	}

	/******************************************************************************/
	//	Enter	-	Enters into the state
	//
	//	Note:	This is a pure virtual function and must be overridden by its children
	/******************************************************************************/
	void	Enter();

	/******************************************************************************/
	//	Update	-	Performs updates for the state
	//
	//	Return:	TRUE if successful
	//	Note:	This is a pure virtual function and must be overridden by its children
	/******************************************************************************/
	int	Update(float fElapsedTime);

	float	GetUtility(void );
	/******************************************************************************/
	//	Exit	-	Exits out of the state
	//
	//	Note:	This is a pure virtual function and must be overridden by its children
	/******************************************************************************/
	void	Exit();
};

class CCoopDodge : public CState
{
public:
	CCoopDodge(CCharacter * pNPC, list<CBase*> pTargets)
	{
		m_pNPC			= pNPC;
		m_pTargets		= pTargets;
	}

	~CCoopDodge()
	{
	}

	/******************************************************************************/
	//	Enter	-	Enters into the state
	//
	//	Note:	This is a pure virtual function and must be overridden by its children
	/******************************************************************************/
	void	Enter();

	/******************************************************************************/
	//	Update	-	Performs updates for the state
	//
	//	Return:	TRUE if successful
	//	Note:	This is a pure virtual function and must be overridden by its children
	/******************************************************************************/
	int	Update(float fElapsedTime);

	float	GetUtility(void );
	/******************************************************************************/
	//	Exit	-	Exits out of the state
	//
	//	Note:	This is a pure virtual function and must be overridden by its children
	/******************************************************************************/
	void	Exit();
};

class CCoopShoot : public CState
{
public:
	CCoopShoot(CCharacter * pNPC, list<CBase*> pTargets)
	{
		m_pNPC			= pNPC;
		m_pTargets		= pTargets;
	}

	~CCoopShoot()
	{
	}

	/******************************************************************************/
	//	Enter	-	Enters into the state
	//
	//	Note:	This is a pure virtual function and must be overridden by its children
	/******************************************************************************/
	void	Enter();

	/******************************************************************************/
	//	Update	-	Performs updates for the state
	//
	//	Return:	TRUE if successful
	//	Note:	This is a pure virtual function and must be overridden by its children
	/******************************************************************************/
	int	Update(float fElapsedTime);

	float	GetUtility(void );
	/******************************************************************************/
	//	Exit	-	Exits out of the state
	//
	//	Note:	This is a pure virtual function and must be overridden by its children
	/******************************************************************************/
	void	Exit();
};

class CCoopJump : public CState
{
public:
	CCoopJump(CCharacter * pNPC, list<CBase*> pTargets)
	{
		m_pNPC			= pNPC;
		m_pTargets		= pTargets;
	}

	~CCoopJump()
	{
	}

	/******************************************************************************/
	//	Enter	-	Enters into the state
	//
	//	Note:	This is a pure virtual function and must be overridden by its children
	/******************************************************************************/
	void	Enter();

	/******************************************************************************/
	//	Update	-	Performs updates for the state
	//
	//	Return:	TRUE if successful
	//	Note:	This is a pure virtual function and must be overridden by its children
	/******************************************************************************/
	int	Update(float fElapsedTime);

	float	GetUtility(void );
	/******************************************************************************/
	//	Exit	-	Exits out of the state
	//
	//	Note:	This is a pure virtual function and must be overridden by its children
	/******************************************************************************/
	void	Exit();
};

class CCoopDash : public CState
{
public:
	CCoopDash(CCharacter * pNPC, list<CBase*> pTargets)
	{
		m_pNPC			= pNPC;
		m_pTargets		= pTargets;
	}

	~CCoopDash()
	{
	}

	/******************************************************************************/
	//	Enter	-	Enters into the state
	//
	//	Note:	This is a pure virtual function and must be overridden by its children
	/******************************************************************************/
	void	Enter();

	/******************************************************************************/
	//	Update	-	Performs updates for the state
	//
	//	Return:	TRUE if successful
	//	Note:	This is a pure virtual function and must be overridden by its children
	/******************************************************************************/
	int	Update(float fElapsedTime);

	float	GetUtility(void );
	/******************************************************************************/
	//	Exit	-	Exits out of the state
	//
	//	Note:	This is a pure virtual function and must be overridden by its children
	/******************************************************************************/
	void	Exit();
};
#endif // CState_h__