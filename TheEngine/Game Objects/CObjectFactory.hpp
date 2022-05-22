//////////////////////////////////////////////////////////////////////////
#include "../Collision/CollisionObject.h"
template<typename T>
T* CObjectFactory::GetObject(int type)
{
	T* pObj;
	if(m_mObjects[type].size())
	{
		m_mObjectCount[type]++;

		pObj = dynamic_cast<T*>(m_mObjects[type].back());
		m_mObjects[type].pop_back();
	}
	else
	{
		m_mObjectCount[type] = 1;

		pObj = new T();

		D3DXMATRIX matIdentity;
		D3DXMatrixIdentity(&matIdentity);

		CBase* pBase = (CBase*)pObj;
		switch(type)
		{
		case OBJ_PLAYER:
			pBase->InitShape(CModelManager::GetInstance()->GetModel(CLoadState::GetInstance()->GetModelID("barron_geoShape")),
				&CRenderContextManager::GetInstance()->GetAnimatedLitContext(), matIdentity, CRenderShape::PNTBBRenderFunc, true);
			break;
		case OBJ_BARRON:
			pBase->InitShape(CModelManager::GetInstance()->GetModel(CLoadState::GetInstance()->GetModelID("barron_geoShape")),
				&CRenderContextManager::GetInstance()->GetAnimatedLitContext(), matIdentity, CRenderShape::PNTBBRenderFunc, true);

			{
				//////////////////////////////////////////////////////////////////////////
				// add gun
				D3DXMATRIX gunMat, gunTrans, gunScale, gunRot;
				D3DXMatrixTranslation(&gunTrans, -5.0f, 15.0f, -2.0f);
				gunMat = gunTrans;

				pBase->AddShape(CModelManager::GetInstance()->GetModel(CLoadState::GetInstance()->GetModelID("BaronsGunShape")), &CRenderContextManager::GetInstance()->GetStaticLitContext(), gunMat, CRenderShape::PNTRenderFunc, false );
				pBase->GetShape(1).GetFrame().SetParent(&pBase->GetFrame());
				//////////////////////////////////////////////////////////////////////////
			}

			break;
		case OBJ_CLAIRE:
			pBase->InitShape(CModelManager::GetInstance()->GetModel(CLoadState::GetInstance()->GetModelID("Claire_geoShape")),
				&CRenderContextManager::GetInstance()->GetAnimatedLitContext(), matIdentity, CRenderShape::PNTBBRenderFunc, true);

			{
				//////////////////////////////////////////////////////////////////////////
				// add gun
				D3DXMATRIX gunMat, gunTrans, gunScale, gunRot;
				D3DXMatrixTranslation(&gunTrans, -5.0f, 15.0f, -2.0f);
				gunMat =  gunTrans;

				pBase->AddShape(CModelManager::GetInstance()->GetModel(CLoadState::GetInstance()->GetModelID("ClairesGunShape")), &CRenderContextManager::GetInstance()->GetStaticLitContext(), gunMat, CRenderShape::PNTRenderFunc, false );
				pBase->GetShape(1).GetFrame().SetParent(&pBase->GetFrame());
				//////////////////////////////////////////////////////////////////////////
			}

			break;
		case OBJ_STRIKER:
			pBase->InitShape(CModelManager::GetInstance()->GetModel(CLoadState::GetInstance()->GetModelID("robot_geoShape")),
				&CRenderContextManager::GetInstance()->GetAnimatedLitContext(), matIdentity, CRenderShape::PNTBBRenderFunc, true);
			break;
		case OBJ_ULTIMUS:
			{
				pBase->InitShape(CModelManager::GetInstance()->GetModel(CLoadState::GetInstance()->GetModelID("barron_geoShape")),
					&CRenderContextManager::GetInstance()->GetAnimatedLitContext(), matIdentity, CRenderShape::PNTBBRenderFunc, true);
				pBase->GetBVSphere().GetSphere().r *= 0.25f;
			}
			break;
		case OBJ_ASSAULTER:
			pBase->InitShape(CModelManager::GetInstance()->GetModel(CLoadState::GetInstance()->GetModelID("Assaulter_geoShape")),
				&CRenderContextManager::GetInstance()->GetAnimatedLitContext(), matIdentity, CRenderShape::PNTBBRenderFunc, true);
			break;
		case OBJ_HELIOS:
			pBase->InitShape(CModelManager::GetInstance()->GetModel(CLoadState::GetInstance()->GetModelID("Helios_geoShape")),
				&CRenderContextManager::GetInstance()->GetAnimatedLitContext(), matIdentity, CRenderShape::PNTBBRenderFunc, true);
			break;
		case OBJ_MACHINE_GUN_BULLET:
			{
				pBase->InitShape(CModelManager::GetInstance()->GetModel(CLoadState::GetInstance()->GetModelID("bullet_geoShape")),
					&CRenderContextManager::GetInstance()->GetAlphaTestContext(), matIdentity, CRenderShape::PNTRenderFunc, false);
			}
			break;
		case OBJ_FLAG:
			{
				//old flag
				//pBase->InitShape(CModelManager::GetInstance()->GetModel(CLoadState::GetInstance()->GetModelID("victoryFlagShape")),
				//	&CRenderContextManager::GetInstance()->GetStaticLitContext(), matIdentity, CRenderShape::PNTRenderFunc, false);

				//new flag
				pBase->InitShape(CModelManager::GetInstance()->GetModel(CLoadState::GetInstance()->GetModelID("Helicopter_BodyShape")),
					&CRenderContextManager::GetInstance()->GetAlphaTestContext(), matIdentity, CRenderShape::PNTRenderFunc, false);

				pBase->AddShape(CModelManager::GetInstance()->GetModel(CLoadState::GetInstance()->GetModelID("Helicopter_TopPropellerShape")),
					&CRenderContextManager::GetInstance()->GetAlphaTestContext(), matIdentity, CRenderShape::PNTRenderFunc, false );
				pBase->AddShape(CModelManager::GetInstance()->GetModel(CLoadState::GetInstance()->GetModelID("Helicopter_RearPropellerShape")),
					&CRenderContextManager::GetInstance()->GetAlphaTestContext(), matIdentity, CRenderShape::PNTRenderFunc, false );
				pBase->AddShape(CModelManager::GetInstance()->GetModel(CLoadState::GetInstance()->GetModelID("TiresBackLeftShape")),
					&CRenderContextManager::GetInstance()->GetAlphaTestContext(), matIdentity, CRenderShape::PNTRenderFunc, false );
				pBase->AddShape(CModelManager::GetInstance()->GetModel(CLoadState::GetInstance()->GetModelID("TiresBackRightShape")),
					&CRenderContextManager::GetInstance()->GetAlphaTestContext(), matIdentity, CRenderShape::PNTRenderFunc, false );
				pBase->AddShape(CModelManager::GetInstance()->GetModel(CLoadState::GetInstance()->GetModelID("TiresInFrontShape")),
					&CRenderContextManager::GetInstance()->GetAlphaTestContext(), matIdentity, CRenderShape::PNTRenderFunc, false );

				pBase->GetShape(1).GetFrame().SetParent(&pBase->GetFrame());
				pBase->GetShape(2).GetFrame().SetParent(&pBase->GetFrame());
				pBase->GetShape(3).GetFrame().SetParent(&pBase->GetFrame());
				pBase->GetShape(4).GetFrame().SetParent(&pBase->GetFrame());
				pBase->GetShape(5).GetFrame().SetParent(&pBase->GetFrame());
			}
			break;
		case OBJ_SHOTGUN_BULLET:
			pBase->InitShape(CModelManager::GetInstance()->GetModel(CLoadState::GetInstance()->GetModelID("Shotgun_PelletShape")),
				&CRenderContextManager::GetInstance()->GetStaticLitContext(), matIdentity, CRenderShape::PNTRenderFunc, false);
			break;
		case OBJ_ROCKET:
				pBase->InitShape(CModelManager::GetInstance()->GetModel(CLoadState::GetInstance()->GetModelID("MissileShape")),
				&CRenderContextManager::GetInstance()->GetStaticLitContext(), matIdentity, CRenderShape::PNTRenderFunc, false);
			break;
		case OBJ_ROCKET_BULLET:
				pBase->InitShape(CModelManager::GetInstance()->GetModel(CLoadState::GetInstance()->GetModelID("RocketShape")),
				&CRenderContextManager::GetInstance()->GetStaticLitContext(), matIdentity, CRenderShape::PNTRenderFunc, false);
			break;
		case OBJ_ROCKET_ALTERNATE:
				pBase->InitShape(CModelManager::GetInstance()->GetModel(CLoadState::GetInstance()->GetModelID("MissileShape")),
				&CRenderContextManager::GetInstance()->GetStaticLitContext(), matIdentity, CRenderShape::PNTRenderFunc, false);
			break;
		case OBJ_ROCKET_ULTIMATE:
				pBase->InitShape(CModelManager::GetInstance()->GetModel(CLoadState::GetInstance()->GetModelID("MissileShape")),
				&CRenderContextManager::GetInstance()->GetStaticLitContext(), matIdentity, CRenderShape::PNTRenderFunc, false);
			break;
		case OBJ_LASER_BULLET:
			pBase->InitShape(CModelManager::GetInstance()->GetModel(CLoadState::GetInstance()->GetModelID("laser_orbShape")),
				&CRenderContextManager::GetInstance()->GetAlphaTestContext(), matIdentity, CRenderShape::PNTRenderFunc, false);
			break;
		case OBJ_CLASER:
			{
				pBase->InitShape(CModelManager::GetInstance()->GetModel(CLoadState::GetInstance()->GetModelID("bossLaserShape")),
					&CRenderContextManager::GetInstance()->GetStaticUVScrollContext(), matIdentity, CRenderShape::UVScrollPNTRenderFunc,false);
				static TUVScrollData data = { 10.0f, D3DXVECTOR2(1.0f, 0.0f) };
				pBase->GetShape(0).SetData((void*)&data);
			}
			break;
		case OBJ_FIZZLE:
			{
				pBase->InitShape(CModelManager::GetInstance()->GetModel(CLoadState::GetInstance()->GetModelID("bossLaserFizzleShape")),
					&CRenderContextManager::GetInstance()->GetStaticUVScrollContext(), matIdentity, CRenderShape::UVScrollPNTRenderFunc,false);
				static TUVScrollData data = { 5.0f, D3DXVECTOR2(1.0f, 0.0f) };
				pBase->GetShape(0).SetData((void*)&data);
			}
			break;
		case OBJ_SMARTLASER:
			pBase->InitShape(CModelManager::GetInstance()->GetModel(CLoadState::GetInstance()->GetModelID("MissileShape")),
				&CRenderContextManager::GetInstance()->GetStaticLitContext(), matIdentity, CRenderShape::PNTRenderFunc, false);
			break;
		case OBJ_PICKUP:
			pBase->InitShape(CModelManager::GetInstance()->GetModel(CLoadState::GetInstance()->GetModelID("crate_stationary_geoShape")),
				&CRenderContextManager::GetInstance()->GetStaticLitContext(), matIdentity, CRenderShape::PNTRenderFunc, false);
			break;
		case OBJ_SHOTGUN_PICKUP:
			{
				pBase->InitShape(CModelManager::GetInstance()->GetModel(CLoadState::GetInstance()->GetModelID("ShotgunShape")),
					&CRenderContextManager::GetInstance()->GetStaticLitContext(), matIdentity, CRenderShape::PNTRenderFunc, false);
				break;
			}
		case OBJ_LIGHTNING_PICKUP:
			{
				pBase->InitShape(CModelManager::GetInstance()->GetModel(CLoadState::GetInstance()->GetModelID("LightningGunShape")),
					&CRenderContextManager::GetInstance()->GetStaticLitContext(), matIdentity, CRenderShape::PNTRenderFunc, false);
				break;
			}
		case OBJ_FLAMETHROWER_PICKUP:
			{
				pBase->InitShape(CModelManager::GetInstance()->GetModel(CLoadState::GetInstance()->GetModelID("FlameThrowerShape")),
					&CRenderContextManager::GetInstance()->GetStaticLitContext(), matIdentity, CRenderShape::PNTRenderFunc, false);
				break;
			}
		case OBJ_ROCKET_PICKUP:
			{
				pBase->InitShape(CModelManager::GetInstance()->GetModel(CLoadState::GetInstance()->GetModelID("RocketLauncherShape")),
					&CRenderContextManager::GetInstance()->GetStaticLitContext(), matIdentity, CRenderShape::PNTRenderFunc, false);
				break;
			}
		case OBJ_SHOTGUN_ALTERNATE:
		case OBJ_SHOTGUN_ULTIMATE:
			pBase->InitShape(CModelManager::GetInstance()->GetModel(CLoadState::GetInstance()->GetModelID("Shotgun_UltimateShape")),
				&CRenderContextManager::GetInstance()->GetStaticLitContext(), matIdentity, CRenderShape::PNTRenderFunc, false);
			break;
		case OBJ_FLAME:
			{
				pBase->InitShape(CModelManager::GetInstance()->GetModel(CLoadState::GetInstance()->GetModelID("Flame_Burst")),
					&CRenderContextManager::GetInstance()->GetAnimatedLitContext(), matIdentity, CRenderShape::PNTBBRenderFunc, true);

				break;
			}
		case OBJ_FLAME_ALTERNATE:
			{
				pBase->InitShape(CModelManager::GetInstance()->GetModel(CLoadState::GetInstance()->GetModelID("Flame_altShape")),
					&CRenderContextManager::GetInstance()->GetAlphaBlendContext(), matIdentity, CRenderShape::PNTRenderFunc, false);
				break;
			}
		case OBJ_NAPALM:
			{
				pBase->InitShape(CModelManager::GetInstance()->GetModel(CLoadState::GetInstance()->GetModelID("Claire_geoShape")),
					&CRenderContextManager::GetInstance()->GetAlphaBlendContext(), matIdentity, CRenderShape::PNTRenderFunc, false);
				break;
			}
		case OBJ_FLAME_ULTIMATE:
			{
				pBase->InitShape(CModelManager::GetInstance()->GetModel(CLoadState::GetInstance()->GetModelID("Flame_ultShape")),
					&CRenderContextManager::GetInstance()->GetAlphaTestContext(), matIdentity, CRenderShape::PNTRenderFunc, false);
				break;
			}
		case OBJ_DEBRIS:
			{
				pBase->InitShape(CModelManager::GetInstance()->GetModel(CLoadState::GetInstance()->GetModelID("sphereofdoomshape")),
			&CRenderContextManager::GetInstance()->GetStaticLitContext(), matIdentity, CRenderShape::PNTRenderFunc, false);
				break;
			}
		case OBJ_EXPLOSION:
			{
				pBase->InitShape(CModelManager::GetInstance()->GetModel(CLoadState::GetInstance()->GetModelID("Claire_geoShape")),
					&CRenderContextManager::GetInstance()->GetAnimatedLitContext(), matIdentity, CRenderShape::PNTBBRenderFunc, true);

				pBase->AddShape(CModelManager::GetInstance()->GetModel(CLoadState::GetInstance()->GetModelID("largeexplosionShape")),
					&CRenderContextManager::GetInstance()->GetAnimatedLitContext(), matIdentity, CRenderShape::PNTBBRenderFunc, true);
				//pBase->GetShape(1).GetFrame().SetParent(&(pBase->GetFrame()));

				pBase->AddShape(CModelManager::GetInstance()->GetModel(CLoadState::GetInstance()->GetModelID("Dirt_explosionAShape")),
					&CRenderContextManager::GetInstance()->GetAnimatedLitContext(), matIdentity, CRenderShape::PNTBBRenderFunc, true);
				//pBase->GetShape(2).GetFrame().SetParent(&(pBase->GetFrame()));

				pBase->AddShape(CModelManager::GetInstance()->GetModel(CLoadState::GetInstance()->GetModelID("Dirt_explosionBShape")),
					&CRenderContextManager::GetInstance()->GetAnimatedLitContext(), matIdentity, CRenderShape::PNTBBRenderFunc, true);
				//pBase->GetShape(3).GetFrame().SetParent(&(pBase->GetFrame()));

				int wtf = 0;
				break;
			}
		case OBJ_CRATE:
			{
				pBase->InitShape(CModelManager::GetInstance()->GetModel(CLoadState::GetInstance()->GetModelID("CrateShape")),
					&CRenderContextManager::GetInstance()->GetAnimatedLitContext(), matIdentity, CRenderShape::PNTBBRenderFunc, true);
				pBase->AddShape(CModelManager::GetInstance()->GetModel(CLoadState::GetInstance()->GetModelID("Helios_geoShape")),
					&CRenderContextManager::GetInstance()->GetAnimatedLitContext(), matIdentity, CRenderShape::PNTBBRenderFunc, true );
				pBase->GetShape(1).GetFrame().SetParent(&pBase->GetFrame());
				break;
			}
		case OBJ_BARREL:
			{
				pBase->InitShape(CModelManager::GetInstance()->GetModel(CLoadState::GetInstance()->GetModelID("barrelexplodeShape")),
				&CRenderContextManager::GetInstance()->GetAnimatedLitContext(), matIdentity, CRenderShape::PNTBBRenderFunc, true);
				break;
			}
		case OBJ_LIGHTNING:
			{
				pBase->InitShape(CModelManager::GetInstance()->GetModel(CLoadState::GetInstance()->GetModelID("SmallredlaserShape")),
					&CRenderContextManager::GetInstance()->GetAlphaBlendContext(), matIdentity, CRenderShape::PNTRenderFunc, false);
				break;
			}
		case OBJ_LIGHTNING_ALTERNATE:
			{
				pBase->InitShape(CModelManager::GetInstance()->GetModel(CLoadState::GetInstance()->GetModelID("SmallredlaserShape")),
					&CRenderContextManager::GetInstance()->GetAlphaBlendContext(), matIdentity, CRenderShape::PNTRenderFunc, false);
				break;
			}
		case OBJ_LIGHTNING_ULTIMATE:
			{
				pBase->InitShape(CModelManager::GetInstance()->GetModel(CLoadState::GetInstance()->GetModelID("SmallredlaserShape")),
					&CRenderContextManager::GetInstance()->GetAlphaBlendContext(), matIdentity, CRenderShape::PNTRenderFunc, false);
				break;
			}
		case OBJ_TOPLASER:
			{
				pBase->InitShape(CModelManager::GetInstance()->GetModel(CLoadState::GetInstance()->GetModelID("boss_gun_topShape")),
					&CRenderContextManager::GetInstance()->GetAnimatedLitContext(), matIdentity, CRenderShape::PNTBBRenderFunc, true);
			}
			break;
		case OBJ_BOTTOMLASER:
			{
				pBase->InitShape(CModelManager::GetInstance()->GetModel(CLoadState::GetInstance()->GetModelID("boss_gun_bottomShape")),
					&CRenderContextManager::GetInstance()->GetAnimatedLitContext(), matIdentity, CRenderShape::PNTBBRenderFunc, true);
			}
			break;
		case OBJ_CANNON:
			{
				pBase->InitShape(CModelManager::GetInstance()->GetModel(CLoadState::GetInstance()->GetModelID("boss_gun_bigShape")),
					&CRenderContextManager::GetInstance()->GetAnimatedLitContext(), matIdentity, CRenderShape::PNTBBRenderFunc, true);
			}
			break;
		case OBJ_CONVEYOR:
			{
				pBase->InitShape(CModelManager::GetInstance()->GetModel(CLoadState::GetInstance()->GetModelID("boss_conveyorShape")),
					&CRenderContextManager::GetInstance()->GetAnimatedLitContext(), matIdentity, CRenderShape::PNTBBRenderFunc, true);
			}
			break;
		case OBJ_BOSSBODY:
			{
				pBase->InitShape(CModelManager::GetInstance()->GetModel(CLoadState::GetInstance()->GetModelID("boss_bodyShape")),
					&CRenderContextManager::GetInstance()->GetAnimatedLitContext(), matIdentity, CRenderShape::PNTBBRenderFunc, true);
				pBase->GetBVAABB().GetAABB().minPt.x += 15;
			}
			break;
		default:
			break;
		}
	}

	((CBase*)pObj)->Initialize();
	return pObj;
}

template<typename T>
void CObjectFactory::ReturnObject(T* pObj, int type)
{
	((CBase*)pObj)->Shutdown();
	m_mObjectCount[type]--;
	m_mObjects[type].push_back(pObj);
}