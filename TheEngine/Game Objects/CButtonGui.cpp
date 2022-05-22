#include "CButtonGui.h"
#include "../Mesh/modelImporter.h"
#include "../Rendering/CRenderer.h"
#include "../Rendering/CShaderManager.h"
#include "../Rendering/CRenderContextManager.h"
#include "../States/CGameState.h"
#include "../States/CLoadState.h"
#include "../Input/CInput.h"
#include "../Mesh/CModelManager.h"
#include "../Core/CGame.h"
#include "../Camera/Camera.h"

CButtonGui::CButtonGui()
{

}

CButtonGui::~CButtonGui() 
{

}

void CButtonGui::Initialize(bool isPaused)
{
	// Selected
	m_Models[0] = CModelManager::GetInstance()->GetModel(CLoadState::GetInstance()->GetModelID("GamePadAccept"));
	m_Shapes[0].Init(&m_Models[0]->GetMesh(),NULL,&CRenderContextManager::GetInstance()->GetHUDContext(),m_mWorldPosition,CRenderShape::PNTRenderFunc);

	m_Models[1] = CModelManager::GetInstance()->GetModel(CLoadState::GetInstance()->GetModelID("GamePadBack"));
	m_Shapes[1].Init(&m_Models[1]->GetMesh(),NULL,&CRenderContextManager::GetInstance()->GetHUDContext(),m_mWorldPosition,CRenderShape::PNTRenderFunc);
	
	m_Models[2] = CModelManager::GetInstance()->GetModel(CLoadState::GetInstance()->GetModelID("GamePadLR"));
	m_Shapes[2].Init(&m_Models[2]->GetMesh(),NULL,&CRenderContextManager::GetInstance()->GetHUDContext(),m_mWorldPosition,CRenderShape::PNTRenderFunc);
	
	m_Models[3] = CModelManager::GetInstance()->GetModel(CLoadState::GetInstance()->GetModelID("GamePadUD"));
	m_Shapes[3].Init(&m_Models[3]->GetMesh(),NULL,&CRenderContextManager::GetInstance()->GetHUDContext(),m_mWorldPosition,CRenderShape::PNTRenderFunc);

	m_Models[4] = CModelManager::GetInstance()->GetModel(CLoadState::GetInstance()->GetModelID("KeyAccept"));
	m_Shapes[4].Init(&m_Models[4]->GetMesh(),NULL,&CRenderContextManager::GetInstance()->GetHUDContext(),m_mWorldPosition,CRenderShape::PNTRenderFunc);

	m_Models[5] = CModelManager::GetInstance()->GetModel(CLoadState::GetInstance()->GetModelID("KeyBack"));
	m_Shapes[5].Init(&m_Models[5]->GetMesh(),NULL,&CRenderContextManager::GetInstance()->GetHUDContext(),m_mWorldPosition,CRenderShape::PNTRenderFunc);

	m_Models[6] = CModelManager::GetInstance()->GetModel(CLoadState::GetInstance()->GetModelID("KeyL"));
	m_Shapes[6].Init(&m_Models[6]->GetMesh(),NULL,&CRenderContextManager::GetInstance()->GetHUDContext(),m_mWorldPosition,CRenderShape::PNTRenderFunc);
	
	m_Models[7] = CModelManager::GetInstance()->GetModel(CLoadState::GetInstance()->GetModelID("KeyR"));
	m_Shapes[7].Init(&m_Models[7]->GetMesh(),NULL,&CRenderContextManager::GetInstance()->GetHUDContext(),m_mWorldPosition,CRenderShape::PNTRenderFunc);
	
	m_Models[8] = CModelManager::GetInstance()->GetModel(CLoadState::GetInstance()->GetModelID("KeyD"));
	m_Shapes[8].Init(&m_Models[8]->GetMesh(),NULL,&CRenderContextManager::GetInstance()->GetHUDContext(),m_mWorldPosition,CRenderShape::PNTRenderFunc);

	m_Models[9] = CModelManager::GetInstance()->GetModel(CLoadState::GetInstance()->GetModelID("KeyU"));
	m_Shapes[9].Init(&m_Models[9]->GetMesh(),NULL,&CRenderContextManager::GetInstance()->GetHUDContext(),m_mWorldPosition,CRenderShape::PNTRenderFunc);

	//Unselected

	m_Models[0] = CModelManager::GetInstance()->GetModel(CLoadState::GetInstance()->GetModelID("GamePadAcceptU"));
	m_ShapesUnselected[0].Init(&m_Models[0]->GetMesh(),NULL,&CRenderContextManager::GetInstance()->GetHUDContext(),m_mWorldPosition,CRenderShape::PNTRenderFunc);

	m_Models[1] = CModelManager::GetInstance()->GetModel(CLoadState::GetInstance()->GetModelID("GamePadBackU"));
	m_ShapesUnselected[1].Init(&m_Models[1]->GetMesh(),NULL,&CRenderContextManager::GetInstance()->GetHUDContext(),m_mWorldPosition,CRenderShape::PNTRenderFunc);
	
	m_Models[2] = CModelManager::GetInstance()->GetModel(CLoadState::GetInstance()->GetModelID("GamePadLRU"));
	m_ShapesUnselected[2].Init(&m_Models[2]->GetMesh(),NULL,&CRenderContextManager::GetInstance()->GetHUDContext(),m_mWorldPosition,CRenderShape::PNTRenderFunc);
	
	m_Models[3] = CModelManager::GetInstance()->GetModel(CLoadState::GetInstance()->GetModelID("GamePadUDU"));
	m_ShapesUnselected[3].Init(&m_Models[3]->GetMesh(),NULL,&CRenderContextManager::GetInstance()->GetHUDContext(),m_mWorldPosition,CRenderShape::PNTRenderFunc);

	m_Models[4] = CModelManager::GetInstance()->GetModel(CLoadState::GetInstance()->GetModelID("KeyAcceptU"));
	m_ShapesUnselected[4].Init(&m_Models[4]->GetMesh(),NULL,&CRenderContextManager::GetInstance()->GetHUDContext(),m_mWorldPosition,CRenderShape::PNTRenderFunc);

	m_Models[5] = CModelManager::GetInstance()->GetModel(CLoadState::GetInstance()->GetModelID("KeyBackU"));
	m_ShapesUnselected[5].Init(&m_Models[5]->GetMesh(),NULL,&CRenderContextManager::GetInstance()->GetHUDContext(),m_mWorldPosition,CRenderShape::PNTRenderFunc);

	m_Models[6] = CModelManager::GetInstance()->GetModel(CLoadState::GetInstance()->GetModelID("KeyLU"));
	m_ShapesUnselected[6].Init(&m_Models[6]->GetMesh(),NULL,&CRenderContextManager::GetInstance()->GetHUDContext(),m_mWorldPosition,CRenderShape::PNTRenderFunc);
	
	m_Models[7] = CModelManager::GetInstance()->GetModel(CLoadState::GetInstance()->GetModelID("KeyRU"));
	m_ShapesUnselected[7].Init(&m_Models[7]->GetMesh(),NULL,&CRenderContextManager::GetInstance()->GetHUDContext(),m_mWorldPosition,CRenderShape::PNTRenderFunc);
	
	m_Models[8] = CModelManager::GetInstance()->GetModel(CLoadState::GetInstance()->GetModelID("KeyDU"));
	m_ShapesUnselected[8].Init(&m_Models[8]->GetMesh(),NULL,&CRenderContextManager::GetInstance()->GetHUDContext(),m_mWorldPosition,CRenderShape::PNTRenderFunc);

	m_Models[9] = CModelManager::GetInstance()->GetModel(CLoadState::GetInstance()->GetModelID("KeyUU"));
	m_ShapesUnselected[9].Init(&m_Models[9]->GetMesh(),NULL,&CRenderContextManager::GetInstance()->GetHUDContext(),m_mWorldPosition,CRenderShape::PNTRenderFunc);

	m_bIsPaused = isPaused;
}

void CButtonGui::Render(D3DXMATRIX camera,bool UD, bool LR, bool Accept, bool Back, bool isController)
{
	D3DXVECTOR3 location = CCamera::GetInstance()->GetViewPosition();

	if(isController)
	{
		//Controller Objects
		if(Accept)
		{
			if(m_bIsPaused)
			D3DXMatrixTranslation(&m_mWorldPosition,location.x,location.y-10,location.z+24);
			else
			D3DXMatrixTranslation(&m_mWorldPosition,0,2.5,45);

			m_Shapes[0].GetFrame().GetLocalMatrix() = m_mWorldPosition * camera;
			m_Shapes[0].GetRenderContext()->AddRenderNode(&m_Shapes[0]);
		}
		else
		{
			if(m_bIsPaused)
			D3DXMatrixTranslation(&m_mWorldPosition,location.x,location.y-10,location.z+24);
			else
			D3DXMatrixTranslation(&m_mWorldPosition,0,2.5,45);

			m_ShapesUnselected[0].GetFrame().GetLocalMatrix() = m_mWorldPosition * camera;
			m_ShapesUnselected[0].GetRenderContext()->AddRenderNode(&m_ShapesUnselected[0]);
		}

		if(Back)
		{

			if(m_bIsPaused)
			D3DXMatrixTranslation(&m_mWorldPosition,location.x+4,location.y-10,location.z+24);
			else
			D3DXMatrixTranslation(&m_mWorldPosition,4,2.5,45);


			m_Shapes[1].GetFrame().GetLocalMatrix() = m_mWorldPosition * camera;
			m_Shapes[1].GetRenderContext()->AddRenderNode(&m_Shapes[1]);
		}
		else
		{
			
			if(m_bIsPaused)
			D3DXMatrixTranslation(&m_mWorldPosition,location.x+4,location.y-10,location.z+24);
			else
			D3DXMatrixTranslation(&m_mWorldPosition,4,2.5,45);


			m_ShapesUnselected[1].GetFrame().GetLocalMatrix() = m_mWorldPosition * camera;
			m_ShapesUnselected[1].GetRenderContext()->AddRenderNode(&m_ShapesUnselected[1]);
		}

		if(LR)
		{
			if(m_bIsPaused)
			D3DXMatrixTranslation(&m_mWorldPosition,location.x-4,location.y-10,location.z+24);
			else
			D3DXMatrixTranslation(&m_mWorldPosition,-4,2.5,45);


			m_Shapes[2].GetFrame().GetLocalMatrix() = m_mWorldPosition * camera;
			m_Shapes[2].GetRenderContext()->AddRenderNode(&m_Shapes[2]);
		}
		else
		{
			if(m_bIsPaused)
			D3DXMatrixTranslation(&m_mWorldPosition,location.x-4,location.y-10,location.z+24);
			else
			D3DXMatrixTranslation(&m_mWorldPosition,-4,2.5,45);


			m_ShapesUnselected[2].GetFrame().GetLocalMatrix() = m_mWorldPosition * camera;
			m_ShapesUnselected[2].GetRenderContext()->AddRenderNode(&m_ShapesUnselected[2]);
		}

		if(UD)
		{
			if(m_bIsPaused)
			D3DXMatrixTranslation(&m_mWorldPosition,location.x-8,location.y-10,location.z+24);
			else
			D3DXMatrixTranslation(&m_mWorldPosition,-8,2.5,45);

			m_Shapes[3].GetFrame().GetLocalMatrix() = m_mWorldPosition * camera;
			m_Shapes[3].GetRenderContext()->AddRenderNode(&m_Shapes[3]);
		}
		else
		{
			if(m_bIsPaused)
			D3DXMatrixTranslation(&m_mWorldPosition,location.x-8,location.y-10,location.z+24);
			else
			D3DXMatrixTranslation(&m_mWorldPosition,-8,2.5,45);

			m_ShapesUnselected[3].GetFrame().GetLocalMatrix() = m_mWorldPosition * camera;
			m_ShapesUnselected[3].GetRenderContext()->AddRenderNode(&m_ShapesUnselected[3]);
		}

	}
	else
	{
		//Keyboard Objects
		if(Accept)
		{
			if(m_bIsPaused)
			D3DXMatrixTranslation(&m_mWorldPosition,location.x,location.y-10,location.z+24);
			else
			D3DXMatrixTranslation(&m_mWorldPosition,0,2.5,45);

			m_Shapes[4].GetFrame().GetLocalMatrix() = m_mWorldPosition * camera;
			m_Shapes[4].GetRenderContext()->AddRenderNode(&m_Shapes[4]);
		}
		else
		{
			if(m_bIsPaused)
			D3DXMatrixTranslation(&m_mWorldPosition,location.x,location.y-10,location.z+24);
			else
			D3DXMatrixTranslation(&m_mWorldPosition,0,2.5,45);

			m_ShapesUnselected[4].GetFrame().GetLocalMatrix() = m_mWorldPosition * camera;
			m_ShapesUnselected[4].GetRenderContext()->AddRenderNode(&m_ShapesUnselected[4]);
		}

		if(Back)
		{
			if(m_bIsPaused)
			D3DXMatrixTranslation(&m_mWorldPosition,location.x+4,location.y-10,location.z+24);
			else
			D3DXMatrixTranslation(&m_mWorldPosition,4,2.5,45);


			m_Shapes[5].GetFrame().GetLocalMatrix() = m_mWorldPosition * camera;
			m_Shapes[5].GetRenderContext()->AddRenderNode(&m_Shapes[5]);
		}
		else
		{
			if(m_bIsPaused)
			D3DXMatrixTranslation(&m_mWorldPosition,location.x+4,location.y-10,location.z+24);
			else
			D3DXMatrixTranslation(&m_mWorldPosition,4,2.5,45);


			m_ShapesUnselected[5].GetFrame().GetLocalMatrix() = m_mWorldPosition * camera;
			m_ShapesUnselected[5].GetRenderContext()->AddRenderNode(&m_ShapesUnselected[5]);
		}

		if(LR)
		{
			if(m_bIsPaused)
			D3DXMatrixTranslation(&m_mWorldPosition,location.x-4,location.y-10.5,location.z+24);
			else
			D3DXMatrixTranslation(&m_mWorldPosition,-4,2.5,45);


			m_Shapes[6].GetFrame().GetLocalMatrix() = m_mWorldPosition * camera;
			m_Shapes[6].GetRenderContext()->AddRenderNode(&m_Shapes[6]);

			if(m_bIsPaused)
			D3DXMatrixTranslation(&m_mWorldPosition,location.x-4,location.y-9.0,location.z+24);
			else
			D3DXMatrixTranslation(&m_mWorldPosition,-4,4,45);


			m_Shapes[7].GetFrame().GetLocalMatrix() = m_mWorldPosition * camera;
			m_Shapes[7].GetRenderContext()->AddRenderNode(&m_Shapes[7]);
		}
		else
		{
			if(m_bIsPaused)
			D3DXMatrixTranslation(&m_mWorldPosition,location.x-4,location.y-10.5,location.z+24);
			else
			D3DXMatrixTranslation(&m_mWorldPosition,-4,2.5,45);


			m_ShapesUnselected[6].GetFrame().GetLocalMatrix() = m_mWorldPosition * camera;
			m_ShapesUnselected[6].GetRenderContext()->AddRenderNode(&m_ShapesUnselected[6]);

			if(m_bIsPaused)
			D3DXMatrixTranslation(&m_mWorldPosition,location.x-4,location.y-9.0,location.z+24);
			else
			D3DXMatrixTranslation(&m_mWorldPosition,-4,4,45);


			m_ShapesUnselected[7].GetFrame().GetLocalMatrix() = m_mWorldPosition * camera;
			m_ShapesUnselected[7].GetRenderContext()->AddRenderNode(&m_ShapesUnselected[7]);
		}

		if(UD)
		{
			if(m_bIsPaused)
			D3DXMatrixTranslation(&m_mWorldPosition,location.x-8,location.y-10.5,location.z+24);
			else
			D3DXMatrixTranslation(&m_mWorldPosition,-8,2.5,45);

			m_Shapes[8].GetFrame().GetLocalMatrix() = m_mWorldPosition * camera;
			m_Shapes[8].GetRenderContext()->AddRenderNode(&m_Shapes[8]);

			if(m_bIsPaused)
			D3DXMatrixTranslation(&m_mWorldPosition,location.x-8,location.y-9.0,location.z+24);
			else
			D3DXMatrixTranslation(&m_mWorldPosition,-8,4,45);

			m_Shapes[9].GetFrame().GetLocalMatrix() = m_mWorldPosition * camera;
			m_Shapes[9].GetRenderContext()->AddRenderNode(&m_Shapes[9]);

		}
		else
		{
			if(m_bIsPaused)
			D3DXMatrixTranslation(&m_mWorldPosition,location.x-8,location.y-10.5,location.z+24);
			else
			D3DXMatrixTranslation(&m_mWorldPosition,-8,2.5,45);

			m_ShapesUnselected[8].GetFrame().GetLocalMatrix() = m_mWorldPosition * camera;
			m_ShapesUnselected[8].GetRenderContext()->AddRenderNode(&m_ShapesUnselected[8]);

			if(m_bIsPaused)
			D3DXMatrixTranslation(&m_mWorldPosition,location.x-8,location.y-9.0,location.z+24);
			else
			D3DXMatrixTranslation(&m_mWorldPosition,-8,4,45);

			m_ShapesUnselected[9].GetFrame().GetLocalMatrix() = m_mWorldPosition * camera;
			m_ShapesUnselected[9].GetRenderContext()->AddRenderNode(&m_ShapesUnselected[9]);
		}
	}
}