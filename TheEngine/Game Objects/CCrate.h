#ifndef CCRATE_H_
#define CCRATE_H_

#include "CBase.h"

#include "../Resources/Sound/Wwise_IDs.h"
#include "../Sound/WwiseManager.h"

class CCrate : public CBase
{
private:
	float m_fLifeSpan;
	bool m_bExplode;
	bool m_bFloating;

	AkSoundPosition pos;
	bool m_bAudioPlayBreak;

public:
	CCrate();
	~CCrate();

	void Update(float fElapsedTime);
	void Render();
	void Initialize();

	bool CheckCollision(CBase* pBase);
	void CollisionReact(CBase* pBase);
	void CreatePickup(void);

};


#endif