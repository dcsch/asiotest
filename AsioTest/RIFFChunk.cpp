#include "common.h"
#include "RIFFChunk.h"
#include "Reader.h"

namespace CMI
{

RIFFChunk::RIFFChunk() :
    _id(0),
    _size(0),
    _type(0),
    _isList(false),
    _position(0)
{
}

bool RIFFChunk::readHead(Reader* file)
{
    // Make a note of where we are in the file, so that when we want to
    // ascend out of the chunk, we can do so successfully
    _position = file->getPosition();

    UInt32 buf[2];
    if (file->read(reinterpret_cast<UInt8*>(buf), 8) == false)
    {
        return false;
    }
    _id = buf[0];
    _size = buf[1];

    if ((_id == fourCC('R', 'I', 'F', 'F') ||
         (_id == fourCC('L', 'I', 'S', 'T'))))
    {
        _isList = true;
        file->read(reinterpret_cast<UInt8*>(&_type), 4);
    }
    return true;
}

} //namespace CMI
