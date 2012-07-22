#ifndef CMI_RIFF_CHUNK_H__
#define CMI_RIFF_CHUNK_H__ 1

namespace CMI
{

class Reader;

/**
 * \author David Schweinsberg
 */
class RIFFChunk
{
public:
    RIFFChunk();

    UInt32 getId() { return _id; }
    UInt32 getSize() { return _size; }
    UInt32 getType() { return _type; }
    bool isList() { return _isList; }
    UInt64 getPosition() { return _position; }

    bool readHead(Reader* file);

    static UInt32 fourCC(UInt8 c1, UInt8 c2, UInt8 c3, UInt8 c4)
    {
        return c4 << 24 | c3 << 16 | c2 << 8 | c1;
    }

private:
    UInt32 _id;
    UInt32 _size;
    UInt32 _type;
    bool _isList;
    UInt64 _position;
};

} //namespace CMI
#endif //CMI_RIFF_CHUNK_H__
