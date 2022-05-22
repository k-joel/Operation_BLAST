#pragma once
#include "CBaseEmitterAttribute.h"
#include <d3dx9math.h>

#include "CEmitter.h"
#include "Helpers.h"

class CAttribInitRot : public CBaseEmitterAttribute
{
	friend class CParticleManager;

private:
	float m_fMinRot;
	float m_fMaxRot;

public:
	CAttribInitRot(CEmitter *pOwner, float fMinRot, float fMaxRot) : m_fMinRot(fMinRot), m_fMaxRot(fMaxRot),
		CBaseEmitterAttribute(EA_INITROT, "Initial Rotation", pOwner) {}
	~CAttribInitRot() {}

	void Initialize(VertexParticle *pParticle)
	{
		pParticle->Rot.x = pParticle->Rot.y = Helpers::GetRandomFloat(m_fMinRot, m_fMaxRot) + m_pOwner->m_fRotation;
	}

	void Update(float fDT) { }

	void Clone(CBaseEmitterAttribute* pAttrib)
	{
		CBaseEmitterAttribute::Clone(pAttrib);

		m_fMinRot = ((CAttribInitRot*)pAttrib)->m_fMinRot;
		m_fMaxRot = ((CAttribInitRot*)pAttrib)->m_fMaxRot;
	}

	void ReadXML(TiXmlElement *pElement)
	{
		CBaseEmitterAttribute::ReadXML(pElement);

		TiXmlElement *pMinRot = pElement->FirstChildElement("MinRot");
		m_fMinRot = Helpers::ConvertStringToFloat(pMinRot->FirstChild()->Value());
		TiXmlElement *pMaxRot = pElement->FirstChildElement("MaxRot");
		m_fMaxRot = Helpers::ConvertStringToFloat(pMaxRot->FirstChild()->Value());
	}
};