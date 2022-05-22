#pragma once
#include "CBaseEmitterAttribute.h"

class CAttribSpawn : public CBaseEmitterAttribute
{
	friend class CParticleManager;

private:
	float m_fSpawnRate;
	unsigned int m_nSpawnMultiplier;
	unsigned int m_nMaxParticles;

	float m_fSpawnRateTimer;
	unsigned int m_nParticleCounter;

public:
	CAttribSpawn(CEmitter *pOwner, float fSpawnRate, unsigned int nSpawnMultiplier) : m_fSpawnRate(fSpawnRate), m_nMaxParticles(0),
		m_nSpawnMultiplier(nSpawnMultiplier), m_fSpawnRateTimer(0.0f), m_nParticleCounter(0),
		CBaseEmitterAttribute(EA_SPAWN, "Spawn", pOwner) {}
	~CAttribSpawn() {}

	void Initialize(VertexParticle *pParticle)
	{
		pParticle->Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		if(m_pOwner->m_mxAttachedMat)
		{
			D3DXVec3TransformCoord(&pParticle->Pos, &pParticle->Pos, m_pOwner->m_mxAttachedMat);
		}
	}

	void ResetPrivate()
	{
		m_fSpawnRateTimer = 0.0f;
		m_nParticleCounter = 0;
	}

	void Update(float fDT)
	{
		if (m_nMaxParticles > 0)
		{
			if (m_nParticleCounter >= m_nMaxParticles)
			{
				m_pOwner->m_bIsCompleted = true;
				m_nParticleCounter = m_nMaxParticles;
			}
		}

		if(m_pOwner->m_bIsCompleted) return;

		m_fSpawnRateTimer += fDT;
		if( m_fSpawnRateTimer >= m_fSpawnRate)
		{
			for(unsigned int i = 0; i < m_nSpawnMultiplier; i++)
			{
				m_nParticleCounter += m_nSpawnMultiplier;
				m_pOwner->AddParticle();
			}

			m_fSpawnRateTimer = 0.0f;
		}
	}

	void Clone(CBaseEmitterAttribute* pAttrib)
	{
		CBaseEmitterAttribute::Clone(pAttrib);

		m_fSpawnRate = ((CAttribSpawn*)pAttrib)->m_fSpawnRate;
		m_nSpawnMultiplier = ((CAttribSpawn*)pAttrib)->m_nSpawnMultiplier;
		m_nMaxParticles = ((CAttribSpawn*)pAttrib)->m_nMaxParticles;
	}

	void ReadXML(TiXmlElement *pElement)
	{
		CBaseEmitterAttribute::ReadXML(pElement);

		TiXmlElement *pSpawnRate = pElement->FirstChildElement("SpawnRate");
		m_fSpawnRate = Helpers::ConvertStringToFloat(pSpawnRate->FirstChild()->Value());
		TiXmlElement *pSpawnMultiplier = pElement->FirstChildElement("SpawnMultiplier");
		m_nSpawnMultiplier = Helpers::ConvertStringToUInt(pSpawnMultiplier->FirstChild()->Value());
		TiXmlElement *pMaxParticles = pElement->FirstChildElement("MaxParticles");
		m_nMaxParticles = Helpers::ConvertStringToUInt(pMaxParticles->FirstChild()->Value());
	}
};