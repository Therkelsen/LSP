#include "Projector.h"

int Projector::initializeProjector(){
	/* General ALP API stuff
				Initialize the ALP device */
	VERIFY_ALP_NO_ECHO(AlpDevAlloc(0, 0, &AlpDevId));
	VERIFY_ALP_NO_ECHO(AlpDevInquire(AlpDevId, ALP_DEV_DISPLAY_WIDTH, &_width));
	VERIFY_ALP_NO_ECHO(AlpDevInquire(AlpDevId, ALP_DEV_DISPLAY_HEIGHT, &_height));
	_tprintf(_T("Projector dimensions: %i x %i pixels\r\n"), _width, _height);

	return 0;
}

int Projector::initializeImageData(const long frames = 1, const long spacing = 4, unsigned long pictureTime = 200000) {
	// Initialize image data (completely done in during construction of the ImageData variable)
	_frames = frames;
	_space = spacing;
	_pictureTime = pictureTime;
	CAlpFramesMovingSquare ImageData(_frames, _width, _height, _space);

	// Allocate a sequence and load data
	VERIFY_ALP_NO_ECHO(AlpSeqAlloc(AlpDevId, 1, _frames, &AlpSeqId));
	VERIFY_ALP_NO_ECHO(AlpSeqPut(AlpDevId, AlpSeqId, 0, _frames, ImageData(0)));
	// 200ms, i.e. 5Hz - Input "1000" for 1[s]
	VERIFY_ALP_NO_ECHO(AlpSeqTiming(AlpDevId, AlpSeqId, 0, _pictureTime, 0, 0, 0));

	return 0;
}

int Projector::initializeLED(long brightness = 75) {
	/* LED Stuff 
	 Initialize a LED.
	 Note: The LED type cannot be detected automatically.
	 It is important that the user enters the correct type.
	 This affects parameters like maximum allowed continuous
	 forward current, but also calculation of junction temperature
	 depends on the correct LED type.*/
	_brightness = brightness;
	long const LedType = ALP_HLD_PT120_BLUE;
	VERIFY_ALP_NO_ECHO(AlpLedAlloc(AlpDevId, LedType, NULL, &AlpLedId));

	// Just for information: inquire the allowed continuous forward current of this LED type
	long nLedContCurrent_mA(0);
	VERIFY_ALP_NO_ECHO(AlpLedInquire(AlpDevId, AlpLedId, ALP_LED_SET_CURRENT, &nLedContCurrent_mA));
	//_tprintf( _T("Note: This LED can be driven with continuous current of %0.1f A\r\n"), (double)nLedContCurrent_mA/1000 );
	// Report the configuration of this HLD. These parameters could be used for AlpLedAlloc.
	// Single-Color configurations usually have bus addresses 24, 64.
	tAlpHldPt120AllocParams LedParams;
	VERIFY_ALP_NO_ECHO(AlpLedInquireEx(AlpDevId, AlpLedId, ALP_LED_ALLOC_PARAMS, &LedParams));
	//_tprintf( _T("Note: The LED driver has I2C bus addresses DAC=%i, ADC=%i\r\n"), LedParams.I2cDacAddr, LedParams.I2cAdcAddr);

	// User enters percentage for LED brightness:
	// Note: this application limits input values to 100 percent,
	//	whilst the API allows overdriving the LED
	long nLedBrightness_Percent(brightness);

	// Switch LED on
	VERIFY_ALP_NO_ECHO(AlpLedControl(AlpDevId, AlpLedId, ALP_LED_BRIGHTNESS, nLedBrightness_Percent));
}