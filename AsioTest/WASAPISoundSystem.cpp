#include "StdAfx.h"
#include "common.h"
#include "WASAPISoundSystem.h"
#include <avrt.h>
#include "Sample.h"

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

WASAPISoundSystem::WASAPISoundSystem(UInt32 latency) :
	SoundSystem(),
	_refCount(1),
	_enumerator(0),
	_device(0),
	_audioClient(0),
	_wfx(0),
	_renderClient(0),
	_audioSessionControl(0),
	_renderThread(0),
    _shutdownEvent(0),
    _audioSamplesReadyEvent(0),
	_engineLatencyInMS(latency),
	_bufferSize(0),
	_offset(-1)
{
}

WASAPISoundSystem::~WASAPISoundSystem()
{
}

bool WASAPISoundSystem::CalculateMixFormatType()
{
    if (_wfx->wFormatTag == WAVE_FORMAT_PCM || 
        _wfx->wFormatTag == WAVE_FORMAT_EXTENSIBLE &&
            reinterpret_cast<WAVEFORMATEXTENSIBLE *>(_wfx)->SubFormat == KSDATAFORMAT_SUBTYPE_PCM)
    {
        if (_wfx->wBitsPerSample == 16)
        {
            _renderSampleType = SampleType16BitPCM;
        }
        else
        {
            printf("Unknown PCM integer sample type\n");
            return false;
        }
    }
    else if (_wfx->wFormatTag == WAVE_FORMAT_IEEE_FLOAT ||
             (_wfx->wFormatTag == WAVE_FORMAT_EXTENSIBLE &&
               reinterpret_cast<WAVEFORMATEXTENSIBLE *>(_wfx)->SubFormat == KSDATAFORMAT_SUBTYPE_IEEE_FLOAT))
    {
        _renderSampleType = SampleTypeFloat;
    }
    else 
    {
        printf("unrecognized device format.\n");
        return false;
    }
    return true;
}

void WASAPISoundSystem::Initialise()
{
	SoundSystem::Initialise();

    //  Create our shutdown and samples ready events- we want auto reset events that start in the not-signaled state.
    _shutdownEvent = CreateEventEx(NULL, NULL, 0, EVENT_MODIFY_STATE | SYNCHRONIZE);
    if (_shutdownEvent == NULL)
    {
        printf("Unable to create shutdown event: %d.\n", GetLastError());
        return;
    }

    _audioSamplesReadyEvent = CreateEventEx(NULL, NULL, 0, EVENT_MODIFY_STATE | SYNCHRONIZE);
    if (_audioSamplesReadyEvent == NULL)
    {
        printf("Unable to create samples ready event: %d.\n", GetLastError());
        return;
    }

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

	CalculateMixFormatType();

	hr = _audioClient->Initialize(
		AUDCLNT_SHAREMODE_SHARED,
		AUDCLNT_STREAMFLAGS_EVENTCALLBACK | AUDCLNT_STREAMFLAGS_NOPERSIST,
		_engineLatencyInMS * 10000,
		0,
		_wfx,
		NULL);
    if (FAILED(hr))
		return;

	// Tell the audio source which format to use.
	//hr = pMySource->SetFormat(pwfx);
	//EXIT_ON_ERROR(hr)

	// Get the actual size of the allocated buffer.
	hr = _audioClient->GetBufferSize(&_bufferSize);
    if (FAILED(hr))
		return;

    hr = _audioClient->SetEventHandle(_audioSamplesReadyEvent);
    if (FAILED(hr))
    {
        printf("Unable to set ready event: %x.\n", hr);
        return;
    }

	hr = _audioClient->GetService(
		IID_IAudioRenderClient,
		(void**)&_renderClient);

	hr = _audioClient->GetService(IID_PPV_ARGS(&_audioSessionControl));
    if (FAILED(hr))
    {
        printf("Unable to retrieve session control: %x\n", hr);
        return;
    }

  //  //  Create the stream switch complete event- we want a manual reset event that starts in the not-signaled state.
  //  _streamSwitchCompleteEvent = CreateEventEx(
		//NULL,
		//NULL,
		//CREATE_EVENT_INITIAL_SET | CREATE_EVENT_MANUAL_RESET,
		//EVENT_MODIFY_STATE | SYNCHRONIZE);
  //  if (_streamSwitchCompleteEvent == NULL)
  //  {
  //      printf("Unable to create stream switch event: %d.\n", GetLastError());
  //      return;
  //  }

	//  Register for session and endpoint change notifications.  
    //  A stream switch is initiated when we receive a session disconnect notification or we receive a default device changed notification.
    hr = _audioSessionControl->RegisterAudioSessionNotification(this);
    if (FAILED(hr))
    {
        printf("Unable to register for stream switch notifications: %x\n", hr);
        return;
    }

    hr = _enumerator->RegisterEndpointNotificationCallback(this);
    if (FAILED(hr))
    {
        printf("Unable to register for stream switch notifications: %x\n", hr);
        return;
    }

	Begin();
}

void WASAPISoundSystem::Finalise()
{
	End();

	SoundSystem::Finalise();

    CoTaskMemFree(_wfx);
    SAFE_RELEASE(_enumerator)
    SAFE_RELEASE(_device)
    SAFE_RELEASE(_audioClient)
    SAFE_RELEASE(_renderClient)
}

void WASAPISoundSystem::play()
{
	SoundSystem::play();

	_offset = 0;
}

void WASAPISoundSystem::Begin()
{
    _renderThread = CreateThread(NULL, 0, WASAPIRenderThread, this, 0, NULL);
    if (_renderThread == NULL)
    {
        printf("Unable to create transport thread: %x.", GetLastError());
        return;
    }

    //  We're ready to go, start rendering!
    HRESULT hr = _audioClient->Start();
    if (FAILED(hr))
    {
        printf("Unable to start render client: %x.\n", hr);
        return;
    }
}

void WASAPISoundSystem::End()
{
    if (_shutdownEvent)
        SetEvent(_shutdownEvent);

    HRESULT hr = _audioClient->Stop();
    if (FAILED(hr))
    {
        printf("Unable to stop audio client: %x\n", hr);
    }

    if (_renderThread)
    {
        WaitForSingleObject(_renderThread, INFINITE);

        CloseHandle(_renderThread);
        _renderThread = NULL;
    }
}

DWORD WASAPISoundSystem::WASAPIRenderThread(LPVOID context)
{
    WASAPISoundSystem *renderer = static_cast<WASAPISoundSystem *>(context);
    return renderer->DoRenderThread();
}

DWORD WASAPISoundSystem::DoRenderThread()
{
    bool stillPlaying = true;
    HANDLE waitArray[2] = {_shutdownEvent, _audioSamplesReadyEvent};
    HANDLE mmcssHandle = NULL;
    DWORD mmcssTaskIndex = 0;

    HRESULT hr = CoInitializeEx(NULL, COINIT_MULTITHREADED);
    if (FAILED(hr))
    {
        printf("Unable to initialize COM in render thread: %x\n", hr);
        return hr;
    }

    mmcssHandle = AvSetMmThreadCharacteristics(L"Audio", &mmcssTaskIndex);
    if (mmcssHandle == NULL)
    {
        printf("Unable to enable MMCSS on render thread: %d\n", GetLastError());
    }

    while (stillPlaying)
    {
        HRESULT hr;
        DWORD waitResult = WaitForMultipleObjects(2, waitArray, FALSE, INFINITE);
        switch (waitResult)
        {
        case WAIT_OBJECT_0 + 0:     // _ShutdownEvent
            stillPlaying = false;       // We're done, exit the loop.
            break;

		case WAIT_OBJECT_0 + 1:     // _AudioSamplesReadyEvent

			//  We need to provide the next buffer of samples to the audio renderer.
            BYTE *pData;
            UINT32 padding;
            UINT32 framesAvailable;

			//  We want to find out how much of the buffer *isn't* available (is padding).
			hr = _audioClient->GetCurrentPadding(&padding);
            if (SUCCEEDED(hr))
            {
                //
                //  Calculate the number of frames available.  We'll render
                //  that many frames or the number of frames left in the buffer, whichever is smaller.
                //
                framesAvailable = _bufferSize - padding;

				hr = _renderClient->GetBuffer(framesAvailable, &pData);

				if (SUCCEEDED(hr))
				{
					if (_offset > -1)
					{
						UInt32 processedFrameCount =
							process(_offset, reinterpret_cast<float *>(pData), framesAvailable);
						if (processedFrameCount)
							_offset += processedFrameCount;
						else
							_offset = -1;
					}

					hr = _renderClient->ReleaseBuffer(framesAvailable, 0);
					if (!SUCCEEDED(hr))
					{
						printf("Unable to release buffer: %x\n", hr);
						//stillPlaying = false;
					}
				}
				else
				{
					printf("Unable to release buffer: %x\n", hr);
					//stillPlaying = false;
				}
            }
            break;
        }
    }

    //
    //  Unhook from MMCSS.
    //
    AvRevertMmThreadCharacteristics(mmcssHandle);

    CoUninitialize();
    return 0;
}

//
//  Called when an audio session is disconnected.  
//
//  When a session is disconnected because of a device removal or format change event, we just want 
//  to let the render thread know that the session's gone away
//
HRESULT WASAPISoundSystem::OnSessionDisconnected(AudioSessionDisconnectReason DisconnectReason)
{
    return S_OK;
}
//
//  Called when the default render device changed.  We just want to set an event which lets the stream switch logic know that it's ok to 
//  continue with the stream switch.
//
HRESULT WASAPISoundSystem::OnDefaultDeviceChanged(EDataFlow Flow, ERole Role, LPCWSTR /*NewDefaultDeviceId*/)
{
    return S_OK;
}

//  IUnknown
HRESULT WASAPISoundSystem::QueryInterface(REFIID iid, void **object)
{
    if (object == NULL)
    {
        return E_POINTER;
    }
    *object = NULL;

    if (iid == IID_IUnknown)
    {
        *object = static_cast<IUnknown *>(static_cast<IAudioSessionEvents *>(this));
        AddRef();
    }
    else if (iid == __uuidof(IMMNotificationClient))
    {
        *object = static_cast<IMMNotificationClient *>(this);
        AddRef();
    }
    else if (iid == __uuidof(IAudioSessionEvents))
    {
        *object = static_cast<IAudioSessionEvents *>(this);
        AddRef();
    }
    else
    {
        return E_NOINTERFACE;
    }
    return S_OK;
}

ULONG WASAPISoundSystem::AddRef()
{
    return InterlockedIncrement(&_refCount);
}

ULONG WASAPISoundSystem::Release()
{
    ULONG returnValue = InterlockedDecrement(&_refCount);
    if (returnValue == 0)
    {
        delete this;
    }
    return returnValue;
}

} //namespace CMI
