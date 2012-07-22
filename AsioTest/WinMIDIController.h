#pragma once

#include "MIDIController.h"
#include <mmsystem.h>

namespace CMI
{

class WinMIDIController : public MIDIController
{
public:

	WinMIDIController();

	virtual ~WinMIDIController();

	virtual void Initialise(UInt32 deviceIndex, Delegate *delegate);

	virtual void Finalise();

private:
	MIDIINCAPS _midiInCaps;
	HMIDIIN _hmi;

	static void CALLBACK MidiInProc(
		HMIDIIN hMidiIn,
		UINT wMsg,
		DWORD_PTR dwInstance,
		DWORD_PTR dwParam1,
		DWORD_PTR dwParam2);
};

} //namespace CMI
