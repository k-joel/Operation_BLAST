#ifndef CBARREL_H_
#define CBARREL_H_

#include "CBase.h"

#include "../Resources/Sound/Wwise_IDs.h"
#include "../Sound/WwiseManager.h"

class CBarrel : public CBase
{
private:
	bool m_bExplode;

	AkSoundPosition pos;
	bool m_bAudioPlayExplode;

public:
	CBarrel();
	~CBarrel();

	void Update(float fElapsedTime);
	void Render();

	void Initialize();

	bool CheckCollision(CBase* pBase);
	void CollisionReact(CBase* pBase);

};


#endif