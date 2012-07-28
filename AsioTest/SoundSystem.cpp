#include "StdAfx.h"
#include "common.h"
#include "SoundSystem.h"
#include "Session.h"
#include "Instrument.h"
#include "Sample.h"

namespace CMI
{

SoundSystem::SoundSystem() :
	_session(0)
{
}

SoundSystem::~SoundSystem()
{
}

void SoundSystem::Initialise()
{
}

void SoundSystem::Finalise()
{
}

const Session *SoundSystem::getSession() const
{
	return _session;
}

void SoundSystem::setSession(Session *session)
{
	_session = session;
}

void SoundSystem::play()
{
	const Instrument *instrument = _session->getInstrument(0);
	const Sample *sample = instrument->getSample(0);
	_samples[0] = sample;
}

} //namespace CMI
