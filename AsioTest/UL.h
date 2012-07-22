#ifndef CMI_UL_H__
#define CMI_UL_H__

#include "UID.h"

namespace CMI
{

/// \author David Schweinsberg
class UL : public UID<16>
{
public:

    /// Default constructor.  Initializes a null UL.
    UL();

    /// Constructor.  Allows the setting of all 16 bytes of the label.
    UL(UInt8 b1, UInt8 b2, UInt8 b3, UInt8 b4, UInt8 b5, UInt8 b6, UInt8 b7, UInt8 b8,
        UInt8 b9, UInt8 b10, UInt8 b11, UInt8 b12, UInt8 b13, UInt8 b14, UInt8 b15, UInt8 b16);

    /// Constructor.  Initializes the label from an array of bytes.
    /// \param value An array of bytes with the initializing data.
    /// \param length The length of the array.  It must be 16 bytes.
    UL(const UInt8* value, UInt32 length);

    /// Constructor.  Initializes the label from a string.
    /// \param str A string with the initializing data.
    /// \param strLen The length of the string.
    UL(const wchar_t* str, UInt32 strLen);

    /// Constructor.  Initializes the label from an array.
    UL(const UInt8 value[16]);

    /// Copy constructor
    UL(const UL& rhs);

    /// Destructor.
    virtual ~UL();

    /// Returns true if the UL is valid, i.e. the first four bytes are:
    /// {06 0e 2b 34}
    bool isValid() const;

    /// Performs a comparison of the label against the value, using an array of
    /// bytes as a mask.  If a bit in the mask is set to 1, the corresponding
    /// bit in the labels will be compared against each other.
    bool isTypeOf(const UL& ul, const UInt8* mask, UInt32 maskLen) const;

    /// Performs a comparison of the label against the value, ignoring any
    /// zeros that occur in the value.  This allows comparison of specific
    /// labels against generalized labels where the "parameter" label values
    /// have been zeroed.
    bool isTypeOf(const UL& ul) const;

    void setValueFromURN(const wchar_t* str, UInt32 strLen);

private:
};

} //namespace CMI

#endif //CMI_UL_H__
