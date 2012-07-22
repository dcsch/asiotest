#ifndef CMI_ESSENCE_CONTENT_HANDLER_H__
#define CMI_ESSENCE_CONTENT_HANDLER_H__ 1

namespace CMI
{

class UL;
class Rational;
class EssenceReader;

/**
 * Delivery flags.
 * \author David Schweinsberg
 */
enum DeliveryFlag
{
    DEFAULT_DELIVERY = 0,
    KLV_DELIVERY = 1,
    REORDERED_DELIVERY = 2,
    GROUPED_DELIVERY = 4
};

/**
 * Abstract content handler interface.  Implement this abstract class and
 * register the concrete implementation with an instance of EssenceReader.
 * \author David Schweinsberg
 */
class EssenceContentHandler
{
public:

    /**
     * Called when the reader is requesting a buffer.
     * \param reader The EssenceReader that is making this call.
     * \param trackIndex The (zero-based) index of the track for the essence
     * element.
     * \param ul The label for the essence element.
     * \param length The requested size of the buffer.
     * \return A pointer to the allocated buffer, or zero if the allocation
     * failed.
     */
    virtual UInt8* allocateBuffer(
        const EssenceReader* reader,
        UInt32 trackIndex,
        const UL& ul,
        UInt32 length) throw() = 0;

    /**
     * Called when the reader is releasing a buffer.
     * \param reader The EssenceReader that is making this call.
     * \param trackIndex The (zero-based) index of the track for the essence
     * element.
     * \param ul The label for the essence element.
     * \param buffer The pointer to the buffer to be freed.
     */
    virtual void freeBuffer(
        const EssenceReader* reader,
        UInt32 trackIndex,
        const UL& ul,
        UInt8* buffer) throw() = 0;

    /**
     * Called when an essence element is to be delivered from the essence
     * stream.
     * \param reader The EssenceReader that is making this call.
     * \param trackIndex The (zero-based) index of the track for the essence
     * element.
     * \param ul The label for the essence element.
     * \param buffer The buffer containing the essence.
     * \param dataLength The actual length of the essence data within the
     * buffer.
     * \param position The position of the element within the essence stream.
     * \param deliveryFlags Flags concerning this data delivery.
     * \param editUnitFlags Flags for this Edit Unit.  See IndexEntry::EditUnitFlag
     * \param editUnit The number of the field / frame / etc. within
     * the stream.
     * \param editRate The duration of the edit unit.
     * \return True if the delivery was successful.
     */
    virtual bool deliver(
        const EssenceReader* reader,
        UInt32 trackIndex,
        const UL& ul,
        UInt8* buffer,
        UInt32 dataLength,
        UInt64 position,
        UInt32 deliveryFlags,
        UInt8 editUnitFlags,
        UInt32 editUnit,
        const Rational& editRate) throw() = 0;

    /**
     * Called when no more essence elements are to be delivered.
     * \param reader The EssenceReader that is making this call.
     * \param trackIndex The (zero-based) index of the track for the essence
     * element.
     * \param ul The label for the essence element.
     */
    virtual void endOfStream(
        const EssenceReader* reader,
        UInt32 trackIndex,
        const UL& ul) throw() = 0;
    
    virtual ~EssenceContentHandler() { }
};

} //namespace CMI
#endif //CMI_ESSENCE_CONTENT_HANDLER_H__
