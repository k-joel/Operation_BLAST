#include "CBaseBV.h"
#include "../Mesh/CMesh.h"

CBaseBV::CBaseBV() : m_nType(BV_BASE), m_pMesh(NULL)
{
}

CBaseBV::~CBaseBV()
{
	SAFE_DELETE(m_pMesh);
}