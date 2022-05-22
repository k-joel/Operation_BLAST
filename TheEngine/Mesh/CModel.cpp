#include "CModel.h"
#include "../Core/Util.h"

CModel::CModel()
{
}

CModel::~CModel()
{
}

void CModel::duplicateControler(CAnimationControler& toDuplicateInto)
{
	toDuplicateInto.animRef = m_animControler.animRef;
	toDuplicateInto.modelRef = m_animControler.modelRef;

	toDuplicateInto.m_palet.resize(m_animControler.getNumberOfMatrixsInPalet());
	toDuplicateInto.m_worlds.resize(m_animControler.getNumberOfMatrixsInPalet());

	toDuplicateInto.SetAnimation("default");
	toDuplicateInto.m_fElapsedTimes[0] = 0.0f;
	toDuplicateInto.m_fElapsedTimes[1] = 0.0f;
	toDuplicateInto.m_fElapsedTimes[2] = 0.0f;
	toDuplicateInto.m_fElapsedTimes[3] = 0.0f;
	//toDuplicateInto.update(0.0f);
}