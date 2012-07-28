#pragma once

namespace CMI
{

class Instrument;

class Session
{
public:
	Session();

	virtual ~Session();

	virtual const Instrument *getInstrument(UInt32 index) const;

	virtual void setInstrument(UInt32 index, Instrument *instrument);

private:
	static const UInt32 MaxInstrumentCount = 24;
	Instrument *_instruments[MaxInstrumentCount];
};

} //namespace CMI
