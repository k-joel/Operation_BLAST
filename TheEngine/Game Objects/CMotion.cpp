#include "CMotion.h"

#include "../Debug System/DebugSystem.h"

CMotionControler::CMotionControler()
{}

CMotionControler::~CMotionControler()
{}

D3DXMATRIX CMotionControler::GetWorldMatrix()
{
	return m_world;
}

D3DXMATRIX CMotionControler::GetWorldMatrix(unsigned int currentFrame)
{
	currentFrame = currentFrame % motionRef->m_Frames.size();

	m_currentFrame = &(motionRef->m_Frames[currentFrame]);
	m_nextFrame = &(motionRef->m_Frames[(currentFrame+1)%motionRef->m_Frames.size()]);

	m_world = m_currentFrame->m_world;
	m_goal = m_currentFrame->m_world;
	m_currentTime = m_currentFrame->m_time;

	return m_world;
}

D3DXMATRIX CMotionControler::GetWorldMatrix(float currentTime, float dt, float rate)
{
	if(m_world == m_goal)
	{
		//applies full matrix
		if(currentTime >= 0.0f)
		{
			//set new current time
			m_currentTime = currentTime;
		}
		else
		{
			//update old current time with dt
			m_currentTime += dt;
		}

		while(m_currentTime > motionRef->m_maxTime)
		{
			m_currentTime = motionRef->m_maxTime;
		}

		while(m_currentTime < 0.0f)
		{
			m_currentTime = 0.0f;
		}

		//find current frames
		for(unsigned int frameIndex = 0; frameIndex < motionRef->m_Frames.size(); frameIndex +=1)
		{
			m_nextFrame = &(motionRef->m_Frames[frameIndex]);
			m_currentFrame = &(motionRef->m_Frames[(frameIndex-1)%motionRef->m_Frames.size()]);
			if(m_currentTime < motionRef->m_Frames[frameIndex].m_time)
			{
				break;
			}
		}

		//calculate lambda
		float lambda =
			((m_currentTime - m_currentFrame->m_time)
			/
			(m_nextFrame->m_time - m_currentFrame->m_time));

		if(lambda > 1.0f || lambda < 0.0f)
		{
			unsigned int badShit = 0;
		}

		//matrix math
		m_goal = m_currentFrame->m_world * (1.0f-lambda) + m_nextFrame->m_world * (lambda);

#ifdef _DEBUG
		{
			CDebugSystem::GetInstance()->RegisterData("camera goal lambda", lambda);
		}
#endif
	}

	//interp bettween current and goal using rate
	D3DXVECTOR3 fromCurrentToGoal;
	float interp;

	fromCurrentToGoal = D3DXVECTOR3(m_goal._41, m_goal._42, m_goal._43) - D3DXVECTOR3(m_world._41, m_world._42, m_world._43);

	interp = min(rate/D3DXVec3Length(&fromCurrentToGoal), 1.0f);

	m_world = m_world * (1.0f - interp) + m_goal * interp;

#ifdef _DEBUG
	{
		CDebugSystem::GetInstance()->RegisterData("camera rail time", m_currentTime);
		CDebugSystem::GetInstance()->RegisterData("camera world to goal interp", interp);
		CDebugSystem::GetInstance()->RegisterData("elapsed time", dt);
	}
#endif

	return m_world;
}

void CMotion::duplicateControler(CMotionControler& toDuplicateInto)
{
	toDuplicateInto.m_currentTime = 0.0f;
	D3DXMatrixIdentity(&toDuplicateInto.m_world);
	toDuplicateInto.motionRef = this;
}