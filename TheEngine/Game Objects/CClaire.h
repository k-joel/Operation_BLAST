#ifndef _CLAIRE_H
#define _CLAIRE_H

#include "CPlayer.h"

#include "../Resources/Sound/Wwise_IDs.h"
#include "../Sound/WwiseManager.h"

class CClaire : public CPlayer
{
private:
	//AkSoundPosition pos;
public:

	CClaire();
	~CClaire();

	void Update(float fElapsedTime);
};

#endif