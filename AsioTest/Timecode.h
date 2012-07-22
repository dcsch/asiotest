#ifndef CMI_TIMECODE_H__
#define CMI_TIMECODE_H__ 1

namespace CMI
{

class GroupOfPicturesHeader;
class Rational;

/// A simple Timecode class.
/// \author David Schweinsberg
class Timecode
{
public:

    /// Default constructor.
    Timecode();

    /// Specific time constructor.
    Timecode(
        UInt16 hours,
        UInt16 minutes,
        UInt16 seconds,
        UInt16 frames,
        UInt16 roundedBase,
        bool dropFrame);

    Timecode(Position pos, UInt16 roundedBase, bool dropFrame);

    /// MPEG2 Video GOP constructor.
    /// \param frameRateCode The MPEG frame rate code
    /// \param gopHeader
    Timecode(UInt8 frameRateCode, const GroupOfPicturesHeader* gopHeader);

    /// Returns <em>True</em> if drop frame timecode is in use.
    /// Returns <em>False</em> if it is not.
    bool getDropFrame() const;

    /// Set <em>True</em> if drop frame timecode is in use.
    /// Set <em>False</em> if it is not.
    void setDropFrame(bool dropFrame);

    /// Returns the timecode's time.
    void getTime(UInt16& hours, UInt16& minutes, UInt16& seconds, UInt16& frames) const;

    void setTime(UInt16 hours, UInt16 minutes, UInt16 seconds, UInt16 frames);

    UInt16 getFrameRate() const;

    Position getTotalFrames() const;

    Timecode& operator++();

    Timecode operator++(int);

    Timecode& operator+=(Position pos);

    Timecode& operator-=(Position pos);

    void reset();

    bool toS331M(UInt8* buf, UInt32 bufLen) const;

private:
    Position _pos;
    UInt16 _fps;
    bool _dropFrame;
};

inline bool Timecode::getDropFrame() const
{
    return _dropFrame;
}

inline void Timecode::setDropFrame(bool dropFrame)
{
    _dropFrame = dropFrame;
}

inline void Timecode::getTime(UInt16& hours, UInt16& minutes, UInt16& seconds, UInt16& frames) const
{
    if (_fps == 0)
    {
        hours = 0;
        minutes = 0;
        seconds = 0;
        frames = frames;
        return;
    }

    UInt32 framesPerHour = 3600 * _fps;
    UInt32 framesPerMinute = 60 * _fps;
    Position pos;
    if (_dropFrame)
    {
        // TODO: This is known to be wrong
        pos = _pos + 2 * (_pos / framesPerMinute - _pos / (10 * framesPerMinute));
    }
    else
    {
        pos = _pos;
    }
    hours = (UInt16)(pos / framesPerHour);
    minutes = (UInt16)(pos % framesPerHour / framesPerMinute);
    seconds = (UInt16)(pos % framesPerMinute / _fps);
    frames = (UInt16)(pos % _fps);
}

inline void Timecode::setTime(UInt16 hours, UInt16 minutes, UInt16 seconds, UInt16 frames)
{
    UInt32 framesPerHour = 3600 * _fps;
    UInt32 framesPerMinute = 60 * _fps;
    _pos = hours * framesPerHour +
        minutes * framesPerMinute +
        seconds * _fps +
        frames;
    if (_dropFrame)
    {
        _pos -= 2 * (_pos / framesPerMinute - _pos / (10 * framesPerMinute));
    }
}

inline UInt16 Timecode::getFrameRate() const
{
    return _fps;
}

inline Position Timecode::getTotalFrames() const
{
    return _pos;
}

inline void Timecode::reset()
{
    _pos = 0;
}

inline Timecode& Timecode::operator++()
{
    ++_pos;
    return *this;
}

inline Timecode Timecode::operator++(int)
{
    Timecode before(*this);
    ++(*this);
    return before;
}

inline Timecode& Timecode::operator+=(Position pos)
{
    _pos += pos;
    return *this;
}

inline Timecode& Timecode::operator-=(Position pos)
{
    _pos -= pos;
    return *this;
}

} //namespace CMI
#endif //CMI_TIMECODE_H__
