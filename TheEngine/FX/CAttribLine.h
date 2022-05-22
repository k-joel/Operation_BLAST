#pragma once
#include "CBaseEmitterAttribute.h"
#include <d3dx9math.h>

#include "CEmitter.h"
#include "Helpers.h"

class CAttribLine : public CBaseEmitterAttribute
{
	friend class CParticleManager;
private:
	D3DXVECTOR3 m_PointA;
	D3DXVECTOR3 m_PointB;

public:
	CAttribLine(CEmitter *pOwner, D3DXVECTOR3 pointA, D3DXVECTOR3 pointB) : m_PointA(pointA), m_PointB(pointB),
		CBaseEmitterAttribute(EA_LINE, "Line", pOwner) {}
	~CAttribLine() {}

	void Initialize(VertexParticle *pParticle)
	{
		D3DXVECTOR3 aToB = m_PointB - m_PointA;
		aToB *= Helpers::GetRandomFloat(0.0f, 1.0f);
		pParticle->Pos = m_PointA + aToB;

		if(m_pOwner->m_mxAttachedMat)
		{
			D3DXVec3TransformCoord(&pParticle->Pos, &pParticle->Pos, m_pOwner->m_mxAttachedMat);
		}
	}

	void Update(float fDT) { }

	void Clone(CBaseEmitterAttribute* pAttrib)
	{
		CBaseEmitterAttribute::Clone(pAttrib);

		m_PointA = ((CAttribLine*)pAttrib)->m_PointA;
		m_PointB = ((CAttribLine*)pAttrib)->m_PointB;
	}

	void ReadXML(TiXmlElement *pElement)
	{
		CBaseEmitterAttribute::ReadXML(pElement);

		TiXmlElement *pPointA = pElement->FirstChildElement("PointA");
		D3DXVECTOR3 a, b;
		a.x = Helpers::ConvertStringToFloat(pPointA->Attribute("X"));
		a.y = Helpers::ConvertStringToFloat(pPointA->Attribute("Y"));
		a.z = Helpers::ConvertStringToFloat(pPointA->Attribute("Z"));
		m_PointA = a;

		TiXmlElement *pPointB = pElement->FirstChildElement("PointB");
		b.x = Helpers::ConvertStringToFloat(pPointB->Attribute("X"));
		b.y = Helpers::ConvertStringToFloat(pPointB->Attribute("Y"));
		b.z = Helpers::ConvertStringToFloat(pPointB->Attribute("Z"));
		m_PointB = b;
	}
};