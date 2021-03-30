# CalibCamera_ReDev
Calibration and localisation of a camera in 360°


This repository is used for a student project at École Centrale de Nantes.

The project consists of researching ways to automatically calibrate and localise a 360° camera in space.
If successful, it will be used in a pedestrian localisation project.


Finally, **only the calibration part is working.**

## Calibration
How to use this program :
* Take pictures of a chessboard with the omnidirectionnal camera
* Put them in ```Calibration/Images/omnidir_images```.
* In the file ```Calibration/src/calibrationCamera.cpp```:
  * Fill the number of images in the line ```17```.
  * Fill the board size (number of corners on the board) line ```21```.
  * Fill the square size in millimeters line ```22```.
* Build with cmake and execute the program.
