////////////////////////////////////////////////////////////////////////////
//
// Class: CCamera
//
// Author: TJ "Ready For My Close Up" Farnie
//
////////////////////////////////////////////////////////////////////////////
#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "../Game Objects/CBase.h"
#include "../Event System/CEvent.h"
#include "../Event System/CEventSystem.h"
#include "../Event System/IListener.h"
#include "../Game Objects/CPlayer.h"

#include "../Game Objects/CMotion.h"

#include <d3d9.h>
#include <d3dx9.h>

class CCamera : public IListener
{
private:
	D3DXMATRIX      m_mProjectionMatrix;
	D3DXMATRIX      m_mViewMatrix;
	D3DXMATRIX		m_mViewProjectionMatrix;
	D3DMATRIX		m_previousCamPos; //Where the camera started at prior to the shake

	D3DXPLANE		m_frustumPlanes[6];

	char			m_cState;

	CPlayer*		m_pPlayerOne; //The first player
	CPlayer*		m_pPlayerTwo; //The second player

	D3DXVECTOR3		m_followPoint; //Point to have the camera follow (used for plane splits)
	D3DXVECTOR3		m_lockPoint; //Point to lock to
	D3DXVECTOR3		m_startPanPoint; //Point that I started panning from.
	D3DXVECTOR3		m_splitPoint; //The point to pan to when a split event occurs.

	CEvent*			m_eLockCamera; //Event for locking the camera
	CEvent*			m_eFollowCamera; //Event to have the camera follow the first player again
	CEvent*			m_eSplitEvent; //Event for a plane split
	CEvent*			m_eMergeEvent; //Event for a plane merge
	CEvent*			m_eShootingGal; //Event for making a shooting gallery

	float			m_fPrevY; //Previous Y position of the character
	float			m_fBaseY;	// Base starting position of the camera
	float			m_fPanTime; //Time the camera has spent panning
	float			m_fCameraRate;
	float			m_fShakeTimer; //How long has the camera been shaking?
	float			m_fMaxShakeTime; //How long to let the camera shake for

	bool			m_bSplitPan; //Bool to track if the camera needs to pan during a split
	bool			m_bCameraShake; //Is the camera shaking?

	CMotion*		m_pCameraMotion;

	CCamera(void);
	~CCamera(void);
	CCamera(const CCamera&);
	CCamera& operator=(const CCamera&);

public:
	CMotionControler m_MC;
	enum { LEFT_PLANE = 0, RIGHT_PLANE, TOP_PLANE, BOTTOM_PLANE, NEAR_PLANE, FAR_PLANE, MAX_PLANE };
	enum { FOLLOW = 0, LOCK, SPLIT };

	D3DXMATRIX GetProjectionMatrix(void);
	void SetProjectionMatrix(D3DXMATRIX *_mMatrix);
	void BuildPerspective(float _fFieldOfView,
		float _fAspect,
		float _fZNear,
		float _fZFar);

	D3DXMATRIX GetViewMatrix(bool _bTranslate = true);
	void SetViewMatrix(D3DXMATRIX *_mMatrix);
	void NormalizeViewMatrix(void);

	D3DXVECTOR3 GetViewXAxis(void);
	void SetViewXAxis(D3DXVECTOR3 _vPosition);
	void SetViewXAxis(float _fX,
		float _fY,
		float _fZ);

	D3DXVECTOR3 GetViewYAxis(void);
	void SetViewYAxis(D3DXVECTOR3 _vPosition);
	void SetViewYAxis(float _fX,
		float _fY,
		float _fZ);

	D3DXVECTOR3 GetViewZAxis(void);
	void SetViewZAxis(D3DXVECTOR3 _vPosition);
	void SetViewZAxis(float _fX,
		float _fY,
		float _fZ);

	D3DXVECTOR3 GetViewPosition(void);
	void SetViewPosition(D3DXVECTOR3 _vPosition);
	void SetViewPosition(float _fX,
		float _fY,
		float _fZ);

	void ViewRotateLocalX(float _fAngle);
	void ViewRotateLocalY(float _fAngle);
	void ViewRotateLocalZ(float _fAngle);

	void ViewTranslateLocal(D3DXVECTOR3 _vAxis, bool _bFPS = false);
	void ViewTranslateLocalX(float _fScale, bool _bFPS = false);
	void ViewTranslateLocalY(float _fScale, bool _bFPS = false);
	void ViewTranslateLocalZ(float _fScale, bool _bFPS = false);

	void ViewRotateGlobalX(float _fAngle);
	void ViewRotateGlobalY(float _fAngle);
	void ViewRotateGlobalZ(float _fAngle);

	void ViewTranslateGlobal(D3DXVECTOR3 _vAxis);
	void ViewTranslateGlobalX(float _fScale);
	void ViewTranslateGlobalY(float _fScale);
	void ViewTranslateGlobalZ(float _fScale);

	//Frustum accessors
	D3DXPLANE* GetFrustum(void) { return m_frustumPlanes; }
	D3DXPLANE GetLeftFrustumPlane(void) { return m_frustumPlanes[LEFT_PLANE]; }
	D3DXPLANE GetRightFrustumPlane(void) { return m_frustumPlanes[RIGHT_PLANE]; }
	D3DXPLANE GetTopFrustumPlane(void) { return m_frustumPlanes[TOP_PLANE]; }
	D3DXPLANE GetBottomFrustumPlane(void) { return m_frustumPlanes[BOTTOM_PLANE]; }
	D3DXPLANE GetNearFrustumPlane(void) { return m_frustumPlanes[NEAR_PLANE]; }
	D3DXPLANE GetFarFrustumPlane(void) { return m_frustumPlanes[FAR_PLANE]; }

	//Generate the frustum off the current view projection matrix
	void GenerateFrustum(void);

	//Set the players
	D3DXVECTOR3 GetFollowPoint(void) { return m_followPoint; }

	//Accessors
	char GetState(void) { return m_cState; }
	float GetPrevY(void) { return m_fPrevY; }
	float GetBaseY(void) { return m_fBaseY; }
	CPlayer* GetPlayerOne(void) { return m_pPlayerOne; }
	CPlayer* GetPlayerTwo(void) { return m_pPlayerTwo; }
	bool GetIsPanning(void) { return m_bSplitPan;}
	D3DXMATRIX GetViewProjectionMatrix(void) { return m_mViewProjectionMatrix; }

	//Mutators
	void SetState(char state) { m_cState = state; }
	void SetPrevY(float y) {m_fPrevY = y; }
	void SetBaseY(float y)	{	m_fBaseY = y; }
	void SetSplitPoint(D3DXVECTOR3 point) { m_splitPoint = point; }
	void SetPlayerOne(CPlayer* player) { m_pPlayerOne = player; }
	void SetPlayerTwo(CPlayer* player) { m_pPlayerTwo = player; }
	void SetStartPanPoint(D3DXVECTOR3 point) { m_startPanPoint = point; }

	//Finally, an update function.
	void Update(float fElapsedTime);

	//Event system functions
	void Register();
	void Deregister();
	void React(const tDispatch& toReactTo);

	//Singleton accessor
	static CCamera* GetInstance(void);

	void ShakeCamera(float time = 1.5f);
};
#endif //_CAMERA_H_