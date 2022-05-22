#pragma once
#include "CBaseEmitterAttribute.h"
#include <d3dx9math.h>

#include "CEmitter.h"
#include "Helpers.h"

class CAttribRotbyLife : public CBaseEmitterAttribute
{
	friend class CParticleManager;

private:
	float m_fStartRot;
	float m_fEndRot;

public:
	CAttribRotbyLife(CEmitter *pOwner, float fStartRot, float fEndRot) : m_fStartRot(fStartRot), m_fEndRot(fEndRot),
		CBaseEmitterAttribute(EA_ROTBYLIFE, "Rot By Life", pOwner) {}
	~CAttribRotbyLife() {}

	void Initialize(VertexParticle *pParticle)
	{
		pParticle->Rot.x = m_fStartRot + m_pOwner->m_fRotation;
		pParticle->Rot.y = m_fEndRot + m_pOwner->m_fRotation;
	}

	void Update(float fDT) { }

	void Clone(CBaseEmitterAttribute* pAttrib)
	{
		CBaseEmitterAttribute::Clone(pAttrib);

		m_fStartRot = ((CAttribRotbyLife*)pAttrib)->m_fStartRot;
		m_fEndRot = ((CAttribRotbyLife*)pAttrib)->m_fEndRot;
	}

	void ReadXML(TiXmlElement *pElement)
	{
		CBaseEmitterAttribute::ReadXML(pElement);

		TiXmlElement *pStartRot = pElement->FirstChildElement("StartRotation");
		m_fStartRot = Helpers::ConvertStringToFloat(pStartRot->FirstChild()->Value());
		TiXmlElement *pEndRot = pElement->FirstChildElement("EndRotation");
		m_fEndRot = Helpers::ConvertStringToFloat(pEndRot->FirstChild()->Value());
	}
};