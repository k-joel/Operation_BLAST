#include "COBB.h"
#include "CAABB.h"
#include "CSphere.h"
#include "CCapsule.h"

void COBB::CreateFromVerts( D3DXVECTOR3 *pVerts, DWORD dwNumVerts, DWORD dwSize )
{
}

void COBB::Clone( CBaseBV *pBV )
{
}

void COBB::Update( D3DXMATRIX &mat )
{
}

void COBB::CreateMesh( CBaseBV *pBV )
{
}

bool COBB::Test( CBaseBV *pBV )
{
	switch(pBV->GetType())
	{
	case BV_AABB:
		{
		}
		break;
	case BV_SPHERE:
		{
		}
		break;
	case BV_OBB:
		{
		}
		break;
	case BV_CAPSULE:
		{
		}
		break;
	}

	return false;
}