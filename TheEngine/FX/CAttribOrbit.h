#pragma once
#include "CBaseEmitterAttribute.h"
#include <d3dx9math.h>

#include "CEmitter.h"
#include "Helpers.h"

class CAttribOrbit : public CBaseEmitterAttribute
{
	friend class CParticleManager;
private:
	D3DXVECTOR3 m_MinPoint;
	D3DXVECTOR3 m_MaxPoint;
	D3DXVECTOR3 m_MinRot;
	D3DXVECTOR3 m_MaxRot;
	D3DXVECTOR3 m_MinRate;
	D3DXVECTOR3 m_MaxRate;

public:
	CAttribOrbit(CEmitter *pOwner, D3DXVECTOR3 minPt, D3DXVECTOR3 maxPt,
		D3DXVECTOR3 minRot, D3DXVECTOR3 maxRot, D3DXVECTOR3 minRate, D3DXVECTOR3 maxRate) : m_MinPoint(minPt), m_MaxPoint(maxPt),
		m_MinRot(minRot), m_MaxRot(maxRot), m_MinRate(minRate), m_MaxRate(maxRate),
		CBaseEmitterAttribute(EA_ORBIT, "Orbit", pOwner) {}
	~CAttribOrbit() {}

	void Initialize(VertexParticle *pParticle)
	{
		pParticle->OrbitLoc = Helpers::GetRandomVec3(m_MinPoint, m_MaxPoint);
		pParticle->OrbitRot = Helpers::GetRandomVec3(m_MinRot, m_MaxRot);
		pParticle->OrbitRotRate = Helpers::GetRandomVec3(m_MinRate, m_MaxRate);

		if(m_pOwner->m_mxAttachedMat)
		{
			pParticle->OrbitRot = Helpers::MultiplyMatrixVector(pParticle->OrbitRot, *m_pOwner->m_mxAttachedMat);
		}
	}

	void Update(float fDT) { }

	void Clone(CBaseEmitterAttribute* pAttrib)
	{
		CBaseEmitterAttribute::Clone(pAttrib);

		m_MinPoint = ((CAttribOrbit*)pAttrib)->m_MinPoint;
		m_MaxPoint = ((CAttribOrbit*)pAttrib)->m_MaxPoint;
		m_MinRot = ((CAttribOrbit*)pAttrib)->m_MinRot;
		m_MaxRot = ((CAttribOrbit*)pAttrib)->m_MaxRot;
		m_MinRate = ((CAttribOrbit*)pAttrib)->m_MinRate;
		m_MaxRate = ((CAttribOrbit*)pAttrib)->m_MaxRate;
	}

	void ReadXML(TiXmlElement *pElement)
	{
		CBaseEmitterAttribute::ReadXML(pElement);

		TiXmlElement *pMinPt = pElement->FirstChildElement("MinPoint");
		D3DXVECTOR3 minPt, maxPt, minRot, maxRot, minRate, maxRate;
		minPt.x = Helpers::ConvertStringToFloat(pMinPt->Attribute("X"));
		minPt.y = Helpers::ConvertStringToFloat(pMinPt->Attribute("Y"));
		minPt.z = Helpers::ConvertStringToFloat(pMinPt->Attribute("Z"));
		m_MinPoint = minPt;

		TiXmlElement *pMaxPt = pElement->FirstChildElement("MaxPoint");
		maxPt.x = Helpers::ConvertStringToFloat(pMaxPt->Attribute("X"));
		maxPt.y = Helpers::ConvertStringToFloat(pMaxPt->Attribute("Y"));
		maxPt.z = Helpers::ConvertStringToFloat(pMaxPt->Attribute("Z"));
		m_MaxPoint = maxPt;

		TiXmlElement *pMinRot = pElement->FirstChildElement("MinRot");
		minRot.x = Helpers::ConvertStringToFloat(pMinRot->Attribute("X"));
		minRot.y = Helpers::ConvertStringToFloat(pMinRot->Attribute("Y"));
		minRot.z = Helpers::ConvertStringToFloat(pMinRot->Attribute("Z"));
		m_MinRot = minRot;

		TiXmlElement *pMaxRot = pElement->FirstChildElement("MaxRot");
		maxRot.x = Helpers::ConvertStringToFloat(pMaxRot->Attribute("X"));
		maxRot.y = Helpers::ConvertStringToFloat(pMaxRot->Attribute("Y"));
		maxRot.z = Helpers::ConvertStringToFloat(pMaxRot->Attribute("Z"));
		m_MaxRot = maxRot;

		TiXmlElement *pMinRate = pElement->FirstChildElement("MinRate");
		minRate.x = Helpers::ConvertStringToFloat(pMinRate->Attribute("X"));
		minRate.y = Helpers::ConvertStringToFloat(pMinRate->Attribute("Y"));
		minRate.z = Helpers::ConvertStringToFloat(pMinRate->Attribute("Z"));
		m_MinRate = minRate;

		TiXmlElement *pMaxRate = pElement->FirstChildElement("MaxRate");
		maxRate.x = Helpers::ConvertStringToFloat(pMaxRate->Attribute("X"));
		maxRate.y = Helpers::ConvertStringToFloat(pMaxRate->Attribute("Y"));
		maxRate.z = Helpers::ConvertStringToFloat(pMaxRate->Attribute("Z"));
		m_MaxRate = maxRate;
	}
};