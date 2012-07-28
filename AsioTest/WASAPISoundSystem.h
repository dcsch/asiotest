#pragma once

#include "SoundSystem.h"
#include <Mmdeviceapi.h>
#include <Audioclient.h>
#include <AudioPolicy.h>

namespace CMI
{

class WASAPISoundSystem :
	public SoundSystem,
	IMMNotificationClient,
	IAudioSessionEvents
{
public:
    enum RenderSampleType
    {
        SampleTypeFloat,
        SampleType16BitPCM,
    };

	WASAPISoundSystem(UInt32 latency);

	virtual ~WASAPISoundSystem();

	virtual void Initialise();

	virtual void Finalise();

	virtual void play();

	STDMETHOD_(ULONG, AddRef)();
	STDMETHOD_(ULONG, Release)();

private:
	void Begin();

	void End();

private:
    LONG _refCount;
	IMMDeviceEnumerator *_enumerator;
	IMMDevice *_device;
	IAudioClient *_audioClient;
	WAVEFORMATEX *_wfx;
	IAudioRenderClient *_renderClient;
	IAudioSessionControl *_audioSessionControl;
    HANDLE _renderThread;
    HANDLE _shutdownEvent;
    HANDLE _audioSamplesReadyEvent;
	UInt32 _engineLatencyInMS;
    RenderSampleType _renderSampleType;
	UINT32 _bufferSize;

	UInt8 *_playPtr;

	bool CalculateMixFormatType();

	static DWORD __stdcall WASAPIRenderThread(LPVOID context);

	DWORD DoRenderThread();

	// IAudioSessionEvents
	STDMETHOD(OnDisplayNameChanged) (LPCWSTR /*NewDisplayName*/, LPCGUID /*EventContext*/) { return S_OK; };
    STDMETHOD(OnIconPathChanged) (LPCWSTR /*NewIconPath*/, LPCGUID /*EventContext*/) { return S_OK; };
    STDMETHOD(OnSimpleVolumeChanged) (float /*NewSimpleVolume*/, BOOL /*NewMute*/, LPCGUID /*EventContext*/) { return S_OK; }
    STDMETHOD(OnChannelVolumeChanged) (DWORD /*ChannelCount*/, float /*NewChannelVolumes*/[], DWORD /*ChangedChannel*/, LPCGUID /*EventContext*/) { return S_OK; };
    STDMETHOD(OnGroupingParamChanged) (LPCGUID /*NewGroupingParam*/, LPCGUID /*EventContext*/) {return S_OK; };
    STDMETHOD(OnStateChanged) (AudioSessionState /*NewState*/) { return S_OK; };
    STDMETHOD(OnSessionDisconnected) (AudioSessionDisconnectReason DisconnectReason);

	// IMMNotification Client
	STDMETHOD(OnDeviceStateChanged) (LPCWSTR /*DeviceId*/, DWORD /*NewState*/) { return S_OK; }
    STDMETHOD(OnDeviceAdded) (LPCWSTR /*DeviceId*/) { return S_OK; };
    STDMETHOD(OnDeviceRemoved) (LPCWSTR /*DeviceId(*/) { return S_OK; };
    STDMETHOD(OnDefaultDeviceChanged) (EDataFlow Flow, ERole Role, LPCWSTR NewDefaultDeviceId);
    STDMETHOD(OnPropertyValueChanged) (LPCWSTR /*DeviceId*/, const PROPERTYKEY /*Key*/){return S_OK; };

    //  IUnknown
    STDMETHOD(QueryInterface)(REFIID iid, void **pvObject);
};

} //namespace CMI
