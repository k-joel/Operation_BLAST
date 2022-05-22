#ifndef _CMOTIONFACTORY_
#define _CMOTIONFACTORY_

#include <vector>
using namespace std;

#include "../Game Objects/CMotion.h"

enum moFact{MOFACT_MAX_MOTION_OBJ = 20};

class CMotionFactory
{
private:
	pair<unsigned int, CMotion> m_Warehouse[MOFACT_MAX_MOTION_OBJ];

	CMotionFactory();
	~CMotionFactory();
public:
	static CMotionFactory* GetInstance()
	{
		static CMotionFactory toReturn;
		return &toReturn;
	}

	void initialize();
	void shutdown();

	CMotion* GetNewMotionObject();

	int GetMotionObjectID(CMotion* toID);
	CMotion* GetMotionObject(int ID);
	void ReleaseMotionObject(CMotion* toRelease);
};

#endif //_CMOTIONFACTORY_