#ifndef CMI_CONVERSION_H__
#define CMI_CONVERSION_H__

#include <string>

namespace CMI
{

    template<class T> inline void unpack(T* lhs, const UInt8* data, UInt32 length)
    {
        //*lhs = T(data, length);
        T borlandSop(data, length);
        *lhs = borlandSop;
    }

    template<> inline void unpack<>(bool* lhs, const UInt8* data, UInt32 length)
    {
#pragma unused (length)
        *lhs = data[0] != 0;
    }

    template<> inline void unpack<>(UInt8* lhs, const UInt8* data, UInt32 length)
    {
#pragma unused (length)
        *lhs = data[0];
    }

    template<> inline void unpack<>(UInt16* lhs, const UInt8* data, UInt32 length)
    {
#pragma unused (length)
        *lhs = ((UInt16) data[0] << 8) |
            data[1];
    }

    template<> inline void unpack<>(UInt32* lhs, const UInt8* data, UInt32 length)
    {
#pragma unused (length)
        *lhs = ((UInt32) data[0] << 24) |
            ((UInt32) data[1] << 16) |
            ((UInt32) data[2] << 8) |
            data[3];
    }

    template<> inline void unpack<>(UInt64* lhs, const UInt8* data, UInt32 length)
    {
#pragma unused (length)
        *lhs = ((UInt64) data[0] << 56) |
            ((UInt64) data[1] << 48) |
            ((UInt64) data[2] << 40) |
            ((UInt64) data[3] << 32) |
            ((UInt64) data[4] << 24) |
            ((UInt64) data[5] << 16) |
            ((UInt64) data[6] << 8) |
            data[7];
    }

    template<> inline void unpack<>(Int8* lhs, const UInt8* data, UInt32 length)
    {
#pragma unused (length)
        *lhs = data[0];
    }

    template<> inline void unpack<>(Int16* lhs, const UInt8* data, UInt32 length)
    {
#pragma unused (length)
        *lhs = ((Int16) data[0] << 8) |
            data[1];
    }

    template<> inline void unpack<>(Int32* lhs, const UInt8* data, UInt32 length)
    {
#pragma unused (length)
        *lhs = ((Int32) data[0] << 24) |
            ((Int32) data[1] << 16) |
            ((Int32) data[2] << 8) |
            data[3];
    }

    template<> inline void unpack<>(Int64* lhs, const UInt8* data, UInt32 length)
    {
#pragma unused (length)
        *lhs = ((Int64) data[0] << 56) |
            ((Int64) data[1] << 48) |
            ((Int64) data[2] << 40) |
            ((Int64) data[3] << 32) |
            ((Int64) data[4] << 24) |
            ((Int64) data[5] << 16) |
            ((Int64) data[6] << 8) |
            data[7];
    }

    template<> inline void unpack<>(std::wstring* lhs, const UInt8* data, UInt32 length)
    {
        if (sizeof(wchar_t) == 2)
        {
            const UInt16* ptr = reinterpret_cast<const UInt16*>(data);
            for (UInt32 i = 0; i < length; i += sizeof(wchar_t))
            {
                if (*ptr == 0)
                {
                    break;
                }
                *lhs += (wchar_t(*ptr >> 8) | wchar_t(*ptr << 8));
                ptr++;
            }
        }
        else if (sizeof(wchar_t) == 4)
        {
            const UInt32* ptr = reinterpret_cast<const UInt32*>(data);
            for (UInt32 i = 0; i < length; i += sizeof(wchar_t))
            {
                if (*ptr == 0)
                {
                    break;
                }
                wchar_t wc;
                unpack((UInt32*) &wc, (UInt8*) ptr, 4);
                *lhs += wc;
                ptr++;
            }
        }
    }

    template<> inline void unpack<>(std::string* lhs, const UInt8* data, UInt32 length)
    {
        const UInt8* ptr = data;
        for (UInt32 i = 0; i < length; i += sizeof(char))
        {
            if (*ptr == 0)
            {
                break;
            }
            *lhs += *ptr;
            ptr++;
        }
    }

    template<typename T> inline void pack(UInt8* data, const T& value)
    {
        value.getValue(data);
    }

    template<> inline void pack<>(UInt8* data, const UInt8& value)
    {
        *data = value;
    }

    template<> inline void pack<>(UInt8* data, const UInt16& value)
    {
        data[0] = (value >> 8) & 0xff;
        data[1] = value & 0xff;
    }

    template<> inline void pack<>(UInt8* data, const UInt32& value)
    {
        data[0] = (UInt8)((value >> 24) & 0xff);
        data[1] = (UInt8)((value >> 16) & 0xff);
        data[2] = (UInt8)((value >> 8) & 0xff);
        data[3] = (UInt8)(value & 0xff);
    }

    template<> inline void pack<>(UInt8* data, const UInt64& value)
    {
        data[0] = (UInt8)((value >> 56) & 0xff);
        data[1] = (UInt8)((value >> 48) & 0xff);
        data[2] = (UInt8)((value >> 40) & 0xff);
        data[3] = (UInt8)((value >> 32) & 0xff);
        data[4] = (UInt8)((value >> 24) & 0xff);
        data[5] = (UInt8)((value >> 16) & 0xff);
        data[6] = (UInt8)((value >> 8) & 0xff);
        data[7] = (UInt8)(value & 0xff);
    }

    template<> inline void pack<>(UInt8* data, const Int8& value)
    {
        *data = value;
    }

    template<> inline void pack<>(UInt8* data, const Int16& value)
    {
        data[0] = (value >> 8) & 0xff;
        data[1] = value & 0xff;
    }

    template<> inline void pack<>(UInt8* data, const Int32& value)
    {
        data[0] = (UInt8)((value >> 24) & 0xff);
        data[1] = (UInt8)((value >> 16) & 0xff);
        data[2] = (UInt8)((value >> 8) & 0xff);
        data[3] = (UInt8)(value & 0xff);
    }

    template<> inline void pack<>(UInt8* data, const Int64& value)
    {
        data[0] = (UInt8)((value >> 56) & 0xff);
        data[1] = (UInt8)((value >> 48) & 0xff);
        data[2] = (UInt8)((value >> 40) & 0xff);
        data[3] = (UInt8)((value >> 32) & 0xff);
        data[4] = (UInt8)((value >> 24) & 0xff);
        data[5] = (UInt8)((value >> 16) & 0xff);
        data[6] = (UInt8)((value >> 8) & 0xff);
        data[7] = (UInt8)(value & 0xff);
    }

    template<> inline void pack<>(UInt8* data, const bool& value)
    {
        *data = value;
    }

    template<> inline void pack<>(UInt8* data, const std::wstring& value)
    {
        if (sizeof(wchar_t) == 2)
        {
            UInt16* ptr = (UInt16*) data;
            for (UInt32 i = 0; i < value.length(); i++)
            {
                *ptr = (value[i] >> 8) | (value[i] << 8);
                ptr++;
            }
        }
        else if (sizeof(wchar_t) == 4)
        {
            UInt32* ptr = (UInt32*) data;
            for (UInt32 i = 0; i < value.length(); i++)
            {
                pack((UInt8*) ptr, (UInt32) value[i]);
                ptr++;
            }
        }
    }

    template<> inline void pack<>(UInt8* data, const std::string& value)
    {
        UInt8* ptr = data;
        for (UInt32 i = 0; i < value.length(); i++)
        {
            *ptr = value[i];
            ptr++;
        }
    }

    template<typename T> inline UInt32 getLength(const T& value)
    {
        return value.getLength();
    }

    template<> inline UInt32 getLength<>(const UInt8& value)
    {
#pragma unused (value)
        return sizeof(UInt8);
    }

    template<> inline UInt32 getLength<>(const UInt16& value)
    {
#pragma unused (value)
        return sizeof(UInt16);
    }

    template<> inline UInt32 getLength<>(const UInt32& value)
    {
#pragma unused (value)
        return sizeof(UInt32);
    }

    template<> inline UInt32 getLength<>(const UInt64& value)
    {
#pragma unused (value)
        return sizeof(UInt64);
    }

    template<> inline UInt32 getLength<>(const Int8& value)
    {
#pragma unused (value)
        return sizeof(Int8);
    }

    template<> inline UInt32 getLength<>(const Int16& value)
    {
#pragma unused (value)
        return sizeof(Int16);
    }

    template<> inline UInt32 getLength<>(const Int32& value)
    {
#pragma unused (value)
        return sizeof(Int32);
    }

    template<> inline UInt32 getLength<>(const Int64& value)
    {
#pragma unused (value)
        return sizeof(Int64);
    }

    template<> inline UInt32 getLength<>(const bool& value)
    {
#pragma unused (value)
        return sizeof(bool);
    }

    template<> inline UInt32 getLength<>(const std::wstring& value)
    {
        return (UInt32)(2 * value.size());
    }

    template<> inline UInt32 getLength<>(const std::string& value)
    {
        return (UInt32) value.size();
    }

    template<typename T> inline bool isEmpty(const T& value)
    {
        return value.isEmpty();
    }

    template<> inline bool isEmpty<>(const UInt8& value)
    {
        return value == 0;
    }

    template<> inline bool isEmpty<>(const UInt16& value)
    {
        return value == 0;
    }

    template<> inline bool isEmpty<>(const UInt32& value)
    {
        return value == 0;
    }

    template<> inline bool isEmpty<>(const UInt64& value)
    {
        return value == 0;
    }

    template<> inline bool isEmpty<>(const Int8& value)
    {
        return value == 0;
    }

    template<> inline bool isEmpty<>(const Int16& value)
    {
        return value == 0;
    }

    template<> inline bool isEmpty<>(const Int32& value)
    {
        return value == 0;
    }

    template<> inline bool isEmpty<>(const Int64& value)
    {
        return value == 0;
    }

    template<> inline bool isEmpty<>(const bool& value)
    {
        return value == false;
    }

    template<> inline bool isEmpty<>(const std::wstring& value)
    {
        return value.size() == 0;
    }

    template<> inline bool isEmpty<>(const std::string& value)
    {
        return value.size() == 0;
    }

    template<typename T> inline void toString(std::wstring& str, const T& value)
    {
        value.toString(str);
    }

    template<> void toString<>(std::wstring& str, const UInt8& value);
    template<> void toString<>(std::wstring& str, const UInt16& value);
    template<> void toString<>(std::wstring& str, const UInt32& value);
    template<> void toString<>(std::wstring& str, const UInt64& value);
    template<> void toString<>(std::wstring& str, const Int8& value);
    template<> void toString<>(std::wstring& str, const Int16& value);
    template<> void toString<>(std::wstring& str, const Int32& value);
    template<> void toString<>(std::wstring& str, const Int64& value);
    template<> void toString<>(std::wstring& str, const bool& value);
    template<> void toString<>(std::wstring& str, const std::wstring& value);

    //template<typename T> inline void fromString(T& value, const std::wstring& str)
    //{
    //  value.fromString(str);
    //}

    //template<> void fromString<>(UInt8& value, const std::wstring& str);
    //template<> void fromString<>(UInt16& value, const std::wstring& str);
    //template<> void fromString<>(UInt32& value, const std::wstring& str);
    //template<> void fromString<>(UInt64& value, const std::wstring& str);
    //template<> void fromString<>(Int8& value, const std::wstring& str);
    //template<> void fromString<>(Int16& value, const std::wstring& str);
    //template<> void fromString<>(Int32& value, const std::wstring& str);
    //template<> void fromString<>(Int64& value, const std::wstring& str);
    //template<> void fromString<>(bool& value, const std::wstring& str);
    //template<> void fromString<>(std::wstring& value, const std::wstring& str);

} //namespace CMI
#endif //CMI_CONVERSION_H__
