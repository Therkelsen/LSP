
#include "Projector.h"
#include <iostream>

const long frames = 1;
const long spacing = 4;
const unsigned long pictureTime = 10000;
const long brightness = 100;

int main() {
	Projector P;
	P.generatePattern(frames, spacing, pictureTime, brightness);
}