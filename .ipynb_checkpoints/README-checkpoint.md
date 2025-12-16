# simpix

C++ starter code
* simpix_start.cpp
use make to build this example

Usage: simapix_start image1 image2 <output=out.png>

Python starter code
* simpix_start.py

Usage: simapix_start image1 image2 <output=out.png>


To build and run first type "g++ simpix.cpp -o simpix `root-config --cflags --libs` -lASImage" to build. Then ./simpix first_image second_image outputfile.

The output file shows the two images on top then under each image is using it's pixels to make the other image.

collage is the fribse and rotunda pictures together (640x480 pixels).
Monet is the two Monet pictures together (2202x1748 pixels).
56pic take two images from the class home page (1024x1024 pixels). 

collage and 56pic take ~2 minutes. Monet takes 3-5.