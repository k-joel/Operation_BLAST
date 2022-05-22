#pragma once
#include "CBaseEmitterAttribute.h"
#include <d3dx9math.h>

class CAttribSphere : public CBaseEmitterAttribute
{
	friend class CParticleManager;

private:
	D3DXVECTOR3 m_Center;
	float m_fRadius;
	bool m_bX;
	bool m_bY;
	bool m_bZ;
	bool m_bSurfaceOnly;
	bool m_bVelocity;
	D3DXVECTOR3 m_VelScale;

public:
	CAttribSphere(CEmitter *pOwner, D3DXVECTOR3 center, float fRadius, bool x, bool y, bool z, bool bSurfaceOnly,
		bool bVel, D3DXVECTOR3 velScale) : m_Center(center), m_fRadius(fRadius), m_bX(x), m_bY(y), m_bZ(z),
		m_bSurfaceOnly(bSurfaceOnly), m_bVelocity(bVel), m_VelScale(velScale),
		CBaseEmitterAttribute(EA_SPHERE, "Sphere", pOwner) {}
	~CAttribSphere() {}

	void Initialize(VertexParticle *pParticle)
	{
		float r = m_bSurfaceOnly ? m_fRadius : Helpers::GetRandomFloat(0.0f, m_fRadius);
		float phi = Helpers::GetRandomFloat(0.0f, D3DX_PI);
		float theta = Helpers::GetRandomFloat(0.0f, D3DX_PI*2.0f);
		D3DXVECTOR3 pos;
		pos.x = m_bX ? (float)(r * cos(theta) * sin(phi)) : 0.0f;
		pos.y = m_bY ? (float)(r * sin(theta) * sin(phi)) : 0.0f;
		pos.z = m_bZ ? (float)(r * cos(phi)) : 0.0f;

		pParticle->Pos = m_Center + pos;

		if(m_bVelocity)
		{
			pos.x *= m_VelScale.x;
			pos.y *= m_VelScale.y;
			pos.z *= m_VelScale.z;

			pParticle->StartVel = pParticle->EndVel = pos;
		}

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

		m_Center =		((CAttribSphere*)pAttrib)->m_Center;
		m_fRadius =		((CAttribSphere*)pAttrib)->m_fRadius;
		m_bX = 			((CAttribSphere*)pAttrib)->m_bX;
		m_bY = 			((CAttribSphere*)pAttrib)->m_bY;
		m_bZ = 			((CAttribSphere*)pAttrib)->m_bZ;
		m_bSurfaceOnly = ((CAttribSphere*)pAttrib)->m_bSurfaceOnly;
		m_bVelocity =	((CAttribSphere*)pAttrib)->m_bVelocity;
		m_VelScale =	((CAttribSphere*)pAttrib)->m_VelScale;
	}

	void ReadXML(TiXmlElement *pElement)
	{
		CBaseEmitterAttribute::ReadXML(pElement);

		TiXmlElement *pCenter = pElement->FirstChildElement("Center");
		D3DXVECTOR3 center;
		center.x = Helpers::ConvertStringToFloat(pCenter->Attribute("X"));
		center.y = Helpers::ConvertStringToFloat(pCenter->Attribute("Y"));
		center.z = Helpers::ConvertStringToFloat(pCenter->Attribute("Z"));
		m_Center = center;

		TiXmlElement *pRadius = pElement->FirstChildElement("Radius");
		m_fRadius = Helpers::ConvertStringToFloat(pRadius->FirstChild()->Value());

		TiXmlElement *pXAxis = pElement->FirstChildElement("X");
		m_bX = Helpers::ConvertStringToBool(pXAxis->FirstChild()->Value());

		TiXmlElement *pYAxis = pElement->FirstChildElement("Y");
		m_bY = Helpers::ConvertStringToBool(pYAxis->FirstChild()->Value());

		TiXmlElement *pZAxis = pElement->FirstChildElement("Z");
		m_bZ = Helpers::ConvertStringToBool(pZAxis->FirstChild()->Value());

		TiXmlElement *pSurfaceOnly = pElement->FirstChildElement("SurfaceOnly");
		m_bSurfaceOnly = Helpers::ConvertStringToBool(pSurfaceOnly->FirstChild()->Value());

		TiXmlElement *pVelocity = pElement->FirstChildElement("Velocity");
		m_bVelocity = Helpers::ConvertStringToBool(pVelocity->FirstChild()->Value());

		TiXmlElement *pVelScale = pElement->FirstChildElement("VelocityScale");
		m_VelScale.x = Helpers::ConvertStringToFloat(pVelScale->Attribute("X"));
		m_VelScale.y = Helpers::ConvertStringToFloat(pVelScale->Attribute("Y"));
		m_VelScale.z = Helpers::ConvertStringToFloat(pVelScale->Attribute("Z"));
	}
};