#ifndef CMI_EXCEPTION_H__
#define CMI_EXCEPTION_H__ 1

#include <string>

namespace CMI
{

/// \author David Schweinsberg
class Exception
{
public:
    Exception(UInt32 code) : _code(code), _extra()
    {
    }

    Exception(UInt32 code, const std::wstring& extra) :
        _code(code),
        _extra(extra)
    {
    }

    UInt32 getCode() const { return _code; }

    const std::wstring& getExtra() const { return _extra; }

private:
    UInt32 _code;
    std::wstring _extra;
};

} //namespace CMI
#endif //CMI_EXCEPTION_H__
