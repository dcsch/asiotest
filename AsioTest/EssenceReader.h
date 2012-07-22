#ifndef CMI_ESSENCE_READER_H__
#define CMI_ESSENCE_READER_H__ 1

#include "common.h"
#include "UL.h"
//#include <mxf/StrongRef.h>
//#include <Batch.h>
#include "Rational.h"
#include "Timecode.h"
#include "Exception.h"

namespace CMI
{

class Reader;
class FileDescriptor;
class Dictionary;
class EssenceContentHandler;
class EssenceErrorHandler;

/// EssenceReader (via specific concrete implementations) will parse essence in
/// a variety of formats.
/// \author David Schweinsberg
class EssenceReader
{
public:

    /// Constructor.
    /// \param id A 32-bit ID to identify the reader.
    /// \param contentHandler The EssenceContentHandler-derived object that
    /// will be called by the EssenceReader.
    /// \param errorHandler The EssenceErrorHandler-derived object that will
    /// be called when errors or warning occur.  This can be null.
    /// \param dict A Dictionary object, so FileDescriptors can be created by
    /// the EssenceReader.
    EssenceReader(
        UInt32 id,
        EssenceContentHandler* contentHandler,
        EssenceErrorHandler* errorHandler);

    /// Destructor.
    virtual ~EssenceReader();

    /// Returns the assigned ID of the reader.  Note that with the
    /// GCEssenceReader subclass, this corresponds to the BodySID.
    UInt32 getID() const;

    /// Returns the assigned Reader.
    const Reader* getReader() const;

    /// Returns the number of tracks within the essence.
    UInt32 getTrackCount() const;
    
    ///// Returns an array of Element keys, one corresponding
    ///// to each track.  The Element key is used within a Generic Essence
    ///// Container as the KLV key for each interleaved edit unit.
    //const Array<UL>& getElementKeys() const;

    ///// Returns an array of Element keys, one corresponding
    ///// to each track.  The Element key is used within a Generic Essence
    ///// Container as the KLV key for each interleaved edit unit.
    //Array<UL>& getElementKeys();

    ///// Returns an array of Data Definition labels, one corresponding
    ///// to each track.
    //const Array<UL>& getTrackDataDefinitions() const;
    //
    ///// Returns an array of Data Definition labels, one corresponding
    ///// to each track.
    //Array<UL>& getTrackDataDefinitions();
    //
    ///// Returns the edit rate of the essence.
    //virtual const Rational& getEditRate() const = 0;
    //
    ///// Sets the edit rate of the essence.
    //virtual void setEditRate(const Rational& rate) = 0;

    ///// Returns a Batch of Essence Container ULs corresponding to the contained
    ///// essences.  Essence Container labels specify the method of wrapping the
    ///// essence, and generally correspond to a published standard, such as the
    ///// MXF Generic Container and its specific essence flavours.
    //virtual Batch<UL>& getEssenceContainers() = 0;

    ///// Returns a FileDescriptor that describe the essence read by the reader.
    ///// If the reader handles multiple tracks, then the returned object will
    ///// be a MultipleDescriptor with the various tracks referenced within.
    //virtual const FileDescriptor* getDescriptor() const = 0;

    ///// Returns a pointer to a buffer of the raw header data, if any.
    //virtual const UInt8* getHeaderData(UInt32 track) const = 0;

    ///// Returns the length of the raw header data buffer.
    //virtual const UInt32 getHeaderDataLength(UInt32 track) const = 0;

    /// Initialiser
    /// \param reader The Reader from which the raw data will be read.
    /// \param descriptor The FD for the essence we are extracting.
    virtual void initialise(
        Reader* reader) throw(Exception);

    /// Finaliser
    virtual void finalise() throw() = 0;

    ///// Configure the type of essence conversion to be performed.
    //virtual bool conversion(const UL& containerLabel) throw();

    /// Reset the position of the reader to the start of the essence.
    virtual void reset() throw() = 0;

    /// Prompt the EssenceReader to handle the next edit unit.  This will cause
    /// the corresponding content handler methods to be called, for each essence
    /// element present in the edit unit.
    /// \return True if the next edit unit was processed, false if it wasn't
    virtual bool nextEditUnit() throw() = 0;

    /// Move the position of the essence reader to the next edit unit.
    /// \return True if the next edit unit was found.
    virtual bool seekNextEditUnit() throw() = 0;

    /// Deliver the specified track's edit unit to the EssenceContentHandler.
    /// Multiple calls to deliver the same track's edit unit is possible, but
    /// this may degrade performance.
    /// \param trackIndex The index of the track to deliver.
    /// \param trackDeliveryIndex The index to identify the track with in the
    /// call to EssenceContentHandler::deliver().
    /// \param trackDeliveryKey The UL key to identify the track with in the
    /// call to EssenceContentHandler::deliver().
    /// \return True if the delivery was successful.
    virtual bool deliverEditUnit(
        UInt32 trackIndex,
        UInt32 trackDeliveryIndex,
        const UL& trackDeliveryKey) throw(Exception) = 0;

    /// True if the essence can be read by the EssenceReader.
    static bool isReadable(Reader* reader) throw();

    /// A factory method to detect the type of essence and create a
    /// corresponding EssenceReader.
    static EssenceReader* createEssenceReader(
        UInt32 id,
        EssenceContentHandler* contentHandler,
        EssenceErrorHandler* errorHandler,
        Reader* reader,
        const Rational* editRate = 0) throw();

protected:

    /// Returns the assigned Reader (a non-const version).
    Reader* getReader();

    /// Returns the current content handler.
    EssenceContentHandler* getContentHandler() const;

    /// Returns the current error handler.
    EssenceErrorHandler* getErrorHandler() const;

//    const Dictionary* getDictionary() const;
    
    void setTrackCount(UInt32 count);

    void warning(UInt32 code, Position position) const;

    void warning(UInt32 code) const;

    void error(UInt32 code, Position position) const;

    void error(UInt32 code) const;

private:
    
    UInt32 _id;
    Reader* _reader;
//    const Dictionary* _dictionary;
    EssenceContentHandler* _contentHandler;
    EssenceErrorHandler* _errorHandler;
    UInt32 _trackCount;
    //Array<UL> _elementKeys;
    //Array<UL> _dataDefinitions;
    
    EssenceReader(const EssenceReader& other);
    
    EssenceReader& operator=(const EssenceReader& rhs);
};

inline UInt32 EssenceReader::getID() const
{
    return _id;
}

inline const Reader* EssenceReader::getReader() const
{
    return _reader;
}

inline UInt32 EssenceReader::getTrackCount() const
{
    return _trackCount;
}

//inline const Array<UL>& EssenceReader::getElementKeys() const
//{
//    return _elementKeys;
//}
//
//inline Array<UL>& EssenceReader::getElementKeys()
//{
//    return _elementKeys;
//}
//
//inline const Array<UL>& EssenceReader::getTrackDataDefinitions() const
//{
//    return _dataDefinitions;
//}
//
//inline Array<UL>& EssenceReader::getTrackDataDefinitions()
//{
//    return _dataDefinitions;
//}

inline Reader* EssenceReader::getReader()
{
    return _reader;
}

inline EssenceContentHandler* EssenceReader::getContentHandler() const
{
    return _contentHandler;
}

inline EssenceErrorHandler* EssenceReader::getErrorHandler() const
{
    return _errorHandler;
}

//inline const Dictionary* EssenceReader::getDictionary() const
//{
//    return _dictionary;
//}

inline void EssenceReader::setTrackCount(UInt32 count)
{
    _trackCount = count;
}

} //namespace CMI

#endif //CMI_ESSENCE_READER_H__
