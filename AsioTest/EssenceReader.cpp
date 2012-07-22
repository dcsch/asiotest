#include "common.h"
#include "EssenceReader.h"
#include "WaveAudioEssenceReader.h"
#include "EssenceContentHandler.h"
#include "EssenceErrorHandler.h"
//#include "TimecodeSource.h"
#include "Reader.h"

namespace CMI
{

EssenceReader::EssenceReader(
    UInt32 id,
    EssenceContentHandler* contentHandler,
    EssenceErrorHandler* errorHandler) :
    _id(id),
    _reader(0),
    _contentHandler(contentHandler),
    _errorHandler(errorHandler),
    _trackCount(0)
{
}

EssenceReader::~EssenceReader()
{
}

EssenceReader::EssenceReader(const EssenceReader& other) :
    _id(other._id),
    _reader(other._reader),
    _contentHandler(other._contentHandler),
    _errorHandler(other._errorHandler),
    _trackCount(other._trackCount)
{
}

EssenceReader& EssenceReader::operator=(const EssenceReader& rhs)
{
#pragma unused(rhs)
    return *this;
}

void EssenceReader::initialise(
    Reader* reader) throw(Exception)
{
    _reader = reader;
}
//
//bool EssenceReader::conversion(const UL& containerLabel) throw()
//{
//#pragma unused(containerLabel)
//    return false;
//}

void EssenceReader::warning(UInt32 code, Position position) const
{
    if (_errorHandler != 0)
    {
        _errorHandler->warning(this, code, position);
    }
}

void EssenceReader::warning(UInt32 code) const
{
    warning(code, _reader != 0 ? _reader->getPosition() : 0);
}

void EssenceReader::error(UInt32 code, Position position) const
{
    if (_errorHandler != 0)
    {
        _errorHandler->error(this, code, position);
    }
}

void EssenceReader::error(UInt32 code) const
{
    error(code, _reader != 0 ? _reader->getPosition() : 0);
}

bool EssenceReader::isReadable(Reader* reader) throw()
{
#pragma unused(reader)
    return false;
}

EssenceReader* EssenceReader::createEssenceReader(
    UInt32 id,
    EssenceContentHandler* contentHandler,
    EssenceErrorHandler* errorHandler,
    Reader* reader,
    const Rational* editRate) throw()
{
    // The essence readers should been arranged so that the lightest-weight
    // checks occur first

    EssenceReader* er = 0;

    if (WaveAudioEssenceReader::isReadable(reader))
    {
        //er = new WaveAudioEssenceReader(
        //    id,
        //    contentHandler,
        //    errorHandler,
        //    reader,
        //    dict,
        //    editRate);
    }

    if (er != 0 && er->getTrackCount() == 0)
    {
        // If the track count is set to zero, then the essence must be bad.
        // Delete the essence reader and return a null.
        delete er;
        er = 0;
    }

    return er;
}

} //namespace CMI
