#ifndef CMI_RIFF_READER_H__
#define CMI_RIFF_READER_H__ 1

#include <stack>

namespace CMI
{

class Reader;
class RIFFChunk;

/**
 * A RIFF file reader, to support AVI and WAV file reading.
 * \author David Schweinsberg
 */
class RIFFReader
{
public:
    enum ErrorCode
    {
        ERROR_STREAM_UNDERFLOW,
        ERROR_PACKET_SYNCHRONIZATION,
        ERROR_INCORRECT_VERSION,
        ERROR_OUT_OF_MEMORY
    };

    RIFFReader(Reader* file);

    ~RIFFReader();

    RIFFChunk* getCurrentChunk() { return _chunkStack.top(); }

    UInt32 descend();
    void ascend();
    void read(UInt8* buffer, UInt32 length);

private:
    Reader* _file;
    std::stack<RIFFChunk*> _chunkStack;
    
    RIFFReader(const RIFFReader&);
    
    void operator=(const RIFFReader&);
};

} //namespace CMI
#endif //CMI_RIFF_READER_H__
