#pragma once
#include <vector>
#include <map>
#include <string>
using namespace std;

#include "CBaseEmitterAttribute.h"
#include "CParticleEffect.h"

enum ParticleEffects { PE_FLAMETHROWER_NORM, PE_LIGHTNING_NORM };

class CParticleManager
{
	friend class CParticleEffect;
private:

	string m_szParticlePath;

	map<string, CParticleEffect*> m_mLoadedParticles;
	list<CParticleEffect*> m_lActiveParticles;

	CParticleEffect* ReadXML(string szFileName );

	// Singleton data
	CParticleManager() : m_szParticlePath("Resources/Particles/") {}
	~CParticleManager() {}
	CParticleManager(const CParticleManager &) {}
	CParticleManager &operator=(const CParticleManager &) {}

	static CParticleManager	m_Instance;			// An instance to this class.

public:

	static CParticleManager *GetInstance(void);

	void LoadParticle( string szName );

	CParticleEffect* SpawnParticle( string szName, D3DXMATRIX worldMat, D3DXMATRIX *pAttached = NULL);

	void Run(float fElapsedTime);

	void RemoveParticles();
	void ShutdownParticleManager();

	void OnLostDevice();
	void OnResetDevice();
};