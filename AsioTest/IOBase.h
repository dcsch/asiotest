#ifndef __CMI_IO_H__
#define __CMI_IO_H__

namespace CMI
{

/**
 * An abstract I/O object class.
 * \author David Schweinsberg
 */
class IOBase
{
public:
    /**
     * Points from which a setPosition can calculate an absolute position.
     */
    enum Origin
    {
        BEGIN,
        CURRENT,
        END
    };

    /**
     * Returns the name of the material being accessed.
     */
    virtual const std::wstring& getName() const = 0;

    /**
     * Returns the absolute position of the I/O object within a stream.
     */
    virtual UInt64 getPosition() const = 0;

    /**
     * Set the position of the I/O object within a stream.
     * \param distance The distance in bytes to move relative to the
     * Origin parameter.
     * \param origin An enumeration specifying the relative position to use
     * when calculating the absolute position to move to.
     * \return The absolute position moved to within the stream.
     */
    virtual UInt64 setPosition(Int64 distance, Origin origin) = 0;

    /**
     * Returns the length of the stream.
     */
    virtual UInt64 getLength() const = 0;
    
    virtual ~IOBase() { }
};

} //namespace CMI
#endif //__CMI_IO_H__
