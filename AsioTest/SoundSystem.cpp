#include "StdAfx.h"
#include "common.h"
#include "SoundSystem.h"
#include "WaveAudioLoader.h"

namespace CMI
{

SoundSystem::SoundSystem() :
	_buffer(0),
	_bufferLength(0)
{
}

SoundSystem::~SoundSystem()
{
}

void SoundSystem::Initialise()
{
}

void SoundSystem::Finalise()
{
}

void SoundSystem::LoadSample(Reader &reader)
{
	WaveAudioLoader loader;
	loader.Load(reader, &_buffer, &_bufferLength);
}

} //namespace CMI
