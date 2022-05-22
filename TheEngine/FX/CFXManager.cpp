#include "CFXManager.h"
#include "../Game Objects/CObjectFactory.h"
#include "../Game Objects/CObjectManager.h"
#include "../Mesh/CModelManager.h"
#include "../States/CLoadState.h"
#include "../Rendering/CRenderContextManager.h"
#include "../Rendering/CRenderShape.h"

CFXManager CFXManager::m_Instance;

int CFXManager::sm_ObjectID = 0;

CFXManager::CFXManager()
{
}

CFXManager::~CFXManager()
{
}

CFXManager *CFXManager::GetInstance(void)
{
	return &m_Instance;
}

void CFXManager::InitializeFXManager()
{
// 	enum Effects { FX_SMALL_RED_LAZER, FX_SMALL_BLUE_LAZER, FX_LARGE_LAZER, FX_CHARGED_LAZER, FX_MGUN_MUZZLEFLASH,
// 		FX_SG_MUZZLEFLASH, FX_SG_ALT_MUZZLEFLASH, FX_SG_ULT_MUZZLEFLASH, FX_EXPLOSION_CRATE, FX_EXPLOSION_BARREL,
// 		FX_DIRT_EXPLOSION, FX_ASSAULTER_MUZZLEFLASH, FX_MAX };

	m_pLS = CLoadState::GetInstance();
	m_pMM = CModelManager::GetInstance();
	m_pOM = CObjectManager::GetInstance();
	m_pOF = CObjectFactory::GetInstance();
}

void CFXManager::ShutdownFXManager()
{
	m_mActiveFX.clear();
}

int CFXManager::LoadEffect(int nType, D3DXMATRIX world, float fLife, bool bLooping, CFrame *pOwnerFrame,
						   bool bLerpSphere, bool bLerpAABB)
{
	CEffect *pFX = (CEffect*)m_pOF->GetObject<CEffect>(OBJ_EFFECT);
	pFX->ClearShapes();
	SetupEffect(nType, pFX, world);

	pFX->m_nFXType = nType;
	pFX->m_fLife = fLife;
	pFX->m_bIsLooping = bLooping;

	pFX->m_bLerpSphere = bLerpSphere;
	pFX->m_bLerpAABB = bLerpAABB;
	pFX->m_StartAABB.Clone(&pFX->GetBVAABB());
	pFX->m_StartSphere.Clone(&pFX->GetBVSphere());
	pFX->m_StartAABB.GetAABB().minPt = pFX->m_StartAABB.GetAABB().maxPt = pFX->m_StartAABB.GetAABB().center;
	pFX->m_StartSphere.GetSphere().r = 0.0f;

	pFX->m_EndAABB.Clone(&pFX->GetBVAABB());
	pFX->m_EndSphere.Clone(&pFX->GetBVSphere());

	if(pOwnerFrame)
	{
		if(pOwnerFrame->m_Children.size()) pOwnerFrame->m_Children.pop_back();
		pFX->GetFrame().SetParent(pOwnerFrame);
		pFX->m_pOwnerFrame = pOwnerFrame;
	}

	m_pOM->AddObject(pFX);

	int id = CFXManager::sm_ObjectID++;
	m_mActiveFX[id] = pFX;
	return id;
}

void CFXManager::Update(float fDT)
{
}

void CFXManager::UnloadEffect(int nID)
{
	m_pOM->RemoveObject(m_mActiveFX[nID]);
	map<int, CEffect*>::iterator iter = m_mActiveFX.find(nID);
	m_mActiveFX.erase(iter);
}

void CFXManager::SetupEffect(int nType, CBase *pObj, D3DXMATRIX world)
{
	pObj->SetRenderType(RT_TRANSPARENT);

	switch(nType)
	{
	case FX_SMALL_RED_LAZER:
		pObj->InitShape(m_pMM->GetModel(m_pLS->GetModelID("SmallredlaserShape")),
			&CRenderContextManager::GetInstance()->GetAlphaBlendContext(), world, CRenderShape::PNTRenderFunc, false);
		break;
	case FX_SMALL_BLUE_LAZER:
		pObj->InitShape(m_pMM->GetModel(m_pLS->GetModelID("SmallbluelaserShape")),
			&CRenderContextManager::GetInstance()->GetStaticLitContext(), world, CRenderShape::PNTRenderFunc, false);
		break;
	case FX_LARGE_LAZER:
		pObj->InitShape(m_pMM->GetModel(m_pLS->GetModelID("LargeLaserShape")),
			&CRenderContextManager::GetInstance()->GetStaticLitContext(), world, CRenderShape::PNTRenderFunc, false);
		break;
	case FX_CHARGED_LAZER:
		pObj->InitShape(m_pMM->GetModel(m_pLS->GetModelID("ChargedLaserShape")),
			&CRenderContextManager::GetInstance()->GetAnimatedLitContext(), world, CRenderShape::PNTBBRenderFunc, true);
		break;
	case FX_MGUN_MUZZLEFLASH:
		// Nice Shot! Fuck that guy up!
		pObj->InitShape(m_pMM->GetModel(m_pLS->GetModelID("Muzzle_fireShape")),
			&CRenderContextManager::GetInstance()->GetAlphaBlendContext(), world, CRenderShape::PNTBBRenderFunc, true);
		break;
	case FX_SG_MUZZLEFLASH:
	case FX_SG_ALT_MUZZLEFLASH:
	case FX_SG_ULT_MUZZLEFLASH:
		pObj->InitShape(m_pMM->GetModel(m_pLS->GetModelID("shotgun_blastShape")),
			&CRenderContextManager::GetInstance()->GetAlphaBlendContext(), world, CRenderShape::PNTBBRenderFunc, true);
		break;
	case FX_EXPLOSION_CRATE:
		pObj->InitShape(m_pMM->GetModel(m_pLS->GetModelID("Dirt_explosionAShape")),
			&CRenderContextManager::GetInstance()->GetAlphaBlendContext(), world, CRenderShape::PNTBBRenderFunc, true);
		break;
	case FX_EXPLOSION_BARREL:
		{
		pObj->InitShape(m_pMM->GetModel(m_pLS->GetModelID("largeexplosionShape")),
 			&CRenderContextManager::GetInstance()->GetAlphaBlendContext(), world, CRenderShape::PNTBBRenderFunc, true);
		pObj->GetBVAABB().GetAABB().maxPt = pObj->GetBVAABB().GetAABB().center + D3DXVECTOR3(3.0f, 3.0f, 3.0f);
		pObj->GetBVAABB().GetAABB().minPt = pObj->GetBVAABB().GetAABB().center - D3DXVECTOR3(3.0f, 3.0f, 3.0f);
		break;
		}
	case FX_FT_PILOTLIGHT:
		pObj->InitShape(m_pMM->GetModel(m_pLS->GetModelID("Flame_pilotShape")),
			&CRenderContextManager::GetInstance()->GetAlphaBlendContext(), world, CRenderShape::PNTBBRenderFunc, true);
		break;
	case FX_ASSAULTER_MUZZLEFLASH:
		pObj->InitShape(m_pMM->GetModel(m_pLS->GetModelID("Laser_flashShape")),
			&CRenderContextManager::GetInstance()->GetAlphaBlendContext(), world, CRenderShape::PNTBBRenderFunc, true);
		break;
	case FX_SHADOW_BLOB:
		pObj->InitShape(m_pMM->GetModel(m_pLS->GetModelID("shadow_characterShape")),
			&CRenderContextManager::GetInstance()->GetAlphaBlendContext(), world, CRenderShape::PNTRenderFunc, false);
		break;
	case FX_HAT_STANDIN:
		pObj->InitShape(m_pMM->GetModel(m_pLS->GetModelID("Barron_helmetShape")),
			&CRenderContextManager::GetInstance()->GetAlphaBlendContext(), world, CRenderShape::PNTRenderFunc, false);
		break;
	case FX_SG_ALT_TRAIL:
		pObj->InitShape(m_pMM->GetModel(m_pLS->GetModelID("Shotgun_UltimateShape")),
			&CRenderContextManager::GetInstance()->GetAlphaBlendContext(), world, CRenderShape::PNTRenderFunc, false);
		break;
	case FX_P1_ID:
		pObj->InitShape(m_pMM->GetModel(m_pLS->GetModelID("P1_Indicator_BlueShape")),
			&CRenderContextManager::GetInstance()->GetAlphaBlendContext(), world, CRenderShape::PNTRenderFunc, false);
		break;
	case FX_P2_ID:
		pObj->InitShape(m_pMM->GetModel(m_pLS->GetModelID("P2_Indicator_RedShape")),
			&CRenderContextManager::GetInstance()->GetAlphaBlendContext(), world, CRenderShape::PNTRenderFunc, false);
		break;
	case FX_ASSAULTER_EXPLODE:
		pObj->InitShape(m_pMM->GetModel(m_pLS->GetModelID("AssaulterExplosionShape")),
			&CRenderContextManager::GetInstance()->GetAnimatedLitContext(), world, CRenderShape::PNTBBRenderFunc, true);
		break;
	case FX_HELIOS_EXPLODE:
		pObj->InitShape(m_pMM->GetModel(m_pLS->GetModelID("HeliosExplosionShape")),
			&CRenderContextManager::GetInstance()->GetAnimatedLitContext(), world, CRenderShape::PNTBBRenderFunc, true);
		break;
	case FX_STRIKER_EXPLODE:
		pObj->InitShape(m_pMM->GetModel(m_pLS->GetModelID("StrikerExplosionShape")),
			&CRenderContextManager::GetInstance()->GetAnimatedLitContext(), world, CRenderShape::PNTBBRenderFunc, true);
		break;
	case FX_DROP_TARGET:
		pObj->InitShape(m_pMM->GetModel(m_pLS->GetModelID("XShape")),
			&CRenderContextManager::GetInstance()->GetAlphaBlendContext(), world, CRenderShape::PNTRenderFunc, true);
		break;
	}
}