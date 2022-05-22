#pragma once
#include "CBaseEmitterAttribute.h"
#include <d3dx9math.h>

class CAttribAccelbyLife : public CBaseEmitterAttribute
{
	friend class CParticleManager;

private:
	D3DXVECTOR3 m_StartAcc;
	D3DXVECTOR3 m_EndAcc;

public:
	CAttribAccelbyLife(CEmitter *pOwner, D3DXVECTOR3 startAcc, D3DXVECTOR3 endAcc) : m_StartAcc(startAcc), m_EndAcc(endAcc),
		CBaseEmitterAttribute(EA_ACCELBYLIFE, "Acc By Life", pOwner) {}
	~CAttribAccelbyLife() {}

	void Initialize(VertexParticle *pParticle)
	{
		pParticle->StartAcc = m_StartAcc;
		pParticle->EndAcc = m_EndAcc;

		if(m_pOwner->m_mxAttachedMat)
		{
			pParticle->StartAcc = Helpers::MultiplyMatrixVector(pParticle->StartAcc, *m_pOwner->m_mxAttachedMat);
			pParticle->EndAcc = Helpers::MultiplyMatrixVector(pParticle->EndAcc, *m_pOwner->m_mxAttachedMat);
		}
	}

	void Update(float fDT)
	{
	}

	void Clone(CBaseEmitterAttribute* pAttrib)
	{
		CBaseEmitterAttribute::Clone(pAttrib);

		m_StartAcc = ((CAttribAccelbyLife*)pAttrib)->m_StartAcc;
		m_EndAcc = ((CAttribAccelbyLife*)pAttrib)->m_EndAcc;
	}

	void ReadXML(TiXmlElement *pElement)
	{
		CBaseEmitterAttribute::ReadXML(pElement);

		TiXmlElement *pStartAcc = pElement->FirstChildElement("StartAcceleration");
		D3DXVECTOR3 start, end;
		start.x = Helpers::ConvertStringToFloat(pStartAcc->Attribute("X"));
		start.y = Helpers::ConvertStringToFloat(pStartAcc->Attribute("Y"));
		start.z = Helpers::ConvertStringToFloat(pStartAcc->Attribute("Z"));
		m_StartAcc = start;

		TiXmlElement *pEndAcc = pElement->FirstChildElement("EndAcceleration");
		end.x = Helpers::ConvertStringToFloat(pEndAcc->Attribute("X"));
		end.y = Helpers::ConvertStringToFloat(pEndAcc->Attribute("Y"));
		end.z = Helpers::ConvertStringToFloat(pEndAcc->Attribute("Z"));
		m_EndAcc = end;
	}
};