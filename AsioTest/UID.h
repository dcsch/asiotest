#ifndef CMI_UID_H__
#define CMI_UID_H__

#include <stdio.h>
#include <memory.h>

namespace CMI
{

/// A Unique ID.
/// David Schweinsberg
template <UInt32 length> class UID
{
public:
    
    /// Default constructor.  Initializes a null UID.
    UID();
    
    /// Constructor.  Initializes from the specified buffer.
    UID(const UInt8* value, UInt32 valLen);
    
    /// Copy constructor.
    UID(const UID<length>& rhs);
    
    /// Destructor.
    virtual ~UID();

    /// Returns the byte length of the UID.
    UInt32 getLength() const;

    /// Returns an individual byte from the label.  The convention when
    /// identifying individual bytes within a label is to index the first byte
    /// as byte 1.  Therefore the range of the index parameter is from 1 to 16.
    /// \param index The index of the byte to return.  The range is from 1 to 16.
    UInt8 getByte(UInt32 index) const;

    /// Sets an individual byte from the label.  The convention when
    /// identifying individual bytes within a label is to index the first byte
    /// as byte 1.  Therefore the range of the index parameter is from 1 to 16.
    /// \param index The index of the byte to set.  The range is from 1 to 16.
    /// \param value The value to set the byte to.
    void setByte(UInt32 index, UInt8 value);

    /// This method is deprecated.  The Doxygen "deprecated" flag seems to be
    /// crashing Doxygen.
    // \deprecated
    void getValue(UInt8* value) const;

    /// Copies the UID value into a buffer.
    void getValue(UInt8* data, UInt32 maxLength) const;

    /// Copies the UID value from a buffer.
    void setValue(const UInt8* value, UInt32 valueLen);

    /// Sets the UID value from a string.  This must be of format either
    /// "xx xx xx ...", or "xx.xx.xx. ...", where the 'x's represent
    /// hexadecimal digits.
    void setValue(const wchar_t* str, UInt32 strLen);

    void setValue(const std::wstring& str);

    /// Returns true if the UID is all zeros
    bool isEmpty() const;

    UID& operator=(const UID& rhs);

    bool operator==(const UID& uid) const;

    bool operator!=(const UID& uid) const;

    bool operator<(const UID& uid) const;

    bool compare(const UID& uid, UInt32 count) const;

    bool compare(const UInt8* value, UInt32 start, UInt32 count) const;

    virtual const UInt8* getBuffer() const;

    virtual UInt8* getBuffer();

    virtual UInt32 getBufferLength() const;

    /// Generates a string representation of the object value
    virtual void toString(std::wstring& str) const;

protected:
    static UInt8 hexCharToVal(wchar_t c);

private:
    UInt8 _value[length];
};

template <UInt32 length>
UID<length>::UID()
{
    memset(_value, 0, length);
}

template <UInt32 length>
UID<length>::UID(const UInt8* value, UInt32 valLen)
{
    if ((value != 0) && (valLen <= length))
    {
        memcpy(_value, value, length);
    }
    else
    {
        memset(_value, 0, length);
    }
}

template <UInt32 length>
UID<length>::UID(const UID<length>& rhs)
{
    memcpy(_value, rhs._value, length);
}

template <UInt32 length>
UID<length>::~UID()
{
}

template <UInt32 length>
UInt32 UID<length>::getLength() const
{
    return length;
}

template <UInt32 length>
UInt8 UID<length>::getByte(UInt32 index) const
{
    if (0 < index && index <= length)
    {
        return _value[index - 1];
    }
    else
    {
        return 0;
    }
}

template <UInt32 length>
void UID<length>::setByte(UInt32 index, UInt8 value)
{
    if (0 < index && index <= length)
    {
        _value[index - 1] = value;
    }
}

template <UInt32 length>
void UID<length>::getValue(UInt8* value) const
{
    memcpy(value, _value, length);
}

template <UInt32 length>
void UID<length>::getValue(UInt8* data, UInt32 maxLength) const
{
    if (length <= maxLength)
    {
        memcpy(data, _value, length);
    }
}

template <UInt32 length>
void UID<length>::setValue(const UInt8* value, UInt32 valueLen)
{
    UInt32 tlen = length; // We do it this way to stop VC6 complaining
    UInt32 len = std::min(valueLen, tlen);
    memcpy(_value, value, len);
}

template <UInt32 length>
void UID<length>::setValue(const wchar_t* str, UInt32 strLen)
{
    UInt32 pos = 0;
    UInt32 byte = 1;
    UInt8 sixteens;
    UInt8 units;
    while ((pos < strLen - 1) && (byte <= length))
    {
        sixteens = 16 * hexCharToVal(str[pos]);
        units = hexCharToVal(str[pos + 1]);
        setByte(byte, sixteens + units);
        pos += 3;
        ++byte;
    }
}

template <UInt32 length>
void UID<length>::setValue(const std::wstring& str)
{
    setValue(str.c_str(), (UInt32) str.length());
}

template <UInt32 length>
bool UID<length>::isEmpty() const
{
    for (UInt32 i = 0; i < length; ++i)
    {
        if (_value[i] != 0)
        {
            return false;
        }
    }
    return true;
}

template <UInt32 length>
UID<length>& UID<length>::operator=(const UID<length>& rhs)
{
    memcpy(_value, rhs._value, length);
    return *this;
}

template <UInt32 length>
bool UID<length>::operator==(const UID& uid) const
{
    return compare(uid._value, 0, length);
}

template <UInt32 length>
bool UID<length>::operator!=(const UID& uid) const
{
    return !operator==(uid);
}

template <UInt32 length>
bool UID<length>::operator<(const UID& uid) const
{
    for (UInt32 i = 0; i < length; ++i)
    {
        if (_value[i] < uid._value[i])
        {
            return true;
        }
        else if (_value[i] > uid._value[i])
        {
            return false;
        }
    }

    return false;
}

template <UInt32 length>
bool UID<length>::compare(const UID& uid, UInt32 count) const
{
    return compare(uid._value, 0, count);
}

template <UInt32 length>
bool UID<length>::compare(const UInt8* value, UInt32 start, UInt32 count) const
{
    for (UInt32 i = start; i < start + count; i++)
    {
        if (_value[i] != value[i])
        {
            return false;
        }
    }
    return true;
}

template <UInt32 length>
const UInt8* UID<length>::getBuffer() const { return _value; }

template <UInt32 length>
UInt8* UID<length>::getBuffer() { return _value; }

template <UInt32 length>
UInt32 UID<length>::getBufferLength() const { return length; }

/// Generates a string representation of the object value
template <UInt32 length>
void UID<length>::toString(std::wstring& str) const
{
    wchar_t buf[4];
    str = L"";
    for (UInt32 i = 0; i < length; ++i)
    {
        swprintf(buf, 4, L"%02X", _value[i]);
        str += buf;
        if (i < length - 1)
        {
            str += L" ";
        }
    }
}

template <UInt32 length>
UInt8 UID<length>::hexCharToVal(wchar_t c)
{
    if (L'0' <= c && c <= L'9')
    {
        return c - L'0';
    }
    else if (L'a' <= c && c <= L'f')
    {
        return c - L'a' + 10;
    }
    else if (L'A' <= c && c <= L'F')
    {
        return c - L'A' + 10;
    }
    return 0;
}

} //namespace CMI

#endif //CMI_UID_H__
