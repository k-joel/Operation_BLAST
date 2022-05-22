#pragma once
#include "CBaseEmitterAttribute.h"
#include <d3dx9math.h>

class CAttribColorByLife : public CBaseEmitterAttribute
{
	friend class CParticleManager;

private:
	D3DCOLOR m_StartColor;
	D3DCOLOR m_EndColor;

public:
	CAttribColorByLife(CEmitter *pOwner, D3DXCOLOR startCol, D3DXCOLOR endCol) : m_StartColor(startCol), m_EndColor(endCol),
		CBaseEmitterAttribute(EA_COLBYLIFE, "Color By Life", pOwner) {}
	~CAttribColorByLife() {}

	void Initialize(VertexParticle *pParticle)
	{
		pParticle->StartColor = m_StartColor;
		pParticle->EndColor = m_EndColor;
	}

	void Update(float fDT)
	{
	}

	void Clone(CBaseEmitterAttribute* pAttrib)
	{
		CBaseEmitterAttribute::Clone(pAttrib);

		m_StartColor = ((CAttribColorByLife*)pAttrib)->m_StartColor;
		m_EndColor = ((CAttribColorByLife*)pAttrib)->m_EndColor;
	}

	void ReadXML(TiXmlElement *pElement)
	{
		CBaseEmitterAttribute::ReadXML(pElement);

		TiXmlElement *pStartCol = pElement->FirstChildElement("StartColor");
		D3DXCOLOR start, end;
		int sA = Helpers::ConvertStringToInt(pStartCol->Attribute("A"));
		int sR = Helpers::ConvertStringToInt(pStartCol->Attribute("R"));
		int sG = Helpers::ConvertStringToInt(pStartCol->Attribute("G"));
		int sB = Helpers::ConvertStringToInt(pStartCol->Attribute("B"));
		m_StartColor = D3DCOLOR_ARGB(sA, sR, sG, sB);

		TiXmlElement *pEndCol = pElement->FirstChildElement("EndColor");
		int eA = Helpers::ConvertStringToInt(pEndCol->Attribute("A"));
		int eR = Helpers::ConvertStringToInt(pEndCol->Attribute("R"));
		int eG = Helpers::ConvertStringToInt(pEndCol->Attribute("G"));
		int eB = Helpers::ConvertStringToInt(pEndCol->Attribute("B"));
		m_EndColor = D3DCOLOR_ARGB(eA, eR, eG, eB);
	}
};