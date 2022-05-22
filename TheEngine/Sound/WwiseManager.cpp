#include "WwiseManager.h"

// Custom alloc/free functions. These are declared as "extern" in AkMemoryMgr.h
// and MUST be defined by the game developer.
//namespace AK
//{
//#ifdef WIN32
//    void * AllocHook( size_t in_size )
//    {
//        return malloc( in_size );
//    }
//    void FreeHook( void * in_ptr )
//    {
//        free( in_ptr );
//    }
//    // Note: VirtualAllocHook() may be used by I/O pools of the default implementation
//    // of the Stream Manager, to allow "true" unbuffered I/O (using FILE_FLAG_NO_BUFFERING
//    // - refer to the Windows SDK documentation for more details). This is NOT mandatory;
//    // you may implement it with a simple malloc().
//    void * VirtualAllocHook(
//        void * in_pMemAddress,
//        size_t in_size,
//        DWORD in_dwAllocationType,
//        DWORD in_dwProtect
//        )
//    {
//        return VirtualAlloc( in_pMemAddress, in_size, in_dwAllocationType, in_dwProtect );
//    }
//    void VirtualFreeHook( 
//        void * in_pMemAddress,
//        size_t in_size,
//        DWORD in_dwFreeType
//        )
//    {
//        VirtualFree( in_pMemAddress, in_size, in_dwFreeType );
//    }
//#endif
//}



CWwiseManager::CWwiseManager(void)
{
}

CWwiseManager::~CWwiseManager(void)
{
}

CWwiseManager* CWwiseManager::GetInstance(void)
{
	static CWwiseManager instance;
	return &instance;
}

// Defined to perform everything related to initialization of the sound engine
bool CWwiseManager::Init(HWND hWnd)
{
	// memory manager
	AkMemSettings memSettings;
	memSettings.uMaxNumPools = 20;

	if( AK::MemoryMgr::Init( &memSettings ) != AK_Success)
	{
		assert( ! "Could not create the memory manager." );
		return false;
	}

	// Streaming manager
	AkStreamMgrSettings stmSettings;
	AK::StreamMgr::GetDefaultSettings( stmSettings );

	if ( !AK::StreamMgr::Create( stmSettings ) )
	{
		assert( ! "Could not create the Streaming Manager" );
		return false;
	}

	AkDeviceSettings deviceSettings;
	AK::StreamMgr::GetDefaultDeviceSettings( deviceSettings );

	if ( g_lowLevelIO.Init( deviceSettings ) != AK_Success )
	{
		assert( ! "Could not create the streaming device and Low-Level I/O system" );
		return false;
	}

	// Sound Engine
	AkInitSettings initSettings;
	AkPlatformInitSettings platformInitSettings;
	AK::SoundEngine::GetDefaultInitSettings( initSettings );
	AK::SoundEngine::GetDefaultPlatformInitSettings( platformInitSettings );

	//platformInitSettings.hWnd = hWnd;

	if ( AK::SoundEngine::Init( &initSettings, &platformInitSettings ) != AK_Success )
	{
		assert( ! "Could not initialize the Sound Engine." );
		return false;
	}

	// Music Engine
	AkMusicSettings musicInit;
	AK::MusicEngine::GetDefaultInitSettings( musicInit );

	if ( AK::MusicEngine::Init( &musicInit ) != AK_Success )
	{
		assert( ! "Could not initialize the Music Engine." );
		return false;
	}

	// Communications
#ifndef AK_OPTIMIZED
	AkCommSettings commSettings;
	AK::Comm::GetDefaultInitSettings( commSettings );
	if ( AK::Comm::Init( commSettings ) != AK_Success )
	{
		assert( ! "Could not initialize communication." );
		return false;
	}
#endif // AK_OPTIMIZED


	// register the codec
	AK::SoundEngine::RegisterCodec(AKCOMPANYID_AUDIOKINETIC, AKCODECID_VORBIS,
		CreateVorbisFilePlugin, CreateVorbisBankPlugin);

	// Rumble Code
	AK::MotionEngine::RegisterMotionDevice(
	AKCOMPANYID_AUDIOKINETIC, /*Plugin’s company ID*/
      AKMOTIONDEVICEID_RUMBLE,  /*Plugin’s device ID*/
      AkCreateRumblePlugin );   /*Creation function*/

	AK::SoundEngine::RegisterPlugin(
	AkPluginTypeMotionSource,     /*Plugin type.  This one is for motion devices*/
      AKCOMPANYID_AUDIOKINETIC,     /*Plugin’s company ID*/
      AKSOURCEID_MOTIONGENERATOR,   /*Plugin’s device ID*/
      AkCreateMotionGenerator,      /*Creation function*/
      AkCreateMotionGeneratorParams);     /*Parameters creation function*/

	// Peak Limiter
	AK::SoundEngine::RegisterPlugin( AkPluginTypeEffect, 
                                  AKCOMPANYID_AUDIOKINETIC, 
                                  AKEFFECTID_PEAKLIMITER,
                                  CreatePeakLimiterFX,
                                  CreatePeakLimiterFXParams );

	// Compressor
	AK::SoundEngine::RegisterPlugin( AkPluginTypeEffect, 
                                 AKCOMPANYID_AUDIOKINETIC, 
                                 AKEFFECTID_COMPRESSOR,
                                 CreateCompressorFX,
                                 CreateCompressorFXParams );

	// Load the banks
	// TODO: Change strings to actual bank names
	AkBankID    bankID; // not used in this sample.
	AKRESULT eResult = AK::SoundEngine::LoadBank( L"Resources/Sound/Windows/Init.bnk", AK_DEFAULT_POOL_ID, bankID );
	if( eResult == AK_Success )
	{
		eResult = AK::SoundEngine::LoadBank( L"Resources/Sound/Windows/SoundBank.bnk", AK_DEFAULT_POOL_ID, bankID );
	}

	return true;

}

bool CWwiseManager::Shutdown(void)
{
	PostEvent(AK::EVENTS::STOP_ALL, AK_UnknownObject);
	ProcessSound();
	// Unregister the game objects
	UnregisterAllObjects();

	// Unload the banks
	AKRESULT eResult = AK::SoundEngine::ClearBanks();

	// close communicatison (if debug only)
#ifndef AK_OPTIMIZED
	AK::Comm::Term();
#endif// AK_OPTIMIZED

	// shutdown the music engine
	AK::MusicEngine::Term();

	// shutdown the sound engine
	AK::SoundEngine::Term();

	// shutdown the streaming manager
	g_lowLevelIO.Term();

	if( AK::IAkStreamMgr::Get() )
	{
		AK::IAkStreamMgr::Get()->Destroy();
	}

	// shutdown the memory manager
	AK::MemoryMgr::Term();

	return true;
}

void CWwiseManager::ProcessSound(void)
{
	AK::SoundEngine::RenderAudio();
}

void CWwiseManager::SetFXVolume(float fVolume)
{
	AK::SoundEngine::SetRTPCValue( AK::GAME_PARAMETERS::FX_VOLUME, fVolume, AK_INVALID_GAME_OBJECT);
}

void CWwiseManager::SetDXVolume(float fVolume)
{
	AK::SoundEngine::SetRTPCValue( AK::GAME_PARAMETERS::DX_VOLUME, fVolume, AK_INVALID_GAME_OBJECT);
}

void CWwiseManager::SetMXVolume(float fVolume)
{
	AK::SoundEngine::SetRTPCValue( AK::GAME_PARAMETERS::MX_VOLUME, fVolume, AK_INVALID_GAME_OBJECT);
}

void CWwiseManager::SetMasterVolume(float fVolume)
{
	AK::SoundEngine::SetRTPCValue( AK::GAME_PARAMETERS::MASTER_VOLUME, fVolume, AK_INVALID_GAME_OBJECT);
}

void CWwiseManager::SetState(AkStateGroupID group, AkStateID state)
{
	AK::SoundEngine::SetState(group, state);
}

void CWwiseManager::SetSwitch(AkSwitchGroupID group, AkSwitchStateID state, AkGameObjectID ID)
{
	AK::SoundEngine::SetSwitch(group, state, ID);
}

int CWwiseManager::RegisterObject(AkGameObjectID ID, const char* szName)
{
	return AK::SoundEngine::RegisterGameObj(ID, szName);
}

void CWwiseManager::UnregisterObject(AkGameObjectID ID)
{
	AK::SoundEngine::UnregisterGameObj(ID);
}

int CWwiseManager::UnregisterAllObjects(void)
{
	return AK::SoundEngine::UnregisterAllGameObj();
}

void CWwiseManager::SetObjectPosition(AkGameObjectID ID, AkSoundPosition Pos)
{
	AK::SoundEngine::SetPosition(ID, Pos);
}

void CWwiseManager::SetListenerPosition(AkListenerPosition Pos)
{
	AK::SoundEngine::SetListenerPosition(Pos);
}

void CWwiseManager::SetAttenuation(AkGameObjectID ID, float fAttenuation)
{
	AK::SoundEngine::SetAttenuationScalingFactor( ID, fAttenuation);
}

void CWwiseManager::SetListenerScale(int nListener, float fScale)
{
	AK::SoundEngine::SetListenerScalingFactor(nListener, fScale);
}

void CWwiseManager::PostEvent(AkUniqueID EventID, AkGameObjectID obj)
{
	AK::SoundEngine::PostEvent(EventID, obj);
}