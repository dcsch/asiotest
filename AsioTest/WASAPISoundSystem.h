#pragma once

#include "SoundSystem.h"
#include <Mmdeviceapi.h>
#include <Audioclient.h>

namespace CMI
{

class WASAPISoundSystem :
	public SoundSystem
{
public:
	WASAPISoundSystem();

	virtual ~WASAPISoundSystem();

	virtual void Initialise();

	virtual void Finalise();

	virtual void LoadSample(Reader &reader);

	virtual void PlaySample();

private:
	IMMDeviceEnumerator *_enumerator;
	IMMDevice *_device;
	IAudioClient *_audioClient;
	WAVEFORMATEX *_wfx;
	IAudioRenderClient *_renderClient;
};

} //namespace CMI
