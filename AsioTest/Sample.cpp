#include "StdAfx.h"
#include "common.h"
#include "Sample.h"
#include "WaveAudioLoader.h"

namespace CMI
{

Sample::Sample() :
	_buffer(0),
	_bufferLength(0)
{
}

Sample::~Sample()
{
}

void Sample::Load(Reader &reader)
{
	WaveAudioLoader loader;
	loader.Load(reader, &_buffer, &_bufferLength);
}

} //namespace CMI
