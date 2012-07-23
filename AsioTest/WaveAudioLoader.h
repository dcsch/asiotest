#pragma once

#include "EssenceContentHandler.h"
#include "EssenceErrorHandler.h"

namespace CMI
{

class Reader;

class WaveAudioLoader :
	public EssenceContentHandler,
	public EssenceErrorHandler
{
public:
	WaveAudioLoader();
	~WaveAudioLoader();

	void WaveAudioLoader::Load(Reader &reader, UInt8 **buffer, Length *bufferLen);

	// EssenceContentHandler

	virtual UInt8* allocateBuffer(
        const EssenceReader* reader,
        UInt32 trackIndex,
        const UL& ul,
        UInt32 length) throw();

	virtual void freeBuffer(
        const EssenceReader* reader,
        UInt32 trackIndex,
        const UL& ul,
        UInt8* buffer) throw();

    virtual bool deliver(
        const EssenceReader* reader,
        UInt32 trackIndex,
        const UL& ul,
        UInt8* buffer,
        UInt32 dataLength,
        UInt64 position,
        UInt32 deliveryFlags,
        UInt8 editUnitFlags,
        UInt32 editUnit,
        const Rational& editRate) throw();

    virtual void endOfStream(
        const EssenceReader* reader,
        UInt32 trackIndex,
        const UL& ul) throw();

	// EssenceErrorHandler

	virtual void warning(const EssenceReader* reader, UInt32 code, Position position) throw();

	virtual void error(const EssenceReader* reader, UInt32 code, Position position) throw();

private:
	UInt8 *_buffer;
	Length _bufferLength;
	Position _bufferOffset;
};

} //namespace CMI
