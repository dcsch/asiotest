#pragma once

namespace CMI
{

class Session;
class SampleStream;

class SoundSystem
{
public:
	SoundSystem();

	virtual ~SoundSystem();

	virtual void Initialise();

	virtual void Finalise();

	virtual const Session *getSession() const;

	virtual void setSession(Session *session);

	virtual void play(UInt8 keyNumber);

	virtual UInt32 getSampleRate() const = 0;

protected:
	Session *_session;
	static const UInt32 MaxPolyphonyCount = 96;
	SampleStream *_sampleStreams[MaxPolyphonyCount];

	virtual UInt32 process(float *frameBuffer, UInt32 frameCount);
};

} //namespace CMI
