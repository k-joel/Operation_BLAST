#pragma once
#include <vector>
#include <list>
using std::vector;
using std::list;

#include "../Rendering/CRenderShape.h"
#include "../Rendering/CRenderContext.h"
#include "../Rendering/CRenderContextManager.h"
#include "../Mesh/CMesh.h"

#include "../Collision/CAABB.h"
#include "../Collision/CSphere.h"

#define MAX_PARTICLES 10000

class CBaseEmitterAttribute;
class CAttribAccelbyLife;
class CAttribBox;
class CAttribColorByLife;
class CAttribInitAcc;
class CAttribInitRot;
class CAttribInitRotRate;
class CAttribInitVel;
class CAttribLifetime;
class CAttribLine;
class CAttribOrbit;
class CAttribRotbyLife;
class CAttribSize;
class CAttribSizeByLife;
class CAttribSpawn;
class CAttribSphere;
class CAttribVelbyLife;

class CEmitter
{
	friend class CBaseEmitterAttribute;
	friend class CAttribAccelbyLife;
	friend class CAttribBox;
	friend class CAttribColorByLife;
	friend class CAttribInitAcc;
	friend class CAttribInitRot;
	friend class CAttribInitRotRate;
	friend class CAttribInitVel;
	friend class CAttribLifetime;
	friend class CAttribLine;
	friend class CAttribOrbit;
	friend class CAttribRotbyLife;
	friend class CAttribSize;
	friend class CAttribSizeByLife;
	friend class CAttribSpawn;
	friend class CAttribSphere;
	friend class CAttribVelbyLife;

	friend class CParticleEffect;
	friend class CParticleManager;

	friend class CParticleTrigger;

	friend class CArrivalState;
private:
	CParticleEffect* m_pOwner;

	float m_fDurationTimer;
	int m_nLoopCounter;
	float m_fDelayTimer;

	bool m_bIsActive;
	bool m_bIsDeactivated;
	bool m_bIsCompleted;

	// bv
	CAABB m_AABB;

	// Emitter props
	string m_szEmitterName;
	string m_szTextureName;
	string m_szEffectName;
	bool m_bKillOnCompleted;
	bool m_bKillOnDeactivate;
	bool m_bUseFixedBoundingBox;
	D3DXVECTOR3	m_MinPoint;
	D3DXVECTOR3	m_MaxPoint;
	float m_fEmitterDelay;
	bool m_bDelayFirstLoopOnly;
	float m_fEmitterDuration;
	int m_nLoopCount;
	string m_szBlendMode;
	bool m_bHUDEffect;

private:
	CRenderShape m_ParticleShape;
	CMesh m_Mesh;

	D3DXMATRIX *m_mxAttachedMat;
	float m_fRotation;

	CSphere m_Sphere;

	vector<CBaseEmitterAttribute*> m_EmitterAttributes;

	//VertexParticle m_MaxParticlePool[MAX_PARTICLES];
	//vector<VertexParticle*> m_DeadParticleList;
	vector<VertexParticle> m_ParticleList;

	CAttribAccelbyLife *m_pAccelbyLife;
	CAttribBox *m_pBox;
	CAttribColorByLife *m_pColbylife;
	CAttribInitAcc *m_pInitAcc;
	CAttribInitRot *m_pInitRot;
	CAttribInitRotRate *m_pRotRate;
	CAttribInitVel *m_pInitVel;
	CAttribLifetime *m_pLifetime;
	CAttribLine *m_pLine;
	CAttribOrbit *m_pOrbit;
	CAttribRotbyLife *m_pRotbyLife;
	CAttribSize *m_pInitSize;
	CAttribSizeByLife *m_pSizeByLife;
	CAttribSpawn *m_pSpawn;
	CAttribSphere *m_pSphere;
	CAttribVelbyLife *m_pVelbyLife;

public:
	CEmitter();
	~CEmitter();

	void Reset();

	void AddParticle();

	bool Begin(float fDT);
	void Update(float fDT);
	void Render();
	bool Collision(CBaseBV *pBV);

	CSphere &GetSphere() { return m_Sphere; }
	//CAABB &GetAABB() { return m_AABB; }

	vector<VertexParticle> &GetParticleList() { return m_ParticleList; }

	void CreateVertexBuffer();
};