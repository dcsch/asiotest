#pragma once

namespace CMI
{

class SoundSystem;

class MIDIController
{
public:

	class Delegate
	{
	public:
		virtual void OnMIDIMessage() = 0;
	};

	MIDIController();

	virtual ~MIDIController();

	virtual void Initialise(UInt32 deviceIndex, Delegate *delegate);

	virtual void Finalise();

	virtual SoundSystem *GetSoundOutput();

	virtual void SetSoundOutput(SoundSystem *soundOutput);

protected:
	virtual void OnMIDIMessage(UInt8 status, UInt8 data1, UInt8 data2, UInt32 timestamp);

	virtual void OnMIDINoteOff(UInt8 channel, UInt8 keyNumber, UInt8 velocity);

	virtual void OnMIDINoteOn(UInt8 channel, UInt8 keyNumber, UInt8 velocity);

private:
	SoundSystem *_soundOutput;
	Delegate *_delegate;
};

} //namespace CMI
