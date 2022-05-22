#ifndef _CMERGE_
#define _CMERGE_

#include "../Game Objects/CBase.h"

class CMergeObject : public CBase
{
public:
	unsigned int m_goalID;

	CMergeObject();
	~CMergeObject();

	void Update(float fElapsedTime);

	void Render();

	bool CheckCollision(CBase* pBase);

	void CollisionReact(CBase* pBase);
};
#endif //_LAYOUT_LOADER_