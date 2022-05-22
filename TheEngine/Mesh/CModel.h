#ifndef _CMODEL_
#define _CMODEL_

#include <d3d9.h>
#include "../Mesh/CMesh.h"
#include "../Mesh/animation.h"
class CModelImporter;

class CModel
{
	friend class CModelImporter;
	friend class CAnimationControler;
public:
	char m_modelName[64];

	// the Mesh
	CMesh m_Mesh;

	//Animation Class
	CAnimation m_animationData;
	//Animation Controler to be duplicated
	CAnimationControler m_animControler;
public:
	CModel();
	~CModel();

	CMesh &GetMesh() { return m_Mesh; }
	char* GetModelName() { return m_modelName; }

	void duplicateControler(CAnimationControler& toDuplicateInto);
};

#endif //_CMODEL_