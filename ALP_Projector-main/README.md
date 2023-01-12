# ALP Projector
This is a project for LSP, wherein we are to R&D an interface for a projector, and further down the line a camera as well.

The project with pre-existing code was handed to us by Madav, who took sample code and made it work with the projector. Our goal is to refactor the projector code into a class and improve it, from which you can easily interface with our or any projector from the same brand of projectors.

## Goals
### Refactor
- [x] Move everything to a Projector class
- [x] Generalize the methods to allow for intercompatibility
- [x] Make hardcoded variables into member variables with get/set methods
- [x] Finding an alternative to memset, for programming patterns
- [x] Add good comments / fix the ones that arent
- [x] Get familiar with the drawing class (AlpFrames) and the use of memset
- [ ] Refactor AlpFrames class (Rename everything, remove inheritance, move drawing out of constructor, remove nonsense _ASSERTs)
- [ ] Add projector patterns that seem useful (i.e. Vertical lines, checkerboard, grids, etc.)
- [ ] Add OpenCV integration (convert image to grayscale image object, image objects to 1d array for memset)
- [ ] Prepare for front-end integration, i.e. GUI's etc.
- [ ] Remember good comments!