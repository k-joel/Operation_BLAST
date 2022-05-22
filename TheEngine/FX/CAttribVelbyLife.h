#pragma once
#include "CBaseEmitterAttribute.h"
#include <d3dx9math.h>

#include "CEmitter.h"
#include "Helpers.h"

class CAttribVelbyLife : public CBaseEmitterAttribute
{
	friend class CParticleManager;

private:
	D3DXVECTOR3 m_StartVelocity;
	D3DXVECTOR3 m_EndVelocity;

public:
	CAttribVelbyLife(CEmitter *pOwner, D3DXVECTOR3 startVel, D3DXVECTOR3 endVel) : m_StartVelocity(startVel), m_EndVelocity(endVel),
		CBaseEmitterAttribute(EA_VELBYLIFE, "Vel By Life", pOwner) {}
	~CAttribVelbyLife() {}

	void Initialize(VertexParticle *pParticle)
	{
		pParticle->StartVel =  m_StartVelocity;
		pParticle->EndVel = m_EndVelocity;

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

		m_StartVelocity = ((CAttribVelbyLife*)pAttrib)->m_StartVelocity;
		m_EndVelocity = ((CAttribVelbyLife*)pAttrib)->m_EndVelocity;
	}

	void ReadXML(TiXmlElement *pElement)
	{
		CBaseEmitterAttribute::ReadXML(pElement);

		TiXmlElement *pStartVel = pElement->FirstChildElement("StartVelocity");
		D3DXVECTOR3 start, end;
		start.x = Helpers::ConvertStringToFloat(pStartVel->Attribute("X"));
		start.y = Helpers::ConvertStringToFloat(pStartVel->Attribute("Y"));
		start.z = Helpers::ConvertStringToFloat(pStartVel->Attribute("Z"));
		m_StartVelocity = start;

		TiXmlElement *pEndVel = pElement->FirstChildElement("EndVelocity");
		end.x = Helpers::ConvertStringToFloat(pEndVel->Attribute("X"));
		end.y = Helpers::ConvertStringToFloat(pEndVel->Attribute("Y"));
		end.z = Helpers::ConvertStringToFloat(pEndVel->Attribute("Z"));
		m_EndVelocity = end;
	}
};