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

UInt32 SoundSystem::process(Position offset, float *frameBuffer, UInt32 frameCount)
{
	Length samplesAvailable = _samples[0]->getBufferLength() - offset;
	if (samplesAvailable > 0)
	{
		const float *src = _samples[0]->getBuffer() + offset;
		float *dst = frameBuffer;
		for (UINT32 j = 0; j < frameCount; ++j)
		{
			if (j <= samplesAvailable)
			{
				float sample = *src;
				src++;

				*dst = sample;
				dst++;

				*dst = sample;
				dst++;
			}
			else
			{
				*dst = 0.f;
				dst++;

				*dst = 0.f;
				dst++;
			}
		}
		return frameCount;
	}

	return 0;
}

} //namespace CMI
