#ifndef _CDEBRIS_H
#define _CDEBRIS_H

#include "CBase.h"

#include "../Resources/Sound/Wwise_IDs.h"
#include "../Sound/WwiseManager.h"

class CDebris : public CBase
{
private:
	AkSoundPosition pos;
public:
	CDebris();
	~CDebris();

	void Update(float fElapsedTime);
	void CollisionReact(CBase* pBase);
};

#endif