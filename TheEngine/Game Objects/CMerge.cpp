#include "CMerge.h"

CMergeObject::CMergeObject()
{
	SetType(OBJ_MERGE);
	SetIsAffectedByGrav(false);

	m_goalID = 0;
}

CMergeObject::~CMergeObject()
{
}

void CMergeObject::Update(float fElapsedTime)
{
}

void CMergeObject::Render()
{
}

bool CMergeObject::CheckCollision(CBase* pBase)
{
	return CBase::CheckCollision(pBase);
}

void CMergeObject::CollisionReact(CBase* pBase)
{
}