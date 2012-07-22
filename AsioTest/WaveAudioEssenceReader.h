#ifndef CMI_WAVE_AUDIO_ESSENCE_READER_H__
#define CMI_WAVE_AUDIO_ESSENCE_READER_H__ 1

#include "EssenceReader.h"
#include "EditUnitSize.h"
#include "UL.h"

namespace CMI
{

//class EssenceTransformation;
//class GenericSoundEssenceDescriptor;

/// A concrete implementation of EssenceReader to handle WAVE (including
/// Broadcast WAVE) audio files.
/// \author David Schweinsberg
class WaveAudioEssenceReader :
    public EssenceReader,
    public EditUnitSize
{
public:

	typedef struct
	{
		UInt16 wFormatTag;
		UInt16 nChannels;
		UInt32 nSamplesPerSec;
		UInt32 nAvgBytesPerSec;
		UInt16 nBlockAlign;
		UInt16 wBitsPerSample;
	} fmt_ck;

    /// Constructor.
    WaveAudioEssenceReader(
        UInt32 id,
        EssenceContentHandler* contentHandler,
        EssenceErrorHandler* errorHandler,
        const Rational* editRate);

    /// Destructor.
    virtual ~WaveAudioEssenceReader();

    /// Returns the edit rate of the essence.
    virtual const Rational& getEditRate() const;
    
    /// Sets the edit rate of the essence.
    virtual void setEditRate(const Rational& rate);

    ///// Returns a Batch of Essence Container ULs corresponding to the contained
    ///// essences.
    //virtual Batch<UL>& getEssenceContainers();

    ///// Returns an Array of references to Descriptors, which describe the
    ///// contained essences.
    //virtual const FileDescriptor* getDescriptor() const;

    /// Returns a pointer to a buffer of the raw header data, if any.
    virtual const UInt8* getHeaderData(UInt32 track) const;

    /// Returns the length of the raw header data buffer.
    virtual const UInt32 getHeaderDataLength(UInt32 track) const;

    /// Initialiser
    /// \param reader The Reader from which the raw data will be read.
    /// \param descriptor Unused, set to 0.
    virtual void initialise(
        Reader* reader) throw(Exception);

    /// Finaliser
    virtual void finalise() throw();

    /// Reset the position of the reader to the start of the essence.
    virtual void reset() throw();

    /// Prompt the EssenceReader to handle the next edit unit.  This will cause
    /// the corresponding content handler methods to be called, for each essence
    /// element present in the edit unit.
    /// \return True if the next edit unit was processed, false if it wasn't
    virtual bool nextEditUnit() throw();

    /// Move the position of the essence reader to the next edit unit.
    /// \return True if the next edit unit was found.
    virtual bool seekNextEditUnit() throw();

    /// Deliver the specified track's edit unit to the EssenceContentHandler.
    /// Multiple calls to deliver the same track's edit unit is possible, but
    /// this may degrade performance.
    virtual bool deliverEditUnit(
        UInt32 trackIndex,
        UInt32 trackDeliveryIndex,
        const UL& trackDeliveryKey) throw(Exception);

    /// True if the essence is WAVE
    static bool isReadable(Reader* reader) throw();

    // EditUnitSize

    /// Returns a value greater than zero if edit units are of constant size.
    virtual UInt32 getConstantEditUnitSize(UInt32 trackIndex) const;
    
    /// Returns a value greater than zero if it is known what the maximum edit
    /// unit size is.
    virtual UInt32 getMaxEditUnitSize(UInt32 trackIndex) const;

	UInt64 getDataLength();

	const fmt_ck *getFormatChunk();

private:

	typedef struct
	{
		char Description[256];
		char Originator[32];
		char OriginatorReference[32];
		char OriginationDate[10];
		char OriginationTime[8];
		UInt32 TimeReferenceLow;
		UInt32 TimeReferenceHigh;
		UInt16 Version;
		UInt8 UMID[64];
		UInt8 Reserved[190];
		char CodingHistory[1];
	} bext_ck;

	//Batch<UL> _essenceContainers;
	//GenericSoundEssenceDescriptor* _descriptor;
	Length _containerDuration;

	UL* _ulAudio;
	fmt_ck* _fmtChunk;
	bext_ck* _bextChunk;
	UInt8* _bextBuffer;
	UInt64 _dataOffset;
	UInt64 _dataLength;
	Position _editUnit;
	Position _currentStartPosition;
	Rational _editRate;
	bool _conversion;

	UInt32 _baseSampleCount;
	UInt32 _additionalSampleCount[5];
	UInt32 _additionalSampleCounts;
	UInt32 _combinedDataLength;

//    EssenceTransformation* _et;

	UInt32 getEditUnitDataLength(Position editUnit) const;
	UInt32 calcStartEditUnitData(Position editUnit) const;

    WaveAudioEssenceReader(const WaveAudioEssenceReader&);
    
    void operator=(const WaveAudioEssenceReader&);
};

} //namespace CMI
#endif //CMI_WAVE_AUDIO_ESSENCE_READER_H__
