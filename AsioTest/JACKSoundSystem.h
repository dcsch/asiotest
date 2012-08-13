#ifndef JACKSOUNDSYSTEM_H
#define JACKSOUNDSYSTEM_H

#include "SoundSystem.h"
#include <jack/jack.h>

namespace CMI
{

class JACKSoundSystem : public SoundSystem
{
public:
    JACKSoundSystem(const std::string &clientName);

    virtual ~JACKSoundSystem();

    virtual void Initialise();

    virtual void Finalise();

    virtual void play(UInt8 keyNumber);

    virtual UInt32 getSampleRate() const;

private:
    std::string _clientName;
    jack_client_t *_client;

};

} //namespace CMI

#endif // JACKSOUNDSYSTEM_H
