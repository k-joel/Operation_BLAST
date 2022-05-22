#include "CParticleManager.h"
#include "../TinyXML/tinyxml.h"
#include "../Rendering/CTextureManager.h"
#include "../Rendering/CRenderContextManager.h"
#include "CEmitter.h"
#include "../Debug System/DebugSystem.h"

#include "CAttribAccelbyLife.h"
#include "CAttribBox.h"
#include "CAttribColorByLife.h"
#include "CAttribInitAcc.h"
#include "CAttribInitRot.h"
#include "CAttribInitRotRate.h"
#include "CAttribInitVel.h"
#include "CAttribLifetime.h"
#include "CAttribLine.h"
#include "CAttribOrbit.h"
#include "CAttribRotbyLife.h"
#include "CAttribSize.h"
#include "CAttribSizeByLife.h"
#include "CAttribSpawn.h"
#include "CAttribSphere.h"
#include "CAttribVelbyLife.h"

CParticleManager CParticleManager::m_Instance;

CParticleManager *CParticleManager::GetInstance()
{
	return &m_Instance;
}

CParticleEffect* CParticleManager::ReadXML(string szFileName)
{
	TiXmlDocument doc;

	if(!doc.LoadFile(szFileName.c_str())) return NULL;

	TiXmlElement *pRoot = doc.RootElement();

	if(!pRoot) return NULL;

	CParticleEffect *pPartEffect = new CParticleEffect();
	pPartEffect->m_szName = szFileName;

	int nNumEmits;
	pRoot->Attribute("NumberOfEmitters", &nNumEmits);

	TiXmlElement *pEmitter = pRoot->FirstChildElement("Emitter");

	while(pEmitter)
	{
		CEmitter *emitter = new CEmitter;

		TiXmlElement *pEmitterProperties = pEmitter->FirstChildElement("EmitterProperties");
		TiXmlElement *pEmitName = pEmitterProperties->FirstChildElement("Name");
		emitter->m_szEmitterName = pEmitName->FirstChild()->Value();
		TiXmlElement *pTexName = pEmitterProperties->FirstChildElement("Texture");
		emitter->m_szTextureName = pTexName->FirstChild()->Value();// Value();

		string filename = "Resources/Textures/" + emitter->m_szTextureName;
		emitter->m_Mesh.SetTextureID(CTextureManager::GetInstance()->LoadTexture(filename.c_str()));

		TiXmlElement *pFXName = pEmitterProperties->FirstChildElement("Effect");
		emitter->m_szEffectName = pFXName->FirstChild()->Value();

		TiXmlElement *pKillOnComplete = pEmitterProperties->FirstChildElement("KillOnComplete");
		emitter->m_bKillOnCompleted = Helpers::ConvertStringToBool(pKillOnComplete->FirstChild()->Value());
		TiXmlElement *pKillOnDeactivate = pEmitterProperties->FirstChildElement("KillOnDeactivate");
		emitter->m_bKillOnDeactivate = Helpers::ConvertStringToBool(pKillOnDeactivate->FirstChild()->Value());
		TiXmlElement *pUseFixedBB = pEmitterProperties->FirstChildElement("FixedBoundingBox");
		emitter->m_bUseFixedBoundingBox = Helpers::ConvertStringToBool(pUseFixedBB->FirstChild()->Value());

		TiXmlElement *pBB = pEmitterProperties->FirstChildElement("BoundingBox");
		TiXmlElement *pMin = pBB->FirstChildElement("Min");
		D3DXVECTOR3 min, max;
		min.x = Helpers::ConvertStringToFloat(pMin->Attribute("X"));
		min.y = Helpers::ConvertStringToFloat(pMin->Attribute("Y"));
		min.z = Helpers::ConvertStringToFloat(pMin->Attribute("Z"));
		emitter->m_MinPoint = min;

		TiXmlElement *pMax = pBB->FirstChildElement("Max");
		max.x = Helpers::ConvertStringToFloat(pMax->Attribute("X"));
		max.y = Helpers::ConvertStringToFloat(pMax->Attribute("Y"));
		max.z = Helpers::ConvertStringToFloat(pMax->Attribute("Z"));
		emitter->m_MaxPoint = max;

		TiXmlElement *pEmitterDelay = pEmitterProperties->FirstChildElement("EmitterDelay");
		emitter->m_fEmitterDelay = Helpers::ConvertStringToFloat(pEmitterDelay->FirstChild()->Value());
		TiXmlElement *pDelayFirstLoopOnly = pEmitterProperties->FirstChildElement("DelayFirstLoopOnly");
		emitter->m_bDelayFirstLoopOnly = Helpers::ConvertStringToBool(pDelayFirstLoopOnly->FirstChild()->Value());
		TiXmlElement *pEmitterDuration = pEmitterProperties->FirstChildElement("EmitterDuration");
		emitter->m_fEmitterDuration = Helpers::ConvertStringToFloat(pEmitterDuration->FirstChild()->Value());
		TiXmlElement *pLoopCount = pEmitterProperties->FirstChildElement("LoopCount");
		emitter->m_nLoopCount = Helpers::ConvertStringToInt(pLoopCount->FirstChild()->Value());
		TiXmlElement *pBlendMode = pEmitterProperties->FirstChildElement("BlendMode");
		emitter->m_szBlendMode = pBlendMode->FirstChild()->Value();
		TiXmlElement *pHUDEffect = pEmitterProperties->FirstChildElement("HUDEffect");
		emitter->m_bHUDEffect = Helpers::ConvertStringToBool(pHUDEffect->FirstChild()->Value());

		TiXmlElement *pEmitterAttribList = pEmitter->FirstChildElement("EmitterAttributeList");
		int nNumAttribs;
		pEmitterAttribList->Attribute("NumberOfEmitterAttributes", &nNumAttribs);

		TiXmlElement *pAttributes = pEmitterAttribList->FirstChildElement("EmitterAttribute");

		while(pAttributes)
		{
			string name = pAttributes->Attribute("Name");

			if(name == emitter->m_pAccelbyLife->m_szAttributeName)
				emitter->m_pAccelbyLife->ReadXML(pAttributes);

			else if(name == emitter->m_pBox->m_szAttributeName)
				emitter->m_pBox->ReadXML(pAttributes);

			else if(name == emitter->m_pColbylife->m_szAttributeName)
				emitter->m_pColbylife->ReadXML(pAttributes);

			else if(name == emitter->m_pInitAcc->m_szAttributeName)
				emitter->m_pInitAcc->ReadXML(pAttributes);

			else if(name == emitter->m_pInitRot->m_szAttributeName)
				emitter->m_pInitRot->ReadXML(pAttributes);

			else if(name == emitter->m_pRotRate->m_szAttributeName)
				emitter->m_pRotRate->ReadXML(pAttributes);

			else if(name == emitter->m_pInitVel->m_szAttributeName)
				emitter->m_pInitVel->ReadXML(pAttributes);

			else if(name == emitter->m_pLifetime->m_szAttributeName)
				emitter->m_pLifetime->ReadXML(pAttributes);

			else if(name == emitter->m_pLine->m_szAttributeName)
				emitter->m_pLine->ReadXML(pAttributes);

			else if(name == emitter->m_pOrbit->m_szAttributeName)
				emitter->m_pOrbit->ReadXML(pAttributes);

			else if(name == emitter->m_pRotbyLife->m_szAttributeName)
				emitter->m_pRotbyLife->ReadXML(pAttributes);

			else if(name == emitter->m_pInitSize->m_szAttributeName)
				emitter->m_pInitSize->ReadXML(pAttributes);

			else if(name == emitter->m_pSizeByLife->m_szAttributeName)
				emitter->m_pSizeByLife->ReadXML(pAttributes);

			else if(name == emitter->m_pSpawn->m_szAttributeName)
				emitter->m_pSpawn->ReadXML(pAttributes);

			else if(name == emitter->m_pSphere->m_szAttributeName)
				emitter->m_pSphere->ReadXML(pAttributes);

			else if(name == emitter->m_pVelbyLife->m_szAttributeName)
				emitter->m_pVelbyLife->ReadXML(pAttributes);

			pAttributes = pAttributes->NextSiblingElement("EmitterAttribute");
		}

		D3DXMATRIX ident;
		D3DXMatrixIdentity(&ident);
		emitter->m_ParticleShape.GetFrame().GetLocalMatrix() = ident;
		emitter->m_mxAttachedMat = NULL;
		emitter->m_pOwner = pPartEffect;

		pPartEffect->m_Emitters.push_back(emitter);
		pEmitter = pEmitter->NextSiblingElement("Emitter");
	}

	return pPartEffect;
}

void CParticleManager::Run(float fElapsedTime)
{
	for(list<CParticleEffect*>::iterator iter = m_lActiveParticles.begin();
		iter != m_lActiveParticles.end(); )
	{
		if((*iter)->GetIsActive())
		{
			(*iter)->RunEmitterLoop(fElapsedTime);
			iter++;
		}
		else
		{
			delete (*iter);
			iter = m_lActiveParticles.erase(iter);
		}
	}

	CDebugSystem::GetInstance()->RegisterData("Num Particles: ", (int)m_lActiveParticles.size());
}

void CParticleManager::LoadParticle( string szName )
{
	string file = m_szParticlePath + szName + ".xml";
	CParticleEffect *newParticle = ReadXML(file);
	m_mLoadedParticles.insert(pair<string, CParticleEffect*>(szName, newParticle));
}

CParticleEffect* CParticleManager::SpawnParticle( string szName, D3DXMATRIX worldMat, D3DXMATRIX *pAttached)
{
	if(!m_mLoadedParticles[szName]) return NULL;

	CParticleEffect *pOriginalEffect = m_mLoadedParticles[szName];

	CParticleEffect *pPartEffect = new CParticleEffect();
	pPartEffect->m_szName = szName;

	for(unsigned int i = 0; i < pOriginalEffect->m_Emitters.size(); i++)
	{
		CEmitter *origEmitter = pOriginalEffect->m_Emitters[i];
		CEmitter *emitter = new CEmitter;

		emitter->m_szEmitterName = origEmitter->m_szEmitterName;
		emitter->m_szTextureName = origEmitter->m_szTextureName;
		emitter->m_Mesh.SetTextureID(origEmitter->m_Mesh.GetTextureID());
		emitter->m_szEffectName = origEmitter->m_szEffectName;
		emitter->m_bKillOnCompleted = origEmitter->m_bKillOnCompleted;
		emitter->m_bKillOnDeactivate = origEmitter->m_bKillOnDeactivate;
		emitter->m_bUseFixedBoundingBox = origEmitter->m_bUseFixedBoundingBox;
		emitter->m_MinPoint = origEmitter->m_MinPoint;
		emitter->m_MaxPoint = origEmitter->m_MaxPoint;
		emitter->m_fEmitterDelay = origEmitter->m_fEmitterDelay;
		emitter->m_bDelayFirstLoopOnly = origEmitter->m_bDelayFirstLoopOnly;
		emitter->m_fEmitterDuration = origEmitter->m_fEmitterDuration;
		emitter->m_nLoopCount = origEmitter->m_nLoopCount;
		emitter->m_szBlendMode = origEmitter->m_szBlendMode;
		emitter->m_bHUDEffect = origEmitter->m_bHUDEffect;

		for(unsigned int i = 0; i < origEmitter->m_EmitterAttributes.size(); i++)
		{
			CBaseEmitterAttribute *origEmitAttrib = origEmitter->m_EmitterAttributes[i];

			int nType = origEmitAttrib->m_nType;

			if(nType == emitter->m_pAccelbyLife->m_nType)
				emitter->m_pAccelbyLife->Clone(origEmitAttrib);

			else if(nType == emitter->m_pBox->m_nType)
				emitter->m_pBox->Clone(origEmitAttrib);

			else if(nType == emitter->m_pColbylife->m_nType)
				emitter->m_pColbylife->Clone(origEmitAttrib);

			else if(nType == emitter->m_pInitAcc->m_nType)
				emitter->m_pInitAcc->Clone(origEmitAttrib);

			else if(nType == emitter->m_pInitRot->m_nType)
				emitter->m_pInitRot->Clone(origEmitAttrib);

			else if(nType == emitter->m_pRotRate->m_nType)
				emitter->m_pRotRate->Clone(origEmitAttrib);

			else if(nType == emitter->m_pInitVel->m_nType)
				emitter->m_pInitVel->Clone(origEmitAttrib);

			else if(nType == emitter->m_pLifetime->m_nType)
				emitter->m_pLifetime->Clone(origEmitAttrib);

			else if(nType == emitter->m_pLine->m_nType)
				emitter->m_pLine->Clone(origEmitAttrib);

			else if(nType == emitter->m_pOrbit->m_nType)
				emitter->m_pOrbit->Clone(origEmitAttrib);

			else if(nType == emitter->m_pRotbyLife->m_nType)
				emitter->m_pRotbyLife->Clone(origEmitAttrib);

			else if(nType == emitter->m_pInitSize->m_nType)
				emitter->m_pInitSize->Clone(origEmitAttrib);

			else if(nType == emitter->m_pSizeByLife->m_nType)
				emitter->m_pSizeByLife->Clone(origEmitAttrib);

			else if(nType == emitter->m_pSpawn->m_nType)
				emitter->m_pSpawn->Clone(origEmitAttrib);

			else if(nType == emitter->m_pSphere->m_nType)
				emitter->m_pSphere->Clone(origEmitAttrib);

			else if(nType == emitter->m_pVelbyLife->m_nType)
				emitter->m_pVelbyLife->Clone(origEmitAttrib);
		}

		emitter->m_ParticleShape.GetFrame().GetLocalMatrix() = worldMat;
		emitter->m_mxAttachedMat = pAttached;
		emitter->m_pOwner = pPartEffect;

		if(!emitter->m_bHUDEffect)
		{
			if(emitter->m_szBlendMode == "AdditiveBlend")
			{
				emitter->m_ParticleShape.SetRenderContext(&CRenderContextManager::GetInstance()->GetParticleAdditiveContext());
			}
			else if(emitter->m_szBlendMode == "TransparencyBlend")
			{
				emitter->m_ParticleShape.SetRenderContext(&CRenderContextManager::GetInstance()->GetParticleTransparencyContext());
			}
			else if(emitter->m_szBlendMode == "NoBlend")
			{
				emitter->m_ParticleShape.SetRenderContext(&CRenderContextManager::GetInstance()->GetParticleNoBlendContext());
			}
		}
		else
		{
			if(emitter->m_szBlendMode == "AdditiveBlend")
			{
				emitter->m_ParticleShape.SetRenderContext(&CRenderContextManager::GetInstance()->GetHUDParticleAdditiveContext());
			}
			else if(emitter->m_szBlendMode == "TransparencyBlend")
			{
				emitter->m_ParticleShape.SetRenderContext(&CRenderContextManager::GetInstance()->GetHUDParticleTransparencyContext());
			}
			else if(emitter->m_szBlendMode == "NoBlend")
			{
				emitter->m_ParticleShape.SetRenderContext(&CRenderContextManager::GetInstance()->GetHUDParticleNoBlendContext());
			}
		}

		pPartEffect->m_Emitters.push_back(emitter);
	}

	m_lActiveParticles.push_back(pPartEffect);
	return pPartEffect;
}

void CParticleManager::RemoveParticles()
{
	for(list<CParticleEffect*>::iterator iter = m_lActiveParticles.begin();
		iter != m_lActiveParticles.end(); iter++)
	{
		delete (*iter);
	}

	m_lActiveParticles.clear();
}

void CParticleManager::ShutdownParticleManager()
{
	for(list<CParticleEffect*>::iterator iter = m_lActiveParticles.begin();
		iter != m_lActiveParticles.end(); iter++)
	{
		delete (*iter);
	}

	for(map<string, CParticleEffect*>::iterator iter = m_mLoadedParticles.begin();
		iter != m_mLoadedParticles.end(); iter++)
	{
		delete (*iter).second;
	}

	m_lActiveParticles.clear();
	m_mLoadedParticles.clear();
}

void CParticleManager::OnLostDevice()
{
	for(list<CParticleEffect*>::iterator iter = m_lActiveParticles.begin();
		iter != m_lActiveParticles.end(); iter++)
	{
		for(unsigned int i = 0; i < (*iter)->m_Emitters.size(); i++)
		{
			SAFE_RELEASE((*iter)->m_Emitters[i]->m_Mesh.GetVertexBuffer());
		}
	}

	for(map<string, CParticleEffect*>::iterator iter = m_mLoadedParticles.begin();
		iter != m_mLoadedParticles.end(); iter++)
	{
		for(unsigned int i = 0; i < (*iter).second->m_Emitters.size(); i++)
		{
			SAFE_RELEASE((*iter).second->m_Emitters[i]->m_Mesh.GetVertexBuffer());
		}
	}
}

void CParticleManager::OnResetDevice()
{
	for(list<CParticleEffect*>::iterator iter = m_lActiveParticles.begin();
		iter != m_lActiveParticles.end(); iter++)
	{
		for(unsigned int i = 0; i < (*iter)->m_Emitters.size(); i++)
		{
			(*iter)->m_Emitters[i]->CreateVertexBuffer();
		}
	}

	for(map<string, CParticleEffect*>::iterator iter = m_mLoadedParticles.begin();
		iter != m_mLoadedParticles.end(); iter++)
	{
		for(unsigned int i = 0; i < (*iter).second->m_Emitters.size(); i++)
		{
			(*iter).second->m_Emitters[i]->CreateVertexBuffer();
		}
	}
}