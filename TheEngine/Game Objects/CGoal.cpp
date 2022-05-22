#include "CGoal.h"

CGoalObject::CGoalObject()
{
	SetType(OBJ_GOAL);
	SetIsAffectedByGrav(false);

	m_goalID = 0;
	m_targetID = 0;
}

CGoalObject::~CGoalObject()
{
}

void CGoalObject::Update(float fElapsedTime)
{
}

void CGoalObject::Render()
{
}

bool CGoalObject::CheckCollision(CBase* pBase)
{
	return CBase::CheckCollision(pBase);
}

void CGoalObject::CollisionReact(CBase* pBase)
{
}