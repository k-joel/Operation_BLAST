#include "CSplit.h"

CSplitObject::CSplitObject()
{
	SetType(OBJ_SPLIT);
	SetIsAffectedByGrav(false);

	m_goalID = 0;
}

CSplitObject::~CSplitObject()
{
}

void CSplitObject::Update(float fElapsedTime)
{
}

void CSplitObject::Render()
{
}

bool CSplitObject::CheckCollision(CBase* pBase)
{
	return CBase::CheckCollision(pBase);
}

void CSplitObject::CollisionReact(CBase* pBase)
{
}