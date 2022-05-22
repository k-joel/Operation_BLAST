#include "CMotionFactory.h"


CMotionFactory::CMotionFactory()
{}

CMotionFactory::~CMotionFactory()
{}

void CMotionFactory::initialize()
{
	for(unsigned int arrayIndex = 0; arrayIndex < MOFACT_MAX_MOTION_OBJ; arrayIndex +=1)
	{
		m_Warehouse[arrayIndex].first = 0;
	}
}

void CMotionFactory::shutdown()
{
	for(unsigned int arrayIndex = 0; arrayIndex < MOFACT_MAX_MOTION_OBJ; arrayIndex +=1)
	{
		if(m_Warehouse[arrayIndex].first != 0)
		{
			int badShit = 0;
		}
	}
}

CMotion* CMotionFactory::GetNewMotionObject()
{
	for(unsigned int arrayIndex = 0; arrayIndex < MOFACT_MAX_MOTION_OBJ; arrayIndex +=1)
	{
		if(m_Warehouse[arrayIndex].first == 0)
		{
			m_Warehouse[arrayIndex].first = 1;
			return &(m_Warehouse[arrayIndex].second);
		}
	}

	int badShit = 0;

	return NULL;
}

int CMotionFactory::GetMotionObjectID(CMotion* toID)
{
	for(unsigned int arrayIndex = 0; arrayIndex < MOFACT_MAX_MOTION_OBJ; arrayIndex +=1)
	{
		if(toID == &(m_Warehouse[arrayIndex].second))
		{
			return arrayIndex;
		}
	}

	int badShit = 0;

	return -1;
}

CMotion* CMotionFactory::GetMotionObject(int ID)
{

	if(0 < ID && ID < MOFACT_MAX_MOTION_OBJ && m_Warehouse[ID].first != 0)
	{
		m_Warehouse[ID].first += 1;
		return &(m_Warehouse[ID].second);
	}

	int badShit = 0;

	return NULL;
}

void CMotionFactory::ReleaseMotionObject(CMotion* toRelease)
{
	int ID = GetMotionObjectID(toRelease);

	if(ID >= 0)
	{
		m_Warehouse[ID].first -= 1;

		if(m_Warehouse[ID].first < 0)
		{
			int badShit = 0;
		}
	}
}