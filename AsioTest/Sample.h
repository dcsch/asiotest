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

private:
	float *_buffer;
	Length _frameCount;
};

} //namespace CMI
