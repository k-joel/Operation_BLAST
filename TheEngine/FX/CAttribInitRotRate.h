#pragma once
#include "CBaseEmitterAttribute.h"
#include <d3dx9math.h>

#include "CEmitter.h"
#include "Helpers.h"

class CAttribInitRotRate : public CBaseEmitterAttribute
{
	friend class CParticleManager;

private:
	float m_fMinRate;
	float m_fMaxRate;

public:
	CAttribInitRotRate(CEmitter *pOwner, float fMinRate, float fMaxRate) : m_fMinRate(fMinRate), m_fMaxRate(fMaxRate),
		CBaseEmitterAttribute(EA_INITROTRATE, "Initial Rot Rate", pOwner) {}
	~CAttribInitRotRate() {}

	void Initialize(VertexParticle *pParticle)
	{
		pParticle->Rot.z = Helpers::GetRandomFloat(m_fMinRate, m_fMaxRate);
	}

	void Update(float fDT) { }

	void Clone(CBaseEmitterAttribute* pAttrib)
	{
		CBaseEmitterAttribute::Clone(pAttrib);

		m_fMinRate = ((CAttribInitRotRate*)pAttrib)->m_fMinRate;
		m_fMaxRate = ((CAttribInitRotRate*)pAttrib)->m_fMaxRate;
	}

	void ReadXML(TiXmlElement *pElement)
	{
		CBaseEmitterAttribute::ReadXML(pElement);

		TiXmlElement *pMinRate = pElement->FirstChildElement("MinRate");
		m_fMinRate = Helpers::ConvertStringToFloat(pMinRate->FirstChild()->Value());
		TiXmlElement *pMaxRate = pElement->FirstChildElement("MaxRate");
		m_fMaxRate = Helpers::ConvertStringToFloat(pMaxRate->FirstChild()->Value());
	}
};