#pragma once
#include "CBaseEmitterAttribute.h"
#include <d3dx9math.h>

#include "CEmitter.h"
#include "Helpers.h"

class CAttribInitVel : public CBaseEmitterAttribute
{
	friend class CParticleManager;

private:
	D3DXVECTOR3 m_MinVelocity;
	D3DXVECTOR3 m_MaxVelocity;

public:
	CAttribInitVel(CEmitter *pOwner, D3DXVECTOR3 minVel, D3DXVECTOR3 maxVel) : m_MinVelocity(minVel), m_MaxVelocity(maxVel),
		CBaseEmitterAttribute(EA_INITVEL, "Initial Velocity", pOwner) {}
	~CAttribInitVel() {}

	void Initialize(VertexParticle *pParticle)
	{
		pParticle->StartVel =  pParticle->EndVel = Helpers::GetRandomVec3(m_MinVelocity, m_MaxVelocity);

		if(m_pOwner->m_mxAttachedMat)
		{
			pParticle->StartVel = Helpers::MultiplyMatrixVector(pParticle->StartVel, *m_pOwner->m_mxAttachedMat);
			pParticle->EndVel = Helpers::MultiplyMatrixVector(pParticle->EndVel, *m_pOwner->m_mxAttachedMat);
		}
	}

	void Update(float fDT) { }

	void Clone(CBaseEmitterAttribute* pAttrib)
	{
		CBaseEmitterAttribute::Clone(pAttrib);

		m_MinVelocity = ((CAttribInitVel*)pAttrib)->m_MinVelocity;
		m_MaxVelocity = ((CAttribInitVel*)pAttrib)->m_MaxVelocity;
	}

	void ReadXML(TiXmlElement *pElement)
	{
		CBaseEmitterAttribute::ReadXML(pElement);

		TiXmlElement *pMinVel = pElement->FirstChildElement("MinVelocity");
		D3DXVECTOR3 min, max;
		min.x = Helpers::ConvertStringToFloat(pMinVel->Attribute("X"));
		min.y = Helpers::ConvertStringToFloat(pMinVel->Attribute("Y"));
		min.z = Helpers::ConvertStringToFloat(pMinVel->Attribute("Z"));
		m_MinVelocity = min;

		TiXmlElement *pMaxVel = pElement->FirstChildElement("MaxVelocity");
		max.x = Helpers::ConvertStringToFloat(pMaxVel->Attribute("X"));
		max.y = Helpers::ConvertStringToFloat(pMaxVel->Attribute("Y"));
		max.z = Helpers::ConvertStringToFloat(pMaxVel->Attribute("Z"));
		m_MaxVelocity = max;
	}
};