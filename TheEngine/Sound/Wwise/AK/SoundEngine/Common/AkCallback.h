//////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2006 Audiokinetic Inc. / All Rights Reserved
//
//////////////////////////////////////////////////////////////////////

// AkCallback.h

/// \file 
/// Declaration of callback prototypes


#ifndef _AK_CALLBACK_H_
#define _AK_CALLBACK_H_

#include <AK/SoundEngine/Common/AkCommonDefs.h>

/// Type of callback. Used as a bitfield in methods AK::SoundEngine::PostEvent() and AK::SoundEngine::DynamicSequence::Open().
enum AkCallbackType
{
	AK_EndOfEvent					= 0x0001,	///< Callback triggered when reaching the end of an event.
	AK_EndOfDynamicSequenceItem		= 0x0002,	///< Callback triggered when reaching the end of a dynamic sequence item.
	AK_Marker						= 0x0004,	///< Callback triggered when encountering a marker during playback.
	AK_Duration						= 0x0008,	///< Callback triggered when the duration of the sound is known by the sound engine.

	AK_SpeakerVolumeMatrix			= 0x0010,   ///< Callback triggered at each frame, letting the client modify the speaker volume matrix

	AK_MusicPlayStarted				= 0x0080,	///< Callback triggered when a "Play" or "Seek" command has been executed ("Seek" commands are issued from AK::SoundEngine::SeekOnEvent()). Applies to objects of the Interactive-Music Hierarchy only.
	
	AK_MusicSyncBeat				= 0x0100,	///< Enable notifications on Music Beat.
	AK_MusicSyncBar					= 0x0200,	///< Enable notifications on Music Bar.
	AK_MusicSyncEntry				= 0x0400,	///< Enable notifications on Music Entry Point.
	AK_MusicSyncExit				= 0x0800,	///< Enable notifications on Music Exit Point.

	AK_MusicSyncGrid				= 0x1000,	///< Enable notifications on Music Grid.
	AK_MusicSyncUserCue				= 0x2000,	///< Enable notifications on Music User Cue.

	AK_MusicSyncPoint				= 0x4000,	///< Enable notifications on Music synchronisation point.

	AK_MusicSyncAll					= 0xff00,	///< Use this flag if you want to receive all notifications concerning AK_MusicSync registration.

	AK_CallbackBits					= 0xffff,	///< Bitmask for all callback types.

	// Not a callback type, but needs to be part of same bitfield for AK::SoundEngine::PostEvent().
	AK_EnableGetSourcePlayPosition = 0x10000,	///< Enable play position information for use by AK::SoundEngine::GetSourcePlayPosition().
	AK_EnableGetMusicPlayPosition  = 0x20000	///< Enable play position information of music objects, queried via AK::MusicEngine::GetPlayingSegmentInfo().
};

/// Callback information structure used as base for all notifications handled by AkCallbackFunc.
/// \sa 
/// - AK::SoundEngine::PostEvent()
/// - \ref soundengine_events
struct AkCallbackInfo
{
	void *			pCookie;		///< User data, passed to PostEvent()
	AkGameObjectID	gameObjID;		///< Game object ID
};

/// Callback information structure corresponding to AK_EndOfEvent and AK_MusicPlayStarted.
/// \sa 
/// - AK::SoundEngine::PostEvent()
/// - \ref soundengine_events
struct AkEventCallbackInfo : public AkCallbackInfo
{
	AkPlayingID		playingID;		///< Playing ID of Event, returned by PostEvent()
	AkUniqueID		eventID;		///< Unique ID of Event, passed to PostEvent()
};

/// Callback information structure corresponding to AK_Marker.
/// \sa 
/// - AK::SoundEngine::PostEvent()
/// - \ref soundengine_events
/// - \ref soundengine_markers
struct AkMarkerCallbackInfo : public AkEventCallbackInfo
{
	AkUInt32	uIdentifier;		///< Cue point identifier
	AkUInt32	uPosition;			///< Position in the cue point (unit: sample frames)
	const char*	strLabel;			///< Label of the marker, read from the file
};

/// Callback information structure corresponding to AK_FileDuration.
/// \sa 
/// - AK::SoundEngine::PostEvent()
/// - \ref soundengine_events
struct AkDurationCallbackInfo : public AkEventCallbackInfo
{
	AkReal32	fDuration;				///< Duration of the sound (unit: milliseconds )
	AkReal32	fEstimatedDuration;		///< Estimated duration of the sound depending on source settings such as pitch. (unit: milliseconds )
	AkUniqueID	audioNodeID;			///< Audio Node ID of playing item
};

/// Callback information structure corresponding to AK_EndOfDynamicSequenceItem.
/// \sa 
/// - AK::SoundEngine::PostEvent()
/// - AK::SoundEngine::DynamicSequence::Open()
/// - \ref soundengine_events
struct AkDynamicSequenceItemCallbackInfo : public AkCallbackInfo
{
	AkPlayingID playingID;			///< Playing ID of Dynamic Sequence, returned by AK::SoundEngine:DynamicSequence::Open()
	AkUniqueID	audioNodeID;		///< Audio Node ID of finished item
	void*		pCustomInfo;		///< Custom info passed to the DynamicSequence::Open function
};

/// Callback information structure corresponding to AK_SpeakerVolumeMatrix.
/// Volumes are read-write, linear values normalized to the [0..1] range. Source channels are following the 
/// order defined in AkCommonDefs.h. This callback will be called at every audio frame for every playing sound 
/// registered to the callback; it is crucial that the processing done in the callback be lightweight and non-blocking.
/// \sa 
/// - AK::SoundEngine::PostEvent()
/// - \ref soundengine_events
struct AkSpeakerVolumeMatrixCallbackInfo : public AkEventCallbackInfo
{
	AkSpeakerVolumes * pVolumesDry[ AK_VOICE_MAX_NUM_CHANNELS ]; ///< For each channel in the sound, pointer to AkSpeakerVolume describing contribution to destination channels (dry mix). 
	AkSpeakerVolumes * pVolumesWet[ AK_VOICE_MAX_NUM_CHANNELS ]; ///< For each channel in the sound, pointer to AkSpeakerVolume describing contribution to destination channels (wet mix). Only present if bIsEnvironmental is true
	AkUInt32 uChannelMask;			///< Speaker setup of the sound (for example, AK_SPEAKER_SETUP_STEREO)
	bool bIsEnvironmental;			///< True if the sound is feeding an environmental bus
};

/// Callback information structure corresponding to Ak_MusicSync.
/// If you need the Tempo, you can compute it using the fBeatDuration
/// Tempo (beats per minute) = 60/fBeatDuration
/// \sa 
/// - \ref soundengine_events
/// - AK::SoundEngine::PostEvent()
/// - \ref soundengine_music_callbacks
struct AkMusicSyncCallbackInfo : public AkCallbackInfo
{
	AkPlayingID playingID;			///< Playing ID of Event, returned by PostEvent()
	AkCallbackType musicSyncType;	///< Would be either AK_MusicSyncEntry, AK_MusicSyncBeat, AK_MusicSyncBar, AK_MusicSyncExit, AK_MusicSyncGrid, AK_MusicSyncPoint or AK_MusicSyncUserCue.
	AkReal32 fBeatDuration;			///< Beat Duration in seconds.
	AkReal32 fBarDuration;			///< Bar Duration in seconds.
	AkReal32 fGridDuration;			///< Grid duration in seconds.
	AkReal32 fGridOffset;			///< Grid offset in seconds.
};

/// Function called on completion of an event, or when a marker is reached.
/// \param in_eType Type of callback.
/// \param in_pCallbackInfo Pointer to structure containing callback information. This pointer is invalidated as soon as the callback function returns.
/// \remarks An event is considered completed once all of its actions have been executed and all the playbacks in this events are terminated.
/// \remarks This callback is executed from the audio processing thread. The processing time in the callback function should be minimal. Having too much processing time could result in slowing down the audio processing.
/// \remarks Before waiting on an AK_EndOfEvent, make sure that the event is going to end. 
/// Some events can be continuously playing or infinitely looping, and the callback will not occur unless a specific stop event is sent to terminate the event.
/// \sa 
/// - AK::SoundEngine::PostEvent()
/// - AK::SoundEngine::DynamicSequence::Open()
/// - \ref soundengine_events
/// - \ref soundengine_markers
/// - \ref soundengine_music_callbacks
AK_CALLBACK( void, AkCallbackFunc )( 
	AkCallbackType in_eType, 
	AkCallbackInfo* in_pCallbackInfo 
	);

/// Callback prototype used with asynchronous bank load/unload requests.
/// This function is called when the bank request has been processed 
/// and indicates if it was successfully executed or if an error occurred.
/// \param in_bankID Identifier of the bank that was explicitly loaded/unloaded. 
/// In the case of PrepareEvent() or PrepareGameSyncs(), this value contains 
/// the AkUniqueID of the event/game sync that was prepared/unprepared, if the array contained only
/// one element. Otherwise, in_bankID equals AK_INVALID_UNIQUE_ID.
/// \param in_eLoadResult Result of the requested action.
///	- AK_Success: Load or unload successful.
/// - AK_IDNotFound: At least one of the event/game sync identifiers passed to PrepareEvent() or PrepareGameSyncs() does not exist.
/// - AK_InsufficientMemory: Insufficient memory to store bank data.
/// - AK_BankReadError: I/O error.
/// - AK_WrongBankVersion: Invalid bank version: make sure the version of Wwise that 
/// you used to generate the SoundBanks matches that of the SDK you are currently using.
/// - AK_InvalidFile: File specified could not be opened.
/// - AK_InvalidParameter: Invalid parameter.
/// - AK_Fail: Load or unload failed for any other reason.
/// \param in_memPoolId ID of the memory pool in which the bank was explicitly loaded/unloaded. 
/// AK_DEFAULT_POOL_ID is returned whenever this callback is issued from an implicit bank load (PrepareEvent(), PrepareGameSyncs()), 
/// the bank memory was managed internally, an error occured, or in the case of.
/// \param in_pCookie Optional cookie that was passed to the bank request.
/// \remarks This callback is executed from the bank thread. The processing time in the callback function should be minimal. Having too much processing time could slow down the bank loading process.
/// \sa 
/// - AK::SoundEngine::LoadBank()
/// - AK::SoundEngine::UnloadBank()
/// - AK::SoundEngine::PrepareEvent()
/// - AK::SoundEngine::PrepareGameSyncs()
/// - \ref soundengine_banks
AK_CALLBACK( void, AkBankCallbackFunc )(
	AkBankID		in_bankID,
	AKRESULT		in_eLoadResult,
	AkMemPoolId		in_memPoolId,
	void *			in_pCookie
	);

/// Callback prototype used for global callback registration.
/// \param in_bLastCall true when called for the last time, upon sound engine termination.
/// \remarks This callback is executed from the main audio thread. The processing time in the callback function should be minimal. Having too much processing time could cause voice starvation.
/// - AK::SoundEngine::RegisterGlobalCallback()
/// - AK::SoundEngine::UnregisterGlobalCallback()
AK_CALLBACK( void, AkGlobalCallbackFunc )(
	bool in_bLastCall
	);

#endif // _AK_CALLBACK_H_

