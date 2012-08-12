#include "common.h"
#include "WaveAudioLoader.h"
#include <memory.h>

namespace CMI
{

WaveAudioLoader::WaveAudioLoader() : _buffer(0), _bufferLength(0), _bufferOffset(0)
{
}

WaveAudioLoader::~WaveAudioLoader()
{
}

void WaveAudioLoader::Load(Reader &reader, UInt8 **buffer, Length *bufferLen)
{
    Rational rate(25, 1);
    WaveAudioEssenceReader *waer = new WaveAudioEssenceReader(0, this, this, &rate);
	waer->initialise(&reader);

    memcpy(&_fmt, waer->getFormatChunk(), sizeof(WaveAudioEssenceReader::fmt_ck));
	_bufferLength = waer->getDataLength();
	_buffer = new UInt8[(unsigned int)_bufferLength];
	_bufferOffset = 0;

	while (waer->nextEditUnit())
	{
	}
	waer->finalise();
	delete waer;
	waer = 0;

	*buffer = _buffer;
	*bufferLen = _bufferLength;
}

UInt8 *WaveAudioLoader::allocateBuffer(
    const EssenceReader* reader,
    UInt32 trackIndex,
    const UL& ul,
    UInt32 length) throw()
{
	UInt8 *ptr = _buffer + _bufferOffset;
	_bufferOffset += length;
	return ptr;
}

void WaveAudioLoader::freeBuffer(
    const EssenceReader* reader,
    UInt32 trackIndex,
    const UL& ul,
    UInt8* buffer) throw()
{
	// NOP
}

bool WaveAudioLoader::deliver(
    const EssenceReader* reader,
    UInt32 trackIndex,
    const UL& ul,
    UInt8* buffer,
    UInt32 dataLength,
    UInt64 position,
    UInt32 deliveryFlags,
    UInt8 editUnitFlags,
    UInt32 editUnit,
    const Rational& editRate) throw()
{
	// Data has already been copied into the buffer, so no work is required here
	return true;
}

void WaveAudioLoader::endOfStream(
    const EssenceReader* reader,
    UInt32 trackIndex,
    const UL& ul) throw()
{
}

void WaveAudioLoader::warning(const EssenceReader* reader, UInt32 code, Position position) throw()
{
}

void WaveAudioLoader::error(const EssenceReader* reader, UInt32 code, Position position) throw()
{
}

} //namespace CMI
