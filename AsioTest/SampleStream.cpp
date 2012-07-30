#include "StdAfx.h"
#include "common.h"
#include "SampleStream.h"
#include "Sample.h"
#include "samplerate.h"

namespace CMI
{

SampleStream::SampleStream() : _sample(0), _offset(-1), _src(0)
{
}

SampleStream::~SampleStream()
{
}

const Sample *SampleStream::getSample() const
{
	return _sample;
}

//void SampleStream::setSample(const Sample *sample)
//{
//	_sample = sample;
//	_offset = -1;
//}

UInt32 SampleStream::getSamplesAvailable() const
{
	if (_sample)
	{
		Length samplesAvailable = _sample->getFrameCount() - _offset;
		return static_cast<UInt32>(max(samplesAvailable, 0));
	}

	return 0;
}

void SampleStream::start(const Sample *sample, UInt8 keyNumber)
{
	_sample = sample;
	_keyNumber = keyNumber;
	_offset = 0;
}

UInt32 SampleStream::read(float *frameBuffer, UInt32 frameCount)
{
	if (_src == 0)
	{
		int error;
		_src = src_new(SRC_SINC_BEST_QUALITY, 2, &error);
		if (error)
		{
		}
	}

	UInt32 samplesAvailable = getSamplesAvailable();

	SRC_DATA srcData;
	memset(&srcData, 0, sizeof(SRC_DATA));
	srcData.data_in = const_cast<float *>(_sample->getBuffer()) + 2 * _offset;
	srcData.input_frames = min(1000, samplesAvailable);
	srcData.data_out = frameBuffer;
	srcData.output_frames = frameCount;
	srcData.src_ratio = 1.0;
	srcData.end_of_input = 0;

	src_process(_src, &srcData);

	_offset += srcData.input_frames_used;

	//const float *src = _sample->getBuffer() + 2 * _offset;
	//float *dst = frameBuffer;
	//UInt32 samplesAvailable = getSamplesAvailable();
	//for (UINT32 j = 0; j < frameCount; ++j)
	//{
	//	if (j <= samplesAvailable)
	//	{
	//		*dst++ = *src++;
	//		*dst++ = *src++;
	//	}
	//	else
	//	{
	//		*dst++ = 0.f;
	//		*dst++ = 0.f;
	//	}
	//}
	//_offset += frameCount;

	return frameCount;
}

} //namespace CMI
