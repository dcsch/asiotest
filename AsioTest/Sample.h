#pragma once

namespace CMI
{

class Reader;

class Sample
{
public:
	Sample();
	virtual ~Sample();

	void Load(Reader &reader);

	const float *getBuffer() const { return _buffer; }
	float *getBuffer() { return _buffer; }
	Length getFrameCount() const { return _frameCount; }
	UInt32 getRate() const { return _rate; }
	UInt32 getChannels() const { return _channels; }

private:
	float *_buffer;
	Length _frameCount;
	UInt32 _rate;
	UInt32 _channels;
};

} //namespace CMI
