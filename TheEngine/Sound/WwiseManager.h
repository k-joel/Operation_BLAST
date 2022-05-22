#pragma once

// Turn into a singleton

#include "../Sound/WWise/AK/SoundEngine/Common/AKTypes.h"

#include "../Sound//WWISE/AK/SoundEngine/Common/AkMemoryMgr.h"
#include "../Sound//WWISE/AK/SoundEngine/Common/AkModule.h"
#include "../Sound//WWISE/AK/SoundEngine/Common/IAkStreamMgr.h"
#include "../Sound//WWISE/AK/Tools/Common/AkPlatformFuncs.h"
#include "../Sound//WWISE/AK/SoundEngine/Common/AkSoundEngine.h"
#include "../Sound//WWISE/AK/MusicEngine/Common/AkMusicEngine.h"

#include "../Sound//WWISE/AK/SoundEngine/Common/AkStreamMgrModule.h"
#include "../Sound//WWISE/SoundIO/AkFilePackageLowLevelIOBlocking.h"
#include "../Sound//WWISE/SoundIO/AkDefaultIOHookBlocking.h"

//#include "../Sound//WWISE/Samples/Wwise_IDs.h"
#include "../Resources/Sound/Wwise_IDs.h"
#include "../Sound//WWISE/AK/Plugin/AkVorbisFactory.h"

#include "../Sound//WWISE/AK/SoundEngine/Common/IAkPlugin.h"
#include "../Sound//WWISE/AK/MotionEngine/Common/AkMotionEngine.h"
#include "../Sound//WWISE/AK/Plugin/AkRumbleFactory.h"
#include "../Sound//WWISE/AK/Plugin/AkMotionGeneratorFactory.h"
#include "../Sound//WWISE/AK/Plugin/AkPeakLimiterFXFactory.h"
#include "../Sound//WWISE/AK/Plugin/AkCompressorFXFactory.h"

#ifndef AK_OPTIMIZED
#include "../Sound//WWISE/AK/Comm/AkCommunication.h"
#endif // AK_OPTIMIZED


class CWwiseManager
{
private:

	CAkFilePackageLowLevelIOBlocking g_lowLevelIO;

	// Default Constructor
	CWwiseManager(void);
	// Copy Constructor
	CWwiseManager(const CWwiseManager&);
	// Assignment Operator
	CWwiseManager& operator=(const CWwiseManager &);
	// Destructor
	~CWwiseManager(void);


public:

	static CWwiseManager* GetInstance(void);

	bool Init(HWND hWnd);
	bool Shutdown(void);
	void ProcessSound(void);

	void SetFXVolume(float fVolume);
	void SetDXVolume(float fVolume);
	void SetMXVolume(float fVolume);
	void SetMasterVolume(float fVolume);

	void SetState(AkStateGroupID group, AkStateID state);
	void SetSwitch(AkSwitchGroupID group, AkSwitchStateID state, AkGameObjectID ID);

	int RegisterObject(AkGameObjectID ID, const char* szName);
	void UnregisterObject(AkGameObjectID ID);
	int UnregisterAllObjects(void);
	
	void SetObjectPosition(AkGameObjectID ID, AkSoundPosition Pos);
	void SetListenerPosition(AkListenerPosition Pos);

	void SetListenerScale(int nListener, float fScale);
	void SetAttenuation(AkGameObjectID obj, float fAttenuation);

	void PostEvent(AkUniqueID EventID, AkGameObjectID obj);


};
