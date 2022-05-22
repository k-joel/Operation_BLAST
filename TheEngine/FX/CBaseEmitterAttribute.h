#pragma once
#include<string>
using namespace std;
#include "../TinyXML/tinyxml.h"
#include "../Rendering/Vertex.h"
#include "CEmitter.h"
#include "Helpers.h"

enum eAttribTypes { EA_BASE, EA_ACCELBYLIFE, EA_BOX, EA_COLBYLIFE, EA_EMITINITLOC, EA_INITACC, EA_INITROT,
				EA_INITROTRATE, EA_INITVEL, EA_LIFETIME, EA_LINE, EA_ORBIT, EA_ROTBYLIFE, EA_SIZE, EA_SIZEBYLIFE,
				EA_SPAWN, EA_SPHERE, EA_VELBYLIFE, EA_MAX};

class CBaseEmitterAttribute
{
	friend class CParticleManager;
	friend class CEmitter;
protected:

	string m_szAttributeName;
	int m_nType;
	CEmitter *m_pOwner;

public:
	CBaseEmitterAttribute(int nType, string szName, CEmitter *pOwner) : m_nType(nType), m_szAttributeName(szName),
		m_pOwner(pOwner) {}

	~CBaseEmitterAttribute() {}

	virtual void ResetPrivate() {}
	virtual void Initialize(VertexParticle *pParticle) {}
	virtual void Update(float fDT) {}

	virtual void Clone(CBaseEmitterAttribute* pAttrib)
	{
		m_pOwner->m_EmitterAttributes.push_back(this);
	}

	virtual void ReadXML(TiXmlElement *pElement)
	{
		m_pOwner->m_EmitterAttributes.push_back(this);
	}
};