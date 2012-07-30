#include "StdAfx.h"
#include "common.h"
#include "Sample.h"
#include "WaveAudioLoader.h"
#include "samplerate.h"

namespace CMI
{

Sample::Sample() :
	_buffer(0),
	_frameCount(0),
	_rate(0),
	_channels(0)
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

	_rate = loader.getFormatChunk()->nSamplesPerSec;
	_channels = loader.getFormatChunk()->nChannels;

	Length rawBufferLength2;
	UInt8 *rawBuffer2;
	if (_channels == 1)
	{
		// Convert to 2-channel
		rawBufferLength2 = 2 * rawBufferLength;
		rawBuffer2 = new UInt8[(UInt32)rawBufferLength2];
		Int16 *src = reinterpret_cast<Int16 *>(rawBuffer);
		Int16 *dst = reinterpret_cast<Int16 *>(rawBuffer2);
		for (UInt32 i = 0; i < rawBufferLength / 2; ++i)
		{
			Int16 sample = *src++;
			*dst++ = sample;
			*dst++ = sample;
		}
		_channels = 2;
	}
	else
	{
		rawBufferLength2 = rawBufferLength;
		rawBuffer2 = rawBuffer;
	}

	// Convert to a float buffer
	_frameCount = rawBufferLength2 / 4;
	_buffer = new float[(UInt32)rawBufferLength2 / 2];
	src_short_to_float_array((const short *)rawBuffer2, _buffer, (int)rawBufferLength2 / 2);

	delete [] rawBuffer;
	if (rawBuffer2 != rawBuffer)
		delete [] rawBuffer2;
}

} //namespace CMI
