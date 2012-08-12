#include "common.h"
#include "Session.h"

namespace CMI
{

Session::Session()
{
	for (UInt32 i = 0; i < MaxInstrumentCount; ++i)
		_instruments[i] = 0;
}

Session::~Session()
{
}

const Instrument *Session::getInstrument(UInt32 index) const
{
	if (index >= MaxInstrumentCount)
		return 0;
	return _instruments[index];
}

void Session::setInstrument(UInt32 index, Instrument *instrument)
{
	if (index >= MaxInstrumentCount)
		return;

	_instruments[index] = instrument;
}

} //namespace CMI
