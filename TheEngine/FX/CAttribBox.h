#pragma once
#include "CBaseEmitterAttribute.h"
#include <d3dx9math.h>

class CAttribBox : public CBaseEmitterAttribute
{
	friend class CParticleManager;

private:
	D3DXVECTOR3 m_MinPoint;
	D3DXVECTOR3 m_MaxPoint;

public:
	CAttribBox(CEmitter *pOwner, D3DXVECTOR3 minPoint, D3DXVECTOR3 maxPoint) : m_MinPoint(minPoint), m_MaxPoint(maxPoint),
		CBaseEmitterAttribute(EA_BOX, "Box", pOwner) {}
	~CAttribBox() {}

	void Initialize(VertexParticle *pParticle)
	{
		pParticle->Pos = Helpers::GetRandomVec3(m_MinPoint, m_MaxPoint);

		if(m_pOwner->m_mxAttachedMat)
		{
			D3DXVec3TransformCoord(&pParticle->Pos, &pParticle->Pos, m_pOwner->m_mxAttachedMat);
		}
	}

	void Update(float fDT)
	{
	}

	void Clone(CBaseEmitterAttribute* pAttrib)
	{
		CBaseEmitterAttribute::Clone(pAttrib);

		m_MinPoint = ((CAttribBox*)pAttrib)->m_MinPoint;
		m_MaxPoint = ((CAttribBox*)pAttrib)->m_MaxPoint;
	}

	void ReadXML(TiXmlElement *pElement)
	{
		CBaseEmitterAttribute::ReadXML(pElement);

		TiXmlElement *pMinPoint = pElement->FirstChildElement("MinPoint");
		D3DXVECTOR3 min, max;
		min.x = Helpers::ConvertStringToFloat(pMinPoint->Attribute("X"));
		min.y = Helpers::ConvertStringToFloat(pMinPoint->Attribute("Y"));
		min.z = Helpers::ConvertStringToFloat(pMinPoint->Attribute("Z"));
		m_MinPoint = min;

		TiXmlElement *pMaxPoint = pElement->FirstChildElement("MaxPoint");
		max.x = Helpers::ConvertStringToFloat(pMaxPoint->Attribute("X"));
		max.y = Helpers::ConvertStringToFloat(pMaxPoint->Attribute("Y"));
		max.z = Helpers::ConvertStringToFloat(pMaxPoint->Attribute("Z"));
		m_MaxPoint = max;
	}
};