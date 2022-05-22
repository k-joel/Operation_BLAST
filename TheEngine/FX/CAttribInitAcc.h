#pragma once
#include "CBaseEmitterAttribute.h"
#include <d3dx9math.h>

#include "CEmitter.h"
#include "Helpers.h"

class CAttribInitAcc : public CBaseEmitterAttribute
{
	friend class CParticleManager;

private:
	D3DXVECTOR3 m_MinAcceleration;
	D3DXVECTOR3 m_MaxAcceleration;

public:
	CAttribInitAcc(CEmitter *pOwner, D3DXVECTOR3 minAcc, D3DXVECTOR3 maxAcc) : m_MinAcceleration(minAcc), m_MaxAcceleration(maxAcc),
		CBaseEmitterAttribute(EA_INITACC, "Initial Acceleration", pOwner) {}
	~CAttribInitAcc() {}

	void Initialize(VertexParticle *pParticle)
	{
		pParticle->StartAcc = pParticle->EndAcc = Helpers::GetRandomVec3(m_MinAcceleration, m_MaxAcceleration);

		if(m_pOwner->m_mxAttachedMat)
		{
			pParticle->StartAcc = Helpers::MultiplyMatrixVector(pParticle->StartAcc, *m_pOwner->m_mxAttachedMat);
			pParticle->EndAcc = Helpers::MultiplyMatrixVector(pParticle->EndAcc, *m_pOwner->m_mxAttachedMat);
		}
	}

	void Update(float fDT) {}

	void Clone(CBaseEmitterAttribute* pAttrib)
	{
		CBaseEmitterAttribute::Clone(pAttrib);

		m_MinAcceleration = ((CAttribInitAcc*)pAttrib)->m_MinAcceleration;
		m_MaxAcceleration = ((CAttribInitAcc*)pAttrib)->m_MaxAcceleration;
	}

	void ReadXML(TiXmlElement *pElement)
	{
		CBaseEmitterAttribute::ReadXML(pElement);

		TiXmlElement *pMinAcc = pElement->FirstChildElement("MinAcceleration");
		D3DXVECTOR3 min, max;
		min.x = Helpers::ConvertStringToFloat(pMinAcc->Attribute("X"));
		min.y = Helpers::ConvertStringToFloat(pMinAcc->Attribute("Y"));
		min.z = Helpers::ConvertStringToFloat(pMinAcc->Attribute("Z"));
		m_MinAcceleration = min;

		TiXmlElement *pMaxAcc = pElement->FirstChildElement("MaxAcceleration");
		max.x = Helpers::ConvertStringToFloat(pMaxAcc->Attribute("X"));
		max.y = Helpers::ConvertStringToFloat(pMaxAcc->Attribute("Y"));
		max.z = Helpers::ConvertStringToFloat(pMaxAcc->Attribute("Z"));
		m_MaxAcceleration = max;
	}
};