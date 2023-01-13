/**
* @class Projector
* @author Thomas Therkelsen
* @author Simon Vinkel
*
* @brief A class that creates and manages an array of frames for an ALP projector, based on sample code.
*
* This class creates an array of frames for an ALP projector. It allows the user to
* draw shapes and lines on the frames, as well as manipulate the frames in other ways.
*/

#include "AlpFrames.h"
#include "AlpUserInterface.h"
#include <crtdbg.h>
#include <memory>
#include <stdio.h>
#include <typeinfo>
#include "Windows.h"
#include <stdexcept>
#include <iostream>

/**
* @brief Constructor for the AlpFrames class
* @param frames The number of frames in the image sequence
* @param width The width of the image
* @param height The height of the image
*
* This constructor takes in three parameters, the number of frames, width and
* height of the image to be projected. It also initializes the _frameCount,
* _width, _height and _imageData. It uses `new` to allocate `frames * width
* height` bytes of storage, hence, _imageData is a non-null pointer to the first
* byte of the block.
*
* @return Initializes the member variables with the given parameters
*
* @throws std::invalid_argument if width or height is less than or equal to zero
* @throws std::invalid_argument if _imageData is null
*/
AlpFrames::AlpFrames(const long frames, const long width, const long height)
	: _frameCount(frames), _width(width), _height(height),
	_imageData(new char unsigned[frames * width * height]) {
	try {
		if (_width <= 0 || _height <= 0)
			throw std::invalid_argument("Error: Projector dimensions must be positive integers.");
		if (_imageData == nullptr)
			throw std::invalid_argument("Error: ImageData pointer can't be null.");
	} catch (std::invalid_argument& e) {
		std::cerr << e.what() << std::endl;
		Pause();
		exit(1);
	}

	memset(_imageData, 0, frames * width * height);
}

/**
 *  @brief Destructor for AlpFrames object
 *
 *  This destructor is responsible for cleaning up any resources that were
 *  allocated during the lifetime of the object. It resets image to all black
 *  to avoid heap errors.
 */
AlpFrames::~AlpFrames(void) {
	memset(_imageData, 0, _width * _height);
}

/**
* @brief  operator() returns a pointer to the start of a frame.
* @param  frameNum: the number of the frame.
* @return Pointer to the start of the frame specified by frameNum.
* @throws std::invalid_argument if frameNum is less than or equal to
* zero or frameNum is greater than the number of frames.
*/
char unsigned* AlpFrames::operator()(const long frameNum) {
	try {
		if (frameNum <= 0 && frameNum > _frameCount)
			throw std::invalid_argument("Error: `frameNum` invalid.");
	} catch (std::invalid_argument& e) {
			std::cerr << e.what() << std::endl;
			Pause();
			exit(1);
	}
	return _imageData + frameNum * _width * _height;
}

/**
* @brief Returns a reference to the pixel at a specified location in the specified frame.
*
* @param  frameNum The number of the frame the pixel is located in.
* @param  x The x-coordinate of the pixel in the frame.
* @param  y The y-coordinate of the pixel in the frame.
*
* This method provides a way to access individual pixels in a frame. It is used to read
* or modify the value of a specific pixel. The method checks if the given frame number
* and coordinates are valid, if not it throws an invalid_argument exception.
*
* @return A reference to the pixel at the specified location in the specified frame.
*
* @throws std::invalid_argument if frame number is less than or equal to 0 and greater
* than the number of frames, or if the x-coordinate is less than 0 or greater than or
* equal to the width of the frame, or if the y-coordinate is less than 0 or greater
* than or equal to the height of the frame.
*/
char unsigned& AlpFrames::at(const long frameNum, const long x, const long y) {
	try {
		if (frameNum <= 0 && frameNum > _frameCount)
			throw std::invalid_argument("Error: `frameNum` invalid.");
		if (x < 0 || x > _width || y < 0 || y > _height)
			throw std::invalid_argument("Error: Pixel out of bounds.");
	} catch (std::invalid_argument& e) {
		std::cerr << e.what() << std::endl;
		Pause();
		exit(1);
	}
	return _imageData[frameNum * _width * _height + y * _width + x];
}

/**
* @brief Returns a reference to the pixel at a specified location in the specified frame.
*
* @param  frameNum The number of the frame the pixel is located in.
* @param  x The x-coordinate of the pixel in the frame.
* @param  y The y-coordinate of the pixel in the frame.
*
* This method provides a way to access individual pixels in a frame. It is used to read
* or modify the value of a specific pixel. The method checks if the given frame number
* and coordinates are valid, if not it throws an invalid_argument exception.
*
* @return A reference to the pixel at the specified location in the specified frame.
*
* @throws std::invalid_argument if frame number is less than 0 and greater than or
* equal to the number of frames, or if the x-coordinate is less than 0 or greater
* than or equal to the width of the frame, or if the y-coordinate is less than 0
* or greater than or equal to the height of the frame.
*/
void AlpFrames::fillRect(const long frameNum, const long x, const long y,
	const long rectWidth, const long height, const char unsigned pixelValue) {
	long const bottom = y + height - 1;

	try {
		if (frameNum < 0 && frameNum > _frameCount)
			throw std::invalid_argument("Error: `frameNum` invalid.");
		if (x < 0 || x + rectWidth > _width || y < 0 || bottom >= _height)
			throw std::invalid_argument("Error: Attempting to draw out of bounds.");
		if (rectWidth <= 0)
			throw std::invalid_argument("Error: `rectWidth` must be a positive integer.");
	} catch (std::invalid_argument& e) {
		std::cerr << e.what() << std::endl;
		Pause();
		exit(1);
	}

	for (long yPos = y; yPos <= bottom; yPos++)
		memset(&at(frameNum, x, yPos), pixelValue, rectWidth);
}

/**
* @brief Draws a moving square on the frames.
*
* The function takes 3 parameters, the number of frames, the width of the frames,
* and the height of the frames. It moves a bright square from the top-left corner to
* the bottom-right corner of the frames. Also, it visualizes the flipped output of the
* projection optics.
*
* @param frames The number of frames to draw the square on.
* @param width The width of the frames.
* @param height The height of the frames.
*
* @throw runtime_error if the frames parameter is less than 2.
*/
void AlpFrames::drawMovingSquare(long frames, long width, long height) {
	const long squareWidth = height / 5, dx = width - squareWidth, dy = height - squareWidth;
	for (long frame = 0; frame < frames; frame++) {
		try{
			if (frames < 2)
				throw std::invalid_argument("Error: `frames` must be a positive integer.");
			fillRect(frame, frame * dx / (frames - 1), frame * dy / (frames - 1), squareWidth, squareWidth, 255);
		} catch (const std::runtime_error& e) {
			std::cerr << e.what() << std::endl;
			Pause();
			exit(1);
		}
	}
}

/**
* @brief Draws a square on the frames.
*
* The function takes 4 parameters, the number of frames, the width of the
* frames, the height of the frames, and the size of the square. It draws
* the square in the center of the frames.
*
* @param frames The number of frames to draw the square on.
* @param pWidth The width of the frames.
* @param pHeight The height of the frames.
* @param sqSize The size of the square.
*
* @throw invalid_argument if the frames parameter is not equal to 1.
*/
void AlpFrames::drawSquare(long frames, long vPad, long hPad, long sqSize) {
		try {
			if (frames != 1)
				throw std::invalid_argument("Error: `frames` must be 1.");
				fillRect(frames - 1, hPad, vPad, sqSize, sqSize, 255);
		} catch (const std::invalid_argument& e) {
			std::cerr << e.what() << std::endl;
			Pause();
			exit(1);
		}
}

/**
* @brief Draws vertical lines on the frames.
*
* The function takes 4 parameters, the number of frames, the horizontal
* padding, the spacing between lines, and the width of the lines.
* It draws vertical lines on the frames starting from the
* horizontal padding and with the specified spacing.
*
* @param frames The number of frames to draw the lines on.
* @param hPad The horizontal padding of the lines.
* @param spacing The spacing between lines.
* @param lWidth The width of the lines.
*
* @throw invalid_argument if the frames parameter is not equal to 1.
*/
void AlpFrames::drawVertialLines(long frames, long hPad, long spacing, long lWidth) {
	try {
		if (frames != 1)
			throw std::invalid_argument("Error: `frames` must be 1.");
		for (int i = hPad; i < _width / lWidth; i++) {
			if(i%2 == 0)
				fillRect(frames - 1, i*lWidth, 0, lWidth, _height, 255);
			i += spacing;
		}
	}
	catch (const std::invalid_argument& e) {
		std::cerr << e.what() << std::endl;
		Pause();
		exit(1);
	}
}

/**
* @brief Draws horizontal lines on the frames.
*
* The function takes 4 parameters, the number of frames, the horizontal
* padding, the spacing between lines, and the width of the lines.
* It draws vertical lines on the frames starting from the
* horizontal padding and with the specified spacing.
*
* @param frames The number of frames to draw the lines on.
* @param vPad The vertical padding of the lines.
* @param spacing The spacing between lines.
* @param lWidth The width of the lines.
*
* @throw invalid_argument if the frames parameter is not equal to 1.
*/
void AlpFrames::drawHorizontalLines(long frames, long hPad, long spacing, long lWidth) {
	try {
		if (frames != 1)
			throw std::invalid_argument("Error: `frames` must be 1.");
		for (int i = 0; i < _height / lWidth; i++) {
			if (i % 2 == 0)
				fillRect(frames - 1, 0, i * lWidth, _width, lWidth, 255);
			i += spacing;
		}
	}
	catch (const std::invalid_argument& e) {
		std::cerr << e.what() << std::endl;
		Pause();
		exit(1);
	}
}

/**
* @brief Draws a grid on the frames.
*
* The function takes 6 parameters, the number of frames, the vertical padding, the horizontal padding,
* the vertical spacing between lines, the horizontal spacing between lines, and the width of the lines.
* It calls the method drawVertialLines() and drawHorizontalLines() to draw the grid on the frames.
*
* @param frames The number of frames to draw the lines on.
* @param vPad The vertical padding of the lines.
* @param hPad The horizontal padding of the lines.
* @param vSpacing The vertical spacing between lines.
* @param hSpacing The horizontal spacing between lines.
* @param pWidth The width of the frames.
* @param pHeight The height of the frames.
* @param lWidth The width of the lines.
*
* @note The method requires a frame count of exactly 1.
*/
void AlpFrames::drawGrid(long frames, long vPad, long hPad, long vSpacing, long hSpacing, long lWidth) {
	drawVertialLines(frames, hPad, hSpacing, lWidth);
	drawHorizontalLines(frames, vPad, vSpacing, lWidth);
}

/**
* @brief Draws a checkerboard pattern on the projector screen
*
* This method takes in a number of frames, a vertical and horizontal padding, and the size of each square as input. It then uses these inputs to draw a checkerboard pattern on the projector screen.
*
* @param frames Number of frames to be generated
* @param vPad Vertical padding of the checkerboard pattern
* @param hPad Horizontal padding of the checkerboard pattern
* @param sqSize Size of each square of the checkerboard pattern
*
* @return None
*/
void AlpFrames::drawCheckerBoard(long frames, long vPad, long hPad, long sqSize) {
	for (int i = 0; i < _height / sqSize; i++) {
		for (int j = 0; j < _width / sqSize; j++) {
			if (j%2 == 0 && i%2 == 0 || j % 2 == 0 && i == 0)
				drawSquare(frames, hPad + i * sqSize, vPad + j * sqSize, sqSize);
			else if (j%2 != 0 && i%2 != 0)
				drawSquare(frames, hPad + i * sqSize, vPad + j * sqSize, sqSize);
		}
	}
}