#ifndef _AK_ALLPLUGINSREGISTRATIONHELPERS_H_
#define _AK_ALLPLUGINSREGISTRATIONHELPERS_H_

#include <AK/Plugin/AllPluginsFactories.h>
#if defined AK_PS3 || defined AK_WII || defined AK_XBOX360 || defined AK_WIN
#include <AK/MotionEngine/Common/AkMotionEngine.h>
#endif


namespace AK
{
	namespace SoundEngine
	{

		#if !defined(AK_WII) && !defined(AK_3DS)
			#define AK_SOFTWARE_EFFECT_PLATFORM
		#endif

		/// Early return if anything goes wrong
		#define RETURN_ON_FAILURE( __FUNCCALL__ )	\
		{											\
			AKRESULT eResult = (__FUNCCALL__);		\
			if ( eResult != AK_Success )			\
			return eResult;							\
		}

		/// Note: This a convenience method for rapid prototyping. 
		/// To reduce executable code size register/link only the plug-ins required by your game 
		static AKRESULT RegisterAllEffectPlugins()
		{
#if defined(AK_SOFTWARE_EFFECT_PLATFORM)

			RETURN_ON_FAILURE( AK::SoundEngine::RegisterPlugin( 
				AkPluginTypeEffect, 
				AKCOMPANYID_AUDIOKINETIC, 
				AKEFFECTID_COMPRESSOR,
				CreateCompressorFX,
				CreateCompressorFXParams ) );

			RETURN_ON_FAILURE( AK::SoundEngine::RegisterPlugin( 
				AkPluginTypeEffect, 
				AKCOMPANYID_AUDIOKINETIC, 
				AKEFFECTID_EXPANDER,
				CreateExpanderFX,
				CreateExpanderFXParams ) );

			RETURN_ON_FAILURE( AK::SoundEngine::RegisterPlugin( 
				AkPluginTypeEffect, 
				AKCOMPANYID_AUDIOKINETIC, 
				AKEFFECTID_PEAKLIMITER,
				CreatePeakLimiterFX,
				CreatePeakLimiterFXParams ) );

			RETURN_ON_FAILURE( AK::SoundEngine::RegisterPlugin( 
				AkPluginTypeEffect, 
				AKCOMPANYID_AUDIOKINETIC, 
				AKEFFECTID_MATRIXREVERB,
				CreateMatrixReverbFX,
				CreateMatrixReverbFXParams ) );

			RETURN_ON_FAILURE( AK::SoundEngine::RegisterPlugin( 
				AkPluginTypeEffect, 
				AKCOMPANYID_AUDIOKINETIC, 
				AKEFFECTID_ROOMVERB,
				CreateRoomVerbFX,
				CreateRoomVerbFXParams ) );

			RETURN_ON_FAILURE( AK::SoundEngine::RegisterPlugin( 
				AkPluginTypeEffect, 
				AKCOMPANYID_AUDIOKINETIC, 
				AKEFFECTID_CONVOLUTIONREVERB,
				CreateConvolutionReverbFX,
				CreateConvolutionReverbFXParams ) );

			RETURN_ON_FAILURE( AK::SoundEngine::RegisterPlugin( 
				AkPluginTypeEffect, 
				AKCOMPANYID_AUDIOKINETIC, 
				AKEFFECTID_DELAY,
				CreateDelayFX,
				CreateDelayFXParams ) );

			RETURN_ON_FAILURE( AK::SoundEngine::RegisterPlugin( 
				AkPluginTypeEffect, 
				AKCOMPANYID_AUDIOKINETIC, 
				AKEFFECTID_FLANGER,
				CreateFlangerFX,
				CreateFlangerFXParams ) );

			RETURN_ON_FAILURE( AK::SoundEngine::RegisterPlugin( 
				AkPluginTypeEffect, 
				AKCOMPANYID_AUDIOKINETIC, 
				AKEFFECTID_TREMOLO,
				CreateTremoloFX,
				CreateTremoloFXParams ) );

			RETURN_ON_FAILURE( AK::SoundEngine::RegisterPlugin( 
				AkPluginTypeEffect, 
				AKCOMPANYID_AUDIOKINETIC, 
				AKEFFECTID_GUITARDISTORTION,
				CreateGuitarDistortionFX,
				CreateGuitarDistortionFXParams ) );

			RETURN_ON_FAILURE( AK::SoundEngine::RegisterPlugin( 
				AkPluginTypeEffect, 
				AKCOMPANYID_AUDIOKINETIC, 
				AKEFFECTID_PARAMETRICEQ,
				CreateParametricEQFX,
				CreateParametricEQFXParams ) );

			RETURN_ON_FAILURE( AK::SoundEngine::RegisterPlugin( 
				AkPluginTypeEffect, 
				AKCOMPANYID_AUDIOKINETIC, 
				AKEFFECTID_METER,
				CreateMeterFX,
				CreateMeterFXParams ) );

#elif defined(AK_WII)

			RETURN_ON_FAILURE( AK::SoundEngine::RegisterPlugin( 
				AkPluginTypeEffect, 
				AKCOMPANYID_AUDIOKINETIC, 
				AKEFFECTID_WII_STANDARD_REVERB,
				CreateWiiReverbStd,
				CreateWiiReverbStdFXParams ) );

			RETURN_ON_FAILURE( AK::SoundEngine::RegisterPlugin( 
				AkPluginTypeEffect, 
				AKCOMPANYID_AUDIOKINETIC, 
				AKEFFECTID_WII_HIGH_QUALITY_REVERB,
				CreateWiiReverbHi,
				CreateWiiReverbHiFXParams ) );

			RETURN_ON_FAILURE( AK::SoundEngine::RegisterPlugin( 
				AkPluginTypeEffect, 
				AKCOMPANYID_AUDIOKINETIC, 
				AKEFFECTID_WII_CHORUS,
				CreateWiiChorus,
				CreateWiiChorusFXParams ) );

			RETURN_ON_FAILURE( AK::SoundEngine::RegisterPlugin( 
				AkPluginTypeEffect, 
				AKCOMPANYID_AUDIOKINETIC, 
				AKEFFECTID_WII_DELAY,
				CreateWiiDelay,
				CreateWiiDelayFXParams ) );

#endif

			return AK_Success;
		}

		/// Note: This a convenience method for rapid prototyping. 
		/// To reduce executable code size register/link only the plug-ins required by your game 
		static AKRESULT RegisterAllSourcePlugins()
		{
			RETURN_ON_FAILURE( AK::SoundEngine::RegisterPlugin( 
				AkPluginTypeSource, 
				AKCOMPANYID_AUDIOKINETIC, 
				AKSOURCEID_SILENCE,
				CreateSilenceSource,
				CreateSilenceSourceParams ) );

			RETURN_ON_FAILURE( AK::SoundEngine::RegisterPlugin( 
				AkPluginTypeSource, 
				AKCOMPANYID_AUDIOKINETIC, 
				AKSOURCEID_SINE,
				CreateSineSource,
				CreateSineSourceParams ) );

			RETURN_ON_FAILURE( AK::SoundEngine::RegisterPlugin( 
				AkPluginTypeSource, 
				AKCOMPANYID_AUDIOKINETIC, 
				AKSOURCEID_TONE,
				CreateToneSource,
				CreateToneSourceParams ) );

#ifdef AK_WIN
			RETURN_ON_FAILURE( AK::SoundEngine::RegisterPlugin( 
				AkPluginTypeSource, 
				AKCOMPANYID_AUDIOKINETIC, 
				AKSOURCEID_MP3,
				CreateMP3Source,
				CreateMP3SourceParams ) );
#endif

#if !defined( AK_NGP ) && !defined( AK_IOS ) && !defined( AK_3DS )
			RETURN_ON_FAILURE( AK::SoundEngine::RegisterPlugin( 
				AkPluginTypeSource, 
				AKCOMPANYID_AUDIOKINETIC, 
				AKSOURCEID_AUDIOINPUT,
				CreateAudioInputSource,
				CreateAudioInputSourceParams ) );
#endif

			return AK_Success;
		}

		/// Note: This a convenience method for rapid prototyping. 
		/// To reduce executable code size register/link only the plug-ins required by your game 
		static AKRESULT RegisterMcDSPPlugins()
		{
#ifdef AK_SOFTWARE_EFFECT_PLATFORM
			RETURN_ON_FAILURE( AK::SoundEngine::RegisterPlugin( 
				AkPluginTypeEffect, 
				AKCOMPANYID_MCDSP, 
				AKEFFECTID_MCDSPFUTZBOX,
				CreateMcDSPFutzBoxFX,
				CreateMcDSPFutzBoxFXParams ) );

			RETURN_ON_FAILURE( AK::SoundEngine::RegisterPlugin( 
				AkPluginTypeEffect, 
				AKCOMPANYID_MCDSP, 
				AKEFFECTID_MCDSPML1,
				CreateMcDSPML1FX,
				CreateMcDSPML1FXParams ) );
#endif

			return AK_Success;
		}

		/// Note: This a convenience method for rapid prototyping. 
		/// To reduce executable code size register/link only the plug-ins required by your game 
		static AKRESULT RegisterSoundSeedPlugins()
		{
#ifdef AK_SOFTWARE_EFFECT_PLATFORM
			RETURN_ON_FAILURE( AK::SoundEngine::RegisterPlugin( 
				AkPluginTypeEffect, 
				AKCOMPANYID_AUDIOKINETIC, 
				AKEFFECTID_SOUNDSEEDIMPACT,
				CreateAkSoundSeedImpactFX,
				CreateAkSoundSeedImpactFXParams ) );
#endif

#ifndef AK_3DS
			RETURN_ON_FAILURE( AK::SoundEngine::RegisterPlugin( 
				AkPluginTypeSource, 
				AKCOMPANYID_AUDIOKINETIC, 
				AKSOURCEID_SOUNDSEEDWIND,
				CreateSoundSeedWind,
				CreateSoundSeedWindParams ) );

			RETURN_ON_FAILURE( AK::SoundEngine::RegisterPlugin( 
				AkPluginTypeSource, 
				AKCOMPANYID_AUDIOKINETIC, 
				AKSOURCEID_SOUNDSEEDWOOSH,
				CreateSoundSeedWoosh,
				CreateSoundSeedWooshParams ) );
#endif

			return AK_Success;
		}

		/// Note: This a convenience method for rapid prototyping. 
		/// To reduce executable code size register/link only the plug-ins required by your game 
		static AKRESULT RegisterAllCodecPlugins()
		{
#ifndef AK_3DS
			AK::SoundEngine::RegisterCodec(
				AKCOMPANYID_AUDIOKINETIC,
				AKCODECID_VORBIS,
				CreateVorbisFilePlugin,
				CreateVorbisBankPlugin );
#endif

#ifdef AK_XBOX360
			AK::SoundEngine::RegisterCodec(
				AKCOMPANYID_AUDIOKINETIC,
				AKCODECID_XWMA,
				CreateXWMAFilePlugin,
				CreateXWMABankPlugin );
#endif

#ifdef AK_APPLE
			AK::SoundEngine::RegisterCodec(
				AKCOMPANYID_AUDIOKINETIC,
				AKCODECID_AAC,
				CreateAACFilePlugin,
				CreateAACBankPlugin );
#endif

			return AK_Success;

		}

		/// Note: This a convenience method for rapid prototyping. 
		/// To reduce executable code size register/link only the plug-ins required by your game 
		static AKRESULT RegisterAllRumblePlugins()
		{
#if defined AK_PS3 || defined AK_WII || defined AK_XBOX360 || defined AK_WIN

			RETURN_ON_FAILURE( AK::SoundEngine::RegisterPlugin( 
				AkPluginTypeMotionSource,
				AKCOMPANYID_AUDIOKINETIC,
				AKSOURCEID_MOTIONGENERATOR,
				AkCreateMotionGenerator,
				AkCreateMotionGeneratorParams ) );

			AK::MotionEngine::RegisterMotionDevice( 
				AKCOMPANYID_AUDIOKINETIC, 
				AKMOTIONDEVICEID_RUMBLE,
				AkCreateRumblePlugin );

#endif

			return AK_Success;
		}

		/// Note: This a convenience method for rapid prototyping. 
		/// To reduce executable code size register/link only the plug-ins required by your game 
		static AKRESULT RegisterAllPlugins()
		{
			RETURN_ON_FAILURE( RegisterAllEffectPlugins() );
			RETURN_ON_FAILURE( RegisterAllSourcePlugins() );
			RETURN_ON_FAILURE( RegisterSoundSeedPlugins() );
			RETURN_ON_FAILURE( RegisterMcDSPPlugins() );
			RETURN_ON_FAILURE( RegisterAllCodecPlugins() );
			RETURN_ON_FAILURE( RegisterAllRumblePlugins() );
			return AK_Success;
		}

	} // SoundEngine
} // AK

#endif // _AK_ALLPLUGINSREGISTRATIONHELPERS_H_
