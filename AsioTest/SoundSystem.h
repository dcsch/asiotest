#pragma once

namespace CMI
{

class Reader;

class SoundSystem
{
public:
	SoundSystem();

	virtual ~SoundSystem();

	virtual void Initialise();

	virtual void Finalise();

	virtual void LoadSample(Reader &reader);

	virtual void PlaySample() = 0;

//	virtual void GenerateSineWaveSample() = 0;

protected:
	UInt8 *_buffer;
	Length _bufferLength;
};

} //namespace CMI
