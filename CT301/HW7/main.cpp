#include <iostream>
#include "grayCorrelation.h"

/**
 * @brief Main function for image correlation program for grayscale images in P2 format
 */

int main(int argc, char* argv[]) {

    // arg check
    if(argc != 3) {
        std::cerr << "To use: " << argv[0] << "image.pgm image2.pgm\n";
        return -1;
    }


    // name Image object
    Image A, B;

    // load check
    if (!A.load(argv[1]) || !B.load(argv[2])) {
        std::cerr << "ERROR: loading 2 images failed\n";
        return -1;
    }

    // call correlation computation
    double correlation = Image::compute(A, B);

    // -2 error detection - this is to avoid confusion with actual returned -1 values
    // if -2 is triggered then the dimension error has been triggered 
    if ( correlation == -2) {
        std::cerr << "ERROR: image dimensions or values differ\n";
        return -1;
    }

    // success print
    std::cout  << correlation << std::endl;
    return 0;
}// end of main