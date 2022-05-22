#ifndef _AK_ALLPLUGINSFACTORIES_H_
#define _AK_ALLPLUGINSFACTORIES_H_

#include <AK/AkPlatforms.h>
#include <AK/SoundEngine/Common/AkTypes.h>

// Effect plug-ins
#include <AK/Plugin/AkCompressorFXFactory.h>					// Compressor
#include <AK/Plugin/AkDelayFXFactory.h>							// Delay
#include <AK/Plugin/AkMatrixReverbFXFactory.h>					// Matrix reverb
#include <AK/Plugin/AkMeterFXFactory.h>							// Meter
#include <AK/Plugin/AkExpanderFXFactory.h>						// Expander
#include <AK/Plugin/AkParametricEQFXFactory.h>					// Parametric equalizer
#include <AK/Plugin/AkPeakLimiterFXFactory.h>					// Peak limiter
#include <AK/Plugin/AkSoundSeedImpactFXFactory.h>				// SoundSeed Impact
#include <AK/Plugin/AkRoomVerbFXFactory.h>						// RoomVerb
#include <AK/Plugin/AkGuitarDistortionFXFactory.h>				// Guitar distortion
#include <AK/Plugin/AkFlangerFXFactory.h>						// Flanger
#include <AK/Plugin/AkConvolutionReverbFXFactory.h>				// Convolution reverb
#include <AK/Plugin/AkTremoloFXFactory.h>						// Tremolo

// Platform specific
#ifdef AK_WII
#include <AK/Plugin/WiiPluginsFXFactory.h>						// All Wii plug-ins
#endif

// McDSP plug-ins
#include <AK/Plugin/McDSPFutzBoxFXFactory.h>					// FutzBox
#include <AK/Plugin/McDSPLimiterFXFactory.h>					// ML1 Limiter

// Sources plug-ins
#include <AK/Plugin/AkSilenceSourceFactory.h>					// Silence generator
#include <AK/Plugin/AkSineSourceFactory.h>						// Sine wave generator
#include <AK/Plugin/AkToneSourceFactory.h>						// Tone generator
#include <AK/Plugin/AkAudioInputSourceFactory.h>				// Audio input
#include <AK/Plugin/AkSoundSeedWooshFactory.h>					// SoundSeed Woosh
#include <AK/Plugin/AkSoundSeedWindFactory.h>					// SoundSeed Wind
#ifdef AK_WIN
#include <AK/Plugin/AkMP3SourceFactory.h>						// MP3 source Note: Useable only on PC. Ok to include it on other platforms as long as it is not referenced.
#endif

// Required by codecs plug-ins
#include <AK/Plugin/AkVorbisFactory.h>
#ifdef AK_XBOX360
#include <AK/Plugin/AkXWMAFactory.h>		// Note: Useable only on Xbox 360. Ok to include it on other platforms as long as it is not referenced.
#endif
#ifdef AK_APPLE
#include <AK/Plugin/AkAACFactory.h>			// Note: Useable only on Apple devices. Ok to include it on other platforms as long as it is not referenced.
#endif

// Rumble support
#if defined AK_PS3 || defined AK_WII || defined AK_XBOX360 || defined AK_WIN
#include <AK/Plugin/AkRumbleFactory.h>
#include <AK/Plugin/AkMotionGeneratorFactory.h>
#endif

#endif // _AK_ALLPLUGINSFACTORIES_H_
