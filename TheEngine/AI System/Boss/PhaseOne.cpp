#ifndef PhaseOne_h__
#define PhaseOne_h__
#include "../CState.h"
#include "../../Game Objects/CSpawner.h"
#include "../../Game Objects/Enemy/CEnemy.h"
#include "../../Game Objects/Enemy/CUltimus.h"
#include "../../Collision/CCollisionTests.h"
#include "../../Camera/Camera.h"

//This state literally does nothing but keep the boss in a loop until the player is close enough

void	CSpawnState::Enter()
{
	m_bFirstTime = true;
}

int	CSpawnState::Update(float fElapsedTime)
{

	if(((CUltimus*)m_pNPC)->GetBossFighting())
			return E_GOTOLLASERATTACK;

		return E_STAYINSTATE;
}

void	CSpawnState::Exit()
{
	m_bFirstTime = false;
}



#endif // PhaseOne_h__

