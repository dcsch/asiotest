#include "common.h"
#include "Instrument.h"

namespace CMI
{

Instrument::Instrument()
{
	for (UInt32 i = 0; i < MaxSampleCount; ++i)
		_samples[i] = 0;
}

Instrument::~Instrument()
{
}

const Sample *Instrument::getSample(UInt32 index) const
{
	if (index >= MaxSampleCount)
		return 0;
	return _samples[index];
}

void Instrument::setSample(UInt32 index, Sample *sample)
{
	if (index >= MaxSampleCount)
		return;

	_samples[index] = sample;
}

} //namespace CMI
