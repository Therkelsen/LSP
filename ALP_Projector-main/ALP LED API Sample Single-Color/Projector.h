
// ALP LED API Sample Single-Color.cpp : Defines the entry point for the console application.

#include "stdafx.h"
#include "alp.h"
#include "AlpUserInterface.h"
#include "AlpFrames.h"
#include <conio.h>
#include <crtdbg.h>
#include <vector>
#include <variant>


// Error handling policy: Quit, whenever an ALP error happens.
// VERIFY_ALP also echoes each successfull ALP API call (in contrast to VERIFY_ALP_NO_ECHO)
#define VERIFY_ALP( AlpApiCall ) \
	if (AlpError(AlpApiCall, _T(#AlpApiCall), true)) { Pause(); return 1; }
#define VERIFY_ALP_NO_ECHO( AlpApiCall ) \
	if (AlpError(AlpApiCall, _T(#AlpApiCall), false)) { Pause(); return 1; }

class Projector {
public:
	Projector() = default;
	virtual ~Projector();

	int initializeProjector();

	int generatePattern(const long frames = 1, const long spacing = 4, const unsigned long pictureTime = 200000, const long brightness = 100);

	//std::vector<std::variant<long, long, unsigned long, long>> getImageDataParams();

	void setImageDataParams(const long frames, const long spacing, const unsigned long pictureTime, const long brightness);
	void setSequenceParams(const long bitPlanes, const long pictureOffset);
	void setTimingParams(const unsigned long illuminateTime, const unsigned long pictureTime, const unsigned long synchDelay, const unsigned long synchPulseWidth, const unsigned long triggerInDelay);

private:

	int initializeLED(long brightness = 100);

	int gatedSynch();

	int display();
	
	/**
	* @var AlpDevId, AlpSeqId, AlpLedId
	* @brief ID's needed for the projector (Device, Sequence, ID)
	* 
	* @var _width, _height
	* @brief Dimensions (width, height), and width of virtual pixel.
	*
	* @var _frames, _spacing, _pictureTime, _brightness
	* @brief Amount of frames to display, width of virtual pixels, time between two consecutive pictures [μs], LED brightness [%]
	* 
	* @var _bitPlanes, _pictureOffset
	* @brief Bit depth of images, and indexing of pictures, i.e. which index to start at.
	*
	* @var _illuminateTime, _pictureTime, _synchDelay, _synchPulseWidth, _triggerInDelay
	* Duration of the display of one picture in the sequence [μs], Time between the start of two consecutive pictures [μs], Specifies the time 
	* between start of the frame synch output pulse and the start of the display (master mode) [μs], Specifies the duration of the frame synch
	* output pulse [μs], Specifies the time between the incoming trigger edge and the start of the display (slave mode) [μs].
	*
	* @var _ledType, _LEDContCurrent, _LEDParams, _AlpSynchGate, _LEDCurrent, _LEDJunctionTemp
	* @brief LED type, LED maximum current [mA], LED parameters, Struct containing 18 bytes in memory, LED current [mA], LED temperature [°C].
	*
	* @var _sleepTime, deviceNum, initFlag
	* @brief Sleep time within loop [s], Device number, Initialization flag
	*/

	ALP_ID AlpDevId, AlpSeqId, AlpLedId;

	long _width, _height;

	long _frames, _spacing, _brightness;
	unsigned long _pictureTime;

	long _bitPlanes, _pictureOffset;
	
	unsigned long _illuminateTime, _pictureTime, _synchDelay, _synchPulseWidth, _triggerInDelay;

	long const _ledType = ALP_HLD_PT120_BLUE;
	long _LEDContCurrent = 0;
	tAlpHldPt120AllocParams _LEDParams;
	tAlpDynSynchOutGate _AlpSynchGate;
	long _LEDCurrent = 0, _LEDJunctionTemp = 0, deviceNum = 0, initFlag = 0;
	unsigned long _sleepTime = 1000;
};

