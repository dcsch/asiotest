#ifndef CMI_RATIONAL_H__
#define CMI_RATIONAL_H__ 1

#include "common.h"

namespace CMI
{

/// \author David Schweinsberg
class Rational
{
public:
	Rational();

	Rational(UInt32 numerator);

	Rational(UInt32 numerator, UInt32 denominator);

	Rational(const UInt8* value, UInt32 length);

	UInt32 getNumerator() const { return _numerator; }

	void setNumerator(UInt32 value) { _numerator = value; }

	UInt32 getDenominator() const { return _denominator; }

	void setDenominator(UInt32 value) { _denominator = value; }

	UInt32 getRoundedValue() const { return (UInt32)((double) *this + 0.5); }

	void getValue(UInt8* value) const;

	void getValue(UInt8* data, UInt32 maxLength) const;

	UInt32 getLength() const { return 8; }

	/// Sets the value of the Rational from an array of bytes.
	/// \param value An array of bytes which the numerator is stored in the
	///              first 4 bytes and the denominator in the second.
	///              value[0] is the MSB of the numerator.
	///              value[4] is the MSB of the denominator.
	/// \param length Must be 8, otherwise the Rational will be set to 0.
	void setValue(const UInt8* value, UInt32 length);

	/// Fills the Rational from a string of format "numerator / denominator".
	/// Can also deal with strings of format "numerator".
	/// \param str The string to extract the value from.
	void setValue(const std::wstring& str);

	bool isEmpty() const { return _numerator == 0; }

	void simplify();

	void toString(std::wstring& str) const;

	/// This is a very simple implementation
	bool operator==(const Rational& rhs) const;

	operator double() const
	{
		return (double) _numerator / (double) _denominator;
	}

private:
	UInt32 _numerator;
	UInt32 _denominator;
};

} //namespace CMI
#endif //CMI_RATIONAL_H__
