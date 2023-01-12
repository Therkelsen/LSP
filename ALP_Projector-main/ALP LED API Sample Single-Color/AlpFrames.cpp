#include "AlpFrames.h"
#include "AlpUserInterface.h"
#include <crtdbg.h>
#include <memory>
#include <stdio.h>
#include <typeinfo>
#include "Windows.h"
#include <stdexcept>
#include <iostream>

AlpFrames::AlpFrames(const long frames, const long width, const long height)
	// shared pointer:
	: _frameCount(frames), _width(width), _height(height),
	// uses `new` to allocate `frames * width * height` bytes of storage
	// hence, _imageData is a non-null pointer to the first byte of the block
	_imageData(new char unsigned[frames * width * height]) {
	// Weird macro definitions
	// Initialize image data as solid black
	memset(_imageData, 0, frames * width * height);
}

AlpFrames::~AlpFrames(void) {
	//Deconstructor, for now just resets image to all black to avoid heap errors
	memset(_imageData, 0, _width * _height);
	//ZeroMemory(_imageData, _width * _height);
	//free(_imageData);
	//delete[] _imageData;
}

char unsigned* AlpFrames::operator()(const long frameNum) {
	_ASSERT(frameNum >= 0 && frameNum < _frameCount);
	return _imageData + frameNum * _width * _height;
}

char unsigned& AlpFrames::at(const long frameNum, const long x, const long y) {
	_ASSERT(frameNum >= 0 && frameNum < _frameCount && x >= 0 && x < _width && y >= 0 && y < _height);

	return _imageData[frameNum * _width * _height + y * _width + x];
}

void AlpFrames::fillRect(const long frameNum, const long left, const long top,
	const long rectWidth, const long height, const char unsigned pixelValue) {
	long const bottom = top + height - 1;
	_ASSERT(frameNum >= 0 && frameNum < _frameCount);
	_ASSERT(0 <= left && 0 < rectWidth && left + rectWidth <= _width);
	_ASSERT(0 <= top && top <= bottom && bottom < _height);
	// row by row: set the area starting from left to PixelValue
	for (long y = top; y <= bottom; y++)
		memset(&at(frameNum, left, y), pixelValue, rectWidth);
}

void AlpFrames::drawMovingSquare(long frames, long width, long height) {
	// Move a bright square from top-left to bottom-right corner of the DMD.
	// (this also visualizes flipped output of projection optics )
	// Method requires a frame count of at least `2`.
	const long squareWidth = height / 5, dx = width - squareWidth, dy = height - squareWidth;
	for (long frame = 0; frame < frames; frame++) {
		try{
			if (frames - 1 == 0)
				throw std::runtime_error("Math Error: Attempted division by zero\nHint: Increase frame count above one.");
			fillRect(frame, frame * dx / (frames - 1), frame * dy / (frames - 1), squareWidth, squareWidth, 255);
		} catch (const std::runtime_error& e) {
			std::cerr << e.what() << std::endl;
			Pause();
			exit(1);
		}
	}
}

void AlpFrames::drawSquare(long frames, long pWidth, long pHeight, long sqSize) {
	//add xy-pos
	long hPad = pWidth / 2 - sqSize / 2;
	long vPad = pHeight / 2 - sqSize / 2;
		try {
			if (frames != 1)
				throw std::invalid_argument("Error: Frame count must be 1.");
				fillRect(frames - 1, hPad, vPad, sqSize, sqSize, 255);
		} catch (const std::invalid_argument& e) {
			std::cerr << e.what() << std::endl;
			Pause();
			exit(1);
		}
}

void AlpFrames::drawVertialLines(long frames, long hPad, long spacing, long pWidth, long pHeight, long lWidth) {
	try {
		if (frames != 1)
			throw std::invalid_argument("Error: Frame count must be 1.");
		for (int i = hPad; i < pWidth / lWidth; i++) {
			if(i%2 == 0)
				fillRect(frames - 1, i*lWidth, 0, lWidth, pHeight, 255);
			i += spacing;
		}
	}
	catch (const std::invalid_argument& e) {
		std::cerr << e.what() << std::endl;
		Pause();
		exit(1);
	}
}

void AlpFrames::drawHorizontalLines(long frames, long vPad, long spacing, long pWidth, long pHeight, long lWidth) {
	try {
		if (frames != 1)
			throw std::invalid_argument("Error: Frame count must be 1.");
		for (int i = 0; i < pHeight / lWidth; i++) {
			if (i % 2 == 0)
				fillRect(frames - 1, 0, i * lWidth, pWidth, lWidth, 255);
			i += spacing;
		}
	}
	catch (const std::invalid_argument& e) {
		std::cerr << e.what() << std::endl;
		Pause();
		exit(1);
	}
}

void AlpFrames::drawGrid(long frames, long vPad, long hPad, long vSpacing, long hSpacing, long pWidth, long pHeight, long lWidth) {
	drawVertialLines(frames, hPad, hSpacing, pWidth, pHeight, lWidth);
	drawHorizontalLines(frames, vPad, vSpacing, pWidth, pHeight, lWidth);
}

void AlpFrames::drawTartanSquares(long frames, long vPad, long hPad, long vSpacing, long hSpacing, long pWidth, long pHeight, long lWidth) {
	//Use draw squares and stack them in a for-loop
}