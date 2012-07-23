#include "StdAfx.h"
#include "WASAPISoundSystem.h"

// REFERENCE_TIME time units per second and per millisecond
#define REFTIMES_PER_SEC  10000000
#define REFTIMES_PER_MILLISEC  10000

#define SAFE_RELEASE(punk)  \
              if ((punk) != NULL)  \
                { (punk)->Release(); (punk) = NULL; }

const CLSID CLSID_MMDeviceEnumerator = __uuidof(MMDeviceEnumerator);
const IID IID_IMMDeviceEnumerator = __uuidof(IMMDeviceEnumerator);
const IID IID_IAudioClient = __uuidof(IAudioClient);
const IID IID_IAudioRenderClient = __uuidof(IAudioRenderClient);

namespace CMI
{

WASAPISoundSystem::WASAPISoundSystem() :
	_enumerator(0),
	_device(0),
	_audioClient(0),
	_wfx(0),
	_renderClient(0)
{
}

WASAPISoundSystem::~WASAPISoundSystem()
{
}

void WASAPISoundSystem::Initialise()
{
	HRESULT hr = CoCreateInstance(
		CLSID_MMDeviceEnumerator,
		NULL,
        CLSCTX_ALL,
		IID_IMMDeviceEnumerator,
        (void **)&_enumerator);
    if (FAILED(hr))
		return;

	hr = _enumerator->GetDefaultAudioEndpoint(eRender, eConsole, &_device);
    if (FAILED(hr))
		return;

	hr = _device->Activate(
		IID_IAudioClient,
		CLSCTX_ALL,
		NULL,
		(void **)&_audioClient);
    if (FAILED(hr))
		return;

	hr = _audioClient->GetMixFormat(&_wfx);
    if (FAILED(hr))
		return;

	REFERENCE_TIME hnsRequestedDuration = REFTIMES_PER_SEC;
	hr = _audioClient->Initialize(
		AUDCLNT_SHAREMODE_SHARED,
		0,
		hnsRequestedDuration,
		0,
		_wfx,
		NULL);
    if (FAILED(hr))
		return;

	// Tell the audio source which format to use.
	//hr = pMySource->SetFormat(pwfx);
	//EXIT_ON_ERROR(hr)

	// Get the actual size of the allocated buffer.
	UINT32 bufferFrameCount;
	hr = _audioClient->GetBufferSize(&bufferFrameCount);
    if (FAILED(hr))
		return;

	hr = _audioClient->GetService(
		IID_IAudioRenderClient,
		(void**)&_renderClient);
}

void WASAPISoundSystem::Finalise()
{
    CoTaskMemFree(_wfx);
    SAFE_RELEASE(_enumerator)
    SAFE_RELEASE(_device)
    SAFE_RELEASE(_audioClient)
    SAFE_RELEASE(_renderClient)
}

void WASAPISoundSystem::LoadSample(Reader &reader)
{
}

void WASAPISoundSystem::PlaySample()
{
}

} //namespace CMI
