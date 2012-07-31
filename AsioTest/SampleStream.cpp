#include "StdAfx.h"
#include "common.h"
#include "SampleStream.h"
#include "Sample.h"
#include "SoundSystem.h"
#include "samplerate.h"
#include <math.h>

namespace CMI
{

SampleStream::SampleStream(SoundSystem *ss) :
	_ss(ss),
	_sample(0),
	_offset(-1),
	_src(0),
	_mutex(0)
{
	_mutex = CreateMutexEx(NULL, NULL, 0, 0);
}

SampleStream::~SampleStream()
{
	CloseHandle(_mutex);
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
	WaitForSingleObject(_mutex, INFINITE);

	_sample = sample;
	_keyNumber = keyNumber;
	_offset = 0;

	if (_src)
	{
		src_reset(_src);
	}

	ReleaseMutex(_mutex);
}

UInt32 SampleStream::read(float *frameBuffer, UInt32 frameCount)
{
	WaitForSingleObject(_mutex, INFINITE);

	double outputScaling = (double)_ss->getSampleRate() / _sample->getRate();

	double f = pow(2.0, (_keyNumber - 69) / 12.0) * 440.0;

	const double C3 = 261.62556530059862;
	double noteRatio = C3 / f;
	
	// TODO This needs to be checked with samples with rates that don't match the output
	double scaledRatio = noteRatio * outputScaling;

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
	srcData.src_ratio = scaledRatio;
	srcData.end_of_input = 0;

	src_process(_src, &srcData);

	ReleaseMutex(_mutex);

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
