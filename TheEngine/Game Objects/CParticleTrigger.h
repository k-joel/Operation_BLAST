#pragma once

#include "CBase.h"

enum ParticleType {
	PRT_SMOKE = 0,
	PRT_BIGSMOKE = 1,
};

class CParticleEffect;
class CParticleTrigger : public CBase
{
	bool m_bIsOn;
	CParticleEffect *m_pEffect;

public:
	unsigned int m_nParticleType;

	CParticleTrigger();
	~CParticleTrigger() {}

	void Update(float fElapsedTime);
};