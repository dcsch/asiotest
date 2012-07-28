#pragma once

namespace CMI
{

class Sample;

class Instrument
{
public:
	Instrument();

	virtual ~Instrument();

	const Sample *getSample(UInt32 index) const;

	void setSample(UInt32 index, Sample *sample);

private:
	static const UInt32 MaxSampleCount = 1024;
	Sample *_samples[MaxSampleCount];
};

} //namespace CMI
