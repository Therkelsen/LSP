#include "AlpFrames.h"
#include <crtdbg.h>
#include <memory.h>
#include <stdio.h>
#include <typeinfo>
#include "Windows.h"
CAlpFrames::CAlpFrames(const long nFrameCount, const long nWidth, const long nHeight, long space) :	m_nFrameCount(nFrameCount), m_nWidth(nWidth), m_nHeight(nHeight), m_pImageData(new char unsigned[nFrameCount*nWidth*nHeight])
{
	printf("\nIn AlpFrames -> Constructor");
	/*_ASSERT (nFrameCount>0 && nWidth>0 && nHeight>0);
	_ASSERT (m_pImageData!=NULL);*/
	// Initialize image data as solid black
	memset(m_pImageData, 0, nWidth*nHeight);
}

CAlpFrames::~CAlpFrames(void)
{
	printf("\nIn AlpFrames -> Deconstructor");
	//memset(m_pImageData, 1, m_nWidth * m_nHeight);
	//ZeroMemory(m_pImageData, m_nWidth * m_nHeight);
	//free(m_pImageData);
	//delete[] m_pImageData;
}

CAlpFramesMovingSquare::CAlpFramesMovingSquare(const long nFrameCount, const long nWidth, const long nHeight, long space) :	CAlpFrames(nFrameCount, nWidth, nHeight, space)
{
	// Move a bright square from top-left to bottom-right corner of the DMD.
	// (this also visualizes flipped output of projection optics )
	printf("\nIn AlpFramesMoving Square -> Constructor");
	printf("\nnHeight: %ld", nHeight);
	const long nSquareWidth = nWidth;
	const long nSquareHeight = nHeight;
	FillRect(0, 0, 0, nSquareWidth, nSquareHeight, 255, space);	// bright
	//for (long nFrame = 1; nFrame <= nFrameCount; nFrame++)
	//{
	//	printf("\nnFrame: %ld", nFrame);
	//	FillRect(nFrame, 0, 0, nSquareWidth, nSquareHeight, 255);	// bright
	//}
}


char unsigned* CAlpFrames::operator()(const long nFrameNumber)
{
	printf("\nIn Operator");
	return m_pImageData;//+nFrameNumber*m_nWidth*m_nHeight;
}

char unsigned& CAlpFrames::at(const long nX, const long nY)
{
	const long data = (nX * nY);
	return m_pImageData[data]; //nFrameNumber*m_nWidth*m_nHeight+nY*m_nWidth+nX
}

void CAlpFrames::FillRect(const long nFrameNumber, const long nLeft, const long nTop, const long nRectWidth, const long nRectHeight, const char unsigned PixelValue, long space)
{
	printf("\nIn FillRect");
	const long pixel = nRectHeight;
	printf("\nCalling at");
	for (long nX = 0; nX <= nRectHeight; nX += space)
	{
		memset(&at(nRectWidth, nX), PixelValue, pixel);
	}		
}