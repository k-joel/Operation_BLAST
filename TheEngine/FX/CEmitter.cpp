#include "CEmitter.h"

#include "CAttribAccelbyLife.h"
#include "CAttribBox.h"
#include "CAttribColorByLife.h"
#include "CAttribInitAcc.h"
#include "CAttribInitRot.h"
#include "CAttribInitRotRate.h"
#include "CAttribInitVel.h"
#include "CAttribLifetime.h"
#include "CAttribLine.h"
#include "CAttribOrbit.h"
#include "CAttribRotbyLife.h"
#include "CAttribSize.h"
#include "CAttribSizeByLife.h"
#include "CAttribSpawn.h"
#include "CAttribSphere.h"
#include "CAttribVelbyLife.h"

#include "CBaseEmitterAttribute.h"

#include "CParticleEffect.h"

CEmitter::CEmitter()
{
	m_pAccelbyLife = new CAttribAccelbyLife(this, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	m_pBox = new CAttribBox(this, D3DXVECTOR3(-10.0f, -10.0f, -10.0f), D3DXVECTOR3(10.0f, 10.0f, 10.0f));
	m_pColbylife = new CAttribColorByLife(this, D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f), D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.5f));
	m_pInitAcc = new CAttribInitAcc(this, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	m_pInitRot = new CAttribInitRot(this, 0.0f, 0.0f);
	m_pRotRate = new CAttribInitRotRate(this, 0.0f, 0.0f);
	m_pInitVel = new CAttribInitVel(this, D3DXVECTOR3(20.0f, -5.0f, -5.0f), D3DXVECTOR3(30.0f, 5.0f, 5.0f));
	m_pLifetime = new CAttribLifetime(this, 1.0f, 1.0f);
	m_pLine = new CAttribLine(this, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	m_pOrbit = new CAttribOrbit(this, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f),
		D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	m_pRotbyLife = new CAttribRotbyLife(this, 0.0f, 0.0f);
	m_pInitSize = new CAttribSize(this, 64.0f, 64.0f);
	m_pSizeByLife = new CAttribSizeByLife(this, 64.0f, 64.0f);
	m_pSpawn = new CAttribSpawn(this, 0.01f, 1);
	m_pSphere = new CAttribSphere(this, D3DXVECTOR3(0.0f, 0.0f, 0.0f), 5.0f, true, true, true, false, false, D3DXVECTOR3(1.0f, 1.0f, 1.0f));
	m_pVelbyLife = new CAttribVelbyLife(this, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	// setup mesh & buffer
	m_Mesh.SetPrimitiveType(D3DPT_POINTLIST);
	m_Mesh.SetPrimitiveCount(m_ParticleList.size());
	m_Mesh.SetTextureID(-1);

	SAFE_RELEASE(m_Mesh.GetVertexBuffer());
	m_Mesh.SetVertexCount(MAX_PARTICLES);
	m_Mesh.SetVertexStride(sizeof(VertexParticle));

	CreateVertexBuffer();

	D3DXMATRIX ident;
	D3DXMatrixIdentity(&ident);
	m_ParticleShape.Init(&m_Mesh, NULL, &CRenderContextManager::GetInstance()->GetParticleAdditiveContext(),
		ident, CRenderShape::ParticleRenderFunc);

	m_bKillOnCompleted = false;
	m_bKillOnDeactivate = false;
	m_bUseFixedBoundingBox = true;
	m_MinPoint = D3DXVECTOR3(-10.0f, -10.0f, -10.0f);
	m_MaxPoint = D3DXVECTOR3(10.0f, 10.0f, 10.0f);
	m_fEmitterDelay = 0.0f;
	m_bDelayFirstLoopOnly = false;
	m_fEmitterDuration = 0.0f;
	m_nLoopCount = 0;
	m_szBlendMode = "AdditiveBlend";
	m_bHUDEffect = false;

	m_mxAttachedMat = NULL;
	m_fRotation = 0.0f;

	m_fDelayTimer = 0.0f;
	m_fDurationTimer = 0.0f;
	m_nLoopCounter = 0;
	m_bIsDeactivated = false;
	m_bIsCompleted = false;
	m_bIsActive = true;

	Reset();
}

CEmitter::~CEmitter()
{
	delete m_pAccelbyLife;
	delete m_pBox;
	delete m_pColbylife;
	delete m_pInitAcc;
	delete m_pInitRot;
	delete m_pRotRate;
	delete m_pInitVel;
	delete m_pLifetime;
	delete m_pLine;
	delete m_pOrbit;
	delete m_pRotbyLife;
	delete m_pInitSize;
	delete m_pSizeByLife;
	delete m_pSpawn;
	delete m_pSphere;
	delete m_pVelbyLife;
}

void CEmitter::Reset()
{
	m_ParticleList.clear();
	for(unsigned int i = 0; i < m_EmitterAttributes.size(); i++)
	{
		m_EmitterAttributes[i]->ResetPrivate();
	}
}

bool CEmitter::Begin(float fDT)
{
	if(m_fEmitterDelay > 0.0f)
	{
		m_fDelayTimer += fDT;
		if(m_fDelayTimer >= m_fEmitterDelay)
		{
			if (!m_bDelayFirstLoopOnly)
				m_fDelayTimer = 0.0f;

			goto l_end;
		}

		return false;
	}

l_end:
	if(m_bIsCompleted && m_bKillOnCompleted && !m_ParticleList.size())
		m_bIsActive = false;

	if(m_fEmitterDuration > 0.0f)
	{
		if(m_fDurationTimer >= m_fEmitterDuration)
		{
			Reset();
			m_fDurationTimer = 0.0f;
			if (m_nLoopCount > 0)
			{
				m_nLoopCounter++;
				if (m_nLoopCounter >= m_nLoopCount)
				{
					m_nLoopCounter = 0;
					m_bIsDeactivated = true;

					if (m_bKillOnDeactivate)
						m_bIsActive = false;
				}
			}
		}
		m_fDurationTimer += fDT;
	}

	if(m_bIsDeactivated)
		return false;

	return true;
}

void CEmitter::AddParticle()
{
	// add a new particle
	VertexParticle pParticle;
	//VertexParticle *pParticle = m_DeadParticleList.back();
	//m_DeadParticleList.pop_back();

	// init all new members
	for(unsigned int i = 0; i < m_EmitterAttributes.size(); i++)
	{
		m_EmitterAttributes[i]->Initialize(&pParticle);
	}

	m_ParticleList.push_back(pParticle);
}

void CEmitter::Render()
{
	if(!m_ParticleList.size()) return;
	if(!m_Mesh.GetVertexBuffer()) return;

	m_Mesh.SetPrimitiveCount(m_ParticleList.size());

	void *pBuffer;
	//VertexParticle *p;
	m_Mesh.GetVertexBuffer()->Lock(0,0, &pBuffer, D3DLOCK_DISCARD);
	//m_Sphere.CreateFromVerts((D3DXVECTOR3*)pBuffer, m_ParticleList.size(), sizeof(VertexParticle));
	//for(int i = 0; i < m_ParticleList.size(); i++)
	//	p[i] = *m_ParticleList[i];
	memcpy(pBuffer, &m_ParticleList[0], m_ParticleList.size()*sizeof(VertexParticle));
	m_Mesh.GetVertexBuffer()->Unlock();

	m_ParticleShape.GetRenderContext()->AddRenderNode(&m_ParticleShape);
}

void CEmitter::Update( float fDT )
{
	for(unsigned int i = 0; i < m_EmitterAttributes.size(); i++)
	{
		m_EmitterAttributes[i]->Update(fDT);
	}
}

bool CEmitter::Collision( CBaseBV *pBV )
{
	/*if(!m_ParticleList.size()) return false;

	m_AABB.CreateFromVerts(&m_ParticleList[0].Pos, m_ParticleList.size(), sizeof(VertexParticle));
	return m_AABB.Test(pBV);*/

	//return m_Sphere.Test(pBV);
	return false;
}

void CEmitter::CreateVertexBuffer()
{
	CDirect3D::GetInstance()->GetDirect3DDevice()->CreateVertexBuffer(m_Mesh.GetVertexCount()*m_Mesh.GetVertexStride(),
		D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY | D3DUSAGE_POINTS,
		0,
		D3DPOOL_DEFAULT,
		&m_Mesh.GetVertexBuffer(),
		0);
}