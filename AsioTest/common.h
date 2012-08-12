#ifndef CMI_COMMON_H__
#define CMI_COMMON_H__

// Windows defines
#ifdef _WIN32

#define NOMINMAX // We want to use only the std versions

// VC++ (all versions) Pragmas
#ifdef _MSC_VER
#pragma warning(disable: 4068) // Unknown pragma warning
#endif //_MSC_VER

// VC++ 7.0 Pragmas
#if _MSC_VER >= 1300
#pragma warning(disable: 4290)
#endif

//#ifndef _WIN32_WINNT
//#define _WIN32_WINNT 0x0400
//#endif

#endif //_WIN32

#include <stdint.h>

#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <set>

namespace CMI
{
#ifdef __GNUC__
    typedef int8_t Int8;
    typedef int16_t Int16;
    typedef int32_t Int32;
    typedef int64_t Int64;
    typedef uint8_t UInt8;
    typedef uint16_t UInt16;
    typedef uint32_t UInt32;
    typedef uint64_t UInt64;
    typedef Int64 Position;
    typedef Int64 Length;
    typedef UInt32 TrackID;
    typedef std::wstring String;
    typedef std::string ISO7String;
#else
    typedef signed char Int8;
    typedef signed short Int16;
    typedef signed long Int32;
    typedef signed __int64 Int64;
    typedef unsigned char UInt8;
    typedef unsigned short UInt16;
    typedef unsigned long UInt32;
    typedef unsigned __int64 UInt64;
    typedef Int64 Position;
    typedef Int64 Length;
    typedef UInt32 TrackID;
    typedef std::wstring String;
    typedef std::string ISO7String;
#endif //__GNUC__
}

#endif //CMI_COMMON_H__
