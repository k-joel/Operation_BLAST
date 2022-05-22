#include "../Mesh/animation.h"
#include "../Mesh/CModel.h"
#include "../Rendering/vertex.h"
#include "../LuaEngine/CLuaEngine.h"

#include "../Debug System/DebugSystem.h"

void CAnimationControler::reset()
{
	m_resetLoops = true;

	m_loopCount[0] = 0;
	m_loopCount[1] = 0;
	m_loopCount[2] = 0;
	m_loopCount[3] = 0;

	m_fElapsedTimes[0] = m_blendAnimStartTime[0];
	m_fElapsedTimes[1] = m_blendAnimStartTime[1];
	m_fElapsedTimes[2] = m_blendAnimStartTime[2];
	m_fElapsedTimes[3] = m_blendAnimStartTime[3];
}

void CAnimationControler::SetAnimation(char* animName)
{
	unsigned int majorIndex = 0;
	for(; majorIndex < 4; majorIndex +=1)
	{
		if(m_blendAnimationWeight[majorIndex] == 1.0f || m_blendAnimationChange[majorIndex] > 0.0f)
		{
			break;
		}
	}

	float oldStart, oldEnd, oldRate;
	oldStart = m_blendAnimStartTime[majorIndex];
	oldEnd = m_blendAnimEndTime[majorIndex];
	oldRate = m_fRates[majorIndex];

	CLuaEngine::GetInstance()->GetAnimationData(m_blendAnimStartTime[0],modelRef->m_modelName, animName, "startTime");
	CLuaEngine::GetInstance()->GetAnimationData(m_blendAnimStartTime[1],modelRef->m_modelName, animName, "startTime");
	CLuaEngine::GetInstance()->GetAnimationData(m_blendAnimStartTime[2],modelRef->m_modelName, animName, "startTime");
	CLuaEngine::GetInstance()->GetAnimationData(m_blendAnimStartTime[3],modelRef->m_modelName, animName, "startTime");

	CLuaEngine::GetInstance()->GetAnimationData(m_blendAnimEndTime[0],modelRef->m_modelName, animName, "endTime");
	CLuaEngine::GetInstance()->GetAnimationData(m_blendAnimEndTime[1],modelRef->m_modelName, animName, "endTime");
	CLuaEngine::GetInstance()->GetAnimationData(m_blendAnimEndTime[2],modelRef->m_modelName, animName, "endTime");
	CLuaEngine::GetInstance()->GetAnimationData(m_blendAnimEndTime[3],modelRef->m_modelName, animName, "endTime");

	CLuaEngine::GetInstance()->GetAnimationData(m_fRates[0], modelRef->m_modelName, animName, "rate");
	CLuaEngine::GetInstance()->GetAnimationData(m_fRates[1], modelRef->m_modelName, animName, "rate");
	CLuaEngine::GetInstance()->GetAnimationData(m_fRates[2], modelRef->m_modelName, animName, "rate");
	CLuaEngine::GetInstance()->GetAnimationData(m_fRates[3], modelRef->m_modelName, animName, "rate");


	m_blendAnimationWeight[0] = 1.0f;
	m_blendAnimationWeight[1] = 0.0f;
	m_blendAnimationWeight[2] = 0.0f;
	m_blendAnimationWeight[3] = 0.0f;

	m_blendAnimationChange[0] = 0.0f;
	m_blendAnimationChange[1] = 0.0f;
	m_blendAnimationChange[2] = 0.0f;
	m_blendAnimationChange[3] = 0.0f;

	if(oldStart != m_blendAnimStartTime[0] ||
		oldEnd != m_blendAnimEndTime[0] ||
		oldRate != m_fRates[0])
	{
		m_resetLoops = true;
	}
	else
	{
		m_resetLoops = false;
	}

	if(m_resetLoops)
	{
		m_loopCount[0] = 0;
		m_loopCount[1] = 0;
		m_loopCount[2] = 0;
		m_loopCount[3] = 0;
	}
}

void CAnimationControler::BlendAnimation(char* animName, float timeToBlend)
{
	unsigned int lowestBlend = 0;
	for(unsigned int blendIndex = 0;blendIndex < 4; blendIndex +=1)
	{
		if( m_blendAnimationWeight[lowestBlend] > m_blendAnimationWeight[blendIndex])
		{
			lowestBlend = blendIndex;
		}
	}

	//all
	float thirdDivision = m_blendAnimationWeight[lowestBlend]/3.0f;
	m_blendAnimationWeight[0] += thirdDivision;
	m_blendAnimationWeight[1] += thirdDivision;
	m_blendAnimationWeight[2] += thirdDivision;
	m_blendAnimationWeight[3] += thirdDivision;

	m_blendAnimationChange[0] = -(m_blendAnimationWeight[0]/timeToBlend);
	m_blendAnimationChange[1] = -(m_blendAnimationWeight[1]/timeToBlend);
	m_blendAnimationChange[2] = -(m_blendAnimationWeight[2]/timeToBlend);
	m_blendAnimationChange[3] = -(m_blendAnimationWeight[3]/timeToBlend);

	//blend index
	CLuaEngine::GetInstance()->GetAnimationData(m_blendAnimStartTime[lowestBlend], modelRef->m_modelName, animName, "startTime");

	CLuaEngine::GetInstance()->GetAnimationData(m_blendAnimEndTime[lowestBlend], modelRef->m_modelName, animName, "endTime");

	CLuaEngine::GetInstance()->GetAnimationData(m_fRates[lowestBlend], modelRef->m_modelName, animName, "rate");

	m_blendAnimationChange[lowestBlend] = 1.0f/timeToBlend;
	m_blendAnimationWeight[lowestBlend] = 0.0f;
	m_fElapsedTimes[lowestBlend] = m_blendAnimStartTime[lowestBlend];

	m_loopCount[lowestBlend] = 0;
}

void CAnimationControler::update(float dt)
{
	if(!modelRef->GetMesh().GetWeightBuffer())
		return;

	float lambda[4];
	tKeyFrame * first[4], * second[4];

	unsigned int numOBones = animRef->m_pose.m_pose.size();
	unsigned int numOKeys = animRef->m_animation.m_keyframes.size();

	float sumOfWeights =
		m_blendAnimationWeight[0] +
		m_blendAnimationWeight[1] +
		m_blendAnimationWeight[2] +
		m_blendAnimationWeight[3];
	if(sumOfWeights > 1.0f || sumOfWeights < 0.0f || sumOfWeights != 1.0f)
	{
		//total weight should never be greater than 1.0 or less than 0.0, in fact it should always be 1.0
		int badShitHere = 0;
	}

	for(unsigned int blendIndex = 0; blendIndex < 4; blendIndex +=1)
	{
		for(unsigned int checkBlendIndex = blendIndex+1; checkBlendIndex < 4; checkBlendIndex +=1)
		{
			if(m_blendAnimationChange[blendIndex] > 0.0f && m_blendAnimationChange[checkBlendIndex] > 0.0f)
			{
				//no two changes should be silmultanious positive, only one channel should be increasing its weight
				int badShitHere = 0;
			}
		}
	}

	//update blend weights and elapsed times
	for(unsigned int blendIndex = 0; blendIndex < 4; blendIndex +=1)
	{
		if(m_blendAnimStartTime[blendIndex] != m_blendAnimEndTime[blendIndex])
		{
			m_fElapsedTimes[blendIndex] += m_fRates[blendIndex]*dt;

			if(m_fElapsedTimes[blendIndex] < m_blendAnimStartTime[blendIndex])
			{
				m_fElapsedTimes[blendIndex] = m_blendAnimEndTime[blendIndex];
				if(!m_resetLoops)
				{
					m_loopCount[blendIndex] -= 1;
				}
			}

			if(m_fElapsedTimes[blendIndex] >= m_blendAnimEndTime[blendIndex])
			{
				m_fElapsedTimes[blendIndex] = m_blendAnimStartTime[blendIndex];
				if(!m_resetLoops)
				{
					m_loopCount[blendIndex] += 1;
				}
			}

			m_blendAnimationWeight[blendIndex] += m_blendAnimationChange[blendIndex] * dt;

			if(m_blendAnimationWeight[blendIndex] < 0.0f)
			{
				m_blendAnimationWeight[blendIndex] = 0.0f;
				m_blendAnimationChange[blendIndex] = 0.0f;
			}

			if(m_blendAnimationWeight[blendIndex] > 1.0f)
			{
				m_blendAnimationWeight[0] = 0.0f;
				m_blendAnimationWeight[1] = 0.0f;
				m_blendAnimationWeight[2] = 0.0f;
				m_blendAnimationWeight[3] = 0.0f;

				m_blendAnimationWeight[blendIndex] = 1.0f;

				m_blendAnimationChange[0] = 0.0f;
				m_blendAnimationChange[1] = 0.0f;
				m_blendAnimationChange[2] = 0.0f;
				m_blendAnimationChange[3] = 0.0f;
			}

			//find correct keyframes
			for(unsigned int keyIndex = 0; keyIndex < numOKeys; keyIndex +=1)
			{
				if(animRef->m_animation.m_keyframes[keyIndex].m_time > m_fElapsedTimes[blendIndex])
				{
					second[blendIndex] = &animRef->m_animation.m_keyframes[keyIndex];
					first[blendIndex] = &animRef->m_animation.m_keyframes[(keyIndex-1)%numOKeys];
					break;
				}
			}
		}
		else
		{
			first[blendIndex] = &animRef->m_animation.m_keyframes[0];
			second[blendIndex] = &animRef->m_animation.m_keyframes[0];
			m_fElapsedTimes[blendIndex] = m_blendAnimStartTime[blendIndex];

			float tempWeight = m_blendAnimationWeight[blendIndex]/3.0f;
			m_blendAnimationWeight[0] += tempWeight;
			m_blendAnimationWeight[1] += tempWeight;
			m_blendAnimationWeight[2] += tempWeight;
			m_blendAnimationWeight[3] += tempWeight;
			m_blendAnimationWeight[blendIndex] = 0.0f;

			float tempChange = m_blendAnimationChange[blendIndex];

			if(tempChange > 0.0f)
			{
				for(unsigned int fixblendIndex = 0; fixblendIndex < 4; fixblendIndex += 1)
				{
					m_blendAnimationChange[fixblendIndex] = -(m_blendAnimationWeight[fixblendIndex]);
				}
				m_blendAnimationChange[(blendIndex+1)%4] = (1.0f-m_blendAnimationWeight[(blendIndex+1)%4]);
			}

			m_blendAnimationChange[blendIndex] = 0.0f;
		}
	}

	//calculate lambda of blends
	for(unsigned int blendIndex = 0; blendIndex < 4; blendIndex +=1)
	{
		lambda[blendIndex] = 0.0f;

		if(m_fElapsedTimes[blendIndex] > 0.0f)
		{
			if(m_fElapsedTimes[blendIndex] != first[blendIndex]->m_time)
			{
				if(first[blendIndex]->m_time < second[blendIndex]->m_time)
				{
					lambda[blendIndex] =
						(m_fElapsedTimes[blendIndex] - first[blendIndex]->m_time)
						/
						(second[blendIndex]->m_time - first[blendIndex]->m_time);
				}
				else
				{
					lambda[blendIndex] =
						(m_fElapsedTimes[blendIndex])
						/
						(second[blendIndex]->m_time);
				}
			}
		}
	}

	for(unsigned int boneIndex = 0; boneIndex < numOBones; boneIndex +=1)
	{
		//zero out bone matrix
		m_palet[boneIndex]._11 = 0.0f;
		m_palet[boneIndex]._12 = 0.0f;
		m_palet[boneIndex]._13 = 0.0f;
		m_palet[boneIndex]._14 = 0.0f;
		m_palet[boneIndex]._21 = 0.0f;
		m_palet[boneIndex]._22 = 0.0f;
		m_palet[boneIndex]._23 = 0.0f;
		m_palet[boneIndex]._24 = 0.0f;
		m_palet[boneIndex]._31 = 0.0f;
		m_palet[boneIndex]._32 = 0.0f;
		m_palet[boneIndex]._33 = 0.0f;
		m_palet[boneIndex]._34 = 0.0f;
		m_palet[boneIndex]._41 = 0.0f;
		m_palet[boneIndex]._42 = 0.0f;
		m_palet[boneIndex]._43 = 0.0f;
		m_palet[boneIndex]._44 = 0.0f;

		for(unsigned int blendIndex = 0; blendIndex < 4; blendIndex +=1)
		{
			D3DXMATRIX toSumWith;

			//lerp to find bone
			toSumWith =
				((first[blendIndex]->m_bones[boneIndex].m_world_matrix) * (1.0f-lambda[blendIndex]))
				+
				((second[blendIndex]->m_bones[boneIndex].m_world_matrix) * (lambda[blendIndex]));

			//sum with weight
			m_palet[boneIndex] += toSumWith * m_blendAnimationWeight[blendIndex];
		}

		//concanate inverse bind pose
		m_worlds[boneIndex] = m_palet[boneIndex];
		D3DXMatrixMultiply(&m_palet[boneIndex], &animRef->m_pose.m_pose[boneIndex].m_inv_world_matrix, &m_palet[boneIndex]);

		//w colum must be this for proper rendering
		m_palet[boneIndex]._14 = 0.0f;
		m_palet[boneIndex]._24 = 0.0f;
		m_palet[boneIndex]._34 = 0.0f;
		m_palet[boneIndex]._44 = 1.0f;

		//D3DXMatrixIdentity(&m_palet[boneIndex]);
	}

	m_resetLoops = false;
}

D3DXMATRIX* CAnimationControler::getMatrixByID(unsigned int specialID)
{
	for(unsigned int boneIndex = 0; boneIndex < animRef->m_pose.m_pose.size(); boneIndex +=1)
	{
		if(animRef->m_pose.m_pose[boneIndex].m_specialID == specialID)
		{
			return &(m_worlds[boneIndex]);
		}
	}

	return NULL;
}

D3DXMATRIX* CAnimationControler::getMatrixByIndex(unsigned int boneIndex)
{
	return &(m_worlds[boneIndex]);
}

tBindBone* CAnimationControler::getBoneByID(unsigned int specialID)
{
	for(unsigned int boneIndex = 0; boneIndex < animRef->m_pose.m_pose.size(); boneIndex +=1)
	{
		if(animRef->m_pose.m_pose[boneIndex].m_specialID == specialID)
		{
			return &(animRef->m_pose.m_pose[boneIndex]);
		}
	}

	return NULL;
}

tBindBone* CAnimationControler::getBoneByIndex(unsigned int boneIndex)
{
	return &(animRef->m_pose.m_pose[boneIndex]);
}

D3DXMATRIX* CAnimationControler::getMatrixPalet()
{
	return &(m_palet[0]);
}
unsigned int CAnimationControler::getNumberOfMatrixsInPalet()
{
	return m_palet.size();
}

unsigned int CAnimationControler::getLoopCount()
{
	for(unsigned int blendIndex = 0; blendIndex < 4; blendIndex +=1)
	{
		if(m_blendAnimationWeight[blendIndex] == 1.0f || m_blendAnimationChange[blendIndex] > 0.0f)
		{
			return m_loopCount[blendIndex];
		}
	}

	return 9001;
}

CModel* CAnimationControler::GetModel() { return modelRef; }
CMesh* CAnimationControler::GetMesh() { return &modelRef->GetMesh(); }