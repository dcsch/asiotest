#ifndef CMI_EDIT_UNIT_SIZE_H__
#define CMI_EDIT_UNIT_SIZE_H__ 1

namespace CMI
{

/// An interface to retrieve EditUnit size information.
/// \author David Schweinsberg
class EditUnitSize
{
public:
    
    /// Returns a value greater than zero if edit units are of constant size.
    virtual UInt32 getConstantEditUnitSize(UInt32 trackIndex) const = 0;
    
    /// Returns a value greater than zero if it is known what the maximum edit
    /// unit size is.
    virtual UInt32 getMaxEditUnitSize(UInt32 trackIndex) const = 0;
    
    virtual ~EditUnitSize() { }
};

} //namespace CMI
#endif //CMI_EDIT_UNIT_SIZE_H__
