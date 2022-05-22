#include "../Game Objects/CBase.h"

enum Effects { FX_SMALL_RED_LAZER, FX_SMALL_BLUE_LAZER, FX_LARGE_LAZER, FX_CHARGED_LAZER, FX_MGUN_MUZZLEFLASH,
				FX_SG_MUZZLEFLASH, FX_SG_ALT_MUZZLEFLASH, FX_SG_ULT_MUZZLEFLASH, FX_EXPLOSION_CRATE, FX_EXPLOSION_BARREL,
				FX_FT_PILOTLIGHT,
				FX_ASSAULTER_MUZZLEFLASH,
				FX_SHADOW_BLOB,
				FX_HAT_STANDIN,
				FX_SG_ALT_TRAIL,
				FX_P1_ID, FX_P2_ID,
				FX_ASSAULTER_EXPLODE, FX_HELIOS_EXPLODE, FX_STRIKER_EXPLODE,
				FX_DROP_TARGET,
				FX_MAX };

class CEffect : public CBase
{
	friend class CFXManager;
private:
	CFrame* m_pOwnerFrame;
	bool m_bIsLooping;
	float m_fLife;
	float m_fCurrentLife;
	int m_nFXType;

	// lerp the BV
	bool m_bLerpSphere;
	bool m_bLerpAABB;
	CAABB m_StartAABB;
	CAABB m_EndAABB;
	CSphere m_StartSphere;
	CSphere m_EndSphere;

public:
	CEffect();
	~CEffect();

	void Update(float fElapsedTime);

	/*void SetOwner(CBase* pOwner) { m_pOwner = pOwner; }
	void SetLooping(bool bLoop) { m_bIsLooping = bLoop; }
	void SetLife(float fLife) { m_fLife = fLife; }
	void SetFXType(int nType) { m_nFXType = nType; }
	void SetLerpSphere(bool bLerp) { m_bLerpSphere = bLerp; }*/

	int GetFXType() { return m_nFXType; }
};