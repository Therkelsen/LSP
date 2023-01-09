
#include "Projector.h"

const long frames = 1;
const long spacing = 4;
unsigned long pictureTime = 200000;

int main() {
	Projector P;
	P.initializeProjector();
	P.initializeImageData(frames, spacing, pictureTime);
	P.initializeLED(100);
}