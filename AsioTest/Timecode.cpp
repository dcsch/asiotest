#include "common.h"
#include "Timecode.h"

namespace CMI
{

Timecode::Timecode() :
    _pos(0),
    _fps(0),
    _dropFrame(false)
{
}

Timecode::Timecode(UInt16 hours,
                   UInt16 minutes,
                   UInt16 seconds,
                   UInt16 frames,
                   UInt16 roundedBase,
                   bool dropFrame) :
    _pos(0),
    _fps(roundedBase),
    _dropFrame(dropFrame)
{
    // Drop frame is only valid for 29.97Hz material
    if (roundedBase != 30)
    {
        dropFrame = false;
    }
    setTime(hours, minutes, seconds, frames);
}

Timecode::Timecode(Position pos, UInt16 roundedBase, bool dropFrame) :
    _pos(pos),
    _fps(roundedBase),
    _dropFrame(dropFrame)
{
    // Drop frame is only valid for 29.97Hz material
    if (roundedBase != 30)
    {
        _dropFrame = false;
    }
}

bool Timecode::toS331M(UInt8* buf, UInt32 bufLen) const
{
    if (bufLen < 17)
    {
        return false;
    }

    UInt16 hours;
    UInt16 minutes;
    UInt16 seconds;
    UInt16 frames;

    getTime(hours, minutes, seconds, frames);

    buf[0] = 0x81;
    buf[1] =
        (frames % 10) |
        ((frames / 10) << 4) |
        (_dropFrame ? 0x40 : 0) |
        0x80;
    buf[2] = (seconds % 10) | ((seconds / 10) << 4);
    buf[3] = (minutes % 10) | ((minutes / 10) << 4);
    buf[4] = (hours % 10) | ((hours / 10) << 4);
    buf[5] = 0;
    buf[6] = 0;
    buf[7] = 0;
    buf[8] = 0;
    buf[9] = 0;
    buf[10] = 0;
    buf[11] = 0;
    buf[12] = 0;
    buf[13] = 0;
    buf[14] = 0;
    buf[15] = 0;
    buf[16] = 0;

    return true;
}

} //namespace CMI
