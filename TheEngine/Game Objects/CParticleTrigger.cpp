#include "CParticleTrigger.h"
#include "../FX/CParticleManager.h"
#include "../Camera/Camera.h"

CParticleTrigger::CParticleTrigger()
{
	m_bIsOn = false;
	m_nType = OBJ_TRIGGER;
	m_nRenderType = RT_DONOTRENDER;
	m_pEffect = NULL;
}

void CParticleTrigger::Update( float fElapsedTime )
{
	int result = CCollisionTests::IntersectFrustrumSphere2(m_Sphere.GetSphere(), CCamera::GetInstance()->GetFrustum());
	if(result && !m_bIsOn)
	{
		D3DXMATRIX center;
		unsigned int i = 0;
		switch (m_nParticleType)
		{
		case PRT_SMOKE:
			D3DXMatrixTranslation(&center, m_AABB.GetAABB().center.x, m_AABB.GetAABB().minPt.y, m_AABB.GetAABB().minPt.z);
			m_pEffect = CParticleManager::GetInstance()->SpawnParticle("BlackSmoke", center);
			break;
		case PRT_BIGSMOKE:
			D3DXMatrixTranslation(&center, m_AABB.GetAABB().center.x-35.0f, m_AABB.GetAABB().minPt.y-140.0f, m_AABB.GetAABB().center.z);
			m_pEffect = CParticleManager::GetInstance()->SpawnParticle("BigBlackSmoke", center);
			break;
		}

		if(m_pEffect)
		{
			m_bIsOn = true;
			for(; i < m_pEffect->GetEmitterList().size(); i++)
			{
				m_pEffect->GetEmitterList()[i]->m_ParticleShape.SetRenderContext(
					&CRenderContextManager::GetInstance()->GetBGParticleTransparencyContext());
			}
		}
	}
	else if(!result && m_bIsOn)
	{
		m_bIsOn = false;
		m_pEffect->SetIsActive(false);
		m_bIsActive = false;
	}
}