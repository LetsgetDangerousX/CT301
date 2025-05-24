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

Image::Image() : rows(0), cols(0), max(0), pixValue(0) {}

//---------- Attempts: Opening Files and Loading Image Data---------------------

bool Image::loadImg(const std::string& ImgFile) {      //opens image file for read
    std::ifstream file(ImgFile);

            if(!file.is_open()) //Check if the file opened
            {std::cerr << "ERROR: unable to open image file" << ImgFile << std::endl;
            return false;} 

//-------------------Reading for P3 header (PPM)---------------------------------
std::string p3;
file >> p3;
            if(p3 != "P3") {
                std::cerr << "Error: invalid image file" << std::endl;
                return false;
            }
//------------------Validating: dimensions and max pixel value--------------------

file >> cols >> rows>> max; //read img dim
            if(rows <= 0 || cols <= 0 || max <= 0) { // validate dim & max
                std::cerr << "Error: Img dimensions or max value invalid" << std::endl;
                return false;
            }

return true;

        }

//---------------------------------checkSums--------------------------------------

//Open the checksum file to read
bool Image::open_checkSum(const std::string& CSfile) {
    std::ifstream file(CSfile);

        // check if open
        if(!file.is_open()) {
            std::cerr << "Error: unable to open checksum" << CSfile << std::endl;
            return false;
        }

return true; 
}

//----------------------------Checksums Validation Processing---------------

int Image::validCsums(const std::string& ImgFile, const std::string& CSfile) {
    int rowSum = 0;
    int rowcheck; //storing here
    std::ifstream imgfile(ImgFile);

    if( !imgfile.is_open()) {
        std::cerr << "ERROR: unable to open image file" << std::endl;
        return -1;
    }

    std::ifstream csfile(CSfile);
    if( !csfile.is_open()) {std::cerr << "ERROR: unable to open checksum file for CSsums const" << std::endl; return -1;}
    
std::string p3;
imgfile >> p3 >> cols >> rows >> max;

    for (int r = 0; r < rows; r++) {
        rowSum = 0;

        // read pix data per pix
        for (int c = 0; c < cols; ++c) {
             int r, g, b;
             imgfile >> r >> g >> b;
             rowSum += r + g +b;
        }
    
        csfile >>rowcheck; // read checksum for row



        //Compare sum with stored checksum
        if(rowSum != rowcheck) {
            std::cerr << "Error: MisMatch : Row " << r << " the sum is: " << rowSum << ", but the checksum is: " << rowcheck
            << std::endl;
        return -(r + 2); //return neg for mismatch & row
        }
            
    }
        std::cout << "Success" << std::endl; return 0; 

}//end; success :)