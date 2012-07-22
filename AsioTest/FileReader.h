#ifndef CMI_FILE_READER_H__
#define CMI_FILE_READER_H__ 1

#include "common.h"
#include "Reader.h"
#include "Exception.h"

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif //_WIN32

namespace CMI
{

class Exception;

/// A concrete implementation of the Reader interface for reading files.
/// \author David Schweinsberg
class FileReader : public Reader
{
public:

    /// Constructor.
    FileReader();

    /// Destructor
    virtual ~FileReader();

    /// Opens a file for reading.
    /// \param filename The path name of the file to open.
    void open(const std::wstring& filename) throw (Exception);

    /// Closes an open file.
    void close() throw();


    /// \return the name of the material being read.
    virtual const std::wstring& getName() const { return _filename; }

    /// \return the absolute position of the reader within a stream.
    virtual UInt64 getPosition() const { return _position; }

    /// Set the position of the reader within a stream.
    /// \param distance The distance in bytes to move relative to the
    /// Origin parameter.
    /// \param origin An enumeration specifying the relative position to use
    /// when calculating the absolute position to move to.
    /// \return The absolute position moved to within the stream.
    virtual UInt64 setPosition(Int64 distance, Origin origin);

    /// \return the length of the stream.
    virtual UInt64 getLength() const;
    
    /// Reads from the stream.
    /// \param buffer A pointer to a buffer to read data into.
    /// \param length The amount of data to read.
    /// \return The actual number of bytes read.
    virtual UInt64 read(UInt8* buffer, UInt64 length);

private:
    std::wstring _filename;
#ifdef _WIN32
    HANDLE _file;
#else
    FILE* _file;
#endif //_WIN32
    UInt64 _position;

#ifdef _DEBUG
    HANDLE _hFileLog;
#endif
    
    FileReader(const FileReader&);
    
    void operator=(const FileReader&);
};

} //namespace CMI
#endif //CMI_FILE_READER_H__
