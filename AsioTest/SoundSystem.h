#pragma once

namespace CMI
{

class Session;
class Sample;

class SoundSystem
{
public:
	SoundSystem();

	virtual ~SoundSystem();

	virtual void Initialise();

	virtual void Finalise();

	virtual const Session *getSession() const;

	virtual void setSession(Session *session);

	virtual void play();

protected:
	Session *_session;
	static const UInt32 MaxPolyphonyCount = 96;
	const Sample *_samples[MaxPolyphonyCount];
};

} //namespace CMI
