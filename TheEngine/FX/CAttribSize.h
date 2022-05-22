#pragma once
#include "CBaseEmitterAttribute.h"

#include "CEmitter.h"
#include "Helpers.h"

class CAttribSize : public CBaseEmitterAttribute
{
	friend class CParticleManager;
private:
	float m_fMinSize;
	float m_fMaxSize;

public:
	CAttribSize(CEmitter *pOwner, float fMinSize, float fMaxSize) : m_fMinSize(fMinSize), m_fMaxSize(fMaxSize),
		CBaseEmitterAttribute(EA_SIZE, "Initial Size", pOwner) {}
	~CAttribSize() {}

	void Initialize(VertexParticle *pParticle)
	{
		pParticle->Size.x = pParticle->Size.y = Helpers::GetRandomFloat(m_fMinSize, m_fMaxSize);
	}

	void Update(float fDT)	{	}

	void Clone(CBaseEmitterAttribute* pAttrib)
	{
		CBaseEmitterAttribute::Clone(pAttrib);

		m_fMinSize = ((CAttribSize*)pAttrib)->m_fMinSize;
		m_fMaxSize = ((CAttribSize*)pAttrib)->m_fMaxSize;
	}

	void ReadXML(TiXmlElement *pElement)
	{
		CBaseEmitterAttribute::ReadXML(pElement);

		TiXmlElement *pMinSize = pElement->FirstChildElement("MinSize");
		m_fMinSize = Helpers::ConvertStringToFloat(pMinSize->FirstChild()->Value());
		TiXmlElement *pMaxSize = pElement->FirstChildElement("MaxSize");
		m_fMaxSize = Helpers::ConvertStringToFloat(pMaxSize->FirstChild()->Value());
	}
};