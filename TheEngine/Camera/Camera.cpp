#include ".\Camera.h"
#include "../Game Objects/CPlayer.h"
#include "../Game Objects/CGoal.h"
#include "../Core/CGame.h"
#include "../Core/Util.h"
#include "../Resources/Sound/Wwise_IDs.h"
#include "../Sound/WwiseManager.h"
#include "../Game Objects/motionImporter.h"
#include "../Game Objects/CMotionFactory.h"
#include "../Game Objects/CObjectManager.h"

#define PAN_TIME 2.0f //How long it takes the camera to pan.

CCamera::CCamera(void)
{
	D3DXMatrixIdentity(&this->m_mProjectionMatrix);
	D3DXMatrixIdentity(&this->m_mViewMatrix);
	D3DXMatrixIdentity(&this->m_mViewProjectionMatrix);
	m_cState = LOCK;
	m_pPlayerOne = NULL;
	m_pPlayerTwo = NULL;
	m_eLockCamera = CEventSystem::GetInstance()->FindEvent("LockCamera");
	m_eFollowCamera = CEventSystem::GetInstance()->FindEvent("FollowCamera");
	m_eSplitEvent = CEventSystem::GetInstance()->FindEvent("SplitEvent");
	m_eMergeEvent = CEventSystem::GetInstance()->FindEvent("MergeEvent");
	m_eShootingGal = CEventSystem::GetInstance()->FindEvent("ShootingGal");

	m_fPrevY = 0.0f;
	m_fBaseY = 0.0f;
	m_fPanTime = 0.0f;

	m_pCameraMotion = CMotionFactory::GetInstance()->GetNewMotionObject();
	CMotionImporter::GetInstance()->LoadMotion("Resources/Motion/ShotCam.NNmove", *m_pCameraMotion);
	m_pCameraMotion->duplicateControler(m_MC);
	m_MC.GetWorldMatrix(0);

	m_bSplitPan = false;
	Register();

	CLuaEngine::GetInstance()->GetData(m_fCameraRate, "cameraRate");

	m_bCameraShake = false;
	m_fShakeTimer = 0.0f;
	m_fMaxShakeTime = 0.0f;
}

CCamera::~CCamera(void)
{
	Deregister();
}

D3DXMATRIX CCamera::GetProjectionMatrix(void)
{
	return this->m_mProjectionMatrix;
}

void CCamera::SetProjectionMatrix(D3DXMATRIX *_mMatrix)
{
	memcpy(&this->m_mProjectionMatrix, _mMatrix, sizeof(D3DXMATRIX));
}

void CCamera::BuildPerspective(float _fFieldOfView,
							   float _fAspectRatio,
							   float _fZNear,
							   float _fZFar)
{
	D3DXMatrixPerspectiveFovLH(&this->m_mProjectionMatrix,
		_fFieldOfView,
		_fAspectRatio,
		_fZNear,
		_fZFar);
}

D3DXMATRIX CCamera::GetViewMatrix(bool _bTranslate)
{
	D3DXMATRIX _mRotation(this->m_mViewMatrix), _mTransform;
	_mRotation._41 = _mRotation._42 = _mRotation._43 = 0;
	D3DXMatrixTranspose(&_mRotation, &_mRotation);

	D3DXMatrixIdentity(&_mTransform);
	_mTransform._41 = -(this->m_mViewMatrix._41);
	_mTransform._42 = -(this->m_mViewMatrix._42);
	_mTransform._43 = -(this->m_mViewMatrix._43);

	D3DXMatrixMultiply(&_mTransform, &_mTransform, &_mRotation);
	return ( _bTranslate ? _mTransform : _mRotation );
}

void CCamera::SetViewMatrix(D3DXMATRIX *_mMatrix)
{
	memcpy(&this->m_mViewMatrix, _mMatrix, sizeof(D3DXMATRIX));
}

void CCamera::NormalizeViewMatrix(void)
{
	/*=================================================================
	D3DXVECTOR3 xAxis,
	yAxis(0.0f, 1.0f, 0.0f),
	zAxis(this->m_mViewMatrix._31, this->m_mViewMatrix._32, this->m_mViewMatrix._33);

	D3DXVec3Normalize(&zAxis, &zAxis);
	D3DXVec3Cross(&xAxis, &zAxis, &yAxis);
	D3DXVec3Normalize(&xAxis, &xAxis);
	D3DXVec3Cross(&yAxis, &xAxis, &zAxis);
	D3DXVec3Normalize(&yAxis, &yAxis);

	this->m_mViewMatrix._11 = xAxis.x; this->m_mViewMatrix._12 = xAxis.y; this->m_mViewMatrix._13 = xAxis.z; this->m_mViewMatrix._14 = 0;
	this->m_mViewMatrix._21 = yAxis.x; this->m_mViewMatrix._22 = yAxis.y; this->m_mViewMatrix._23 = yAxis.z; this->m_mViewMatrix._24 = 0;
	this->m_mViewMatrix._31 = zAxis.x; this->m_mViewMatrix._32 = zAxis.y; this->m_mViewMatrix._33 = zAxis.z; this->m_mViewMatrix._34 = 0;
	//===============================================================*/
}

D3DXVECTOR3 CCamera::GetViewXAxis(void)
{
	return D3DXVECTOR3(m_mViewMatrix._11,
		m_mViewMatrix._12,
		m_mViewMatrix._13);
}

void CCamera::SetViewXAxis(D3DXVECTOR3 _vPosition)
{
	this->m_mViewMatrix._11 = _vPosition.x;
	this->m_mViewMatrix._12 = _vPosition.y;
	this->m_mViewMatrix._13 = _vPosition.z;
}

void CCamera::SetViewXAxis(float _fX,
						   float _fY,
						   float _fZ)
{
	this->m_mViewMatrix._11 = _fX;
	this->m_mViewMatrix._12 = _fY;
	this->m_mViewMatrix._13 = _fZ;
}

D3DXVECTOR3 CCamera::GetViewYAxis(void)
{
	return D3DXVECTOR3(m_mViewMatrix._21,
		m_mViewMatrix._22,
		m_mViewMatrix._23);
}

void CCamera::SetViewYAxis(D3DXVECTOR3 _vPosition)
{
	this->m_mViewMatrix._21 = _vPosition.x;
	this->m_mViewMatrix._22 = _vPosition.y;
	this->m_mViewMatrix._23 = _vPosition.z;
}

void CCamera::SetViewYAxis(float _fX,
						   float _fY,
						   float _fZ)
{
	this->m_mViewMatrix._21 = _fX;
	this->m_mViewMatrix._22 = _fY;
	this->m_mViewMatrix._23 = _fZ;
}

D3DXVECTOR3 CCamera::GetViewZAxis(void)
{
	return D3DXVECTOR3(m_mViewMatrix._31,
		m_mViewMatrix._32,
		m_mViewMatrix._33);
}

void CCamera::SetViewZAxis(D3DXVECTOR3 _vPosition)
{
	this->m_mViewMatrix._31 = _vPosition.x;
	this->m_mViewMatrix._32 = _vPosition.y;
	this->m_mViewMatrix._33 = _vPosition.z;
}

void CCamera::SetViewZAxis(float _fX,
						   float _fY,
						   float _fZ)
{
	this->m_mViewMatrix._31 = _fX;
	this->m_mViewMatrix._32 = _fY;
	this->m_mViewMatrix._33 = _fZ;
}

D3DXVECTOR3 CCamera::GetViewPosition(void)
{
	return D3DXVECTOR3(m_mViewMatrix._41,
		m_mViewMatrix._42,
		m_mViewMatrix._43);
}

void CCamera::SetViewPosition(D3DXVECTOR3 _vPosition)
{
	this->m_mViewMatrix._41 = _vPosition.x;
	this->m_mViewMatrix._42 = _vPosition.y;
	this->m_mViewMatrix._43 = _vPosition.z;
}

void CCamera::SetViewPosition(float _fX,
							  float _fY,
							  float _fZ)
{
	this->m_mViewMatrix._41 = _fX;
	this->m_mViewMatrix._42 = _fY;
	this->m_mViewMatrix._43 = _fZ;
}

void CCamera::ViewRotateLocalX(float _fAngle)
{
	D3DXMATRIX  _mRotation;
	D3DXVECTOR3 _vLocalX(m_mViewMatrix._11, m_mViewMatrix._12, m_mViewMatrix._13);
	D3DXMatrixRotationAxis(&_mRotation, &_vLocalX, _fAngle);

	D3DXVECTOR4 Position(this->m_mViewMatrix._41, this->m_mViewMatrix._42,
		this->m_mViewMatrix._43, this->m_mViewMatrix._44);
	this->m_mViewMatrix._41 = this->m_mViewMatrix._42 = this->m_mViewMatrix._43 = 0.0f;

	D3DXMatrixMultiply(&m_mViewMatrix, &m_mViewMatrix, &_mRotation);
	this->m_mViewMatrix._41 = Position.x; this->m_mViewMatrix._42 = Position.y;
	this->m_mViewMatrix._43 = Position.z; this->m_mViewMatrix._44 = Position.w;
}

void CCamera::ViewRotateLocalY(float _fAngle)
{
	D3DXMATRIX  _mRotation;
	D3DXVECTOR3 _vLocalY(m_mViewMatrix._21, m_mViewMatrix._22, m_mViewMatrix._23);
	D3DXMatrixRotationAxis(&_mRotation, &_vLocalY, _fAngle);

	D3DXVECTOR4 Position(this->m_mViewMatrix._41, this->m_mViewMatrix._42,
		this->m_mViewMatrix._43, this->m_mViewMatrix._44);
	this->m_mViewMatrix._41 = this->m_mViewMatrix._42 = this->m_mViewMatrix._43 = 0.0f;

	D3DXMatrixMultiply(&m_mViewMatrix, &m_mViewMatrix, &_mRotation);
	this->m_mViewMatrix._41 = Position.x; this->m_mViewMatrix._42 = Position.y;
	this->m_mViewMatrix._43 = Position.z; this->m_mViewMatrix._44 = Position.w;
}

void CCamera::ViewRotateLocalZ(float _fAngle)
{
	D3DXMATRIX  _mRotation;
	D3DXVECTOR3 _vLocalZ(m_mViewMatrix._31, m_mViewMatrix._32, m_mViewMatrix._33);
	D3DXMatrixRotationAxis(&_mRotation, &_vLocalZ, _fAngle);

	D3DXVECTOR4 Position(this->m_mViewMatrix._41, this->m_mViewMatrix._42,
		this->m_mViewMatrix._43, this->m_mViewMatrix._44);
	this->m_mViewMatrix._41 = this->m_mViewMatrix._42 = this->m_mViewMatrix._43 = 0.0f;

	D3DXMatrixMultiply(&m_mViewMatrix, &m_mViewMatrix, &_mRotation);
	this->m_mViewMatrix._41 = Position.x; this->m_mViewMatrix._42 = Position.y;
	this->m_mViewMatrix._43 = Position.z; this->m_mViewMatrix._44 = Position.w;
}

void CCamera::ViewTranslateLocal(D3DXVECTOR3 _vAxis, bool _bFPS)
{
	if (_bFPS)
	{
		ViewTranslateLocalX(_vAxis.x, _bFPS);
		ViewTranslateLocalY(_vAxis.y, _bFPS);
		ViewTranslateLocalZ(_vAxis.z, _bFPS);
		return;
	}

	ViewTranslateLocalX(_vAxis.x);
	ViewTranslateLocalY(_vAxis.y);
	ViewTranslateLocalZ(_vAxis.z);
}

void CCamera::ViewTranslateLocalX(float _fScale, bool _bFPS)
{
	if (_bFPS)
	{
		D3DXVECTOR3 _vTranslate((this->m_mViewMatrix._11),
			0.0f,
			(this->m_mViewMatrix._13));
		D3DXVec3Normalize(&_vTranslate,
			&_vTranslate);

		this->m_mViewMatrix._41 += _vTranslate.x * _fScale;
		this->m_mViewMatrix._42 += _vTranslate.y * _fScale;
		this->m_mViewMatrix._43 += _vTranslate.z * _fScale;

		return;
	}
	this->m_mViewMatrix._41 += (this->m_mViewMatrix._11 * _fScale);
	this->m_mViewMatrix._42 += (this->m_mViewMatrix._12 * _fScale);
	this->m_mViewMatrix._43 += (this->m_mViewMatrix._13 * _fScale);
}

void CCamera::ViewTranslateLocalY(float _fScale, bool _bFPS)
{
	if (_bFPS)
	{
		D3DXVECTOR3 _vTranslate((this->m_mViewMatrix._21),
			0.0f,
			(this->m_mViewMatrix._23));
		D3DXVec3Normalize(&_vTranslate,
			&_vTranslate);

		this->m_mViewMatrix._41 += _vTranslate.x * _fScale;
		this->m_mViewMatrix._42 += _vTranslate.y * _fScale;
		this->m_mViewMatrix._43 += _vTranslate.z * _fScale;

		return;
	}
	this->m_mViewMatrix._41 += (this->m_mViewMatrix._21 * _fScale);
	this->m_mViewMatrix._42 += (this->m_mViewMatrix._22 * _fScale);
	this->m_mViewMatrix._43 += (this->m_mViewMatrix._23 * _fScale);
}

void CCamera::ViewTranslateLocalZ(float _fScale, bool _bFPS)
{
	if (_bFPS)
	{
		D3DXVECTOR3 _vTranslate((this->m_mViewMatrix._31),
			0.0f,
			(this->m_mViewMatrix._33));
		D3DXVec3Normalize(&_vTranslate,
			&_vTranslate);

		this->m_mViewMatrix._41 += _vTranslate.x * _fScale;
		this->m_mViewMatrix._42 += _vTranslate.y * _fScale;
		this->m_mViewMatrix._43 += _vTranslate.z * _fScale;

		return;
	}
	this->m_mViewMatrix._41 += (this->m_mViewMatrix._31 * _fScale);
	this->m_mViewMatrix._42 += (this->m_mViewMatrix._32 * _fScale);
	this->m_mViewMatrix._43 += (this->m_mViewMatrix._33 * _fScale);
}

void CCamera::ViewRotateGlobalX(float _fAngle)
{
	D3DXMATRIX _mRotation;
	D3DXVECTOR4 Position(this->m_mViewMatrix._41, this->m_mViewMatrix._42,
		this->m_mViewMatrix._43, this->m_mViewMatrix._44);
	D3DXMatrixRotationX(&_mRotation, _fAngle);

	this->m_mViewMatrix._41 = this->m_mViewMatrix._42 = this->m_mViewMatrix._43 = 0.0f;

	D3DXMatrixMultiply(&this->m_mViewMatrix, &this->m_mViewMatrix, &_mRotation);
	this->m_mViewMatrix._41 = Position.x; this->m_mViewMatrix._42 = Position.y;
	this->m_mViewMatrix._43 = Position.z; this->m_mViewMatrix._44 = Position.w;
}

void CCamera::ViewRotateGlobalY(float _fAngle)
{
	D3DXMATRIX _mRotation;
	D3DXVECTOR4 Position(this->m_mViewMatrix._41, this->m_mViewMatrix._42,
		this->m_mViewMatrix._43, this->m_mViewMatrix._44);
	D3DXMatrixRotationY(&_mRotation, _fAngle);

	this->m_mViewMatrix._41 = this->m_mViewMatrix._42 = this->m_mViewMatrix._43 = 0.0f;

	D3DXMatrixMultiply(&this->m_mViewMatrix, &this->m_mViewMatrix, &_mRotation);
	this->m_mViewMatrix._41 = Position.x; this->m_mViewMatrix._42 = Position.y;
	this->m_mViewMatrix._43 = Position.z; this->m_mViewMatrix._44 = Position.w;
}

void CCamera::ViewRotateGlobalZ(float _fAngle)
{
	D3DXMATRIX _mRotation;
	D3DXVECTOR4 Position(this->m_mViewMatrix._41, this->m_mViewMatrix._42,
		this->m_mViewMatrix._43, this->m_mViewMatrix._44);
	D3DXMatrixRotationZ(&_mRotation, _fAngle);

	this->m_mViewMatrix._41 = this->m_mViewMatrix._42 = this->m_mViewMatrix._43 = 0.0f;

	D3DXMatrixMultiply(&this->m_mViewMatrix, &this->m_mViewMatrix, &_mRotation);
	this->m_mViewMatrix._41 = Position.x; this->m_mViewMatrix._42 = Position.y;
	this->m_mViewMatrix._43 = Position.z; this->m_mViewMatrix._44 = Position.w;
}

void CCamera::ViewTranslateGlobal(D3DXVECTOR3 _vAxis)
{
	this->m_mViewMatrix._41 += _vAxis.x;
	this->m_mViewMatrix._42 += _vAxis.y;
	this->m_mViewMatrix._43 += _vAxis.z;
}

void CCamera::ViewTranslateGlobalX(float _fScale)
{
	this->m_mViewMatrix._41 += _fScale;
}

void CCamera::ViewTranslateGlobalY(float _fScale)
{
	this->m_mViewMatrix._42 += _fScale;
}

void CCamera::ViewTranslateGlobalZ(float _fScale)
{
	this->m_mViewMatrix._43 += _fScale;
}

void CCamera::GenerateFrustum()
{
	// Left plane
	m_frustumPlanes[0].a = m_mViewProjectionMatrix._14 + m_mViewProjectionMatrix._11;
	m_frustumPlanes[0].b = m_mViewProjectionMatrix._24 + m_mViewProjectionMatrix._21;
	m_frustumPlanes[0].c = m_mViewProjectionMatrix._34 + m_mViewProjectionMatrix._31;
	m_frustumPlanes[0].d = m_mViewProjectionMatrix._44 + m_mViewProjectionMatrix._41;

	// Right plane
	m_frustumPlanes[1].a = m_mViewProjectionMatrix._14 - m_mViewProjectionMatrix._11;
	m_frustumPlanes[1].b = m_mViewProjectionMatrix._24 - m_mViewProjectionMatrix._21;
	m_frustumPlanes[1].c = m_mViewProjectionMatrix._34 - m_mViewProjectionMatrix._31;
	m_frustumPlanes[1].d = m_mViewProjectionMatrix._44 - m_mViewProjectionMatrix._41;

	// Top plane
	m_frustumPlanes[2].a = m_mViewProjectionMatrix._14 - m_mViewProjectionMatrix._12;
	m_frustumPlanes[2].b = m_mViewProjectionMatrix._24 - m_mViewProjectionMatrix._22;
	m_frustumPlanes[2].c = m_mViewProjectionMatrix._34 - m_mViewProjectionMatrix._32;
	m_frustumPlanes[2].d = m_mViewProjectionMatrix._44 - m_mViewProjectionMatrix._42;

	// Bottom plane
	m_frustumPlanes[3].a = m_mViewProjectionMatrix._14 + m_mViewProjectionMatrix._12;
	m_frustumPlanes[3].b = m_mViewProjectionMatrix._24 + m_mViewProjectionMatrix._22;
	m_frustumPlanes[3].c = m_mViewProjectionMatrix._34 + m_mViewProjectionMatrix._32;
	m_frustumPlanes[3].d = m_mViewProjectionMatrix._44 + m_mViewProjectionMatrix._42;

	// Near plane
	m_frustumPlanes[4].a = m_mViewProjectionMatrix._13;
	m_frustumPlanes[4].b = m_mViewProjectionMatrix._23;
	m_frustumPlanes[4].c = m_mViewProjectionMatrix._33;
	m_frustumPlanes[4].d = m_mViewProjectionMatrix._43;

	// Far plane
	m_frustumPlanes[5].a = m_mViewProjectionMatrix._14 - m_mViewProjectionMatrix._13;
	m_frustumPlanes[5].b = m_mViewProjectionMatrix._24 - m_mViewProjectionMatrix._23;
	m_frustumPlanes[5].c = m_mViewProjectionMatrix._34 - m_mViewProjectionMatrix._33;
	m_frustumPlanes[5].d = m_mViewProjectionMatrix._44 - m_mViewProjectionMatrix._43;

	//Now normalizing the planes
	for(char i = 0; i < 6; ++i)
	{
		D3DXPlaneNormalize(&m_frustumPlanes[i], &m_frustumPlanes[i]);
	}
}

void CCamera::Update(float fElapsedTime)
{

	if(!m_bCameraShake)
		m_mViewMatrix = m_MC.GetWorldMatrix();

	AkListenerPosition pos;
	m_mViewProjectionMatrix = GetViewMatrix() * GetProjectionMatrix();
	pos.Position.X = GetViewPosition().x;
	pos.Position.Y = GetViewPosition().y;
	pos.Position.Z = GetViewPosition().z;
	pos.OrientationFront.X = 0;
	pos.OrientationFront.Y = 0;
	pos.OrientationFront.Z = 1;
	pos.OrientationTop.X = 0;
	pos.OrientationTop.Y = 1;
	pos.OrientationTop.Z = 0;
	CWwiseManager::GetInstance()->SetListenerPosition(pos);
	GenerateFrustum();

	if(m_pPlayerOne && !m_pPlayerOne->GetIsActive())
	{
		m_pPlayerOne = NULL;
	}

	if(m_pPlayerTwo && !m_pPlayerTwo->GetIsActive())
	{
		m_pPlayerTwo = NULL;
	}

	if(!m_bCameraShake)
	{
		if(!m_bSplitPan)
		{
			//Running normal camera panning logic
			switch(m_cState)
			{
				//Logic for the LOCK state.  Camera should basically just sit there while the player runs around all
				//crazy like
			case LOCK:
				{
					break;
				}
				//Logic for the FOLLOW state.  Camera will follow one character, keeping them centered on the screen.
				//The camera will not be able to go backwards, but will only move forwards once the player is moving past
				//the center of the screen.
			case FOLLOW:
				{
					if(m_pPlayerOne && m_pPlayerTwo)
					{
						//Get the average x position between the two players
						float avX = m_pPlayerOne->GetPosition().x + m_pPlayerTwo->GetPosition().x;
						avX *= 0.5f;

						if(avX > GetViewPosition().x + 20.0f)
						{
							m_mViewMatrix = m_MC.GetWorldMatrix(-1.0f, fElapsedTime, 2.0f);
						}
					}
					else
					{
						if(m_pPlayerOne && m_pPlayerOne->GetPosition().x > GetViewPosition().x - 60.0f)
						{
							m_mViewMatrix = m_MC.GetWorldMatrix(-1.0f, fElapsedTime, m_fCameraRate);
						}
						else if(m_pPlayerTwo && m_pPlayerTwo->GetPosition().x > GetViewPosition().x - 60.0f)
						{
							m_mViewMatrix = m_MC.GetWorldMatrix(-1.0f, fElapsedTime, m_fCameraRate);
						}
					}
					break;
				}
			}
		}
		else
		{
			if(m_splitPoint.x > GetViewPosition().x)
			{
				m_mViewMatrix = m_MC.GetWorldMatrix(-1.0, fElapsedTime, m_fCameraRate);
			}

			if(m_splitPoint.x <= GetViewPosition().x)
			{
				m_bSplitPan = false;
				m_fPanTime = 0.0f;
				char* buffer = "whatever";
				CEventSystem::GetInstance()->DispatchEvent(*m_eShootingGal, NULL, 2.0f);
			}
		}
	}
	else
	{
		m_fShakeTimer += fElapsedTime;
		
		float randX, randY;
		
		randX = (rand() % 4 - 2 / 10.0f);
		randY = (rand() % 4 - 2 / 10.0f);

		D3DXMATRIX newPos = m_previousCamPos;
		newPos._41 += randX;
		newPos._42 += randY;

		m_mViewMatrix = newPos;

		if(m_fShakeTimer >= m_fMaxShakeTime)
		{
			m_bCameraShake = false;
			m_fShakeTimer = 0.0f;
			m_fMaxShakeTime = 0.0f;
		}
	}

}

void CCamera::Register()
{
	CEventSystem::GetInstance()->RegisterForEvent(*this, *m_eLockCamera, 1.0f);
	CEventSystem::GetInstance()->RegisterForEvent(*this, *m_eFollowCamera, 1.0f);
	CEventSystem::GetInstance()->RegisterForEvent(*this, *m_eSplitEvent, 1.0f);
	CEventSystem::GetInstance()->RegisterForEvent(*this, *m_eMergeEvent, 1.0f);
	CEventSystem::GetInstance()->RegisterForEvent(*this, *m_eShootingGal, 1.0f);
}

void CCamera::Deregister()
{
	CEventSystem::GetInstance()->DeregisterForEvent(*this, *m_eLockCamera);
	CEventSystem::GetInstance()->DeregisterForEvent(*this, *m_eFollowCamera);
	CEventSystem::GetInstance()->DeregisterForEvent(*this, *m_eSplitEvent);
	CEventSystem::GetInstance()->DeregisterForEvent(*this, *m_eMergeEvent);
	CEventSystem::GetInstance()->DeregisterForEvent(*this, *m_eShootingGal);
}

void CCamera::React(const tDispatch& toReactTo)
{
	char buffer[16];
	strcpy_s(buffer, 16,((tDispatch*)(&toReactTo))->m_pEvent->GetEventName());

	//React to the lock camera event
	if(strcmp(buffer, "LockCamera") == 0)
	{
		m_cState = LOCK;
	}

	//React to the follow camera event
	if(strcmp(buffer, "FollowCamera") == 0)
	{
		m_cState = FOLLOW;
	}

	//Split event just happened
	if(strcmp(buffer, "SplitEvent") == 0)
	{
		m_bSplitPan = true;

		//using the goal object associated with the group and the camera
		CGoalObject* cameraGoal;
		cameraGoal = (CGoalObject*)CObjectManager::GetInstance()->GetGoalObject(*((unsigned int*)(toReactTo.m_buffer)),2);

		m_splitPoint = cameraGoal->GetPosition();
	}

	//Merge event just happened
	if(strcmp(buffer, "MergeEvent") == 0)
	{
		m_cState = FOLLOW;
	}
}

CCamera* CCamera::GetInstance(void)
{
	static CCamera instance;
	return &instance;
}

void CCamera::ShakeCamera(float time)
{
	m_previousCamPos = m_MC.GetWorldMatrix();

	m_fMaxShakeTime = time;
	m_bCameraShake = true;
}
