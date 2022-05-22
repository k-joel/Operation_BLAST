#ifndef CGUI_H_
#define CGUI_H_

#include "../Mesh/CModel.h"
#include "../Rendering/CRenderShape.h"

class CGui
{
private:
	CRenderShape m_Shapes[2];
	CModel* m_Models[2];
	D3DXMATRIX m_mWorldPosition;
	bool m_bIsSelected;
	bool m_bBlink;
	float m_fBlinkTimer;
public:
	CGui();
	~CGui();
	void SetPostion(float x,float y,float z);
	void SetScale(float x, float y, float z);

	//For the Two Model gui. Affects the render call
	void SetIsSelected(bool isSelected){m_bIsSelected = isSelected;}
	//Two Models
	void Initialize(string model1,string model2,float x = 0.0f, float y = 0.0f, float z = 0.0f,float sx = 1.0f, float sy = 1.0f, float sz = 1.0f, bool isSelected = false);
	//One Model
	void Initialize(string model1,float x = 0.0f,float y = 0.0f, float z = 0.0f,float sx = 1.0f, float sy = 1.0f, float sz = 1.0f);
	//Two Models
	void Render(D3DXMATRIX camera);
	//Two Models with Blink
	void RenderBlink(D3DXMATRIX camera);
	//One Model Blink
	void RenderSingleBlink(D3DXMATRIX camera);

	//One Model
	void Render(D3DXMATRIX world,D3DXMATRIX camera);

	void UpdateBlink(float fElapsedTime,float timer);
};


#endif