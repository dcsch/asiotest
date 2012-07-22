#pragma once

namespace CMI
{

class Reader;

class SoundSystem
{
public:
	virtual ~SoundSystem() { };

	virtual void Initialise() = 0;

	virtual void Finalise() = 0;

	virtual void LoadSample(Reader &reader) = 0;

	virtual void PlaySample() = 0;

	virtual void GenerateSineWaveSample() = 0;
};

} //namespace CMI
