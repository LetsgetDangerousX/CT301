#include "Image.h"
#include <iostream>
#include <string>
#include <fstream>

/*Functions needed:
    OPEN image file ; check validity : return true; if false return Error message
    read image header; extract rows columns and pixel value
    read image data; compute row sums
    store image data 

    read checksum file
    store values in a checksum vector
    validate image with comparison 
    */

    



//---------- Attempts: Opening Files and Loading Image Data---------------------

bool Image::loadImg(const char* imgFile) {      //opens image file for read
    std::ifstream file(imgFile);

            if(!file.is_open()) //Check if the file opened
            {std::cerr << "ERROR: unable to open image file" << imgFile << std::endl;
            return false;} 

//-------------------Reading for P3 header (PPM)---------------------------------
char letter1, letter2;
file >> letter1 >> letter2;
            if(letter1 != 'P' || letter2 != '3') {
                std::cerr << "Error: invalid image file" << std::endl;
                return false;
            }

//-------------------Reading Dimsions and Max Pixel Value------------------------
file >> cols >> rows >> max;
if (rows <= 0 || cols <= 0 || max <= 0 ) {
    std::cerr << "ERROR: Invalid image dimensions or max value" << std::endl;
    return false;
}
return true;
}


//---------------------------------checkSums--------------------------------------

//Open the checksum file to read
bool Image::open_checkSum(const char* CSfile) {
    std::ifstream file(CSfile);

        // check if open
        if(!file.is_open()) {
            std::cerr << "Error: unable to open checksums" << CSfile << std::endl;
            return false;
        }
        

        // read checksum vals from file until end of file is reached
        while (file >> sumValue) {
            csCount++; 
            } 
    
    
        // check if number of checksums match number of rows in the image
        if (csCount != rows) {
            std::cerr << "Error: mismatch : image rows and checksums" << std::endl;
        return false;
        }

return true; //success
}

//----------------------------Checksums Validation Processing---------------

int Image::validCSsums() {

        std::ifstream imgFile("Image.ppm");

        if(!imgFile.is_open()){
            std::cerr << "ERROR: unable to open image file" << std::endl;
            return -1;
        }
    
    char letter1, letter2;
    imgFile >> letter1 >> letter2;
    imgFile >> cols >> rows >> max;
    int rowChecksum= 0; // to hold value

    // loop for each row
    for (int r = 0; r < rows; ++r){ 
        int sum =0;
        
        //loop through each column
        for(int c = 0; c < cols; ++c) {

            //loop through each RBG vals for curr pix
            for(int rgb =0; rgb < 3; ++rgb) {
                    imgFile >> pixVal;
                    if(pixVal < 0 || pixVal > max) {
                        std::cerr << "ERROR: invalid pixel val in image" << std::endl;
                        return -1;
                    }
                    sum += pixVal;
        }
    }
        //Compare sum with stored checksum
        if(sum != rowChecksum) {
            std::cerr << "Error: MisMatch : Row " << r << "checksum. Sum: " << 
            sum << ", correct sum: " << rowChecksum 
            << std::endl;
        return -(r + 2); //return neg for mismatch & row
        }
    }
    // return 0 & print success :)
     std::cout << "Success" << std::endl;
     return 0;
}