#pragma once

struct SRC_STATE_tag;
typedef struct SRC_STATE_tag SRC_STATE ;

namespace CMI
{

class SoundSystem;
class Sample;

class SampleStream
{
public:
	SampleStream(SoundSystem *ss);
	virtual ~SampleStream();

	const Sample *getSample() const;

	//void setSample(const Sample *sample);

	UInt32 getSamplesAvailable() const;

	virtual void start(const Sample *sample, UInt8 keyNumber);

	virtual UInt32 read(float *frameBuffer, UInt32 frameCount);

private:
	SoundSystem *_ss;
	const Sample *_sample;
	Position _offset;
	UInt8 _keyNumber;
	SRC_STATE *_src;
};

} //namespace CMI
