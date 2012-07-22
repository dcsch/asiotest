#pragma once

#include "SoundSystem.h"
#include "EssenceContentHandler.h"
#include "EssenceErrorHandler.h"
#include "asiosys.h"
#include "asio.h"
#include "asiodrivers.h"

namespace CMI
{

class ASIOSoundSystem :
	public SoundSystem,
	public EssenceContentHandler, // TODO: Put these handlers in a delegate
	public EssenceErrorHandler
{
public:
	ASIOSoundSystem();

	virtual ~ASIOSoundSystem();

	virtual void Initialise();

	virtual void Finalise();

	virtual void LoadSample(Reader &reader);

	virtual void PlaySample();

	virtual void GenerateSineWaveSample();

public:
    virtual UInt8* allocateBuffer(
        const EssenceReader* reader,
        UInt32 trackIndex,
        const UL& ul,
        UInt32 length) throw();

	virtual void freeBuffer(
        const EssenceReader* reader,
        UInt32 trackIndex,
        const UL& ul,
        UInt8* buffer) throw();

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
        const Rational& editRate) throw();

    virtual void endOfStream(
        const EssenceReader* reader,
        UInt32 trackIndex,
        const UL& ul) throw();

	virtual void warning(const EssenceReader* reader, UInt32 code, Position position) throw();

	virtual void error(const EssenceReader* reader, UInt32 code, Position position) throw();

private:
	enum {
		kMaxInputChannels = 32,
		kMaxOutputChannels = 32
	};

	ASIODriverInfo driverInfo;

	// ASIOGetChannels()
	long           inputChannels;
	long           outputChannels;

	// ASIOGetBufferSize()
	long           minSize;
	long           maxSize;
	long           preferredSize;
	long           granularity;

	// ASIOGetSampleRate()
	ASIOSampleRate sampleRate;

	// ASIOOutputReady()
	bool           postOutput;

	// ASIOGetLatencies ()
	long           inputLatency;
	long           outputLatency;

	// ASIOCreateBuffers ()
	long inputBuffers;	// becomes number of actual created input buffers
	long outputBuffers;	// becomes number of actual created output buffers
	ASIOBufferInfo bufferInfos[kMaxInputChannels + kMaxOutputChannels]; // buffer info's

	// ASIOGetChannelInfo()
	ASIOChannelInfo channelInfos[kMaxInputChannels + kMaxOutputChannels]; // channel info's
	// The above two arrays share the same indexing, as the data in them are linked together

	// Information from ASIOGetSamplePosition()
	// data is converted to double floats for easier use, however 64 bit integer can be used, too
	double         nanoSeconds;
	double         samples;
	double         tcSamples;	// time code samples

	// bufferSwitchTimeInfo()
	ASIOTime       tInfo;			// time info state
	unsigned long  sysRefTime;      // system reference time, when bufferSwitch() was called

	// Signal the end of processing in this example
	bool           stopped;

	UInt8 *_buffer;
	Length _bufferLength;
	Position _bufferOffset;

	UInt8 *_playPtr;

	long init_asio_static_data();

	ASIOError create_asio_buffers();

	static ASIOTime *bufferSwitchTimeInfo(ASIOTime *timeInfo, long index, ASIOBool processNow);

	static void bufferSwitch(long index, ASIOBool processNow);

	static void sampleRateChanged(ASIOSampleRate sRate);

	static long asioMessages(long selector, long value, void* message, double* opt);
};

} //namespace CMI
