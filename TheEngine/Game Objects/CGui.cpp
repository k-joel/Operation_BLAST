#include "CGui.h"
#include "../Mesh/modelImporter.h"
#include "../Rendering/CRenderer.h"
#include "../Rendering/CShaderManager.h"
#include "../Rendering/CRenderContextManager.h"
#include "../States/CGameState.h"
#include "../States/CLoadState.h"
#include "../Input/CInput.h"
#include "../Mesh/CModelManager.h"
#include "../Core/CGame.h"
#include "../States/CLoadState.h"

CGui::CGui()
{
	m_fBlinkTimer = 0.0f;
}

CGui::~CGui()
{

}

void CGui::SetPostion(float x,float y,float z)
{
	D3DXMatrixIdentity(&m_mWorldPosition);
	D3DXMatrixTranslation(&m_mWorldPosition,x,y,z);
}
void CGui::SetScale(float x, float y, float z)
{
	D3DXMATRIX scale;
	D3DXMatrixScaling(&scale,x,y,z);
	D3DXMatrixMultiply(&m_mWorldPosition,&scale,&m_mWorldPosition);
}

// Using two models
void CGui::Initialize(string model1,string model2,float x, float y, float z,float sx, float sy, float sz, bool isSelected)
{
	//Set pointers to models
	m_Models[0] = CModelManager::GetInstance()->GetModel(CLoadState::GetInstance()->GetModelID(model1));
	m_Models[1] = CModelManager::GetInstance()->GetModel(CLoadState::GetInstance()->GetModelID(model2));
	
	//Set the world position and scale
	SetPostion(x,y,z);
	SetScale(sx,sy,sz);

	//Set the shapes
	m_Shapes[0].Init(&m_Models[0]->GetMesh(),NULL,&CRenderContextManager::GetInstance()->GetHUDContext(),m_mWorldPosition,CRenderShape::PNTRenderFunc);
	m_Shapes[1].Init(&m_Models[1]->GetMesh(),NULL,&CRenderContextManager::GetInstance()->GetHUDContext(),m_mWorldPosition,CRenderShape::PNTRenderFunc); 
	
	m_bIsSelected = isSelected;
}

//Using one model
void CGui::Initialize(string model1,float x,float y, float z,float sx, float sy, float sz)
{
	m_Models[0] = CModelManager::GetInstance()->GetModel(CLoadState::GetInstance()->GetModelID(model1));
	SetPostion(x,y,z);
	SetScale(sx,sy,sz);
	m_Shapes[0].Init(&m_Models[0]->GetMesh(),NULL,&CRenderContextManager::GetInstance()->GetHUDContext(),m_mWorldPosition,CRenderShape::PNTRenderFunc);
}

//Render Two Models
void CGui::Render(D3DXMATRIX camera)
{
	if(m_bIsSelected)
	{
		m_Shapes[0].GetFrame().GetLocalMatrix() = m_mWorldPosition * camera;
		m_Shapes[0].GetRenderContext()->AddRenderNode(&m_Shapes[0]);
	}
	else
	{	
		m_Shapes[1].GetFrame().GetLocalMatrix() = m_mWorldPosition * camera;
		m_Shapes[1].GetRenderContext()->AddRenderNode(&m_Shapes[1]);
	}
}

void CGui::RenderSingleBlink(D3DXMATRIX camera)
{
	if(m_bBlink)
	{	
		m_Shapes[0].GetFrame().GetLocalMatrix() = m_mWorldPosition * camera;
		m_Shapes[0].GetRenderContext()->AddRenderNode(&m_Shapes[0]);
	}
}

void CGui::RenderBlink(D3DXMATRIX camera)
{
	if(m_bBlink)
	{	
		m_Shapes[0].GetFrame().GetLocalMatrix() = m_mWorldPosition * camera;
		m_Shapes[0].GetRenderContext()->AddRenderNode(&m_Shapes[0]);
	}
	else
	{
		m_Shapes[1].GetFrame().GetLocalMatrix() = m_mWorldPosition * camera;
		m_Shapes[1].GetRenderContext()->AddRenderNode(&m_Shapes[1]);
	}
}

//Render One Model
void CGui::Render(D3DXMATRIX world,D3DXMATRIX camera)
{
	m_Shapes[0].GetFrame().GetLocalMatrix() = world * camera;
	m_Shapes[0].GetRenderContext()->AddRenderNode(&m_Shapes[0]);
}

void CGui::UpdateBlink(float fElapsedTime,float timer)
{
	m_fBlinkTimer += fElapsedTime;

	if(m_fBlinkTimer > timer)
	{
		m_bBlink = !m_bBlink;
		m_fBlinkTimer = 0.0;
	}
}