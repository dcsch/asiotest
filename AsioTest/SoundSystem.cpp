#include "StdAfx.h"
#include "common.h"
#include "SoundSystem.h"
#include "Session.h"
#include "Instrument.h"
#include "Sample.h"
#include "SampleStream.h"

namespace CMI
{

SoundSystem::SoundSystem() :
	_session(0)
{
	for (UInt32 i = 0; i < MaxPolyphonyCount; ++i)
		_sampleStreams[i] = new SampleStream(this);
}

SoundSystem::~SoundSystem()
{
	for (UInt32 i = 0; i < MaxPolyphonyCount; ++i)
		delete _sampleStreams[i];
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

void SoundSystem::play(UInt8 keyNumber)
{
	const Instrument *instrument = _session->getInstrument(0);
	const Sample *sample = instrument->getSample(0);
	_sampleStreams[0]->start(sample, keyNumber);
}

UInt32 SoundSystem::process(float *frameBuffer, UInt32 frameCount)
{
//	for (UInt32 i = 0; i < MaxPolyphonyCount; ++i)
	for (UInt32 i = 0; i < 1; ++i)
	{
		if (_sampleStreams[i]->getSamplesAvailable() > 0)
		{
			_sampleStreams[i]->read(frameBuffer, frameCount);
		}
	}

	return 0;
}

} //namespace CMI
