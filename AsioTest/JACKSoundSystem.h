#ifndef JACKSOUNDSYSTEM_H
#define JACKSOUNDSYSTEM_H

#include "SoundSystem.h"

namespace CMI
{

class JACKSoundSystem : public SoundSystem
{
public:
    JACKSoundSystem();

    virtual ~JACKSoundSystem();

    virtual void Initialise();

    virtual void Finalise();

    virtual void play(UInt8 keyNumber);

    virtual UInt32 getSampleRate() const;
};

} //namespace CMI

#endif // JACKSOUNDSYSTEM_H
