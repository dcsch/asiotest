#include "common.h"
#include "WaveAudioEssenceReader.h"
#include "EssenceContentHandler.h"
#include "Reader.h"
#include "RIFFReader.h"
#include "RIFFChunk.h"
//#include <mxf/Labels.h>
//#include <mxf/Keys.h>
//#include <mxf/WaveAudioEssenceDescriptor.h>
//#include <mxf/Dictionary.h>
#include "Timecode.h"
//#include <mxf/IndexEntry.h>
#include "errorcodes.h"
//#include <MXFAssert.h>

#include <math.h>

namespace CMI
{

WaveAudioEssenceReader::WaveAudioEssenceReader(
    UInt32 id,
    EssenceContentHandler* contentHandler,
    EssenceErrorHandler* errorHandler,
    const Rational* editRate) :
    EssenceReader(id, contentHandler, errorHandler),
	_containerDuration(0),
    _ulAudio(0),
    _fmtChunk(0),
    _bextChunk(0),
    _bextBuffer(0),
    _dataOffset(0),
    _dataLength(0),
    _editUnit(0),
    _currentStartPosition(0),
    _editRate(*editRate),
    _conversion(false),
    _baseSampleCount(0),
    _additionalSampleCount(),
    _additionalSampleCounts(0),
    _combinedDataLength(0)
{
}

WaveAudioEssenceReader::~WaveAudioEssenceReader()
{
    finalise();
}

UInt64 WaveAudioEssenceReader::getDataLength()
{
	return _dataLength;
}

const WaveAudioEssenceReader::fmt_ck *WaveAudioEssenceReader::getFormatChunk()
{
	return _fmtChunk;
}

const Rational& WaveAudioEssenceReader::getEditRate() const
{
    //return _descriptor != 0 ? _descriptor->getSampleRate() : _editRate;
    return _editRate;
}
    
void WaveAudioEssenceReader::setEditRate(const Rational& rate)
{
    _editRate = rate;

    //WaveAudioEssenceDescriptor* waed =
    //    dynamic_cast<WaveAudioEssenceDescriptor*>(_descriptor);
    //if (waed == 0)
    //{
    //    /// \todo report error
    //    return;
    //}

    //waed->setSampleRate(rate);

    // A call to set the edit rate will also set the container duration.

    // Calculate frame and buffer sizes
    _baseSampleCount = 0; // reset so we can detect error
    if (rate == Rational(30000, 1001))
    {
        if (_fmtChunk->nSamplesPerSec == 48000)
        {
            _baseSampleCount = 1600;
            _additionalSampleCounts = 5;
            _additionalSampleCount[0] = 2;
            _additionalSampleCount[1] = 1;
            _additionalSampleCount[2] = 2;
            _additionalSampleCount[3] = 1;
            _additionalSampleCount[4] = 2;
        }
        /// \todo warn if audio data isn't 48kHz
    }
    else
    {
        _baseSampleCount = (UInt32)(
            _fmtChunk->nSamplesPerSec *
            (UInt64) rate.getDenominator() /
            (UInt64) rate.getNumerator());
        _additionalSampleCounts = 0;
    }

    _combinedDataLength = 0;
    if (_additionalSampleCounts > 0)
    {
        // Calculate the average buffer size
        for (UInt32 i = 0; i < _additionalSampleCounts; ++i)
        {
            _combinedDataLength +=
                (_baseSampleCount + _additionalSampleCount[i]) *
                (_fmtChunk->wBitsPerSample / 8) *
                _fmtChunk->nChannels;
        }
    }
    else
    {
        _combinedDataLength =
            _baseSampleCount *
            (_fmtChunk->wBitsPerSample / 8) *
            _fmtChunk->nChannels;
    }

    if (_baseSampleCount == 0 || _combinedDataLength == 0)
    {
        // We've failed to determine the size of an edit unit
        error(ESS_ERROR_FailedToDetermineFrameSize);
        return;
    }

    // Update the duration for the essence descriptor
    if (_additionalSampleCounts > 0)
    {
        Length duration = ((_dataLength * _additionalSampleCounts) / _combinedDataLength);

        // we have to do the rounding manually as ceil() takes a double
        // and VC6 can't handle unsigned int64 to double...
        Length rem = (_dataLength * _additionalSampleCounts) % _combinedDataLength;
        if (rem != 0)
            duration++; // round up
        
        //waed->setContainerDuration(duration);
		_containerDuration = duration;
    }
    else
    {
        Length duration = _dataLength / _combinedDataLength;

        // we have to do the rounding manually as ceil() takes a double
        // and VC6 can't handle unsigned int64 to double...
        Length rem = _dataLength % _combinedDataLength;
        if (rem != 0)
            duration++; // round up 

        //waed->setContainerDuration(duration);
		_containerDuration = duration;
    }
}

//Batch<UL>& WaveAudioEssenceReader::getEssenceContainers()
//{
//    return _essenceContainers;
//}
//
//const FileDescriptor* WaveAudioEssenceReader::getDescriptor() const
//{
//    if (_conversion)
//    {
//        return _et->getDescriptor();
//    }
//    else
//    {
//        return _descriptor;
//    }
//}

const UInt8* WaveAudioEssenceReader::getHeaderData(UInt32 track) const
{
#pragma unused(track)
    return 0;
}

const UInt32 WaveAudioEssenceReader::getHeaderDataLength(UInt32 track) const
{
#pragma unused(track)
    return 0;
}

UInt32 WaveAudioEssenceReader::getEditUnitDataLength(Position editUnit) const
{
    if (_additionalSampleCounts > 0)
    {
        UInt32 offset = (UInt32)(editUnit % _additionalSampleCounts);
        return
            (_baseSampleCount + _additionalSampleCount[offset]) *
            (_fmtChunk->wBitsPerSample / 8) *
            _fmtChunk->nChannels;
    }
    else
    {
        return
            _baseSampleCount *
            (_fmtChunk->wBitsPerSample / 8) *
            _fmtChunk->nChannels;
    }
}

UInt32 WaveAudioEssenceReader::calcStartEditUnitData(Position editUnit) const
{
    if (_additionalSampleCounts > 0)
    {
        UInt32 q = (UInt32)(editUnit / _additionalSampleCounts);
        UInt32 r = (UInt32)(editUnit % _additionalSampleCounts);

        // First, calculate how much data the combined
        Position qPos = q * _combinedDataLength;
        Position pos = qPos;
        for (UInt32 i = 0; i < r; ++i)
        {
            pos += getEditUnitDataLength(editUnit * _additionalSampleCounts + i);
        }

        return (UInt32) pos;
    }
    else
    {
        return (UInt32)(editUnit * getEditUnitDataLength(0));
    }
}

void WaveAudioEssenceReader::initialise(Reader* reader) throw(Exception)
{
    EssenceReader::initialise(reader);

    reader->setPosition(0, Reader::BEGIN);
    RIFFReader riffReader(reader);

    if (riffReader.descend() == RIFFChunk::fourCC('R', 'I', 'F', 'F'))
    {
        // Check that this is a WAVE file
        if (riffReader.getCurrentChunk()->getType() !=
            RIFFChunk::fourCC('W', 'A', 'V', 'E'))
        {
            error(RIFF_ERROR_NotWAVE);
        }

        UInt32 fourcc;
        while ((fourcc = riffReader.descend()) != 0)
        {
            if (fourcc == RIFFChunk::fourCC('f', 'm', 't', ' '))
            {
                // Get the 'fmt ' chunk
                if (_fmtChunk != 0)
                {
                    delete _fmtChunk;
                }

                _fmtChunk = new fmt_ck;
                riffReader.read(
                    (UInt8*) _fmtChunk,
                    sizeof(fmt_ck));
            }
            else if (fourcc == RIFFChunk::fourCC('b', 'e', 'x', 't'))
            {
                // Get the 'bext' chunk
                if (_bextBuffer != 0)
                {
                    delete _bextBuffer;
                }

                _bextBuffer = new UInt8[riffReader.getCurrentChunk()->getSize()];
                _bextChunk = reinterpret_cast<bext_ck*>(_bextBuffer);
                riffReader.read(
                    _bextBuffer,
                    riffReader.getCurrentChunk()->getSize());
            }
            else if (fourcc == RIFFChunk::fourCC('d', 'a', 't', 'a'))
            {
                // Note the position of the expected 'data' chunk
                _dataOffset = reader->getPosition();
                _dataLength = riffReader.getCurrentChunk()->getSize();
            }

            riffReader.ascend();
        }

        if (_fmtChunk == 0)
        {
            error(RIFF_ERROR_MissingFMTChunk);
            return;
        }
        else if (_dataOffset == 0)
        {
            error(RIFF_ERROR_FileIsEmpty);
            return;
        }
    }
    else
    {
        error(RIFF_ERROR_NotRIFF);
        return;
    }

    //UL* ulContainer = new UL(LABEL_EssenceContainerGC_AES_BWAV);
    //ulContainer->setByte(15, 0x01); // Essence Element Type - Wave Frame Wrapped

    //_essenceContainers.add(ulContainer);

    //_ulAudio = new UL(KEY_GCSoundEssenceElement);
    //_ulAudio->setByte(14, 0x01);    // Essence Element Count
    //_ulAudio->setByte(15, 0x01);    // Essence Element Type - Wave Frame Wrapped
    //_ulAudio->setByte(16, 0x01);    // Essence Element Number
    //getElementKeys().add(_ulAudio);

    //getTrackDataDefinitions().add(new UL(LABEL_SoundEssenceTrack));

    //// Configure the descriptor
    //WaveAudioEssenceDescriptor* waed = static_cast<WaveAudioEssenceDescriptor*>(
    //    getDictionary()->createInstance(KEY_WaveAudioEssenceDescriptor));
    //waed->setSampleRate(_editRate);
    //waed->setEssenceContainer(*ulContainer);
    //waed->setAudioSamplingRate(_fmtChunk->nSamplesPerSec);
    //waed->setChannelCount(_fmtChunk->nChannels);
    //waed->setQuantizationBits(_fmtChunk->wBitsPerSample);
    //waed->setBlockAlign(_fmtChunk->nBlockAlign);
    //waed->setAvgBps(_fmtChunk->nAvgBytesPerSec);
    //_descriptor = waed;

    setEditRate(_editRate);

    // Exit at this point if we failed
    if (_baseSampleCount == 0 || _combinedDataLength == 0)
    {
        return;
    }

    setTrackCount(1);

    // Position the reader at the start of the data
    reset();
}

void WaveAudioEssenceReader::finalise() throw()
{
    delete _fmtChunk;
    _fmtChunk = 0;
    delete _bextBuffer;
    _bextBuffer = 0;
}

void WaveAudioEssenceReader::reset() throw()
{
    if (getReader() == 0)
    {
        return;
    }

    _currentStartPosition = 0;
    _editUnit = 0;
    getReader()->setPosition(_dataOffset, Reader::BEGIN);
}

bool WaveAudioEssenceReader::nextEditUnit() throw()
{
    bool success = false;
    if (seekNextEditUnit())
    {
        success = deliverEditUnit(0, 0, *_ulAudio);
    }

    if (!success)
    {
        getContentHandler()->endOfStream(this, 0, *_ulAudio);
    }

    return success;
}

bool WaveAudioEssenceReader::seekNextEditUnit() throw()
{
    if (getReader() == 0)
    {
        return false;
    }

//    if (_editUnit < _descriptor->getContainerDuration() - 1)
    if (_editUnit < _containerDuration - 1)
    {
        if (_currentStartPosition > 0)
        {
            ++_editUnit;
        }

        _currentStartPosition = calcStartEditUnitData(_editUnit) + _dataOffset;
        return (Position) getReader()->setPosition(_currentStartPosition, Reader::BEGIN) ==
            _currentStartPosition;
    }

	return false;
}

bool WaveAudioEssenceReader::deliverEditUnit(
    UInt32 trackIndex,
    UInt32 trackDeliveryIndex,
    const UL& trackDeliveryKey) throw(Exception)
{
#pragma unused(trackIndex)

    if (getReader() == 0)
    {
        return false;
    }

    //if (_editUnit >= _descriptor->getContainerDuration())
    if (_editUnit >= _containerDuration)
    {
        return false;
    }

    if (_currentStartPosition != static_cast<Position>(getReader()->getPosition()))
    {
        getReader()->setPosition(_currentStartPosition, Reader::BEGIN);
        /// \todo check the resulting position
    }

    UInt64 length = getEditUnitDataLength(_editUnit);
    UInt64 finalLength = length;

    UInt8* buffer = getContentHandler()->allocateBuffer(
        this,
        trackDeliveryIndex,
        trackDeliveryKey,
        (UInt32) finalLength);

    // How much data can we expect to read?
    UInt64 dataRemaining =
        _dataLength - (getReader()->getPosition() - _dataOffset);
    UInt64 toRead = std::min(length, dataRemaining);

    UInt64 position = getReader()->getPosition();
    UInt64 read = getReader()->read(buffer, toRead);

    //// zero out all the left-over bytes (as we are padding the end of an editUnit)
    //memset(
    //    buffer + static_cast<UInt32>(read),
    //    0,
    //    static_cast<UInt32>(length - read));

    bool success = false;
    if (read > 0)
    {
        success = getContentHandler()->deliver(
            this,
            trackDeliveryIndex,
            trackDeliveryKey,
            buffer,
            (UInt32) finalLength,
            position,
            DEFAULT_DELIVERY,
//            IndexEntry::RANDOM_ACCESS,
            0,
            (UInt32) _editUnit,
            getEditRate());

        if ((dataRemaining < length) || (read < length))
        {
            warning(ESS_WARNING_IncompleteEditUnit);
            // success = false; not false as have padded out with 0s
        }
    }

    getContentHandler()->freeBuffer(
        this,
        trackDeliveryIndex,
        trackDeliveryKey,
        buffer);

    return success;
}

bool WaveAudioEssenceReader::isReadable(Reader* reader) throw()
{
    UInt8 buffer[12];

    reader->setPosition(0, Reader::BEGIN);
    reader->read(buffer, 12);

    if (*((UInt32*)buffer) == RIFFChunk::fourCC('R', 'I', 'F', 'F')
        && *((UInt32*)(buffer + 8)) == RIFFChunk::fourCC('W', 'A', 'V', 'E'))
    {
        return true;
    }

    return false;
}

UInt32 WaveAudioEssenceReader::getConstantEditUnitSize(UInt32 trackIndex) const
{
    return 0;
}

UInt32 WaveAudioEssenceReader::getMaxEditUnitSize(UInt32 trackIndex) const
{
    return 0;
}

} //namespace CMI
