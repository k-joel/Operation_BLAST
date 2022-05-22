#include "CSceneObject.h"

CSceneObject::CSceneObject()
{
	SetType(OBJ_SCENE);
	SetIsAffectedByGrav(false);
}

CSceneObject::~CSceneObject()
{
}