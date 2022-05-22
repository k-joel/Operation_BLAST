#pragma once
#include "CBaseEmitterAttribute.h"

#include "CEmitter.h"
#include "Helpers.h"

class CAttribLifetime : public CBaseEmitterAttribute
{
	friend class CParticleManager;

private:
	float m_fMinLife;
	float m_fMaxLife;

public:
	CAttribLifetime(CEmitter *pOwner, float fMinLife, float fMaxLife) : m_fMinLife(fMinLife), m_fMaxLife(fMaxLife),
		CBaseEmitterAttribute(EA_LIFETIME, "Lifetime", pOwner) {}
	~CAttribLifetime() {}

	void Initialize(VertexParticle *pParticle)
	{
		pParticle->Life.x = 0.0f;
		pParticle->Life.y = Helpers::GetRandomFloat(m_fMinLife, m_fMaxLife);
	}

	void Update(float fDT)
	{
		for(std::vector<VertexParticle>::iterator iter = m_pOwner->m_ParticleList.begin();
			iter != m_pOwner->m_ParticleList.end(); )
		{
			(*iter).Life.x += fDT;

			if((*iter).Life.x >= (*iter).Life.y)
				iter = m_pOwner->m_ParticleList.erase(iter);
			else
				iter++;
		}
	}

	void Clone(CBaseEmitterAttribute* pAttrib)
	{
		CBaseEmitterAttribute::Clone(pAttrib);

		m_fMinLife = ((CAttribLifetime*)pAttrib)->m_fMinLife;
		m_fMaxLife = ((CAttribLifetime*)pAttrib)->m_fMaxLife;
	}

	void ReadXML(TiXmlElement *pElement)
	{
		CBaseEmitterAttribute::ReadXML(pElement);

		TiXmlElement *pMinLife = pElement->FirstChildElement("MinLife");
		m_fMinLife = Helpers::ConvertStringToFloat(pMinLife->FirstChild()->Value());
		TiXmlElement *pMaxLife = pElement->FirstChildElement("MaxLife");
		m_fMaxLife = Helpers::ConvertStringToFloat(pMaxLife->FirstChild()->Value());
	}
};