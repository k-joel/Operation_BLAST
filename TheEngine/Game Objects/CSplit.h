#ifndef _CSPLIT_
#define _CSPLIT_

#include "../Game Objects/CBase.h"

class CSplitObject : public CBase
{
public:
	unsigned int m_goalID;

	CSplitObject();
	~CSplitObject();

	void Update(float fElapsedTime);

	void Render();

	bool CheckCollision(CBase* pBase);

	void CollisionReact(CBase* pBase);
};
#endif //_LAYOUT_LOADER_