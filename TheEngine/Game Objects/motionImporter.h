#ifndef _MOTION_LOADER_
#define _MOTION_LOADER_

#include "../Game Objects/CMotion.h"

class CMotionImporter
{
private:
	CMotionImporter();
	~CMotionImporter();
public:
	static CMotionImporter* GetInstance()
	{
		static CMotionImporter toReturn;
		return &toReturn;
	}

	void LoadMotion(char* filename, CMotion& toLoadInto);
};

#endif //_MOTION_LOADER_