/**
* @class Projector
* @author Thomas Therkelsen
* @author Simon Vinkel
*
* The Projector class allows the user to interact with the ALP device, and control a projector.
* This includes initializing the device, generating patterns to be displayed on the projector,
* controlling the LED and its brightness, and displaying the generated pattern on the projector.
*
* The class includes the following methods:
* - initializeProjector: Initializes the ALP device and inquires the projector's dimensions.
* - generatePattern: Initializes the image data and allocates memory for the image sequence.
*   It also sets the timing properties of the sequence, loads user-supplied data into the
*   ALP memory of a previously allocated sequence.
* - initializeLED: Initializes the LED and inquires the LED's brightness.
* - gatedSynch: Certain ViALUX single-LED devices have a LED enable connected to Gated Synch 3
*   This method asserts this signal permanently
* - display: Start the continuous display and monitor LED current and temperature.
*/



#include "Projector.h"

Projector::~Projector() {}

/**
* @brief Initialize the ALP device and sets the projector's dimensions
*
* This method initializes the ALP device and inquires the projector's dimensions by
* using the AlpDevAlloc and AlpDevInquire functions.
* 
* @note AlpDevInquire: inquires the device of certain parameters i.e. height or width.
*
* @return  int, representing the status of the initialization, returns 0 if initialization was successful, otherwise returns an error code.
*/
int Projector::initializeProjector(){
	VERIFY_ALP_NO_ECHO(AlpDevAlloc(deviceNum, initFlag, &AlpDevId));
	VERIFY_ALP_NO_ECHO(AlpDevInquire(AlpDevId, ALP_DEV_DISPLAY_WIDTH, &_width));
	VERIFY_ALP_NO_ECHO(AlpDevInquire(AlpDevId, ALP_DEV_DISPLAY_HEIGHT, &_height));
	_tprintf(_T("Projector dimensions: %i x %i pixels\r\n"), _width, _height);

	return 0;
}

/**
* @brief Generates a pattern and displays it on the projector using the ALP-4 API.
*
* @param frames The number of frames that the image data consists of.
* @param spacing The width of the virtual pixel projected by the projector.
* @param pictureTime The time it takes to display each picture in the nanoseconds.
* @param brightness The brightness of the projected image in %.
*
* This method generates a pattern by initializing the
* image data using the CAlpFramesMovingSquare class. The method then allocates a sequence,
* loads the data into the ALP memory and sets the timing properties of the sequence using the
* AlpSeqAlloc, AlpSeqPut and AlpSeqTiming functions of the ALP-4 API.
* It also call other function to drive LED,  set gatedSynch and start display
* 
* @note AlpSeqAlloc: allocates a sequence.
* @note AlpSeqPut: loads the data into the ALP memory
* @note AlpSeqTiming: sets the timing properties of the sequence
*
* The function verifies the returned value of the ALP-4 API functions using the `VERIFY_ALP_NO_ECHO`
* macro, which includes error handling.
*
* @return int, 0 if the function executed successfully, otherwise returns an error code.
*/

int Projector::generatePattern(const long frames, const long spacing, const unsigned long pictureTime, const long brightness) {
	setImageDataParams(frames, spacing, pictureTime, brightness);

	CAlpFramesMovingSquare ImageData(_frames, _width, _height, _spacing);

	VERIFY_ALP_NO_ECHO(AlpSeqAlloc(AlpDevId, _bitPlanes, _frames, &AlpSeqId));
	VERIFY_ALP_NO_ECHO(AlpSeqPut(AlpDevId, AlpSeqId, _pictureOffset, _frames, ImageData(0)));
	VERIFY_ALP_NO_ECHO(AlpSeqTiming(AlpDevId, AlpSeqId, _illuminateTime, _pictureTime, _synchDelay, _synchPulseWidth, _triggerInDelay));

	initializeLED(brightness);

	gatedSynch();

	display();

	return 0;
}

//std::vector<std::variant< long, long, unsigned long, long >> Projector::getImageDataParams()
//{
//	std::vector<std::variant<long, long, unsigned long, long>> params{_frames, _spacing, _pictureTime, _brightness };
//	return ;
//}

void Projector::setImageDataParams(const long frames, const long spacing, const unsigned long pictureTime, const long brightness) {
	_frames = frames;
	_spacing = spacing;
	_pictureTime = pictureTime;
	_brightness = brightness;
}

void Projector::setSequenceParams(const long bitPlanes, const long pictureOffset) {
	_bitPlanes = bitPlanes;
	_pictureOffset = pictureOffset;
}

void Projector::setTimingParams(const unsigned long illuminateTime, const unsigned long pictureTime, const unsigned long synchDelay, const unsigned long synchPulseWidth, const unsigned long triggerInDelay){
	_illuminateTime = illuminateTime;
	_pictureOffset = pictureTime;
	_synchDelay = synchDelay;
	_synchPulseWidth = synchPulseWidth;
	_triggerInDelay = triggerInDelay;
}

/**
* @brief Initialize a LED.
*
* Initializes and allocates a LED driver of the given type using the AlpLedAlloc function.
* The LED is addressed by its identifier `AlpLedId` in subsequent ALP LED API calls.
*
* @param[in] brightness The brightness of the LED in percentage (default: 100%).
* @note The LED type cannot be detected automatically. It is important that the user enters the correct type.
* This affects parameters like maximum allowed continuous forward current,
* but also calculation of junction temperature depends on the correct LED type.
* 
* @note Using AlpLedInquire function this function reports the allowed continuous forward current of the LED type
* and the configuration of this HLD. These parameters could be used for AlpLedAlloc.
* 
* @note AlpLedAlloc initializes and allocates a LED driver of the given type. It is addressed by its
* identifier LedId in subsequent ALP LED API calls. 
* 
* @note Using AlpLedInquireEx function, this function will report the I2C bus addresses of the DAC and ADC of the LED driver.
* 
* @note Using AlpLedControl function, this function will set the brightness of the LED to the desired percentage and switch the LED on.
*
* @note Single-Color configurations usually have bus addresses 24, 64.
* 
* @return int 0 on success, 1 on failure
*/
int Projector::initializeLED(const long brightness) {
	_brightness = brightness;
	VERIFY_ALP_NO_ECHO(AlpLedAlloc(AlpDevId, _ledType, NULL, &AlpLedId));

	VERIFY_ALP_NO_ECHO(AlpLedInquire(AlpDevId, AlpLedId, ALP_LED_SET_CURRENT, &_LEDContCurrent));
	_tprintf( _T("This LED can be driven with continuous current of %0.1f A\r\n"), (double)_LEDContCurrent/1000. );
	
	VERIFY_ALP_NO_ECHO(AlpLedInquireEx(AlpDevId, AlpLedId, ALP_LED_ALLOC_PARAMS, &_LEDParams));
	_tprintf( _T("The LED driver has I2C bus addresses DAC=%i, ADC=%i\r\n"), _LEDParams.I2cDacAddr, _LEDParams.I2cAdcAddr);

	VERIFY_ALP_NO_ECHO(AlpLedControl(AlpDevId, AlpLedId, ALP_LED_BRIGHTNESS, _brightness));
}

/**
* @brief Asserts the Gated Synch signal permanently for certain ViALUX single-LED devices
*
* This method uses the `AlpDevControlEx` function to permanently assert the Gated Synch signal.
* This is achieved by filling out the `tAlpDynSynchOutGate` data structure and setting the `Period` member to `1`.
* This results in a permanent high level signal being sent to Gated Synch 3 on the device. Resulting in the
* LED constantly being on.
* 
* @note _ASSERT: verify that the compiler correctly aligns members of this structure. tAlpDynSynchOutGate is a struct 
* with period, polarity and gate as membervariables i.e. 18 bytes in total.
* 
* @note memset: at a given memory address, set a given value for a given amount of bytes
*
* This method is usually used in single-LED devices where the LED enable is connected to Gated Synch 3.
*/
int Projector::gatedSynch() {
	_ASSERT(sizeof(_AlpSynchGate) == 18);
	memset(&_AlpSynchGate, 0, sizeof(_AlpSynchGate)); // research better ways to do this

	_AlpSynchGate.Period = 1;

	VERIFY_ALP_NO_ECHO(AlpDevControlEx(AlpDevId, ALP_DEV_DYN_SYNCH_OUT3_GATE, &_AlpSynchGate));
}

/**
* @brief Continuous projection of a pre-defined sequence of images, with monitoring of LED current and temperature
*
* This method starts the continuous projection of the images that have been previously loaded into the ALP memory using the 
* `AlpSeqPut` function and sets the timing properties of the sequence using `AlpSeqTiming`. It also monitors the LED current
* and temperature using the `AlpLedInquire` function and  runs until a key has been hit or until the temperature 
* exceeds a certain limit (by default 100*256). In case of this happening, the LED is switched off and the program stops.
*
* @note The LED current is measured in milliamperes (mA) and the temperature is measured in 1°C/256.
* 
* @note AlpProjStartCont (ALP_ID DeviceId, ALP_ID SequenceId): This function displays the specified sequence in an infinite loop.  
* The sequence display can be stopped using AlpProjHalt or AlpDevHalt.
* 
* @note _kbhit(): keyboard hit. Has value 0 until a key is pressed, after which it gains the value 1.
* 
* @note AlpDevInquire: inquires the device of certain parameters i.e. current or temperature.
* 
* @note If the temperature sensor is disconnected from the HLD, then very low temperatures (below -200°C or so) are reported. 
* Projector switches off at 100°C (generally, LEDs can stand much higher temperatures, but this differs between LED types).
* 
* @note pause: pauses the execution until a key is pressed. 
* 
* @note When terminating the ALP system, use AlpDevFree before disconnecting it from the USB to
* avoid problems after USB re-connection. 
*/
int Projector::display() {
	VERIFY_ALP_NO_ECHO(AlpProjStartCont(AlpDevId, AlpSeqId));

	// Monitor LED current and temperature (run until a key has been hit, or until "break;")
	_tprintf(_T("\r\nPress a key to stop projection.\r\n"));
	while (_kbhit() == 0) {
		Sleep(_sleepTime);

		VERIFY_ALP_NO_ECHO(AlpLedInquire(AlpDevId, AlpLedId, ALP_LED_MEASURED_CURRENT, &_LEDCurrent));
		VERIFY_ALP_NO_ECHO(AlpLedInquire(AlpDevId, AlpLedId, ALP_LED_TEMPERATURE_JUNCTION, &_LEDJunctionTemp));

		_tprintf(_T("Note: Actual LED current=%0.1f A; Junction Temperature=%0.1f °C\r"),
			(double)_LEDCurrent / 1000, (double)_LEDJunctionTemp / 256);

		if (_LEDJunctionTemp < 0)
			_tprintf(_T("\nWarning: It seems like the thermistor cable is not properly connected.\r"));

		if (_LEDJunctionTemp > 100 * 256) {
			_tprintf(_T("\nWarning: LED becomes quite hot. Stopping.\r"));
			break;
		}
	}
	_tprintf(_T("\r\n\r\n"));

	VERIFY_ALP_NO_ECHO(AlpDevHalt(AlpDevId));
	VERIFY_ALP_NO_ECHO(AlpDevFree(AlpDevId));
	_tprintf(_T("Finished.\r\n"));
	Pause();
}

