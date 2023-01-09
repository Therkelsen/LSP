
// ALP LED API Sample Single-Color.cpp : Defines the entry point for the console application.

#include "stdafx.h"
#include "alp.h"
#include "AlpUserInterface.h"
#include "AlpFrames.h"
#include <conio.h>
#include <crtdbg.h>

// Error handling policy: Quit, whenever an ALP error happens.
// VERIFY_ALP also echoes each successfull ALP API call (in contrast to VERIFY_ALP_NO_ECHO)
#define VERIFY_ALP( AlpApiCall ) \
	if (AlpError(AlpApiCall, _T(#AlpApiCall), true)) { Pause(); return 1; }
#define VERIFY_ALP_NO_ECHO( AlpApiCall ) \
	if (AlpError(AlpApiCall, _T(#AlpApiCall), false)) { Pause(); return 1; }

class Projector {
public:
	Projector() = default;
	//virtual ~Projector();

	int initializeProjector();

	int initializeImageData(const long frames, const long spacing, unsigned long pictureTime);

	int initializeLED(long brightness);

private:
	// ID's needed for the projector
	ALP_ID AlpDevId, AlpSeqId, AlpLedId;

	// Width and Height of the projector [px]
	long _width, _height;

	// Pixel size (Confirm this)
	long _space;
	
	// Amount of frames to display
	long _frames;

	// Picture time [ns]
	unsigned long _pictureTime;
	
	// LED brightness [%]
	long _brightness;
};

