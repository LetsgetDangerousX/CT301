#include "NormPPM.h"
#include <iostream>

int main(int argc, char* argv[]) {
    if(argc != 3) { // validate args
        std::cerr << "Usage: " << argv[0] << "<input_file> <output_file>"<< std::endl;
    return -1; }

    // store pixel values here
        std::vector<int> red, green, blue;

    // store image dimensions and max val here
        int width, height, maxValue;

    // -----error catch to return proper values:-----
       
    // read input file and validate => return -1
       if(readPPM(argv[1], red, green, blue, width, height, maxValue)!=0) {
        std::cerr << "ERROR: Problem reading the PPM file" << std::endl;
            return -1;
        }
    
        //normalize the image
        if(normalizeImg(red,green,blue) != 0) {std::cerr << "Normalization failure" << std::endl;
             return -1;}

    // write output file => return -1 
     if(writePPM(argv[2], red, green, blue, width, height)!=0) {
        std::cerr << "ERROR: Problem writing the output PPM file" << std::endl;
            return -1;
        }

        std::cout << "Success"<<std::endl;
        return 0; // successful run
}