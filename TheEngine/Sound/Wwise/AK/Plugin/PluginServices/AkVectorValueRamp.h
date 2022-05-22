//////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2010 Audiokinetic Inc. / All Rights Reserved
//
//////////////////////////////////////////////////////////////////////

#ifndef _AK_VECTORVALUERAMP_H_
#define _AK_VECTORVALUERAMP_H_

#include <AK/SoundEngine/Common/AkSimd.h>

class CAkVectorValueRamp
{
public:

	AkForceInline AKSIMD_V4F32 Setup( AkReal32 in_fStartValue, AkReal32 in_fStopValue, AkUInt32 in_uNumFrames )
	{
		const AkReal32 fIncrement = (in_fStopValue-in_fStartValue)/in_uNumFrames;
		const AkReal32 f4xInc = 4.f*fIncrement;
		vIncrement = AKSIMD_LOAD1_V4F32( f4xInc);
		AK_ALIGN_SIMD( AkReal32 fVal[4] );
		fVal[0] = in_fStartValue;
		fVal[1] = fVal[0] + fIncrement;
		fVal[2] = fVal[1] + fIncrement;
		fVal[3] = fVal[2] + fIncrement;
		vValueRamp = AKSIMD_LOAD_V4F32( fVal );
		return vValueRamp;
	}

	AkForceInline AKSIMD_V4F32 Tick( )
	{
		vValueRamp = AKSIMD_ADD_V4F32( vValueRamp, vIncrement );
		return vValueRamp;
	}

private:
	AKSIMD_V4F32 vIncrement;
	AKSIMD_V4F32 vValueRamp;
};

#endif //_AK_VECTORVALUERAMP_H_
