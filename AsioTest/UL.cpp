#include "common.h"
#include "UL.h"

namespace CMI
{

UL::UL()
{
}

UL::UL(UInt8 b1, UInt8 b2, UInt8 b3, UInt8 b4, UInt8 b5, UInt8 b6, UInt8 b7, UInt8 b8,
       UInt8 b9, UInt8 b10, UInt8 b11, UInt8 b12, UInt8 b13, UInt8 b14, UInt8 b15, UInt8 b16) :
    UID<16>()
{
    setByte(1, b1);
    setByte(2, b2);
    setByte(3, b3);
    setByte(4, b4);
    setByte(5, b5);
    setByte(6, b6);
    setByte(7, b7);
    setByte(8, b8);
    setByte(9, b9);
    setByte(10, b10);
    setByte(11, b11);
    setByte(12, b12);
    setByte(13, b13);
    setByte(14, b14);
    setByte(15, b15);
    setByte(16, b16);
}

UL::UL(const UInt8* value, UInt32 length) : UID<16>(value, length)
{
}

UL::UL(const wchar_t* str, UInt32 strLen) : UID<16>()
{
    setValue(str, strLen);
}

UL::UL(const UInt8 value[16]) : UID<16>()
{
    memcpy(getBuffer(), value, 16);
}

UL::UL(const UL& rhs) : UID<16>(rhs)
{
}

UL::~UL()
{
}

bool UL::isValid() const
{
    return
        (getByte(1) == 0x06) &&
        (getByte(2) == 0x0e) &&
        (getByte(3) == 0x2b) &&
        (getByte(4) == 0x34);
}

bool UL::isTypeOf(const UL& ul, const UInt8* mask, UInt32 maskLen) const
{
    for (UInt32 i = 1; i <= 16; i++)
    {
        // Skip the version byte if we have no explicit mask
        if (maskLen == 0 && i == 8)
        {
            continue;
        }

        // If the right-hand side is a zero, then we'll move on to the
        // next byte.
        if (ul.getByte(i) == 0)
        {
            continue;
        }

        // Now compare the two bytes, masking both
        UInt8 maskByte = (maskLen >= i) ? mask[i - 1] : 0xff;

        if ((getByte(i) & maskByte) != (ul.getByte(i) & maskByte))
        {
            return false;
        }
    }
    return true;
}

bool UL::isTypeOf(const UL& ul) const
{
    return isTypeOf(ul, 0, 0);
}

void UL::setValueFromURN(const wchar_t* str, UInt32 strLen)
{
    // Check if the length is correct
    if (strLen != 45)
    {
        return;
    }

    // Check if the prefix is correct
    if (wcsncmp(str, L"urn:x-ul:", 9) != 0)
    {
        return;
    }

    // OK, lets crunch those hex digits
    str += 9;
    UInt32 pos = 0;
    UInt32 byte = 1;
    UInt8 sixteens;
    UInt8 units;
    while ((pos < strLen - 1) && (byte <= 16))
    {
        sixteens = 16 * hexCharToVal(str[pos]);
        units = hexCharToVal(str[pos + 1]);
        setByte(byte, sixteens + units);
        if (pos == 6 || pos == 11 || pos == 16 || pos == 25)
        {
            pos += 3;
        }
        else
        {
            pos += 2;
        }
        ++byte;
    }
}

} //namespace CMI
