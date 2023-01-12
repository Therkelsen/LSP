#include "AlpFrames.h"
#include <crtdbg.h>
#include <memory>
#include <stdio.h>
#include <typeinfo>
#include "Windows.h"

CAlpFrames::CAlpFrames(const long nFrameCount, const long nWidth, const long nHeight)
	// shared pointer:
	: m_nFrameCount(nFrameCount), m_nWidth(nWidth), m_nHeight(nHeight),
	// uses `new` to allocate `nFrameCount * nWidth * nHeight` bytes of storage
	// hence, m_pImageData is a non-null pointer to the first byte of the block
	m_pImageData(new char unsigned[nFrameCount * nWidth * nHeight]) {
	// Weird macro definitions
	_ASSERT(nFrameCount > 0 && nWidth > 0 && nHeight > 0);
	_ASSERT(m_pImageData != NULL);
	// Initialize image data as solid black
	memset(m_pImageData, 0, nFrameCount * nWidth * nHeight);
}

CAlpFrames::~CAlpFrames(void) {
	//Deconstructor, for now just resets image to all black to avoid heap errors
	memset(m_pImageData, 0, m_nWidth * m_nHeight);
	//ZeroMemory(m_pImageData, m_nWidth * m_nHeight);
	//free(m_pImageData);
	//delete[] m_pImageData;
}

CAlpDraw::CAlpDraw(const long nFrameCount, const long nWidth, const long nHeight) : CAlpFrames(nFrameCount, nWidth, nHeight) {
	drawMovingSquare(nFrameCount, nWidth, nHeight);
}

char unsigned* CAlpFrames::operator()(const long nFrameNumber) {
	_ASSERT(nFrameNumber >= 0 && nFrameNumber < m_nFrameCount);
	return m_pImageData + nFrameNumber * m_nWidth * m_nHeight;
}

char unsigned& CAlpFrames::at(const long nFrameNumber, const long nX, const long nY) {
	_ASSERT(nFrameNumber >= 0 && nFrameNumber < m_nFrameCount
		&& nX >= 0 && nX < m_nWidth
		&& nY >= 0 && nY < m_nHeight);

	return m_pImageData[nFrameNumber * m_nWidth * m_nHeight + nY * m_nWidth + nX];
}

void CAlpFrames::FillRect(const long nFrameNumber, const long nLeft, const long nTop,
	const long nRectWidth, const long nRectHeight, const char unsigned PixelValue) {
	long const nBottom = nTop + nRectHeight - 1;
	_ASSERT(nFrameNumber >= 0 && nFrameNumber < m_nFrameCount);
	_ASSERT(0 <= nLeft && 0 < nRectWidth && nLeft + nRectWidth <= m_nWidth);
	_ASSERT(0 <= nTop && nTop <= nBottom && nBottom < m_nHeight);
	// row by row: set the area starting from nLeft to PixelValue
	for (long nY = nTop; nY <= nBottom; nY++)
		memset(&at(nFrameNumber, nLeft, nY), PixelValue, nRectWidth);
}

void CAlpDraw::drawMovingSquare(long nFrameCount, long nWidth, long nHeight) {
	// Move a bright square from top-left to bottom-right corner of the DMD.
		// (this also visualizes flipped output of projection optics )
	const long nSquareWidth = nHeight / 2,
		nTravelExtentX = nWidth - nSquareWidth,
		nTravelExtentY = nHeight - nSquareWidth;
	for (long nFrame = 0; nFrame < nFrameCount; nFrame++)
		FillRect(nFrame, nFrame * nTravelExtentX / (nFrameCount - 1), nFrame * nTravelExtentY / (nFrameCount - 1),
			nSquareWidth, nSquareWidth, 255);
}