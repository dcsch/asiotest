#ifndef CMI_ESSENCE_ERROR_HANDLER_H__
#define CMI_ESSENCE_ERROR_HANDLER_H__ 1

namespace CMI
{

class EssenceReader;

/**
 * Abstract content error interface.  Implement this abstract class and
 * register the concrete implementation with an instance of EssenceReader.
 * \author David Schweinsberg
 */
class EssenceErrorHandler
{
public:

    /**
     * Called when the reader encounters a recoverable problem with the stream.
     * \param reader The EssenceReader that is making this call.
     * \param code The warning code.
     * \param position The position within the file the error occured.
     */
    virtual void warning(const EssenceReader* reader, UInt32 code, Position position) throw() = 0;

    /**
     * Called when the reader encounters a non-recoverable error with the stream.
     * \param reader The EssenceReader that is making this call.
     * \param code The error code.
     * \param position The position within the file the error occured.
     */
    virtual void error(const EssenceReader* reader, UInt32 code, Position position) throw() = 0;
    
    virtual ~EssenceErrorHandler() { }
};

} //namespace CMI
#endif //CMI_ESSENCE_ERROR_HANDLER_H__
