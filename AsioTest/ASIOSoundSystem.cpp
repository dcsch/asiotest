#include "StdAfx.h"
#include "common.h"
#include "ASIOSoundSystem.h"
#include "Sample.h"

#define ASIO_DRIVER_NAME    "ASIO4ALL v2"
#define TEST_RUN_TIME  20.0		// run for 20 seconds

// external reference
extern AsioDrivers *asioDrivers;
bool loadAsioDriver(char *name);

namespace CMI
{

// We have to use a singleton to cope with the callbacks
// TODO See if there is a better way
static ASIOSoundSystem *ass;

ASIOCallbacks asioCallbacks;

ASIOSoundSystem::ASIOSoundSystem() :
	SoundSystem(),
	_bufferOffset(0),
	_playPtr(0)
{
	ass = this;
}

ASIOSoundSystem::~ASIOSoundSystem()
{
}

void ASIOSoundSystem::Initialise()
{
	SoundSystem::Initialise();

	if (loadAsioDriver(ASIO_DRIVER_NAME))
	{
		// initialize the driver
		if (ASIOInit(&driverInfo) == ASE_OK)
		{
			printf ("asioVersion:   %d\n"
					"driverVersion: %d\n"
					"Name:          %s\n"
					"ErrorMessage:  %s\n",
					driverInfo.asioVersion, driverInfo.driverVersion,
					driverInfo.name, driverInfo.errorMessage);

			if (init_asio_static_data() == 0)
			{
				//ASIOControlPanel(); // you might want to check wether the ASIOControlPanel() can open

				// set up the asioCallback structure and create the ASIO data buffer
				asioCallbacks.bufferSwitch = &bufferSwitch;
				asioCallbacks.sampleRateDidChange = &sampleRateChanged;
				asioCallbacks.asioMessage = &asioMessages;
				asioCallbacks.bufferSwitchTimeInfo = &bufferSwitchTimeInfo;
				if (create_asio_buffers() == ASE_OK)
				{
					if (ASIOStart() == ASE_OK)
					{
					}
				}
			}
		}
	}
}

void ASIOSoundSystem::Finalise()
{
	SoundSystem::Finalise();

	ASIOStop();
	ASIODisposeBuffers();
	ASIOExit();
	asioDrivers->removeCurrentDriver();
}

void ASIOSoundSystem::play(UInt8 keyNumber)
{
	SoundSystem::play(keyNumber);

	//_playPtr = _samples[0]->getBuffer();
}

long ASIOSoundSystem::init_asio_static_data()
{
	// collect the informational data of the driver
	// get the number of available channels
	if( ASIOGetChannels(&inputChannels, &outputChannels) == ASE_OK)
	{
		printf ("ASIOGetChannels (inputs: %d, outputs: %d);\n", inputChannels, outputChannels);

		// get the usable buffer sizes
		if (ASIOGetBufferSize(&minSize, &maxSize, &preferredSize, &granularity) == ASE_OK)
		{
			printf ("ASIOGetBufferSize (min: %d, max: %d, preferred: %d, granularity: %d);\n",
					 minSize, maxSize,
					 preferredSize, granularity);

			// get the currently selected sample rate
			if (ASIOGetSampleRate(&sampleRate) == ASE_OK)
			{
				printf ("ASIOGetSampleRate (sampleRate: %f);\n", sampleRate);
				if (sampleRate <= 0.0 || sampleRate > 96000.0)
				{
					// Driver does not store it's internal sample rate, so set it to a know one.
					// Usually you should check beforehand, that the selected sample rate is valid
					// with ASIOCanSampleRate().
					if (ASIOSetSampleRate(44100.0) == ASE_OK)
					{
						if (ASIOGetSampleRate(&sampleRate) == ASE_OK)
							printf ("ASIOGetSampleRate (sampleRate: %f);\n", sampleRate);
						else
							return -6;
					}
					else
						return -5;
				}

				// check wether the driver requires the ASIOOutputReady() optimization
				// (can be used by the driver to reduce output latency by one block)
				if (ASIOOutputReady() == ASE_OK)
					postOutput = true;
				else
					postOutput = false;
				printf ("ASIOOutputReady(); - %s\n", postOutput ? "Supported" : "Not supported");

				return 0;
			}
			return -3;
		}
		return -2;
	}
	return -1;
}

ASIOError ASIOSoundSystem::create_asio_buffers()
{
	// create buffers for all inputs and outputs of the card with the 
	// preferredSize from ASIOGetBufferSize() as buffer size
	long i;
	ASIOError result;

	// fill the bufferInfos from the start without a gap
	ASIOBufferInfo *info = bufferInfos;

	// prepare inputs (Though this is not necessaily required, no opened inputs will work, too
	if (inputChannels > kMaxInputChannels)
		inputBuffers = kMaxInputChannels;
	else
		inputBuffers = inputChannels;
	for(i = 0; i < inputBuffers; i++, info++)
	{
		info->isInput = ASIOTrue;
		info->channelNum = i;
		info->buffers[0] = info->buffers[1] = 0;
	}

	// prepare outputs
	if (outputChannels > kMaxOutputChannels)
		outputBuffers = kMaxOutputChannels;
	else
		outputBuffers = outputChannels;
	for(i = 0; i < outputBuffers; i++, info++)
	{
		info->isInput = ASIOFalse;
		info->channelNum = i;
		info->buffers[0] = info->buffers[1] = 0;
	}

	// create and activate buffers
	result = ASIOCreateBuffers(bufferInfos,
		inputBuffers + outputBuffers,
		preferredSize, &asioCallbacks);
	if (result == ASE_OK)
	{
		// now get all the buffer details, sample word length, name, word clock group and activation
		for (i = 0; i < inputBuffers + outputBuffers; i++)
		{
			channelInfos[i].channel = bufferInfos[i].channelNum;
			channelInfos[i].isInput = bufferInfos[i].isInput;
			result = ASIOGetChannelInfo(&channelInfos[i]);
			if (result != ASE_OK)
				break;
		}

		if (result == ASE_OK)
		{
			// get the input and output latencies
			// Latencies often are only valid after ASIOCreateBuffers()
			// (input latency is the age of the first sample in the currently returned audio block)
			// (output latency is the time the first sample in the currently returned audio block requires to get to the output)
			result = ASIOGetLatencies(&inputLatency, &outputLatency);
			if (result == ASE_OK)
				printf ("ASIOGetLatencies (input: %d, output: %d);\n", inputLatency, outputLatency);
		}
	}
	return result;
}

//----------------------------------------------------------------------------------
// conversion from 64 bit ASIOSample/ASIOTimeStamp to double float
#if NATIVE_INT64
	#define ASIO64toDouble(a)  (a)
#else
	const double twoRaisedTo32 = 4294967296.;
	#define ASIO64toDouble(a)  ((a).lo + (a).hi * twoRaisedTo32)
#endif

ASIOTime *ASIOSoundSystem::bufferSwitchTimeInfo(ASIOTime *timeInfo, long index, ASIOBool processNow)
{	// the actual processing callback.
	// Beware that this is normally in a seperate thread, hence be sure that you take care
	// about thread synchronization. This is omitted here for simplicity.
	static long processedSamples = 0;

	// store the timeInfo for later use
	ass->tInfo = *timeInfo;

	// get the time stamp of the buffer, not necessary if no
	// synchronization to other media is required
	if (timeInfo->timeInfo.flags & kSystemTimeValid)
		ass->nanoSeconds = ASIO64toDouble(timeInfo->timeInfo.systemTime);
	else
		ass->nanoSeconds = 0;

	if (timeInfo->timeInfo.flags & kSamplePositionValid)
		ass->samples = ASIO64toDouble(timeInfo->timeInfo.samplePosition);
	else
		ass->samples = 0;

	if (timeInfo->timeCode.flags & kTcValid)
		ass->tcSamples = ASIO64toDouble(timeInfo->timeCode.timeCodeSamples);
	else
		ass->tcSamples = 0;

//	// get the system reference time
//	ass->sysRefTime = timeGetTime();
//
//#if WINDOWS && _DEBUG
//	// a few debug messages for the Windows device driver developer
//	// tells you the time when driver got its interrupt and the delay until the app receives
//	// the event notification.
//	static double last_samples = 0;
//	char tmp[128];
//	sprintf (tmp, "diff: %d / %d ms / %d ms / %d samples                 \n", ass->sysRefTime - (long)(ass->nanoSeconds / 1000000.0), ass->sysRefTime, (long)(ass->nanoSeconds / 1000000.0), (long)(ass->samples - last_samples));
//	OutputDebugStringA(tmp);
//	last_samples = ass->samples;
//#endif

	// buffer size in samples
	long buffSize = ass->preferredSize;

	// perform the processing
	for (int i = 0; i < ass->inputBuffers + ass->outputBuffers; i++)
	{
		if (ass->bufferInfos[i].isInput == false)
		{
			// OK do processing for the outputs only
			switch (ass->channelInfos[i].type)
			{
			case ASIOSTInt16LSB:
				memset (ass->bufferInfos[i].buffers[index], 0, buffSize * 2);
				break;
			case ASIOSTInt24LSB:		// used for 20 bits as well
				memset (ass->bufferInfos[i].buffers[index], 0, buffSize * 3);
				break;
			case ASIOSTInt32LSB:

				if (i % 2 == 1 && ass->_playPtr)
				{
					//Length samplesAvailable = (ass->_samples[0]->getBuffer() + ass->_samples[0]->getBufferLength() - ass->_playPtr) / 2;
					Length samplesAvailable = 0;
					if (samplesAvailable > 0)
					{
						const Int16 *src = reinterpret_cast<const Int16 *>(ass->_playPtr);
						Int32 *dst = static_cast<Int32 *>(ass->bufferInfos[i].buffers[index]);
						for (long j = 0; j < buffSize; ++j)
						{
							if (j <= samplesAvailable)
							{
								*dst = *src << 16;
								src++;
								dst++;
							}
							else
							{
								*dst = 0;
								dst++;
							}
						}
						ass->_playPtr += (2 * buffSize);
					}
					else
					{
						ass->_playPtr = 0;
						memset(ass->bufferInfos[i].buffers[index], 0, buffSize * 4);
					}
				}
				else
					memset(ass->bufferInfos[i].buffers[index], 0, buffSize * 4);

				break;

			case ASIOSTFloat32LSB:		// IEEE 754 32 bit float, as found on Intel x86 architecture
				memset (ass->bufferInfos[i].buffers[index], 0, buffSize * 4);
				break;
			case ASIOSTFloat64LSB: 		// IEEE 754 64 bit double float, as found on Intel x86 architecture
				memset (ass->bufferInfos[i].buffers[index], 0, buffSize * 8);
				break;

				// these are used for 32 bit data buffer, with different alignment of the data inside
				// 32 bit PCI bus systems can more easily used with these
			case ASIOSTInt32LSB16:		// 32 bit data with 18 bit alignment
			case ASIOSTInt32LSB18:		// 32 bit data with 18 bit alignment
			case ASIOSTInt32LSB20:		// 32 bit data with 20 bit alignment
			case ASIOSTInt32LSB24:		// 32 bit data with 24 bit alignment
				memset (ass->bufferInfos[i].buffers[index], 0, buffSize * 4);
				break;

			case ASIOSTInt16MSB:
				memset (ass->bufferInfos[i].buffers[index], 0, buffSize * 2);
				break;
			case ASIOSTInt24MSB:		// used for 20 bits as well
				memset (ass->bufferInfos[i].buffers[index], 0, buffSize * 3);
				break;
			case ASIOSTInt32MSB:
				memset (ass->bufferInfos[i].buffers[index], 0, buffSize * 4);
				break;
			case ASIOSTFloat32MSB:		// IEEE 754 32 bit float, as found on Intel x86 architecture
				memset (ass->bufferInfos[i].buffers[index], 0, buffSize * 4);
				break;
			case ASIOSTFloat64MSB: 		// IEEE 754 64 bit double float, as found on Intel x86 architecture
				memset (ass->bufferInfos[i].buffers[index], 0, buffSize * 8);
				break;

				// these are used for 32 bit data buffer, with different alignment of the data inside
				// 32 bit PCI bus systems can more easily used with these
			case ASIOSTInt32MSB16:		// 32 bit data with 18 bit alignment
			case ASIOSTInt32MSB18:		// 32 bit data with 18 bit alignment
			case ASIOSTInt32MSB20:		// 32 bit data with 20 bit alignment
			case ASIOSTInt32MSB24:		// 32 bit data with 24 bit alignment
				memset (ass->bufferInfos[i].buffers[index], 0, buffSize * 4);
				break;
			}
		}
	}

	// finally if the driver supports the ASIOOutputReady() optimization, do it here, all data are in place
	if (ass->postOutput)
		ASIOOutputReady();

	if (processedSamples >= ass->sampleRate * TEST_RUN_TIME)	// roughly measured
		ass->stopped = true;
	else
		processedSamples += buffSize;

	return 0L;
}

//----------------------------------------------------------------------------------
void ASIOSoundSystem::bufferSwitch(long index, ASIOBool processNow)
{	// the actual processing callback.
	// Beware that this is normally in a seperate thread, hence be sure that you take care
	// about thread synchronization. This is omitted here for simplicity.

	// as this is a "back door" into the bufferSwitchTimeInfo a timeInfo needs to be created
	// though it will only set the timeInfo.samplePosition and timeInfo.systemTime fields and the according flags
	ASIOTime  timeInfo;
	memset (&timeInfo, 0, sizeof (timeInfo));

	// get the time stamp of the buffer, not necessary if no
	// synchronization to other media is required
	if (ASIOGetSamplePosition(&timeInfo.timeInfo.samplePosition, &timeInfo.timeInfo.systemTime) == ASE_OK)
		timeInfo.timeInfo.flags = kSystemTimeValid | kSamplePositionValid;

	bufferSwitchTimeInfo (&timeInfo, index, processNow);
}


//----------------------------------------------------------------------------------
void ASIOSoundSystem::sampleRateChanged(ASIOSampleRate sRate)
{
	// do whatever you need to do if the sample rate changed
	// usually this only happens during external sync.
	// Audio processing is not stopped by the driver, actual sample rate
	// might not have even changed, maybe only the sample rate status of an
	// AES/EBU or S/PDIF digital input at the audio device.
	// You might have to update time/sample related conversion routines, etc.
}

//----------------------------------------------------------------------------------
long ASIOSoundSystem::asioMessages(long selector, long value, void* message, double* opt)
{
	// currently the parameters "value", "message" and "opt" are not used.
	long ret = 0;
	switch(selector)
	{
		case kAsioSelectorSupported:
			if(value == kAsioResetRequest
			|| value == kAsioEngineVersion
			|| value == kAsioResyncRequest
			|| value == kAsioLatenciesChanged
			// the following three were added for ASIO 2.0, you don't necessarily have to support them
			|| value == kAsioSupportsTimeInfo
			|| value == kAsioSupportsTimeCode
			|| value == kAsioSupportsInputMonitor)
				ret = 1L;
			break;
		case kAsioResetRequest:
			// defer the task and perform the reset of the driver during the next "safe" situation
			// You cannot reset the driver right now, as this code is called from the driver.
			// Reset the driver is done by completely destruct is. I.e. ASIOStop(), ASIODisposeBuffers(), Destruction
			// Afterwards you initialize the driver again.
			ass->stopped;  // In this sample the processing will just stop
			ret = 1L;
			break;
		case kAsioResyncRequest:
			// This informs the application, that the driver encountered some non fatal data loss.
			// It is used for synchronization purposes of different media.
			// Added mainly to work around the Win16Mutex problems in Windows 95/98 with the
			// Windows Multimedia system, which could loose data because the Mutex was hold too long
			// by another thread.
			// However a driver can issue it in other situations, too.
			ret = 1L;
			break;
		case kAsioLatenciesChanged:
			// This will inform the host application that the drivers were latencies changed.
			// Beware, it this does not mean that the buffer sizes have changed!
			// You might need to update internal delay data.
			ret = 1L;
			break;
		case kAsioEngineVersion:
			// return the supported ASIO version of the host application
			// If a host applications does not implement this selector, ASIO 1.0 is assumed
			// by the driver
			ret = 2L;
			break;
		case kAsioSupportsTimeInfo:
			// informs the driver wether the asioCallbacks.bufferSwitchTimeInfo() callback
			// is supported.
			// For compatibility with ASIO 1.0 drivers the host application should always support
			// the "old" bufferSwitch method, too.
			ret = 1;
			break;
		case kAsioSupportsTimeCode:
			// informs the driver wether application is interested in time code info.
			// If an application does not need to know about time code, the driver has less work
			// to do.
			ret = 0;
			break;
	}
	return ret;
}

} //namespace CMI
