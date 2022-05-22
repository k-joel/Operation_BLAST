#pragma once

#include "CBase.h"

enum AITypes{TEST};
class CCharacter: public CBase
{
private:
	bool m_bIsDead;
	float m_fHealth;
	float m_fMaxHealth;
	int m_nLives;
	bool m_bOnPlatform;
	CCharacter* m_pCurrentTarget;
	D3DXVECTOR3 m_vRunVelocity;

protected:
	int m_nName;
	
public:
	CCharacter();
	~CCharacter();

	//Accessors
	bool GetIsDead()		const { return m_bIsDead;	}
	float GetCurrentHealth()	const { return m_fHealth;	}
	float GetMaxHealth()		const { return m_fMaxHealth;}
	int GetNumLives()		const { return m_nLives; }
	bool GetIsOnPlatform() const {return m_bOnPlatform; }
	D3DXVECTOR3 GetRunVelocity() const {return m_vRunVelocity; }

	// Mutators
	void SetIsDead(bool bIsDead)		{ m_bIsDead = bIsDead; }
	void SetCurrentHealth(float fHealth)	{ m_fHealth = fHealth; }
	void SetMaxHealth(float fHealth)	{ m_fMaxHealth = fHealth; }
	void SetNumLives(int nLives)		{ m_nLives = nLives; }
	void SetIsonPlatform(bool bOnPlatform) {m_bOnPlatform = bOnPlatform; }
	void SetRunVelocity(D3DXVECTOR3 vRunVelocity) {m_vRunVelocity = vRunVelocity; }


	/************************************************************************/
	/* AI STUFF                                                             */
	/************************************************************************/
	void		SelectTarget(list<CBase*> pTargets, bool bIsEnemy = true);
	void		SetCurrentTarget(CCharacter* pTarget) {m_pCurrentTarget = pTarget;}
	CCharacter* GetCurrentTarget() const {return m_pCurrentTarget; }
	bool		GetInDistance(float fRadius);
	void		MoveTowardsTarget();

	
};