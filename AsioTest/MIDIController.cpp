#include "StdAfx.h"
#include "common.h"
#include "MIDIController.h"
#include "SoundSystem.h"

namespace CMI
{

MIDIController::MIDIController() : _soundOutput(0), _delegate(0)
{
}

MIDIController::~MIDIController()
{
}

void MIDIController::Initialise(UInt32 deviceIndex, Delegate *delegate)
{
	_delegate = delegate;
}

void MIDIController::Finalise()
{
}

SoundSystem *MIDIController::GetSoundOutput()
{
	return _soundOutput;
}

void MIDIController::SetSoundOutput(SoundSystem *soundOutput)
{
	_soundOutput = soundOutput;
}

void MIDIController::OnMIDIMessage(UInt8 status, UInt8 data1, UInt8 data2, UInt32 timestamp)
{
	if (_delegate)
		_delegate->OnMIDIMessage();

	switch (status & 0xF0)
	{
	case 0x80:
		OnMIDINoteOff(status & 0x0F, data1, data2);
		break;
	case 0x90:
		if (data2 > 0) // Velocity 0 is note off
			OnMIDINoteOn(status & 0x0F, data1, data2);
		else
			OnMIDINoteOff(status & 0x0F, data1, data2);
		break;
	}
}

void MIDIController::OnMIDINoteOff(UInt8 channel, UInt8 keyNumber, UInt8 velocity)
{
}

void MIDIController::OnMIDINoteOn(UInt8 channel, UInt8 keyNumber, UInt8 velocity)
{
	if (_soundOutput)
		_soundOutput->PlaySample();
}

} //namespace CMI
