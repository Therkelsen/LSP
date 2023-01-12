#pragma once
#include <memory>

// class AlpFrames:
// - manage storage of Image Data for one or more DMD frames.
// - provide some basic methods for accessing these data (at, (), FillRect)
// - Error handling: To keep things simple in this sample,
//	 parameter validation uses only debug checks (_ASSERT macro)
// class CAlpFramesMovingSquare:
// - specialization of AlpFrames (just to remove its code from the main function)
// - initializes image data with a bright square moving diagonally over the DMD

class AlpFrames {
	AlpFrames const& operator=(AlpFrames const& rhs); // hidden, not required
public:
	AlpFrames(const long frames, const long width, const long height);
	~AlpFrames(void);

	// This conversion operator returns a pointer to the image data of a frame.
	// nFrameNumber, nX, nY are zero-based indexes.
	char unsigned* operator()(const long frameNum);
	// access a single pixel
	char unsigned& at(const long frameNum, const long x, const long y);

	// Drawing routine
	void fillRect(const long frameNum, const long left, const long top,
		const long width, const long height, const char unsigned pixelValue);

	void drawMovingSquare(long frames, long width, long height);
	void drawSquare(long frames, long pWidth, long pHeight, long sqSize);
	void drawVertialLines(long frames, long hPad, long spacing, long pWidth, long pHeight, long lWidth);
	void drawHorizontalLines(long frames, long hPad, long spacing, long pWidth, long pHeight, long lWidth);
	void drawGrid(long frames, long vPad, long hPad, long vSpacing,
		long hSpacing, long pWidth, long pHeight, long lWidth);
	void drawTartanSquares(long frames, long vPad, long hPad,
		long vSpacing, long hSpacing, long pWidth, long pHeight, long lWidth);

	// These members contain parameters, supplied during construction.
	const long _frameCount, _width, _height;

private:
	// pointer to the data
	char unsigned* const _imageData;
};