//////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2006 Audiokinetic Inc. / All Rights Reserved
//
//////////////////////////////////////////////////////////////////////

// AkLock.h

/// \file 
/// Platform independent synchronization services for plug-ins.

#ifndef _AK_TOOLS_COMMON_AKLOCK_H
#define _AK_TOOLS_COMMON_AKLOCK_H

#include <AK/AkPlatforms.h>

#ifdef AK_WIN
#include <AK/Tools/Win32/AkLock.h>

#elif defined (AK_XBOX360)
#include <AK/Tools/XBox360/AkLock.h>

#elif defined (AK_PS3)
#include <AK/Tools/PS3/AkLock.h>

#elif defined (AK_WII)
#include <AK/Tools/Wii/AkLock.h>

#elif defined (AK_APPLE)
#include <AK/Tools/Mac/AkLock.h>

#elif defined (AK_NGP)
#include <AK/Tools/NGP/AkLock.h>

#elif defined (AK_3DS)
#include <AK/Tools/3DS/AkLock.h>

#else
#error AkLock.h: Undefined platform
#endif

#endif // _AK_TOOLS_COMMON_AKLOCK_H
