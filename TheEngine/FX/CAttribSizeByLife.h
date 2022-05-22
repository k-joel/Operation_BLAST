#pragma once
#include "CBaseEmitterAttribute.h"

#include "CEmitter.h"
#include "Helpers.h"

class CAttribSizeByLife : public CBaseEmitterAttribute
{
	friend class CParticleManager;

private:
	float m_fStartSize;
	float m_fEndSize;

public:
	CAttribSizeByLife(CEmitter *pOwner, float fStartSize, float fEndSize) : m_fStartSize(fStartSize), m_fEndSize(fEndSize),
		CBaseEmitterAttribute(EA_SIZEBYLIFE, "Size By Life", pOwner) {}
	~CAttribSizeByLife() {}

	void Initialize(VertexParticle *pParticle)
	{
		pParticle->Size.x = m_fStartSize;
		pParticle->Size.y = m_fEndSize;
	}

	void Update(float fDT)	{	}

	void Clone(CBaseEmitterAttribute* pAttrib)
	{
		CBaseEmitterAttribute::Clone(pAttrib);

		m_fStartSize = ((CAttribSizeByLife*)pAttrib)->m_fStartSize;
		m_fEndSize = ((CAttribSizeByLife*)pAttrib)->m_fEndSize;
	}

	void ReadXML(TiXmlElement *pElement)
	{
		CBaseEmitterAttribute::ReadXML(pElement);

		TiXmlElement *pStartSize = pElement->FirstChildElement("StartSize");
		m_fStartSize = Helpers::ConvertStringToFloat(pStartSize->FirstChild()->Value());
		TiXmlElement *pEndSize = pElement->FirstChildElement("EndSize");
		m_fEndSize = Helpers::ConvertStringToFloat(pEndSize->FirstChild()->Value());
	}
};