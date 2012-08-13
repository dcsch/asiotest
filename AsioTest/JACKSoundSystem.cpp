#include "common.h"
#include "JACKSoundSystem.h"

namespace CMI
{

JACKSoundSystem::JACKSoundSystem(const std::string &clientName) : SoundSystem(), _clientName(clientName), _client(0)
{
}

JACKSoundSystem::~JACKSoundSystem()
{
}

void JACKSoundSystem::Initialise()
{
    SoundSystem::Initialise();

    _client = jack_client_open(_clientName.c_str(), JackNoStartServer, NULL);
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
