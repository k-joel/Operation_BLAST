#include <fstream>
#include <string>
using namespace std;

#include "../Game Objects/layoutImporter.h"
#include "../Mesh/modelImporter.h"
#include "../Game Objects/CObjectManager.h"
#include "../Game Objects/CSceneObject.h"
#include "../Game Objects/CObjectFactory.h"
#include "../Game Objects/Scene/CGround.h"
#include "../Game Objects/Scene/CPlatform.h"
#include "../Game Objects/CSplit.h"
#include "../Game Objects/CMerge.h"
#include "../Game Objects/CGoal.h"
#include "../Game Objects/CAudioTrigger.h"
#include "../Game Objects/CTrigger.h"
#include "../Game Objects/Scene/CWall.h"
#include "../Game Objects/Scene/CJumpPad.h"
#include "../Game Objects/CParticleTrigger.h"
#include "../fx/Helpers.h"

CLayoutImporter::CLayoutImporter()
{}

CLayoutImporter::~CLayoutImporter()
{}

void CLayoutImporter::LoadLayout(char* filename)
{
	CObjectManager* objMang = CObjectManager::GetInstance();

	unsigned int number, type, ID;
	unsigned int size, numOCore, numOInstances;
	char *nameBuffer = 0;
	D3DXMATRIX tempOreintation;

	fstream fin;
	fin.open(filename, ios_base::in | ios_base::binary);

	if(fin.is_open())
	{
		//unsigned int: version
		fin.read((char*)&number, sizeof(unsigned int));

		//unsigned int: number of core (models to load)
		fin.read((char*)&numOCore, sizeof(unsigned int));

		//resize model container
		unsigned int listIndex = 0;

		//for each of core
		for(unsigned int coreIndex = 0; coreIndex < numOCore; coreIndex += 1)
		{
			//unsigned int: size of model name
			fin.read((char*)&size, sizeof(unsigned int));
			//char array[size]: model name
			delete[] nameBuffer;
			nameBuffer = new char[size];
			fin.read(nameBuffer, size);

			//TODO: load core model
			string fixedFilename;
			fixedFilename = nameBuffer;
			fixedFilename = "Resources/Models/" + fixedFilename;
			CLoadState::GetInstance()->m_mModelIDMap[nameBuffer] = CModelManager::GetInstance()->LoadModel(fixedFilename.c_str());

			//unsigned int: number of instances of this core
			fin.read((char*)&numOInstances, sizeof(unsigned int));

			//for each instance
			for(unsigned int instanceIndex = 0; instanceIndex < numOInstances; instanceIndex +=1)
			{
				//unsigned int: type
				fin.read((char*)&type, sizeof(unsigned int));
				//unsigned int: ID
				fin.read((char*)&ID, sizeof(unsigned int));
				//unsigned int: number
				fin.read((char*)&number, sizeof(unsigned int));
				//float array[16]: world matrix
				fin.read((char*)&tempOreintation, sizeof(D3DXMATRIX));

				//TODO: using the lua file connect the objects with events if nessesary

				//TODO: a swtich based on the unsigned ints per object to sort them logicaly
				CBase* toAdd;

				string name = nameBuffer;

				switch(type)
				{
				case IMPORT_DEFAULT:
				case IMPORT_SHADOWRECEIVE:
					toAdd = (CSceneObject*)CObjectFactory::GetInstance()->GetObject<CSceneObject>(OBJ_SCENE);
					toAdd->InitShape(CModelManager::GetInstance()->GetModel(CLoadState::GetInstance()->GetModelID(nameBuffer)),
						&CRenderContextManager::GetInstance()->GetStaticLitContext(),
						tempOreintation, CRenderShape::PNTRenderFunc, false);
					objMang->AddRenderable(toAdd);
					break;
				case IMPORT_GROUND:
					toAdd = (CGround*)CObjectFactory::GetInstance()->GetObject<CGround>(OBJ_GROUND);
					toAdd->InitShape(CModelManager::GetInstance()->GetModel(CLoadState::GetInstance()->GetModelID(nameBuffer)),
						NULL, tempOreintation, NULL, false);
					objMang->AddCollidable(toAdd);
					break;
				case IMPORT_PLATFORM:
					toAdd = (CPlatform*)CObjectFactory::GetInstance()->GetObject<CPlatform>(OBJ_PLATFORM);
					toAdd->InitShape(CModelManager::GetInstance()->GetModel(CLoadState::GetInstance()->GetModelID(nameBuffer)),
						NULL, tempOreintation, NULL, false);
					objMang->AddCollidable(toAdd);
					break;
				case IMPORT_WALL:
					toAdd = (CWall*)CObjectFactory::GetInstance()->GetObject<CWall>(OBJ_WALL);
					toAdd->InitShape(CModelManager::GetInstance()->GetModel(CLoadState::GetInstance()->GetModelID(nameBuffer)),
						NULL, tempOreintation, NULL, false);
					objMang->AddCollidable(toAdd);
					break;
				case IMPORT_SPLIT:
					toAdd = (CSplitObject*)CObjectFactory::GetInstance()->GetObject<CSplitObject>(OBJ_SPLIT);
					toAdd->SetRenderType(RT_DONOTRENDER);
					toAdd->InitShape(CModelManager::GetInstance()->GetModel(CLoadState::GetInstance()->GetModelID(nameBuffer)),
						&CRenderContextManager::GetInstance()->GetStaticLitContext(),
						tempOreintation, CRenderShape::PNTRenderFunc, false);
					objMang->AddTrigger(toAdd);

					((CSplitObject*)toAdd)->m_goalID = ID;
					break;
				case IMPORT_MERGE:
					toAdd = (CMergeObject*)CObjectFactory::GetInstance()->GetObject<CMergeObject>(OBJ_MERGE);
					toAdd->SetRenderType(RT_DONOTRENDER);
					toAdd->InitShape(CModelManager::GetInstance()->GetModel(CLoadState::GetInstance()->GetModelID(nameBuffer)),
						&CRenderContextManager::GetInstance()->GetStaticLitContext(),
						tempOreintation, CRenderShape::PNTRenderFunc, false);
					objMang->AddTrigger(toAdd);

					((CMergeObject*)toAdd)->m_goalID = ID;
					break;
				case IMPORT_GOAL:
					toAdd = (CGoalObject*)CObjectFactory::GetInstance()->GetObject<CGoalObject>(OBJ_GOAL);
					toAdd->SetRenderType(RT_DONOTRENDER);
					toAdd->InitShape(CModelManager::GetInstance()->GetModel(CLoadState::GetInstance()->GetModelID(nameBuffer)),
						&CRenderContextManager::GetInstance()->GetStaticLitContext(),
						tempOreintation, CRenderShape::PNTRenderFunc, false);
					objMang->AddTrigger(toAdd);

					((CGoalObject*)toAdd)->m_goalID = ID;
					((CGoalObject*)toAdd)->m_targetID = number;
					break;
				case IMPORT_TRANSPARENT:
					toAdd = (CSceneObject*)CObjectFactory::GetInstance()->GetObject<CSceneObject>(OBJ_SCENE);
					toAdd->SetRenderType(RT_TRANSPARENT);
					if(name == "smoke_cloudShape" )
						toAdd->SetRenderType(RT_DONOTRENDER);
					toAdd->InitShape(CModelManager::GetInstance()->GetModel(CLoadState::GetInstance()->GetModelID(nameBuffer)),
						&CRenderContextManager::GetInstance()->GetAlphaBlendContext(),
						tempOreintation, CRenderShape::PNTRenderFunc, false);
					objMang->AddRenderable(toAdd);
					break;
				case IMPORT_ALPHATEST:
					toAdd = (CSceneObject*)CObjectFactory::GetInstance()->GetObject<CSceneObject>(OBJ_SCENE);
					toAdd->SetRenderType(RT_TRANSPARENT);
					toAdd->InitShape(CModelManager::GetInstance()->GetModel(CLoadState::GetInstance()->GetModelID(nameBuffer)),
						&CRenderContextManager::GetInstance()->GetAlphaTestContext(),
						tempOreintation, CRenderShape::PNTRenderFunc, false);
					objMang->AddRenderable(toAdd);
					break;
				case IMPORT_UVSCROLL:
					{
						toAdd = (CSceneObject*)CObjectFactory::GetInstance()->GetObject<CSceneObject>(OBJ_SCENE);
						toAdd->InitShape(CModelManager::GetInstance()->GetModel(CLoadState::GetInstance()->GetModelID(nameBuffer)),
							&CRenderContextManager::GetInstance()->GetStaticUVScrollContext(),
							tempOreintation, CRenderShape::UVScrollPNTRenderFunc, false);

						static TUVScrollData data = { 0.03f, D3DXVECTOR2(1.0f, 0.0f) };

						toAdd->GetShape(0).SetData((void*)&data);
						objMang->AddRenderable(toAdd);
					}
					break;

				case IMPORT_UVATLAS:
					{
						toAdd = (CSceneObject*)CObjectFactory::GetInstance()->GetObject<CSceneObject>(OBJ_SCENE);
						toAdd->SetRenderType(RT_TRANSPARENT);
						toAdd->InitShape(CModelManager::GetInstance()->GetModel(CLoadState::GetInstance()->GetModelID(nameBuffer)),
							&CRenderContextManager::GetInstance()->GetAlphaBlendContext(),
							tempOreintation, CRenderShape::UVAtlasPNTRenderFunc, false);

						static TUVAtlasData smokeData = { Helpers::GetRandomFloat(3.0f, 5.0f), D3DXVECTOR2(4.0f, 4.0f) };
						//static TUVAtlasData leavesData = { 1.0f, D3DXVECTOR2(2.0f, 2.0f) };
						static TUVAtlasData lightningData = { Helpers::GetRandomFloat(15.0f, 20.0f), D3DXVECTOR2(4.0f, 4.0f) };

						switch(ID)
						{
						case 0: // smoke
							toAdd->GetShape(0).SetData((void*)&smokeData);
							break;
						//case 1: // leaves
						//	toAdd->GetShape(0).SetData((void*)&leavesData);
						//	break;
						case 2: // lightning
							toAdd->GetShape(0).SetData((void*)&lightningData);
							toAdd->GetShape(0).m_fCurTime = 0.0f;
							toAdd->GetShape(0).m_fDelay = Helpers::GetRandomFloat(2.0f, 4.0f);
							break;
						}
						objMang->AddRenderable(toAdd);
					}

					break;
				/*case IMPORT_SHADOWRECEIVE:
					toAdd = (CSceneObject*)CObjectFactory::GetInstance()->GetObject<CSceneObject>(OBJ_SCENE);
					toAdd->InitShape(CModelManager::GetInstance()->GetModel(CLoadState::GetInstance()->GetModelID(nameBuffer)),
						&CRenderContextManager::GetInstance()->GetStaticShadowRecieveContext(),
						tempOreintation, CRenderShape::ShadowPNTRenderFunc, false);
					objMang->AddRenderable(toAdd);
					break;*/
				case IMPORT_JUMPPAD:
					toAdd = (CJumpPad*)CObjectFactory::GetInstance()->GetObject<CJumpPad>(OBJ_JUMPPAD);
					toAdd->SetRenderType(RT_DONOTRENDER);
					toAdd->InitShape(CModelManager::GetInstance()->GetModel(CLoadState::GetInstance()->GetModelID(nameBuffer)),
						&CRenderContextManager::GetInstance()->GetStaticLitContext(),
						tempOreintation, CRenderShape::PNTRenderFunc, false);
					objMang->AddTrigger(toAdd);
					break;
				case IMPORT_TRIGGER:
					toAdd = (CAudioTrigger*)CObjectFactory::GetInstance()->GetObject<CAudioTrigger>(OBJ_AUDIO_TRIGGER);
					toAdd->InitShape(CModelManager::GetInstance()->GetModel(CLoadState::GetInstance()->GetModelID(nameBuffer)),
						&CRenderContextManager::GetInstance()->GetStaticLitContext(),
						tempOreintation, CRenderShape::PNTRenderFunc, false);
					objMang->AddTrigger(toAdd);
					break;
				case IMPORT_SPAWN:
					toAdd = (CTriggerObject*)CObjectFactory::GetInstance()->GetObject<CTriggerObject>(OBJ_TRIGGER);
					toAdd->InitShape(CModelManager::GetInstance()->GetModel(CLoadState::GetInstance()->GetModelID(nameBuffer)),
						&CRenderContextManager::GetInstance()->GetStaticLitContext(),
						tempOreintation, CRenderShape::PNTRenderFunc, false);
					((CTriggerObject*)toAdd)->m_spawnType = ID;
					((CTriggerObject*)toAdd)->m_numSpawn = number;
					objMang->AddTrigger(toAdd);
					break;
				case IMPORT_PARTICLETRIGGER:
					toAdd = (CParticleTrigger*)CObjectFactory::GetInstance()->GetObject<CParticleTrigger>(OBJ_PARTICLETRIGGER);
					toAdd->InitShape(CModelManager::GetInstance()->GetModel(CLoadState::GetInstance()->GetModelID(nameBuffer)),
						&CRenderContextManager::GetInstance()->GetStaticLitContext(),
						tempOreintation, CRenderShape::PNTRenderFunc, false);
					((CParticleTrigger*)toAdd)->m_nParticleType = ID;
					toAdd->GetBVSphere().GetSphere().r += 500.0f;
					toAdd->GetBVSphere().GetOriginalSphere().r += 500.0f;

					objMang->AddTrigger(toAdd);
					break;
				default:
					toAdd = (CSceneObject*)CObjectFactory::GetInstance()->GetObject<CSceneObject>(OBJ_SCENE);
					toAdd->InitShape(CModelManager::GetInstance()->GetModel(CLoadState::GetInstance()->GetModelID(nameBuffer)),
						&CRenderContextManager::GetInstance()->GetStaticLitContext(),
						tempOreintation, CRenderShape::PNTRenderFunc, false);
					objMang->AddRenderable(toAdd);
					break;
				}
			}
		}

		delete[] nameBuffer;
	}
	else
	{
			char szBuffer[256] = {0};
			sprintf(szBuffer, "Failed to Load Level - %s", filename);
			MessageBox(0, szBuffer, "Level Importer Error", MB_OK);
	}

	fin.clear();
	fin.close();
}