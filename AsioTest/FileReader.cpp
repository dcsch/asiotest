#include "FileReader.h"
#include "errorcodes.h"

#ifdef __GNUC__
#include <sys/types.h>
#include <sys/stat.h>
#endif //__GNUC__

namespace CMI
{

FileReader::FileReader() :
    _filename(),
    _file(0),
    _position(0)
{
#ifdef _DEBUG
    _hFileLog = 0;
#endif
}

FileReader::~FileReader()
{
    close();
}

void FileReader::open(const std::wstring& filename) throw (Exception)
{
    _filename = filename;
#ifdef _WIN32
    _file = CreateFileW(
        _filename.c_str(),
        GENERIC_READ,
        FILE_SHARE_READ,
        NULL,
        OPEN_EXISTING,
        0,
        NULL);
    if (_file == INVALID_HANDLE_VALUE)
    {
        _file = 0;
        throw Exception(ERROR_FileNotFound, _filename);
    }
#else
    char buf[255];
    wcstombs(buf, _filename.c_str(), 255);
    _file = fopen(buf, "rb");
    if (_file == 0)
    {
        throw Exception(ERROR_FileNotFound, _filename);
    }
#endif //_WIN32

#ifdef _DEBUG
    //_hFileLog = CreateFileW(
    //    (_filename + L".log").c_str(),
    //    GENERIC_WRITE,
    //    FILE_SHARE_WRITE,
    //    NULL,
    //    CREATE_ALWAYS,
    //    0,
    //    NULL);
    //if (_hFileLog == INVALID_HANDLE_VALUE)
    //{
    //    _hFileLog = 0;
    //}
#endif
}

void FileReader::close() throw()
{
#ifdef _WIN32
    if (_file != 0)
    {
        CloseHandle(_file);
    }
    _file = 0;
#else
    if (_file != 0)
    {
        fclose(_file);
    }
    _file = 0;
#endif //_WIN32

#ifdef _DEBUG
    if (_hFileLog != 0)
    {
        CloseHandle(_hFileLog);
        _hFileLog = 0;
    }
#endif
}

UInt64 FileReader::setPosition(Int64 distance, Origin origin)
{
#ifdef _DEBUG
    //if (_hFileLog != 0)
    //{
    //    CHAR* originStr[] = {
    //        "BEGIN  ",
    //        "CURRENT",
    //        "END    "
    //    };
    //    CHAR buf[128];

    //    DWORD delta = 0;
    //    if (origin == BEGIN)
    //    {
    //        delta = (DWORD)(distance - _position);
    //    }
    //    else if (origin == CURRENT)
    //    {
    //        delta = (DWORD) distance;
    //    }
    //    else if (origin == END)
    //    {
    //        LARGE_INTEGER liSize;
    //        liSize.LowPart = GetFileSize(_file, (DWORD*) &liSize.HighPart);
    //        delta = (DWORD)((liSize.LowPart - distance) - _position);
    //    }

    //    int len = wsprintfA(
    //        buf,
    //        "Position: %d, Delta: %d, (Origin: %s, Distance: %d)\r\n",
    //        (DWORD) _position + delta,
    //        delta,
    //        originStr[(int) origin],
    //        distance);
    //    
    //    DWORD written;
    //    WriteFile(_hFileLog, buf, len, &written, NULL);
    //}
#endif

#ifdef _WIN32
    if (_file == 0)
    {
        return 0;
    }

    // Check if we can get out of this work
    if (origin == BEGIN && _position == (UInt64) distance)
    {
        return _position;
    }

    DWORD moveMethod;
    switch (origin)
    {
    default:
    case BEGIN:
        moveMethod = FILE_BEGIN;
        break;
    case CURRENT:
        moveMethod = FILE_CURRENT;
        break;
    case END:
        moveMethod = FILE_END;
        break;
    }
    LARGE_INTEGER liDistance;
    LARGE_INTEGER liPosition;
    liDistance.QuadPart = distance;

    //SetFilePointerEx(_hFile, liDistance, &liPosition, moveMethod);
    
    liPosition.LowPart = SetFilePointer(
        _file,
        liDistance.LowPart,
        &liDistance.HighPart,
        moveMethod);
    liPosition.HighPart = liDistance.HighPart;
    
    _position = liPosition.QuadPart;
#else
    if (_file == 0)
    {
        return 0;
    }
    
    // Check if we can get out of this work
    if (origin == BEGIN && _position == (UInt64) distance)
    {
        return _position;
    }
    
    int whence;
    switch (origin)
    {
        default:
        case BEGIN:
            whence = SEEK_SET;
            break;
        case CURRENT:
            whence = SEEK_CUR;
            break;
        case END:
            whence = SEEK_END;
            break;
    }

    fseeko(_file, distance, whence);
    
    _position = ftello(_file);
#endif //_WIN32

    return _position;
}

UInt64 FileReader::getLength() const
{
#ifdef _WIN32
    if (_file == 0)
    {
        return 0;
    }

    LARGE_INTEGER liSize;
    //GetFileSizeEx(_hFile, &liSize);
    liSize.LowPart = GetFileSize(_file, (DWORD*) &liSize.HighPart);
    return liSize.QuadPart;
#else
    if (_file == 0)
    {
        return 0;
    }
    
    struct stat sb;
    fstat(fileno(_file), &sb);
    return sb.st_size;
#endif //_WIN32
}

UInt64 FileReader::read(UInt8* buffer, UInt64 length)
{
#ifdef _WIN32
    if (_file == 0)
    {
        return 0;
    }

    DWORD bytesRead;
    ReadFile(_file, buffer, (DWORD) length, &bytesRead, NULL);
#else
    if (_file == 0)
    {
        return 0;
    }

    size_t bytesRead = fread(buffer, 1, length, _file);
#endif //_WIN32

    _position += bytesRead;
    return bytesRead;
}

} //namespace CMI
