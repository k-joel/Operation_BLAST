/***********************************************************************
The content of this file includes source code for the sound engine
portion of the AUDIOKINETIC Wwise Technology and constitutes "Level
Two Source Code" as defined in the Source Code Addendum attached
with this file.  Any use of the Level Two Source Code shall be
subject to the terms and conditions outlined in the Source Code
Addendum and the End User License Agreement for Wwise(R).

Version: <VERSION>  Build: <BUILDNUMBER>
Copyright (c) <COPYRIGHTYEAR> Audiokinetic Inc.
***********************************************************************/

// Accumulate (+=) signal into output buffer
// To be used on mono signals, create as many instances as there are channels if need be

#ifndef _AKAPPLYGAIN_H_
#define _AKAPPLYGAIN_H_

#include <AK/SoundEngine/Common/AkTypes.h>
#include <AK/SoundEngine/Common/AkCommonDefs.h>
#include <AK/SoundEngine/Common/AkSimd.h>
#ifdef AKSIMD_V4F32_SUPPORTED
#include <AK/Plugin/PluginServices/AkVectorValueRamp.h>
#endif

namespace AK
{
	namespace DSP
	{
		/// Single channel, in-place interpolating gain helper (do not call directly) use ApplyGain instead.
		static inline void ApplyGainRamp(	
			AkSampleType * AK_RESTRICT io_pfBuffer, 
			AkReal32 in_fCurGain,
			AkReal32 in_fTargetGain,
			AkUInt32 in_uNumFrames )
		{
			AkSampleType * AK_RESTRICT pfBuf = (AkSampleType * AK_RESTRICT) io_pfBuffer;
			const AkSampleType * const pfEnd = io_pfBuffer + in_uNumFrames;

#ifdef AKSIMD_V4F32_SUPPORTED
			const AkUInt32 uNumVecIter = in_uNumFrames/4;
			CAkVectorValueRamp vGainRamp;
			AKSIMD_V4F32 vfGain = vGainRamp.Setup(in_fCurGain,in_fTargetGain,uNumVecIter*4);	
			const AkReal32 * const pfVecEnd = (AkReal32 *) io_pfBuffer + uNumVecIter*4;
			while ( pfBuf < pfVecEnd )
			{
				AKSIMD_V4F32 vfIn = AKSIMD_LOAD_V4F32(pfBuf);
				AKSIMD_V4F32 vfOut = AKSIMD_MUL_V4F32( vfIn, vfGain );
				AKSIMD_STORE_V4F32( pfBuf, vfOut );
				vfGain = vGainRamp.Tick();
				pfBuf+=4;
			}
#endif
			if ( pfBuf < pfEnd )
			{
				const AkReal32 fInc = (in_fTargetGain - in_fCurGain) / in_uNumFrames;
				while ( pfBuf < pfEnd )
				{
					*pfBuf = (AkSampleType)(*pfBuf * in_fCurGain);
					in_fCurGain += fInc;
					pfBuf++;
				}
			}

		}

		/// Single channel, out-of-place interpolating gain helper (do not call directly) use ApplyGain instead.
		static inline void ApplyGainRamp(	
			AkSampleType * AK_RESTRICT in_pfBufferIn, 
			AkSampleType * AK_RESTRICT out_pfBufferOut, 
			AkReal32 in_fCurGain,
			AkReal32 in_fTargetGain,
			AkUInt32 in_uNumFrames )
		{
			AkSampleType * AK_RESTRICT pfInBuf = (AkSampleType * AK_RESTRICT) in_pfBufferIn;
			AkSampleType * AK_RESTRICT pfOutBuf = (AkSampleType * AK_RESTRICT) out_pfBufferOut;
			const AkSampleType * const pfEnd = pfInBuf + in_uNumFrames;

#ifdef AKSIMD_V4F32_SUPPORTED
			const AkUInt32 uNumVecIter = in_uNumFrames/4;
			CAkVectorValueRamp vGainRamp;
			AKSIMD_V4F32 vfGain = vGainRamp.Setup(in_fCurGain,in_fTargetGain,uNumVecIter*4);	
			const AkReal32 * const pfVecEnd = (AkReal32 *) in_pfBufferIn + uNumVecIter*4;
			while ( pfInBuf < pfVecEnd )
			{
				AKSIMD_V4F32 vfIn = AKSIMD_LOAD_V4F32(pfInBuf);
				AKSIMD_V4F32 vfOut = AKSIMD_MUL_V4F32( vfIn, vfGain );
				AKSIMD_STORE_V4F32( pfOutBuf, vfOut );
				vfGain = vGainRamp.Tick();
				pfInBuf+=4;
				pfOutBuf+=4;
			}
#endif
			if ( pfInBuf < pfEnd )
			{
				const AkReal32 fInc = (in_fTargetGain - in_fCurGain) / in_uNumFrames;
				while ( pfInBuf < pfEnd )
				{
					*pfOutBuf++ = (AkSampleType)(*pfInBuf++ * in_fCurGain);
					in_fCurGain += fInc;
				}
			}
		}	

		/// Single channel, in-place static gain.
		static inline void ApplyGain(	
			AkSampleType * AK_RESTRICT io_pfBuffer, 
			AkReal32 in_fGain,
			AkUInt32 in_uNumFrames )
		{
			if ( in_fGain != 1.f )
			{
				AkSampleType * AK_RESTRICT pfBuf = (AkSampleType * AK_RESTRICT) io_pfBuffer;
				const AkSampleType * const pfEnd = io_pfBuffer + in_uNumFrames;

#ifdef AKSIMD_V4F32_SUPPORTED
				const AkUInt32 uNumVecIter = in_uNumFrames/4;
				const AkReal32 * const pfVecEnd = (AkReal32 *) io_pfBuffer + uNumVecIter*4;
				const AKSIMD_V4F32 vfGain = AKSIMD_LOAD1_V4F32( in_fGain );
				while ( pfBuf < pfVecEnd )
				{
					AKSIMD_V4F32 vfIn = AKSIMD_LOAD_V4F32(pfBuf);
					AKSIMD_V4F32 vfOut = AKSIMD_MUL_V4F32( vfIn, vfGain );
					AKSIMD_STORE_V4F32( pfBuf, vfOut );
					pfBuf+=4;
				}
#endif
				while ( pfBuf < pfEnd )
				{
					*pfBuf = (AkSampleType)(*pfBuf * in_fGain);
					pfBuf++;
				}
			}
		}

		/// Single channel, Out-of-place static gain.
		static inline void ApplyGain(	
			AkSampleType * AK_RESTRICT in_pfBufferIn, 
			AkSampleType * AK_RESTRICT out_pfBufferOut, 
			AkReal32 in_fGain,
			AkUInt32 in_uNumFrames )
		{
			AkSampleType * AK_RESTRICT pfInBuf = (AkSampleType * AK_RESTRICT) in_pfBufferIn;
			AkSampleType * AK_RESTRICT pfOutBuf = (AkSampleType * AK_RESTRICT) out_pfBufferOut;
			const AkSampleType * const pfEnd = in_pfBufferIn + in_uNumFrames;

#ifdef AKSIMD_V4F32_SUPPORTED
			const AkUInt32 uNumVecIter = in_uNumFrames/4;			
			const AkReal32 * const pfVecEnd = (AkReal32 *) in_pfBufferIn + uNumVecIter*4;
			const AKSIMD_V4F32 vfGain = AKSIMD_LOAD1_V4F32( in_fGain );
			while ( pfInBuf < pfVecEnd )
			{
				AKSIMD_V4F32 vfIn = AKSIMD_LOAD_V4F32(pfInBuf);
				AKSIMD_V4F32 vfOut = AKSIMD_MUL_V4F32( vfIn, vfGain );
				AKSIMD_STORE_V4F32( pfOutBuf, vfOut );
				pfInBuf+=4;
				pfOutBuf+=4;
			}
#endif
			while ( pfInBuf < pfEnd )
			{
				*pfOutBuf++ = (AkSampleType)(*pfInBuf++ * in_fGain);
			}
		}


		/// Single channel, In-place (possibly interpolating) gain.
		static inline void ApplyGain(	
			AkSampleType * AK_RESTRICT io_pfBuffer, 
			AkReal32 in_fCurGain,
			AkReal32 in_fTargetGain,
			AkUInt32 in_uNumFrames )
		{

			if ( in_fTargetGain == in_fCurGain )
				ApplyGain(io_pfBuffer, in_fCurGain, in_uNumFrames );
			else
				ApplyGainRamp( io_pfBuffer, in_fCurGain, in_fTargetGain, in_uNumFrames );
		}

		/// Single channel, Out-of-place (possibly interpolating) gain.
		static inline void ApplyGain(	
			AkSampleType * AK_RESTRICT in_pfBufferIn, 
			AkSampleType * AK_RESTRICT out_pfBufferOut, 
			AkReal32 in_fCurGain,
			AkReal32 in_fTargetGain,
			AkUInt32 in_uNumFrames )
		{
			if ( in_fTargetGain == in_fCurGain )
				ApplyGain(in_pfBufferIn, out_pfBufferOut, in_fCurGain, in_uNumFrames );
			else
				ApplyGainRamp( in_pfBufferIn, out_pfBufferOut, in_fCurGain, in_fTargetGain, in_uNumFrames );
		}

		/// Multi-channel in-place (possibly interpolating) gain.
		static inline void ApplyGain( 
			AkAudioBuffer * io_pBuffer,
			AkReal32 in_fCurGain,
			AkReal32 in_fTargetGain,
			bool in_bProcessLFE = true )
		{
			AkUInt32 uNumChannels = io_pBuffer->NumChannels();
			if ( !in_bProcessLFE && io_pBuffer->HasLFE() )
				uNumChannels--;
			const AkUInt32 uNumFrames = io_pBuffer->uValidFrames;
			if ( in_fTargetGain == in_fCurGain )
			{
				// No need for interpolation
				for ( AkUInt32 i = 0; i < uNumChannels; i++ )
				{
					AkSampleType * pfChan = io_pBuffer->GetChannel( i );
					ApplyGain(pfChan, in_fCurGain, uNumFrames );
				}
			}
			else
			{
				// Interpolate gains toward target
				for ( AkUInt32 i = 0; i < uNumChannels; i++ )
				{
					AkSampleType * pfChan = io_pBuffer->GetChannel( i );
					ApplyGainRamp(pfChan, in_fCurGain, in_fTargetGain, uNumFrames );
				}
			}
		}

		/// Multi-channel out-of-place (possibly interpolating) gain.
		static inline void ApplyGain( 
			AkAudioBuffer * in_pBuffer,
			AkAudioBuffer * out_pBuffer,
			AkReal32 in_fCurGain,
			AkReal32 in_fTargetGain,
			bool in_bProcessLFE = true )
		{
			AKASSERT( in_pBuffer->NumChannels() == out_pBuffer->NumChannels() );
			AkUInt32 uNumChannels = in_pBuffer->NumChannels();
			if ( !in_bProcessLFE && in_pBuffer->HasLFE() )
				uNumChannels--;
			const AkUInt32 uNumFrames = AkMin( in_pBuffer->uValidFrames, out_pBuffer->MaxFrames() );
			if ( (in_fTargetGain == in_fCurGain) )
			{
				// No need for interpolation
				for ( AkUInt32 i = 0; i < uNumChannels; i++ )
				{
					AkSampleType * pfInChan = in_pBuffer->GetChannel( i );
					AkSampleType * pfOutChan = out_pBuffer->GetChannel( i );
					ApplyGain(pfInChan, pfOutChan, in_fCurGain, uNumFrames );
				}
			}
			else
			{
				// Interpolate gains toward target
				for ( AkUInt32 i = 0; i < uNumChannels; i++ )
				{
					AkSampleType * pfInChan = in_pBuffer->GetChannel( i );
					AkSampleType * pfOutChan = out_pBuffer->GetChannel( i );
					ApplyGainRamp( pfInChan, pfOutChan, in_fCurGain, in_fTargetGain, uNumFrames );
				}
			}
		}

	} // namespace DSP
} // namespace AK

#endif // _AKAPPLYGAIN_H_
