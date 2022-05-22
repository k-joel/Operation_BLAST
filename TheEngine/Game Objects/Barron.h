#ifndef _CBARRON_H
#define _CBARRON_H

#include "CPlayer.h"

#include "../Resources/Sound/Wwise_IDs.h"
#include "../Sound/WwiseManager.h"

class CBarron : public CPlayer
{
private:
	//AkSoundPosition pos;
public:

	CBarron();
	~CBarron();

	void Update(float fElapsedTime);
};

#endif