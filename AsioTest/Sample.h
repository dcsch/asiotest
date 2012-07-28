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
	Length getBufferLength() const { return _bufferLength; }

private:
	float *_buffer;
	Length _bufferLength;
};

} //namespace CMI
