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

// Length of delay line is mapped on 4 frames boundary (i.e. may not be suited for reverberation for example)
// Handling code for triple buffering processing on position independent code platforms is provided
// This is not a delay line implementation, but rather just some services for memory managment related 
// to specific delay line execution needs as detailed by clients

#ifndef _AKDSP_DELAYLINEMEMORY_
#define _AKDSP_DELAYLINEMEMORY_

#include <AK/SoundEngine/Common/AkTypes.h>
#include <AK/SoundEngine/Common/IAkPluginMemAlloc.h>
#include <AK/Tools/Common/AkPlatformFuncs.h>
#ifdef __SPU__
#include <AK/Plugin/PluginServices/PS3/SPUServices.h>
#endif

#define AK_ALIGN_TO_NEXT_BOUNDARY( __num__, __boundary__ ) (((__num__) + ((__boundary__)-1)) & ~((__boundary__)-1))

namespace AK
{
	namespace DSP
	{
		template < class T_SAMPLETYPE, AkUInt32 T_MAXNUMCHANNELS > 
		class CAkDelayLineMemory
		{
		public:

#ifndef __SPU__

			CAkDelayLineMemory( )
				: m_uDelayLineLength( 0 )
				, m_uOffset( 0 )
				, m_uNumChannels( 0 )
			{
				AKPLATFORM::AkMemSet( m_pDelay, 0, T_MAXNUMCHANNELS*sizeof(T_SAMPLETYPE *) );
			}

			AKRESULT Init( 
				AK::IAkPluginMemAlloc * in_pAllocator, 
				AkUInt32 in_uDelayLineLength,  
				AkUInt32 in_uNumChannels )
			{
				m_uNumChannels = in_uNumChannels;
				// Align delay length on 4 frame boundary to simplify DMA and SIMD alignement
				m_uDelayLineLength = AK_ALIGN_TO_NEXT_BOUNDARY( in_uDelayLineLength, 4 ); 
				m_uOffset = 0;
				if ( m_uDelayLineLength )
				{
					for ( AkUInt32 i = 0; i < m_uNumChannels; i++ )
					{
						m_pDelay[i] = (T_SAMPLETYPE*)AK_PLUGIN_ALLOC( in_pAllocator, AK_ALIGN_SIZE_FOR_DMA( sizeof(T_SAMPLETYPE) * m_uDelayLineLength ) );
						if ( m_pDelay[i] == NULL )
							return AK_InsufficientMemory;
					}
				}
				return AK_Success;
			}

			void Term( AK::IAkPluginMemAlloc * in_pAllocator )
			{
				for ( AkUInt32 i = 0; i < m_uNumChannels; i++ )
				{
					if ( m_pDelay[i] )
					{
						AK_PLUGIN_FREE( in_pAllocator, m_pDelay[i] );
						m_pDelay[i] = NULL;
					}
				}
				m_uDelayLineLength = 0;
			}

			void Reset( )
			{
				if ( m_uDelayLineLength )
				{
					for ( AkUInt32 i = 0; i < m_uNumChannels; i++ )
					{
						if (m_pDelay[i])
							AKPLATFORM::AkMemSet( (void*) m_pDelay[i], 0, m_uDelayLineLength*sizeof(T_SAMPLETYPE) );
					}
				}
				m_uOffset = 0;
			}

#endif	// #ifndef __SPU__	

			AkForceInline AkUInt32 GetCurrentOffset()
			{
				return m_uOffset;
			}

			AkForceInline void SetCurrentOffset( AkUInt32 in_uOffset )
			{
				m_uOffset = in_uOffset;
			}

			AkForceInline AkUInt32 GetDelayLength()
			{
				return m_uDelayLineLength;
			}

#ifdef __PPU__
			// Use this to DMA data from PPU with job scheduling to avoid blocking job upon entrance
			AkUInt32 GetPrimeDelayMemory( 
				AkUInt32 in_uMaxFrames, 
				T_SAMPLETYPE * out_pAddressToDMA[2], 
				AkUInt32 out_uDMASize[2], 
				AkUInt32 & out_uOutputBufferSize )
			{
				AKPLATFORM::AkMemSet( out_pAddressToDMA, 0, 2*sizeof(T_SAMPLETYPE*) );
				AKPLATFORM::AkMemSet( out_uDMASize, 0, 2*sizeof(AkUInt32) );
				m_uMaxStreamBufferLength = in_uMaxFrames;
				AkUInt32 uNumDMA = 0;
				out_uOutputBufferSize = AkMin((m_uNumChannels-1),2)*AkMin(m_uDelayLineLength,in_uMaxFrames)*sizeof(T_SAMPLETYPE);
				if ( m_uNumChannels )
				{
					if ( m_uDelayLineLength <= in_uMaxFrames )
					{
						// Small delay may wrap multiple times per execution, send whole delay line over
						out_pAddressToDMA[0] = m_pDelay[0];
						out_uDMASize[0] = m_uDelayLineLength*sizeof(T_SAMPLETYPE);
						++uNumDMA;
					}
					else
					{
						// Large delay, only DMA necessary section for this execution
						const AkUInt32 uNumFramesFirstDMA = AkMin( m_uDelayLineLength - m_uOffset, in_uMaxFrames );		
						out_pAddressToDMA[0] = m_pDelay[0] + m_uOffset;	
						out_uDMASize[0] = uNumFramesFirstDMA*sizeof(T_SAMPLETYPE);
						++uNumDMA;
						if ( uNumFramesFirstDMA < in_uMaxFrames )
						{
							// Large DMA will wrap during execution, 2 DMAs are necessary
							out_pAddressToDMA[1] = m_pDelay[0];
							out_uDMASize[1] = (in_uMaxFrames-uNumFramesFirstDMA)*sizeof(T_SAMPLETYPE);
							++uNumDMA;
						}
					}
				}
				return uNumDMA;
			}

#endif	// #ifdef __PPU__

#ifdef __SPU__
			// Setup delay memory double buffering
			void InitSPU( AkReal32 * in_pPrimeDataDMABuffer, AkReal32 * in_pOutputBuffer )
			{
				m_uStreamIndex = 0;
				AKPLATFORM::AkMemSet( m_pfSreamedData, 0, 3*sizeof(T_SAMPLETYPE*) );
				if ( m_uNumChannels >= 1 )
					m_pfSreamedData[0] = in_pPrimeDataDMABuffer;
				if ( m_uNumChannels >= 2 ) 
					m_pfSreamedData[1] = in_pOutputBuffer;
				if ( m_uNumChannels >= 3 ) 
					m_pfSreamedData[2] = &in_pOutputBuffer[m_uMaxStreamBufferLength];
			}

			void GetChannel( AkUInt32 in_uChannelIndex, AkUInt32 in_uDMATag )
			{
				if ( (in_uChannelIndex < m_uNumChannels) && (in_uChannelIndex >= 1) )
				{
					const AkUInt32 uNextBuffer = (m_uStreamIndex+1)%3;
					AKASSERT( m_pfSreamedData[uNextBuffer] != NULL );
					if ( m_uDelayLineLength <= m_uMaxStreamBufferLength )
					{
						// Small delay may wrap multiple times per execution, send whole delay line over
						T_SAMPLETYPE * pAddressToGet = m_pDelay[in_uChannelIndex];
						AkUInt32 uDataSize = m_uDelayLineLength*sizeof(T_SAMPLETYPE);
						AkDmaGet( "DelayFXDSP::DelayMem", m_pfSreamedData[uNextBuffer], (std::uint64_t)pAddressToGet, uDataSize, in_uDMATag, 0, 0);
					}
					else
					{
						// Large delay, only DMA necessary section for this execution
						T_SAMPLETYPE * pAddressToGet = m_pDelay[in_uChannelIndex] + m_uOffset;
						AkUInt32 uNumFramesFirstDMA = AkMin( m_uDelayLineLength - m_uOffset, m_uMaxStreamBufferLength );
						AkUInt32 uDataSize = uNumFramesFirstDMA*sizeof(T_SAMPLETYPE);
						AkDmaGet( "DelayFXDSP::DelayMem(1)", m_pfSreamedData[uNextBuffer], (std::uint64_t)pAddressToGet, uDataSize, in_uDMATag, 0, 0);
						if ( uNumFramesFirstDMA < m_uMaxStreamBufferLength )
						{
							// Large DMA will wrap during execution, 2 DMAs are necessary
							pAddressToGet = m_pDelay[in_uChannelIndex];
							uDataSize = (m_uMaxStreamBufferLength-uNumFramesFirstDMA)*sizeof(T_SAMPLETYPE);
							AkDmaGet( "DelayFXDSP::DelayMem(2)", m_pfSreamedData[uNextBuffer]+uNumFramesFirstDMA, (std::uint64_t)pAddressToGet, uDataSize, in_uDMATag, 0, 0);
						}
					}
				}
			}

			void Wait( AkUInt32 in_uDMATag )
			{
				// no more than 1 (set) DMA in flight (wait on previous DMA completion)
				if ( m_uNumChannels > 1 )
					AkDmaWait(1<<in_uDMATag);  
			}

			void PutChannel( AkUInt32 in_uChannelIndex, AkUInt32 in_uDMATag )
			{	
				AKASSERT( m_pfSreamedData[m_uStreamIndex] != NULL );
				if ( m_uDelayLineLength <= m_uMaxStreamBufferLength )
				{
					// Small delay may wrap multiple times per execution, send whole delay line over
					T_SAMPLETYPE * pAddressToPut = m_pDelay[in_uChannelIndex];
					AkUInt32 uDataSize = m_uDelayLineLength*sizeof(T_SAMPLETYPE);
					AkDmaPut( "DelayFXDSP::DelayMem", m_pfSreamedData[m_uStreamIndex], (std::uint64_t)pAddressToPut, uDataSize, in_uDMATag, 0, 0);
				}
				else
				{
					// Large delay, only DMA necessary section for this execution
					T_SAMPLETYPE * pAddressToPut = m_pDelay[in_uChannelIndex] + m_uOffset;
					AkUInt32 uNumFramesFirstDMA = AkMin( m_uDelayLineLength - m_uOffset, m_uMaxStreamBufferLength );
					AkUInt32 uDataSize = uNumFramesFirstDMA*sizeof(T_SAMPLETYPE);
					AkDmaPut( "DelayFXDSP::DelayMem(1)", m_pfSreamedData[m_uStreamIndex], (std::uint64_t)pAddressToPut, uDataSize, in_uDMATag, 0, 0);

					if ( uNumFramesFirstDMA < m_uMaxStreamBufferLength )
					{
						// Large DMA will wrap during execution, 2 DMAs are necessary
						pAddressToPut = m_pDelay[in_uChannelIndex];
						uDataSize = (m_uMaxStreamBufferLength-uNumFramesFirstDMA)*sizeof(T_SAMPLETYPE);
						AkDmaPut( "DelayFXDSP::DelayMem(2)", m_pfSreamedData[m_uStreamIndex]+uNumFramesFirstDMA, (std::uint64_t)pAddressToPut, uDataSize, in_uDMATag, 0, 0);
					}
				}

				m_uStreamIndex = ++m_uStreamIndex % 3;
			}
#endif // #ifdef __SPU__

			T_SAMPLETYPE * GetCurrentPointer( AkUInt32 in_uOffset, AkUInt32 in_uChannelIndex )
			{
#ifndef __SPU__
				return m_pDelay[in_uChannelIndex] + in_uOffset;
#else	
				// Use triple buffer local storage memory
				// m_pfSreamedData[m_uStreamIndex] points to the proper (ready) channel
				T_SAMPLETYPE * pfReadyData = m_pfSreamedData[m_uStreamIndex];
				AKASSERT( pfReadyData != NULL );

				if ( m_uDelayLineLength <= m_uMaxStreamBufferLength )
				{
					// Small delays (which can wrap multiple times per execution) are always retrieved entirely, use absolute offset
					pfReadyData += in_uOffset;
				}
				else
				{		
					// Large delays are always retrieved partially, use relative offset
					if ( in_uOffset >= m_uOffset )
					{
						// delay has not wrapped, simply consider relative offset
						AkUInt32 uDMAOffset = in_uOffset - m_uOffset;
						pfReadyData += uDMAOffset;
					}
					else
					{
						// Delay has wrapped, need to offset by the size of the first DMA
						pfReadyData += AkMin( m_uDelayLineLength - m_uOffset, m_uMaxStreamBufferLength ) + in_uOffset;
					}
				}

				return pfReadyData;
#endif
			}

		protected:

			T_SAMPLETYPE *	m_pDelay[T_MAXNUMCHANNELS];	// Delay lines for each channel
			AkUInt32		m_uDelayLineLength;			// Total delay line length    
			AkUInt32		m_uOffset;					// Current delay line write position
			AkUInt32		m_uNumChannels;				// Number of delayed channels

#ifdef AK_PS3 // Necessary handling for triple buffering in PIC context
			T_SAMPLETYPE *	m_pfSreamedData[3];
			AkUInt32		m_uStreamIndex;
			AkUInt32		m_uMaxStreamBufferLength;
#endif

		};

	} // namespace DSP
} // namespace AK

#endif // _AKDSP_DELAYLINEMEMORY_