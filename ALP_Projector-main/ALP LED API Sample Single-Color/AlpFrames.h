#pragma once

class AlpFrames {
public:
	AlpFrames(const long frames, const long width, const long height);
	~AlpFrames(void);

	char unsigned* operator()(const long frameNum);

	char unsigned& at(const long frameNum, const long x, const long y);

	void fillRect(const long frameNum, const long x, const long y,
		const long width, const long height, const char unsigned pixelValue);

	void drawMovingSquare(long frames, long width, long height);
	void drawSquare(long frames, long vPad, long hPad, long sqSize);
	void drawVertialLines(long frames, long hPad, long spacing, long lWidth);
	void drawHorizontalLines(long frames, long hPad, long spacing, long lWidth);
	void drawGrid(long frames, long vPad, long hPad, long vSpacing, long hSpacing, long lWidth);
	void drawCheckerBoard(long frames, long vPad, long hPad, long sqSize);

	const long _frameCount, _width, _height;

private:
	char unsigned* const _imageData;
};