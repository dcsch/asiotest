#include "common.h"
#include "JACKSoundSystem.h"

namespace CMI
{

JACKSoundSystem::JACKSoundSystem() : SoundSystem()
{
}

JACKSoundSystem::~JACKSoundSystem()
{
}

void JACKSoundSystem::Initialise()
{
    SoundSystem::Initialise();

}

void JACKSoundSystem::Finalise()
{
    SoundSystem::Finalise();
}

void JACKSoundSystem::play(UInt8 keyNumber)
{
    SoundSystem::play(keyNumber);
}

UInt32 JACKSoundSystem::getSampleRate() const
{
    return 48000;
}

} //namespace CMI
