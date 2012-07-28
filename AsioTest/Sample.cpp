#include "StdAfx.h"
#include "common.h"
#include "Sample.h"
#include "WaveAudioLoader.h"
#include "samplerate.h"

namespace CMI
{

Sample::Sample() :
	_buffer(0),
	_bufferLength(0)
{
}

Sample::~Sample()
{
	delete [] _buffer;
}

void Sample::Load(Reader &reader)
{
	UInt8 *rawBuffer;
	Length rawBufferLength;
	WaveAudioLoader loader;
	loader.Load(reader, &rawBuffer, &rawBufferLength);

	// Convert to a float buffer
	_bufferLength = rawBufferLength / 2;
	_buffer = new float[(UInt32)_bufferLength];
	src_short_to_float_array((const short *)rawBuffer, _buffer, (int)_bufferLength);

	delete [] rawBuffer;
}

} //namespace CMI
