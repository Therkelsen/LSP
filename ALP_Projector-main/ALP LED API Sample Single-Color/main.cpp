
#include "Projector.h"
#include <iostream>

const long frames = 1;
const long spacing = 4;
unsigned long pictureTime = 200000;
long brightness = 100;

int main() {
	Projector P; std::cout << "Constructor called" << std::endl;
	P.initializeProjector(); std::cout << "initializeProjector called" << std::endl;
	P.generatePattern(frames, spacing, pictureTime, brightness);
}