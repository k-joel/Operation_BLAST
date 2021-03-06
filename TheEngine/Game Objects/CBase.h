#ifndef CBASE_H_
#define CBASE_H_

#include "IBaseInterface.h"
#include "D3DX9math.h"
#include "../Rendering/CRenderShape.h"
#include "../Collision/CAABB.h"
#include "../Collision/CSphere.h"

#include "../Mesh/animation.h"
#include "../Mesh/CModel.h"

#define COLLISION_EPSILON 1.5f
#define JUMP_EPSILON COLLISION_EPSILON+0.1f

const D3DXVECTOR3 g_Gravity = D3DXVECTOR3(0.0f, -300.0f, 0.0f);

enum ObjectType
{	OBJ_BASE, OBJ_SCENE, OBJ_SPLIT, OBJ_MERGE, OBJ_AUDIO_TRIGGER, OBJ_GOAL, OBJ_TRIGGER,
	OBJ_PLAYER, OBJ_BARRON, OBJ_CLAIRE, OBJ_ENEMY, OBJ_STRIKER, OBJ_ASSAULTER, OBJ_HELIOS, OBJ_ULTIMUS, OBJ_MOTORCYCLE,
	OBJ_PLATFORM, OBJ_GROUND, OBJ_WALL, OBJ_JUMPPAD, OBJ_WORLD, OBJ_PROJECTILE, OBJ_EFFECT,
	OBJ_MACHINE_GUN_BULLET, OBJ_SHOTGUN_BULLET,OBJ_LASER_BULLET, OBJ_ROCKET_BULLET, OBJ_CLASER,OBJ_SMARTLASER, OBJ_ROCKET, OBJ_LIGHTNING,
	OBJ_FLAME, OBJ_FLAME_ALTERNATE, OBJ_FLAME_ULTIMATE, OBJ_CRATE, OBJ_BARREL, OBJ_PICKUP, OBJ_SHOTGUN_PICKUP, OBJ_FLAMETHROWER_PICKUP,
	OBJ_LIGHTNING_PICKUP, OBJ_ROCKET_PICKUP, OBJ_FLAG, OBJ_SHOTGUN_ALTERNATE, OBJ_SHOTGUN_ULTIMATE, OBJ_ROCKET_ALTERNATE, OBJ_ROCKET_ULTIMATE,
	OBJ_LIGHTNING_ALTERNATE, OBJ_LIGHTNING_ULTIMATE,
	OBJ_DEBRIS, OBJ_EXPLOSION, OBJ_NAPALM, OBJ_TOPLASER, OBJ_BOTTOMLASER, OBJ_CANNON, OBJ_CONVEYOR, OBJ_BOSSBODY, OBJ_FIZZLE, OBJ_PARTICLETRIGGER, OBJ_MAX };

enum RenderType { RT_OPAQUE, RT_TRANSPARENT, RT_DONOTRENDER, RT_DONOTCULL };

class CLayoutImporter;
class CBase : public IBaseInterface
{
	friend class CLayoutImporter;
public:
	unsigned int m_uiRefCount;	// How many objects have a pointer to me

	vector<CRenderShape> m_Shapes;
	CRenderShape	m_BVShape;

	CSphere m_Sphere;
	CAABB m_AABB;

	CBaseBV *m_pBV;

	// Collision info
	CBase* m_pNextObject;
	int m_nBucket;
	int m_nLevel;

	//Dynamic info
	D3DXVECTOR3 m_vPrevPosition;
	D3DXVECTOR3 m_vPosition;
	D3DXVECTOR3 m_vCenter;
	D3DXVECTOR3 m_vVelocity;
	D3DXVECTOR3 m_vAcceleration;
	float m_fRotationY;
	float m_fRotationZ;
	float m_fRotationX;
	float m_fSpeed;
	float m_fScale;

	D3DXVECTOR3 m_faceNormal;

	// ground collision
	bool m_bIsOnGround;
	bool m_bHitWall;
	// gravity
	bool m_bIsAffectedByGrav;
	//IsEnemy
	bool m_bIsEnemy;
	//IsActive
	bool m_bIsActive;

protected:
	int m_nType;
	int m_nRenderType;
	bool m_bHasAnimation;
	bool m_bIsFacingLeft;

public:
	CBase();
	virtual ~CBase();

	//Accessors
	//AnimationManager* GetAnimationmanager(void)

	D3DXVECTOR3 GetPosition(void)	{ return m_vPosition; }
	D3DXVECTOR3 GetCenter(void)		{ return m_vCenter; }
	D3DXVECTOR3 GetVelocity(void)	{ return m_vVelocity; }
	D3DXVECTOR3 GetAcceleration()	{ return m_vAcceleration; }
	float GetRotationX()		{ return m_fRotationX; }
	float GetRotationY()		{ return m_fRotationY; }
	float GetRotationZ()		{ return m_fRotationZ; }
	float GetSpeed()			{ return m_fSpeed; }
	bool GetIsEnemy(void)		{ return m_bIsEnemy; }
	bool GetIsActive(void)		{ return m_bIsActive; }
	int GetType(void)			{ return m_nType; }
	int GetRenderType()			{ return m_nRenderType; }
	bool GetHasAnimation(void)	{ return m_bHasAnimation; }
	bool GetIsFacingLeft(void)	{ return m_bIsFacingLeft; }
	CBaseBV *GetBV()			{ return m_pBV; }
	CAABB &GetBVAABB()			{ return m_AABB; }
	CSphere &GetBVSphere()		{ return m_Sphere; }
	bool GetIsOnGround()		{ return m_bIsOnGround; }
	bool GetHitWall()			{ return m_bHitWall; }
	float GetScale()			{ return m_fScale; }
	bool GetIsAffectedByGrav()	{ return m_bIsAffectedByGrav; }

	// Mutators
	void SetPosition(D3DXVECTOR3 vPosition)			{ m_vPosition	= vPosition; }
	void SetPosition(float fX, float fY, float fZ)	{ m_vPosition = D3DXVECTOR3(fX, fY , fZ); }
	void SetCenter(D3DXVECTOR3 vCenter)				{ m_vCenter		= vCenter;   }
	void SetVelocity(D3DXVECTOR3 vVelocity)			{ m_vVelocity	= vVelocity; }
	void SetVelocity(float fX, float fY, float fZ)	{ m_vVelocity = D3DXVECTOR3(fX, fY, fZ); }
	void SetAcceleration(D3DXVECTOR3 vAccel)		{ m_vAcceleration = vAccel; }
	void SetRotationY(float fRot)					{ m_fRotationY = fRot; }
	void SetRotationZ(float fRot)					{ m_fRotationZ = fRot; }
	void SetRotationX(float fRot)					{ m_fRotationX = fRot; }
	void SetSpeed(float fSpeed)					{ m_fSpeed = fSpeed; }
	void SetIsEnemy(bool bIsEnemy)				{ m_bIsEnemy	= bIsEnemy;  }
	void SetIsActive(bool bIsActive)			{ m_bIsActive	= bIsActive; }
	void SetType(int nType)						{ m_nType		= nType; }
	void SetRenderType(int nRType)				{ m_nRenderType = nRType; }
	void SetHasAnimation(bool bHasAnimation)	{ m_bHasAnimation = bHasAnimation; }
	void SetIsFacingLeft(bool bIsFacingLeft)	{ m_bIsFacingLeft = bIsFacingLeft; }
	void SetIsOnGround(bool bOnGround)			{ m_bIsOnGround = bOnGround; }
	void SetHitWall(bool bHitWall)				{ m_bHitWall = bHitWall; }
	void SetScale(float fScale)						{ m_fScale = fScale; }
	void SetIsAffectedByGrav(bool bGrav)		{ m_bIsAffectedByGrav = bGrav; }

	//
	CBase* GetNextObject() const { return m_pNextObject; }
	void SetNextObject(CBase* val) { m_pNextObject = val; }
	int GetBucket() const { return m_nBucket; }
	void SetBucket(int val) { m_nBucket = val; }
	int GetLevel() const { return m_nLevel; }
	void SetLevel(int val) { m_nLevel = val; }

	/************************************************************************/
	/* Initializes all data members                                         */
	/************************************************************************/
	virtual void Initialize();

	/************************************************************************/
	/* Shutdown and release memory                                          */
	/************************************************************************/
	virtual void Shutdown();

	///////////////////////////////////////////////////////////////////////////////////////////
	// Function: Update
	//
	// Purpose: Perform logic to update object over time
	//			Put input code at the top of update if you need it
	//////////////////////////////////////////////////////////////////////////////////////////
	virtual void Update(float fElapsedTime);

	///////////////////////////////////////////////////////////////////////////////////////////
	// Function: Render
	//
	// Purpose: Draw the object in world space
	//
	//////////////////////////////////////////////////////////////////////////////////////////
	virtual void Render();

	///////////////////////////////////////////////////////////////////////////////////////////
	// Function: AddRef
	//
	// Purpose: Increase the reference counter
	//
	//////////////////////////////////////////////////////////////////////////////////////////
	void AddRef(void);

	///////////////////////////////////////////////////////////////////////////////////////////
	// Function: Release
	//
	// Purpose: Decrease reference counter, clean up object if there are no references
	//
	//////////////////////////////////////////////////////////////////////////////////////////
	void Release(void);

	virtual bool CheckCollision(CBase* pBase);
	virtual void CollisionReact(CBase* pBase);

	virtual bool CheckSceneCollision(CBase* pBase);
	virtual void SceneCollisionReact(CBase* pBase);

	void InitShape(CModel *model, CRenderContext* renderContext, D3DXMATRIX &mxWorld,
		void (*RenderFunc)(CRenderNode&), bool bIsAnimated );
	void AddShape(CModel *model, CRenderContext* renderContext, D3DXMATRIX &mxWorld,
		void (*RenderFunc)(CRenderNode&), bool bIsAnimated );

	void AddToRenderSet();
	void AddToRenderSet(CRenderSet &renderSet);
	virtual void AddToBVRenderSet();
	void SetZDepth(float fDepth);
	void SetRenderContext(CRenderContext &context);
	void SetRenderFunc(void (*RenderFunc)(CRenderNode&));

	CRenderShape &GetShape(int nShape) { return m_Shapes[nShape]; }
	CFrame &GetFrame() { return m_Shapes[0].GetFrame(); }
	CFrame &GetFrameAt(int idx) { return m_Shapes[idx].GetFrame(); }
	CRenderShape &GetBVShape() { return m_BVShape; }
	void ClearShapes() { m_Shapes.clear(); }
};
#endif // CBase_h__