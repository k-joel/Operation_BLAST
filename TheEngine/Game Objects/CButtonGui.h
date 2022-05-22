#ifndef CBUTTONGUI_H_
#define CBUTTONGUI_H_

#include "../Mesh/CModel.h"
#include "../Rendering/CRenderShape.h"

class CButtonGui
{
private:
	CRenderShape m_Shapes[10];
	CRenderShape m_ShapesUnselected[10];

	CModel* m_Models[10];
	D3DXMATRIX m_mWorldPosition;
	bool m_bIsController;
	bool m_bIsPaused;
public:
	CButtonGui();
	~CButtonGui();
	void SetPosition(float x,float y,float z);
	void SetScale(float x, float y, float z);
	void Initialize(bool isPaused);
	void Render(D3DXMATRIX camera,bool UD, bool LR, bool Accept, bool Back, bool isController);
};


#endif