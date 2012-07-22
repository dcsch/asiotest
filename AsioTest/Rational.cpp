// Class which can hold Rational numbers (numerator and denominator).

#include "Rational.h"
#include "conversion.h"

#include <stdio.h>
#include <stdlib.h>

namespace CMI
{

Rational::Rational() :
    _numerator(0),
    _denominator(1)
{
}

Rational::Rational(UInt32 numerator) :
    _numerator(numerator),
    _denominator(1)
{
}

Rational::Rational(UInt32 numerator, UInt32 denominator) :
    _numerator(numerator),
    _denominator(denominator)
{
}

Rational::Rational(const UInt8* value, UInt32 length) :
    _numerator(0),
    _denominator(1)
{
    setValue(value, length);
}

void Rational::simplify()
{
    // Naive highest common multiple
    UInt32 lower = std::min(_numerator, _denominator);
    UInt32 higher = std::max(_numerator, _denominator);
    UInt32 i;
    for (i = lower; i > 1; --i)
    {
        if ((lower % i == 0) && (higher % i == 0))
        {
            break;
        }
    }

    if (i > 1)
    {
        _numerator /= i;
        _denominator /= i;
    }
}

void Rational::toString(std::wstring& str) const
{
    wchar_t buf[80];
    swprintf(buf, 80, L"%d / %d", getNumerator(), getDenominator());
    str = buf;
}

void Rational::getValue(UInt8* buffer) const
{
    pack(buffer, getNumerator());
    pack(buffer + 4, getDenominator());
}

void Rational::getValue(UInt8* data, UInt32 maxLength) const
{
#pragma unused(maxLength)
    pack(data, getNumerator());
    pack(data + 4, getDenominator());
}

void Rational::setValue(const UInt8* value, UInt32 length)
{
    if (length == 8)
    {
        // put first 4 values into numerator where value[0] is MSB
        _numerator = (value[0] << 24) | (value[1] << 16) | (value[2] << 8) | value[3];
        // put last 4 values into denominator where value[4] is MSB
        _denominator = (value[4] << 24) | (value[5] << 16) | (value[6] << 8) | value[7];
    }
    else
    {
        // we've been given an invalid value, so fill the rational with 0
        // (remember 0 in the denominator is illegal!)
        _numerator = 0;
        _denominator = 1;
    }
}

void Rational::setValue(const std::wstring& str)
{
    std::wstring::size_type pos = str.find(L'/');
    if (pos != std::wstring::npos)
    {
        setNumerator(wcstol(str.substr(0, pos).c_str(), 0, 10));
        setDenominator(wcstol(str.substr(pos + 1).c_str(), 0, 10));
    }
    else
    {
        setNumerator(wcstol(str.c_str(), 0, 10));
        setDenominator(1);
    }
}

bool Rational::operator==(const Rational& rhs) const
{
    bool equal = getNumerator() == rhs.getNumerator() &&
        getDenominator() == rhs.getDenominator();
    if (!equal)
    {
        double d1 = (double) getNumerator() / (double) getDenominator();
        double d2 = (double) rhs.getNumerator() / (double) rhs.getDenominator();
        return d1 == d2;
    }
    else
    {
        return true;
    }
}


} // namespace CMI
