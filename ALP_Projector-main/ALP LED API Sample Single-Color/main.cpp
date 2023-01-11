
#include "Projector.h"
#include <iostream>

const long frames = 1;
const long spacing = 4;
unsigned long pictureTime = 200000;
long brightness = 100;

int main() {
	Projector P; std::cout << "Constructor called" << std::endl;
	P.printParameters(P.getImageDataParams());
	P.printParameters(P.getSequenceParams());
	P.printParameters(P.getTimingParams());

	P.generatePattern(frames, spacing, pictureTime, brightness);
}