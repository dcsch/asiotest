#include "StdAfx.h"
#include "common.h"
#include "WinMIDIController.h"
#include "SoundSystem.h"

namespace CMI
{

WinMIDIController::WinMIDIController() : MIDIController(), _midiInCaps(), _hmi(0)
{
}

WinMIDIController::~WinMIDIController()
{
}

void WinMIDIController::Initialise(UInt32 deviceIndex, Delegate *delegate)
{
	MIDIController::Initialise(deviceIndex, delegate);

	UINT devices = midiInGetNumDevs();
	if (devices > deviceIndex)
	{
		MMRESULT result = midiInGetDevCaps(deviceIndex, &_midiInCaps, sizeof(MIDIINCAPS));
		result = midiInOpen(
			&_hmi,
			deviceIndex,
			reinterpret_cast<DWORD_PTR>(MidiInProc),
			reinterpret_cast<DWORD_PTR>(this),
			CALLBACK_FUNCTION);
		result = midiInStart(_hmi);
	}
}

void WinMIDIController::Finalise()
{
	MIDIController::Finalise();

	MMRESULT result = midiInStop(_hmi);
	result = midiInClose(_hmi);
}

void CALLBACK WinMIDIController::MidiInProc(
	HMIDIIN hMidiIn,
	UINT wMsg,
	DWORD_PTR dwInstance,
	DWORD_PTR dwParam1,
	DWORD_PTR dwParam2)
{
	if (wMsg == MIM_DATA)
	{
		UInt8 status = LOBYTE(LOWORD(dwParam1));
		UInt8 data1 = HIBYTE(LOWORD(dwParam1));
		UInt8 data2 = LOBYTE(HIWORD(dwParam1));
		reinterpret_cast<WinMIDIController *>(dwInstance)->OnMIDIMessage(status, data1, data2, dwParam2);
	}
}

} //namespace CMI
