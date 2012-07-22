#include "common.h"
#include "conversion.h"
#ifdef _MSC_VER
#include <cstdio>
#else
#include <stdio.h>
#endif

namespace CMI
{

template<> void toString<>(std::wstring& str, const UInt8& value)
{
    wchar_t buf[64];
    swprintf(buf, 64,  L"0x%02x (%d)", value, value);
    str = buf;
}

template<> void toString<>(std::wstring& str, const UInt16& value)
{
    wchar_t buf[64];
    swprintf(buf, 64, L"0x%04x (%d)", value, value);
    str = buf;
}

template<> void toString<>(std::wstring& str, const UInt32& value)
{
    wchar_t buf[64];
    swprintf(buf, 64, L"0x%08x (%d)", value, value);
    str = buf;
}

template<> void toString<>(std::wstring& str, const UInt64& value)
{
    // It would appear that swprintf can't deal with __int64
    wchar_t buf[64];
    swprintf(buf, 64, L"0x%08x%08x", (UInt32)(value >> 32), (UInt32) value);
    str = buf;
}

template<> void toString<>(std::wstring& str, const Int8& value)
{
    wchar_t buf[64];
    swprintf(buf, 64, L"0x%02x (%d)", value, value);
    str = buf;
}

template<> void toString<>(std::wstring& str, const Int16& value)
{
    wchar_t buf[64];
    swprintf(buf, 64, L"0x%04x (%d)", value, value);
    str = buf;
}

template<> void toString<>(std::wstring& str, const Int32& value)
{
    wchar_t buf[64];
    swprintf(buf, 64, L"0x%08x (%d)", value, value);
    str = buf;
}

template<> void toString<>(std::wstring& str, const Int64& value)
{
    // It would appear that swprintf can't deal with __int64
    wchar_t buf[64];
    swprintf(buf, 64, L"0x%08x%08x", (Int32)(value >> 32), (UInt32) value);
    str = buf;
}

template<> void toString<>(std::wstring& str, const bool& value)
{
    str = value ? L"true" : L"false";
}

template<> void toString<>(std::wstring& str, const std::wstring& value)
{
    str = value;
}

//template<> void fromString<>(UInt8& value, const std::wstring& str)
//{
//}
//
//template<> void fromString<>(UInt16& value, const std::wstring& str)
//{
//}
//
//template<> void fromString<>(UInt32& value, const std::wstring& str)
//{
//}
//
//template<> void fromString<>(UInt64& value, const std::wstring& str)
//{
//}
//
//template<> void fromString<>(Int8& value, const std::wstring& str)
//{
//}
//
//template<> void fromString<>(Int16& value, const std::wstring& str)
//{
//}
//
//template<> void fromString<>(Int32& value, const std::wstring& str)
//{
//}
//
//template<> void fromString<>(Int64& value, const std::wstring& str)
//{
//}
//
//template<> void fromString<>(bool& value, const std::wstring& str)
//{
//}
//
//template<> void fromString<>(std::wstring& value, const std::wstring& str)
//{
//}

} //namespace CMI
