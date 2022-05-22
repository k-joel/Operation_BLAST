//////////////////////////////////////////////////////////////////////////////////////////
//																						//
// File: "CRenderContextManager.h"														//
//																						//
// Author: Kahran Ghosalkar																//
//																						//
// Purpose: To Create, setup and render the render contexts								//
//																						//
//////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include "CRenderShape.h"
#include "CRenderContext.h"

class CRenderContextManager
{
private:
	// Singleton data
	CRenderContextManager();
	~CRenderContextManager();
	CRenderContextManager(const CRenderContextManager &) {}
	CRenderContextManager &operator=(const CRenderContextManager &) {}

	static CRenderContextManager	m_Instance;			// An instance to this class.

	bool m_bRenderBVH;

	// render stuff
	TDirLight	m_tLight; // we only have 1 dir light for the entire game
	float		m_fGamma;

	///////////////////////////////////////////////
	// Render contexts
	CRenderContext m_StaticLitRenderContext;
	CRenderContext m_StaticSceneLitRenderContext;
	CRenderContext m_AnimatedLitTexturedRenderContext;
	CRenderContext m_AlphaTestAnimatedTexLitRenderContext;
	CRenderContext m_WireframeBVRenderContext;
	CRenderContext m_AlphaBlendStaticRenderContext;
	CRenderContext m_AlphaTestStaticRenderContext;
	CRenderContext m_HUDStaticRenderContext;
	CRenderContext m_BGParticleAdditiveRenderContext;
	CRenderContext m_BGParticleTransparencyRenderContext;
	CRenderContext m_ParticleNoBlendRenderContext;
	CRenderContext m_ParticleAdditiveRenderContext;
	CRenderContext m_ParticleTransparencyRenderContext;
	CRenderContext m_HUDParticleNoBlendRenderContext;
	CRenderContext m_HUDParticleAdditiveRenderContext;
	CRenderContext m_HUDParticleTransparencyRenderContext;
	CRenderContext m_StaticUVScrollRenderContext;
	CRenderContext m_StaticUVAtlasRenderContext;
	CRenderContext m_DepthContext;

	// Shader IDs
	int m_nTexLit;
	int m_nSkinnedTexLit;
	int m_nFlat;
	int m_nHUD;
	int m_nTransTexLit;
	int m_nTransSkinnedTexLit;
	int m_nParticle;
	int m_nHUDParticle;
	int m_nParticleBloom;
	int m_nUVScroll;
	int m_nUVAtlas;
	int m_nDepth;
	int m_nShadowMap;

	void LoadShaders();
	void BuildRenderContexts();
	void RenderContexts();
	void ClearContexts();

public:
	///////////////////////////////////////////////
	// Singleton public methods

	// Gets a pointer to the instance
	static CRenderContextManager *GetInstance(void);

	///////////////////////////////////////////////
	// Standard methods

	void Initialize();
	void Render();

	//////////////////////////////////////////////////////////////////////////
	TDirLight &GetLight() { return m_tLight; }
	float &GetGamma() { return m_fGamma; }
	void SetGamma(int nGamma) { m_fGamma = ((float)nGamma / 100.0f) * 2.2f; }

	int GetTexLitFXID()	{ return m_nTexLit; }
	int GetSkinTexLitFXID() {return m_nSkinnedTexLit; }
	int GetFlatFXID() {return m_nFlat; }
	int GetHUDFXID() { return m_nHUD; }
	int GetTransFXID() { return m_nTransTexLit; }
	int GetTransAnimFX() { return m_nTransSkinnedTexLit; }
	int GetParticleFX() { return m_nParticle; }
	int GetHUDParticleFX()	{ return m_nHUDParticle; }
	int GetParticleBloomFX() { return m_nParticleBloom; }
	int GetUVScrollFX()		{ return m_nUVScroll; }
	int GetDepthFX() { return m_nDepth; }
	int GetShadowFX()	{ return m_nShadowMap; }

	void SetRenderBVH(bool bVal) { m_bRenderBVH = bVal; }
	bool GetRenderBVH() { return m_bRenderBVH; }

	///////////////////////////////////////////////
	// Render context accessors
	CRenderContext &GetStaticLitContext()			{ return m_StaticLitRenderContext; }
	CRenderContext &GetStaticSceneLitContext()		{ return m_StaticLitRenderContext; }
	CRenderContext &GetAnimatedLitContext()			{ return m_AnimatedLitTexturedRenderContext; }
	CRenderContext &GetWireframeContext()			{ return m_WireframeBVRenderContext; }
	CRenderContext &GetAlphaBlendContext()			{ return m_AlphaBlendStaticRenderContext; }
	CRenderContext &GetAlphaTestContext()			{ return m_AlphaTestStaticRenderContext; }
	CRenderContext &GetHUDContext()					{ return m_HUDStaticRenderContext; }
	CRenderContext &GetAlphaTestAnimatedContext()	{ return m_AlphaTestAnimatedTexLitRenderContext; }
	CRenderContext &GetParticleNoBlendContext()		{ return m_ParticleNoBlendRenderContext; }
	CRenderContext &GetParticleAdditiveContext()	{ return m_ParticleAdditiveRenderContext; }
	CRenderContext &GetParticleTransparencyContext() { return m_ParticleTransparencyRenderContext; }
	CRenderContext &GetHUDParticleNoBlendContext()	{ return m_HUDParticleNoBlendRenderContext; }
	CRenderContext &GetHUDParticleAdditiveContext() { return m_HUDParticleAdditiveRenderContext; }
	CRenderContext &GetHUDParticleTransparencyContext() { return m_HUDParticleTransparencyRenderContext; }
	CRenderContext &GetStaticUVScrollContext()		{ return m_StaticUVScrollRenderContext; }
	CRenderContext &GetStaticUVAtlasContext()		{ return m_StaticUVAtlasRenderContext; }
	CRenderContext &GetDepthContext()				{ return m_DepthContext; }
	CRenderContext &GetBGParticleAdditiveContext()	{ return m_BGParticleAdditiveRenderContext; }
	CRenderContext &GetBGParticleTransparencyContext()	{ return m_BGParticleTransparencyRenderContext; }
};