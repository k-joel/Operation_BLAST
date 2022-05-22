//////////////////////////////////////////////////////////////////////////////////////////
//																						//
// File: "CRenderContextManager.cpp"														//
//																						//
// Author: Kahran Ghosalkar																//
//																						//
// Purpose: To Create, setup and render the render contexts								//
//																						//
//////////////////////////////////////////////////////////////////////////////////////////

#include "CRenderContextManager.h"
#include "../States/CLoadState.h"
#include "../Camera/Camera.h"

CRenderContextManager CRenderContextManager::m_Instance;

CRenderContextManager::CRenderContextManager()
{
	m_bRenderBVH = false;

	// Light
	m_tLight.m_Ambient = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
	m_tLight.m_Diffuse = D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f);
	m_tLight.m_vDirection = D3DXVECTOR3(0.0f, -1.0f, 0.0f);

	m_fGamma = 1.0f;
}

CRenderContextManager::~CRenderContextManager()
{
}

CRenderContextManager *CRenderContextManager::GetInstance()
{
	return &m_Instance;
}

void CRenderContextManager::Initialize()
{
	LoadShaders();
	BuildRenderContexts();
}

void CRenderContextManager::Render()
{
	RenderContexts();
	ClearContexts();
}

void CRenderContextManager::LoadShaders()
{
	m_nFlat = CShaderManager::GetInstance()->LoadShader("Resources/Shaders/Flat.fx");
	m_nHUD = CShaderManager::GetInstance()->LoadShader("Resources/Shaders/TexturedWVP.fx");
	m_nSkinnedTexLit = CShaderManager::GetInstance()->LoadShader("Resources/Shaders/SkinnedTexturedLit.fx");
	m_nTexLit = CShaderManager::GetInstance()->LoadShader("Resources/Shaders/TexturedLit.fx");
	m_nTransTexLit = CShaderManager::GetInstance()->LoadShader("Resources/Shaders/TransTexturedLit.fx");
	m_nTransSkinnedTexLit = CShaderManager::GetInstance()->LoadShader("Resources/Shaders/TransSkinnedTexturedLit.fx");
	m_nParticle = CShaderManager::GetInstance()->LoadShader("Resources/Shaders/Particle.fx");
	m_nHUDParticle = CShaderManager::GetInstance()->LoadShader("Resources/Shaders/HUDParticle.fx");
	m_nParticleBloom = CShaderManager::GetInstance()->LoadShader("Resources/Shaders/ParticleBloom.fx");
	m_nUVScroll = CShaderManager::GetInstance()->LoadShader("Resources/Shaders/TexturedLitUVScroll.fx");
	m_nUVAtlas = CShaderManager::GetInstance()->LoadShader("Resources/Shaders/TexturedLitUVAtlas.fx");
	m_nDepth = CShaderManager::GetInstance()->LoadShader("Resources/Shaders/Depth.fx");
	m_nShadowMap = CShaderManager::GetInstance()->LoadShader("Resources/Shaders/ShadowMap.fx");
}

void CRenderContextManager::BuildRenderContexts()
{
	// Static objects
	m_StaticLitRenderContext.SetEffect(CShaderManager::GetInstance()->GetEffect(m_nTexLit));
	m_StaticLitRenderContext.RenderFunc = CRenderContext::TexturedLitRenderContextFunc;
	m_StaticLitRenderContext.CreateRenderSet();

	// static scene objects
	m_StaticSceneLitRenderContext.SetEffect(CShaderManager::GetInstance()->GetEffect(m_nTexLit));
	m_StaticSceneLitRenderContext.RenderFunc = CRenderContext::TexturedLitRenderContextFunc;
	m_StaticSceneLitRenderContext.CreateRenderSet();

	// Alpha blended sorted static objects
	m_AlphaBlendStaticRenderContext.SetEffect(CShaderManager::GetInstance()->GetEffect(m_nTransTexLit));
	m_AlphaBlendStaticRenderContext.RenderFunc = CRenderContext::TexturedLitRenderContextFunc;
	m_AlphaBlendStaticRenderContext.CreateRenderSet(true, CRenderSetSorted::ZSortSmaller);
	m_AlphaBlendStaticRenderContext.m_hTechnique = CShaderManager::GetInstance()->GetEffect(m_nTransTexLit)->GetTechniqueByName("AlphaBlendScene");

	m_AlphaTestStaticRenderContext.SetEffect(CShaderManager::GetInstance()->GetEffect(m_nTransTexLit));
	m_AlphaTestStaticRenderContext.RenderFunc = CRenderContext::TexturedLitRenderContextFunc;
	m_AlphaTestStaticRenderContext.CreateRenderSet(true, CRenderSetSorted::ZSortSmaller);
	m_AlphaTestStaticRenderContext.m_hTechnique = CShaderManager::GetInstance()->GetEffect(m_nTransTexLit)->GetTechniqueByName("AlphaTestScene");

	// water uv scrolling
	m_StaticUVScrollRenderContext.SetEffect(CShaderManager::GetInstance()->GetEffect(m_nUVScroll));
	m_StaticUVScrollRenderContext.RenderFunc = CRenderContext::TexUVScrollRenderContextFunc;
	m_StaticUVScrollRenderContext.CreateRenderSet();

	// smoke uv atlas walk
	m_StaticUVAtlasRenderContext.SetEffect(CShaderManager::GetInstance()->GetEffect(m_nUVAtlas));
	m_StaticUVAtlasRenderContext.RenderFunc = CRenderContext::TexUVScrollRenderContextFunc;
	m_StaticUVAtlasRenderContext.CreateRenderSet(true, CRenderSetSorted::ZSortSmaller);

	// Animated objects
	m_AnimatedLitTexturedRenderContext.SetEffect(CShaderManager::GetInstance()->GetEffect(m_nSkinnedTexLit));
	m_AnimatedLitTexturedRenderContext.RenderFunc = CRenderContext::SkinnedTexturedLitRenderContextFunc;
	m_AnimatedLitTexturedRenderContext.CreateRenderSet();

	// Transparent sorted animated objects
	m_AlphaTestAnimatedTexLitRenderContext.SetEffect(CShaderManager::GetInstance()->GetEffect(m_nTransSkinnedTexLit));
	m_AlphaTestAnimatedTexLitRenderContext.RenderFunc = CRenderContext::SkinnedTexturedLitRenderContextFunc;
	m_AlphaTestAnimatedTexLitRenderContext.CreateRenderSet(true, CRenderSetSorted::ZSortSmaller);

	// objects that have their wvp matrix as their world mat ( only the hud )
	m_HUDStaticRenderContext.SetEffect(CShaderManager::GetInstance()->GetEffect(m_nHUD));
	m_HUDStaticRenderContext.RenderFunc = CRenderContext::TexturedWVPRenderContextFunc;
	m_HUDStaticRenderContext.CreateRenderSet();

	// wireframe objects ( for bounding volumes)
	m_WireframeBVRenderContext.SetEffect(CShaderManager::GetInstance()->GetEffect(m_nFlat));
	m_WireframeBVRenderContext.RenderFunc = CRenderContext::WireframeRenderContextFunc;
	m_WireframeBVRenderContext.CreateRenderSet();

	// particle context
	LPD3DXEFFECT particleEffect = CShaderManager::GetInstance()->GetEffect(m_nParticle);
	m_ParticleNoBlendRenderContext.SetEffect(particleEffect);
	m_ParticleNoBlendRenderContext.RenderFunc = CRenderContext::ParticleRenderContextFunc;
	m_ParticleNoBlendRenderContext.CreateRenderSet();
	m_ParticleNoBlendRenderContext.m_hTechnique = particleEffect->GetTechniqueByName("NoBlend");

	m_ParticleAdditiveRenderContext.SetEffect(particleEffect);
	m_ParticleAdditiveRenderContext.RenderFunc = CRenderContext::ParticleRenderContextFunc;
	m_ParticleAdditiveRenderContext.CreateRenderSet();
	m_ParticleAdditiveRenderContext.m_hTechnique = particleEffect->GetTechniqueByName("AdditiveBlend");

	m_ParticleTransparencyRenderContext.SetEffect(particleEffect);
	m_ParticleTransparencyRenderContext.RenderFunc = CRenderContext::ParticleRenderContextFunc;
	m_ParticleTransparencyRenderContext.CreateRenderSet();
	m_ParticleTransparencyRenderContext.m_hTechnique = particleEffect->GetTechniqueByName("TransparencyBlend");

	// bg particles
	m_BGParticleAdditiveRenderContext.SetEffect(particleEffect);
	m_BGParticleAdditiveRenderContext.RenderFunc = CRenderContext::ParticleRenderContextFunc;
	m_BGParticleAdditiveRenderContext.CreateRenderSet();
	m_BGParticleAdditiveRenderContext.m_hTechnique = particleEffect->GetTechniqueByName("AdditiveBlend");

	m_BGParticleTransparencyRenderContext.SetEffect(particleEffect);
	m_BGParticleTransparencyRenderContext.RenderFunc = CRenderContext::ParticleRenderContextFunc;
	m_BGParticleTransparencyRenderContext.CreateRenderSet();
	m_BGParticleTransparencyRenderContext.m_hTechnique = particleEffect->GetTechniqueByName("TransparencyBlend");

	// hud particles
	LPD3DXEFFECT hudParticleEffect = CShaderManager::GetInstance()->GetEffect(m_nHUDParticle);
	m_HUDParticleNoBlendRenderContext.SetEffect(hudParticleEffect);
	m_HUDParticleNoBlendRenderContext.RenderFunc = CRenderContext::HUDParticleRenderContextFunc;
	m_HUDParticleNoBlendRenderContext.CreateRenderSet();
	m_HUDParticleNoBlendRenderContext.m_hTechnique = hudParticleEffect->GetTechniqueByName("NoBlend");

	m_HUDParticleAdditiveRenderContext.SetEffect(hudParticleEffect);
	m_HUDParticleAdditiveRenderContext.RenderFunc = CRenderContext::HUDParticleRenderContextFunc;
	m_HUDParticleAdditiveRenderContext.CreateRenderSet();
	m_HUDParticleAdditiveRenderContext.m_hTechnique = hudParticleEffect->GetTechniqueByName("AdditiveBlend");

	m_HUDParticleTransparencyRenderContext.SetEffect(hudParticleEffect);
	m_HUDParticleTransparencyRenderContext.RenderFunc = CRenderContext::HUDParticleRenderContextFunc;
	m_HUDParticleTransparencyRenderContext.CreateRenderSet();
	m_HUDParticleTransparencyRenderContext.m_hTechnique = hudParticleEffect->GetTechniqueByName("TransparencyBlend");

	// render depth buffer
	LPD3DXEFFECT depthEffect = CShaderManager::GetInstance()->GetEffect(m_nDepth);
	m_DepthContext.SetEffect(depthEffect);
	m_DepthContext.RenderFunc = CRenderContext::DepthRenderContextFunc;
	m_DepthContext.CreateRenderSet();
	m_DepthContext.m_hTechnique = depthEffect->GetTechniqueByName("RenderShadow");
	m_DepthContext.AddRenderNode(&m_StaticLitRenderContext);
}

void CRenderContextManager::RenderContexts()
{
	// Opaques
	m_StaticSceneLitRenderContext.RenderProcess();
	m_StaticLitRenderContext.RenderProcess();
	m_StaticUVScrollRenderContext.RenderProcess();
	m_AnimatedLitTexturedRenderContext.RenderProcess();

	if(m_bRenderBVH)
		m_WireframeBVRenderContext.RenderProcess();

	m_StaticUVAtlasRenderContext.RenderProcess();

	// Transparent
	m_AlphaTestStaticRenderContext.RenderProcess();
	m_AlphaBlendStaticRenderContext.RenderProcess();

	m_AlphaTestAnimatedTexLitRenderContext.RenderProcess();

	m_BGParticleAdditiveRenderContext.RenderProcess();
	m_BGParticleTransparencyRenderContext.RenderProcess();

	m_ParticleNoBlendRenderContext.RenderProcess();
	m_ParticleAdditiveRenderContext.RenderProcess();
	m_ParticleTransparencyRenderContext.RenderProcess();

	m_HUDStaticRenderContext.RenderProcess();

	m_HUDParticleNoBlendRenderContext.RenderProcess();
	m_HUDParticleAdditiveRenderContext.RenderProcess();
	m_HUDParticleTransparencyRenderContext.RenderProcess();
}

void CRenderContextManager::ClearContexts()
{
	m_StaticSceneLitRenderContext.ClearRenderSet();
	m_StaticLitRenderContext.ClearRenderSet();
	m_StaticUVScrollRenderContext.ClearRenderSet();
	m_StaticUVAtlasRenderContext.ClearRenderSet();
	m_AnimatedLitTexturedRenderContext.ClearRenderSet();
	m_WireframeBVRenderContext.ClearRenderSet();
	m_HUDStaticRenderContext.ClearRenderSet();
	m_AlphaTestStaticRenderContext.ClearRenderSet();
	m_AlphaBlendStaticRenderContext.ClearRenderSet();
	m_AlphaTestAnimatedTexLitRenderContext.ClearRenderSet();
	m_BGParticleAdditiveRenderContext.ClearRenderSet();
	m_BGParticleTransparencyRenderContext.ClearRenderSet();
	m_ParticleNoBlendRenderContext.ClearRenderSet();
	m_ParticleAdditiveRenderContext.ClearRenderSet();
	m_ParticleTransparencyRenderContext.ClearRenderSet();
	m_HUDParticleNoBlendRenderContext.ClearRenderSet();
	m_HUDParticleAdditiveRenderContext.ClearRenderSet();
	m_HUDParticleTransparencyRenderContext.ClearRenderSet();
}