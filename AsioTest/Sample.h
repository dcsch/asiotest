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

	const UInt8 *getBuffer() const { return _buffer; }
	Length getBufferLength() const { return _bufferLength; }

private:
	UInt8 *_buffer;
	Length _bufferLength;
};

} //namespace CMI
