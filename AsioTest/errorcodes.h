#ifndef CMI_ERROR_CODES_H__
#define CMI_ERROR_CODES_H__ 1

namespace CMI
{

// General Error Codes (0)
const UInt32 ERROR_InvalidParameter				= 0xc0000000;
const UInt32 ERROR_FileNotFound					= 0xc0000001;
const UInt32 ERROR_FileExists					= 0xc0000002;
const UInt32 ERROR_CannotCreateFile				= 0xc0000003;
const UInt32 ERROR_OutOfMemory					= 0xc0000004;
const UInt32 ERROR_IndexOutOfRange				= 0xc0000005;
const UInt32 ERROR_LengthOutOfRange				= 0xc0000006;
const UInt32 ERROR_UnexpectedStreamEnd			= 0xc0000007;
const UInt32 ERROR_NotImplemented				= 0xc0000008;
const UInt32 ERROR_CannotCreateDirectory		= 0xc0000009;

// Essence Error Codes (1)
const UInt32 ESS_WARNING_IncompleteTrailingEssence			= 0x80010000;
const UInt32 ESS_WARNING_UnexpectedSampleCountInPacket		= 0x80010001;
const UInt32 ESS_WARNING_IncompleteEditUnit					= 0x80010002;
const UInt32 ESS_WARNING_EssenceTruncated					= 0x80010003;
const UInt32 ESS_WARNING_MultipleEssenceDurationMismatch	= 0x80010004;
const UInt32 ESS_WARNING_ExternalEssenceUnsupported			= 0x80010005;
const UInt32 ESS_ERROR_FailedToCreateEssenceReader			= 0xc0010000;
const UInt32 ESS_ERROR_FailedToCreateEssenceWriter			= 0xc0010001;
const UInt32 ESS_ERROR_NoEssenceFound						= 0xc0010002;
const UInt32 ESS_ERROR_UnsupportedEssenceType				= 0xc0010003;
const UInt32 ESS_ERROR_FailedToDetermineFrameSize			= 0xc0010004;
const UInt32 ESS_ERROR_PacketSampleCountOverflow			= 0xc0010005;
const UInt32 ESS_ERROR_BufferOverflow						= 0xc0010006;
const UInt32 ESS_ERROR_UnsupportedEditRateChange			= 0xc0010007;

// Metadata Error Codes (2)
const UInt32 META_ERROR_CannotReadMetadata	= 0x80020000;
const UInt32 META_ERROR_BadReferenceChain	= 0x80020001;
const UInt32 META_ERROR_InvalidMaterialPackageMapping	= 0x80020002;

// MXF File Structure Error Codes (3)
const UInt32 MXF_WARNING_ThisPartitionBadValue	  = 0x80030000;
const UInt32 MXF_WARNING_IndexSIDBadValue		  = 0x80030001;
const UInt32 MXF_WARNING_IndexByteCountBadValue	  = 0x80030002;
const UInt32 MXF_WARNING_IndexTableSegmentInvalid = 0x80030003;
const UInt32 MXF_WARNING_HeaderByteCountBadValue  = 0x80030004;

// RIFF File Structure Error Codes (4)
const UInt32 RIFF_ERROR_NotInitialized				= 0xc0040000;
const UInt32 RIFF_ERROR_NotRIFF						= 0xc0040001;
const UInt32 RIFF_ERROR_NotWAVE						= 0xc0040002;
const UInt32 RIFF_ERROR_MissingFMTChunk				= 0xc0040003;
const UInt32 RIFF_ERROR_FileIsEmpty					= 0xc0040004;
const UInt32 RIFF_ERROR_NotAVI						= 0xc0040005;
const UInt32 RIFF_ERROR_MissingHeaderList			= 0xc0040006;
const UInt32 RIFF_ERROR_MissingMainHeader			= 0xc0040007;
const UInt32 RIFF_ERROR_MissingOpenDMLHeader		= 0xc0040008;
const UInt32 RIFF_ERROR_UnexpectedChunkInHeader		= 0xc0040009;
const UInt32 RIFF_ERROR_UnexpectedChunkInStreamList	= 0xc004000a;

// MPEG File Structure Error Codes (5)
const UInt32 MPEG_WARNING_SequenceHeaderNotAtStart	= 0x80050000;
const UInt32 MPEG_WARNING_InvalidSequenceHeader		= 0x80050001;
const UInt32 MPEG_WARNING_PackHeaderNotAtStart		= 0x80050002;
const UInt32 MPEG_ERROR_SequenceHeaderNotFound		= 0xc0050000;
const UInt32 MPEG_ERROR_PackHeaderNotFound			= 0xc0050001;

// AIFF File Structure Error Codes (6)
const UInt32 AIFF_ERROR_NotAIFF						= 0xc0060000;
const UInt32 AIFF_ERROR_MissingCOMMChunk			= 0xc0060001;
const UInt32 AIFF_ERROR_MissingSSNDChunk			= 0xc0060002;

} //namespace CMI
#endif //CMI_ERROR_CODES_H__
