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

#ifndef _AK_BANKREADHELPERS_H_
#define _AK_BANKREADHELPERS_H_
/// Read data from bank and advance pointer.
template< typename T > 
inline T ReadBankData( 
						AkUInt8*& in_rptr 
#ifndef AK_OPTIMIZED
						,AkUInt32& in_rSize
#endif
						)
{
	T l_Value;
#if defined(AK_IOS) || defined(AK_3DS)
	typedef struct {T t;} __attribute__((__packed__)) packedStruct;
	l_Value = ((packedStruct *)in_rptr)->t;
#else
	l_Value = *( ( T* )in_rptr );
#endif

	in_rptr += sizeof( T );
#ifndef AK_OPTIMIZED
	in_rSize -= sizeof( T );
#endif
	return l_Value;
}

#ifdef AK_XBOX360
/// Handle reading float not aligned on proper memory boundaries (banks are byte packed).
inline AkReal32 AlignFloat( AkReal32* __unaligned ptr )
{
	return *ptr;
}

/// Read data from bank and advance pointer.
template<> 
inline AkReal32 ReadBankData<AkReal32>( 
									   AkUInt8*& in_rptr
#ifndef AK_OPTIMIZED
									   ,AkUInt32& in_rSize
#endif
									   )
{
	AkReal32 l_Value = AlignFloat( ( AkReal32* )in_rptr );
	in_rptr += sizeof( AkReal32 );
#ifndef AK_OPTIMIZED
	in_rSize -= sizeof( AkReal32 );
#endif
	return l_Value;
}

/// Handle reading float not aligned on proper memory boundaries (banks are byte packed).
inline AkReal64 AlignFloat( AkReal64* __unaligned ptr )
{
	return *ptr;
}

/// Read data from bank and advance pointer.
template<> 
inline AkReal64 ReadBankData<AkReal64>( 
									   AkUInt8*& in_rptr
#ifndef AK_OPTIMIZED
									   ,AkUInt32& in_rSize
#endif
									   )
{
	AkReal64 l_Value = AlignFloat( ( AkReal64* )in_rptr );
	in_rptr += sizeof( AkReal64 );
#ifndef AK_OPTIMIZED
	in_rSize -= sizeof( AkReal64 );
#endif
	return l_Value;
}

#endif //AK_XBOX360

#if defined( AK_NGP )
	/// Handle reading float not aligned on proper memory boundaries (banks are byte packed).
	inline AkReal32 AlignFloat( AkReal32 __unaligned * ptr )
	{
		return *ptr;
	}
	
	/// Read data from bank and advance pointer.
	template<> 
	inline AkReal32 ReadBankData<AkReal32>( 
										   AkUInt8*& in_rptr
	#ifndef AK_OPTIMIZED
										   ,AkUInt32& in_rSize
	#endif
										   )
	{
		AkReal32 l_Value = AlignFloat( ( AkReal32* )in_rptr );
		in_rptr += sizeof( AkReal32 );
	#ifndef AK_OPTIMIZED
		in_rSize -= sizeof( AkReal32 );
	#endif
		return l_Value;
	}
	
	/// Handle reading float not aligned on proper memory boundaries (banks are byte packed).
	inline AkReal64 AlignFloat( AkReal64 __unaligned * ptr )
	{
		return *ptr;
	}
	
	/// Read data from bank and advance pointer.
	template<> 
	inline AkReal64 ReadBankData<AkReal64>( 
										   AkUInt8*& in_rptr
	#ifndef AK_OPTIMIZED
										   ,AkUInt32& in_rSize
	#endif
										   )
	{
		AkReal64 l_Value = AlignFloat( ( AkReal64* )in_rptr );
		in_rptr += sizeof( AkReal64 );
	#ifndef AK_OPTIMIZED
		in_rSize -= sizeof( AkReal64 );
	#endif
		return l_Value;
	}

#endif // defined( AK_NGP )

#if defined (AK_PS3) || defined(AK_WII)
/// Type conversion helper on some platforms.
template < typename TO, typename FROM >
inline TO union_cast( FROM value )
{
    union { FROM from; TO to; } convert;
    convert.from = value;
    return convert.to;
}

/// Handle reading float not aligned on proper memory boundaries (banks are byte packed).
inline AkReal32 AlignFloat( AkReal32* ptr )
{
	AkUInt32 *puint = reinterpret_cast<AkUInt32 *>( ptr );
    volatile AkUInt32 uint = *puint;
    return union_cast<AkReal32>( uint );
}

/// Read data from bank and advance pointer.
template<> 
inline AkReal32 ReadBankData<AkReal32>( 
									   AkUInt8*& in_rptr
#ifndef AK_OPTIMIZED
									   ,AkUInt32& in_rSize
#endif
									   )
{
	AkReal32 l_Value = AlignFloat( ( AkReal32* )in_rptr );
	in_rptr += sizeof( AkReal32 );
#ifndef AK_OPTIMIZED
	in_rSize -= sizeof( AkReal32 );
#endif
	return l_Value;
}

/// Handle reading float not aligned on proper memory boundaries (banks are byte packed).
inline AkReal64 AlignFloat( AkReal64* ptr )
{
	AkUInt64 *puint = reinterpret_cast<AkUInt64 *>( ptr );
    volatile AkUInt64 uint = *puint;
    return union_cast<AkReal64>( uint );
}

/// Read data from bank and advance pointer.
template<> 
inline AkReal64 ReadBankData<AkReal64>( 
									   AkUInt8*& in_rptr
#ifndef AK_OPTIMIZED
									   ,AkUInt32& in_rSize
#endif
									   )
{
	AkReal64 l_Value = AlignFloat( ( AkReal64* )in_rptr );
	in_rptr += sizeof( AkReal64 );
#ifndef AK_OPTIMIZED
	in_rSize -= sizeof( AkReal64 );
#endif
	return l_Value;
}
#endif // AK_PS3 || AK_WII


#ifndef AK_OPTIMIZED

/// Read and return bank data of a given type, incrementing running pointer and decrementing block size for debug tracking purposes
#define READBANKDATA( _Type, _Ptr, _Size )		\
		ReadBankData<_Type>( _Ptr, _Size )

/// Skip over some bank data  of a given type, incrementing running pointer and decrementing block size for debug tracking purposes
#define SKIPBANKDATA( _Type, _Ptr, _Size )		\
		( _Ptr ) += sizeof( _Type );	\
		( _Size ) -= sizeof( _Type )

/// Skip over some bank data by a given size in bytes, incrementing running pointer and decrementing block size for debug tracking purposes
#define SKIPBANKBYTES( _NumBytes, _Ptr, _Size )	\
		( _Ptr ) += _NumBytes;		\
		( _Size ) -= _NumBytes

#else

/// Read and return bank data of a given type, incrementing running pointer and decrementing block size for debug tracking purposes
#define READBANKDATA( _Type, _Ptr, _Size )		\
		ReadBankData<_Type>( _Ptr )

/// Skip over some bank data  of a given type, incrementing running pointer and decrementing block size for debug tracking purposes
#define SKIPBANKDATA( _Type, _Ptr, _Size )		\
		( _Ptr ) += sizeof( _Type )

/// Skip over some bank data by a given size in bytes, incrementing running pointer and decrementing block size for debug tracking purposes
#define SKIPBANKBYTES( _NumBytes, _Ptr, _Size )	\
		( _Ptr ) += _NumBytes;

#endif


#ifndef AK_OPTIMIZED

/// Helper macro to determine whether the full content of a block of memory was properly parsed
#define CHECKBANKDATASIZE( _DATASIZE_, _ERESULT_ )		\
	if( _DATASIZE_ != 0 && _ERESULT_ == AK_Success )	\
	{													\
		AKASSERT( _DATASIZE_ == 0 );					\
		_ERESULT_ = AK_Fail;							\
	}

#else
/// Helper macro to determine whether the full content of a block of memory was properly parsed
#define CHECKBANKDATASIZE(_DATASIZE_, _ERESULT_ )
#endif


#endif //_AK_BANKREADHELPERS_H_
