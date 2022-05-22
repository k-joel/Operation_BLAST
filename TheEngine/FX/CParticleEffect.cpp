#include "CParticleEffect.h"

CParticleEffect::~CParticleEffect()
{
	for(unsigned int i = 0; i < m_Emitters.size(); i++)
		delete m_Emitters[i];

	m_Emitters.clear();
}

void CParticleEffect::RunEmitterLoop( float fElapsedTime )
{
	for(vector<CEmitter*>::iterator iter = m_Emitters.begin(); iter != m_Emitters.end();)
	{
		if((*iter)->Begin(fElapsedTime))
		{
			(*iter)->Update(fElapsedTime);
			(*iter)->Render();
		}

		if(!(*iter)->m_bIsActive)
		{
			delete (*iter);
			iter = m_Emitters.erase(iter);
		}
		else
			iter++;
	}

	if(!m_Emitters.size())
		m_bIsActive = false;
}

void CParticleEffect::SetWorld( D3DXMATRIX world )
{
	for(unsigned int i = 0; i < m_Emitters.size(); i++)
		m_Emitters[i]->m_ParticleShape.GetFrame().GetLocalMatrix() = world;
}

void CParticleEffect::SetAttached(D3DXMATRIX *pAttach)
{
	for(unsigned int i = 0; i < m_Emitters.size(); i++)
		m_Emitters[i]->m_mxAttachedMat = pAttach;
}

bool CParticleEffect::CheckCollision( CBaseBV *pBV )
{
	for(unsigned int i = 0; i < m_Emitters.size(); i++)
	{
		if(m_Emitters[i]->Collision(pBV))
			return true;
	}

	return false;
}

void CParticleEffect::SetComplete( bool bComplete )
{
	for(unsigned int i = 0; i < m_Emitters.size(); i++)
	{
		m_Emitters[i]->m_bIsCompleted = true;
		m_Emitters[i]->m_bKillOnCompleted = true;
	}
}

void CParticleEffect::SetRotation( float fRot )
{
	for(unsigned int i = 0; i < m_Emitters.size(); i++)
		m_Emitters[i]->m_fRotation = fRot;
}