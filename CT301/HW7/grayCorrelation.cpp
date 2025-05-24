#include "grayCorrelation.h"
#include <fstream>  // read files
#include <sstream>  // for text parsing
#include <cmath>    // sqrt()


/**
 * @brief Loads an image from a PGM filein P2 Format
 */
bool Image::load(const std::string& filename){

    // open file
    std::ifstream inFile(filename);

    // error check
    if(!inFile) return false;

    // P2 check
    std::string header;
    inFile >> header;
    if(header != "P2") return false;
    
   

    // read in width and height and maxval
    inFile >> width >> height;
    int max;
    inFile >> max;

    // fills 2D vector with pixel values - changed to unsigned char = 1 byte
    pixels.assign(height, std::vector<unsigned char>(width));
    // pixels will have height outer vectors and
    // an inner vector with width elements
  

    // iterate pixel values r and c  to 2D vector
    for(int r = 0; r < height; ++r) {
        for(int c = 0; c < width; ++c) {
            inFile >> pixels[r][c];
        }
    }
return true;
}

/**
 * @brief Computes the average brightness of each image
 *
 */
double Image::getAverage() const {
    double total = 0.0;
    for (const auto& row : pixels) {
        for(unsigned char val : row) {
        total += val;
        }
    }
    return total / (width * height);

/**
 * @brief Computes the Pearson Correlation Coefficient between two images
 */
double Image::compute(const Image& A, const Image& B){

    /*
        
            Σ (Ai - A_) * (Bi - B)
    r = _________________________________
        sqrt(Σ (Ai - A_)^2 * Σ(Bi - B)^2)


    */  

    // dimension mismatch
    if (A.width != B.width || A.height != B.height){ return -2;}

    // get average of both images using getAverage()
    // correlation is comparing patterns of light/dark 
    // "is the pixel lighter or darker than average"

    const int width = A.width;
    const int height = A.height;
    double average_A = A.getAverage();
    double average_B = B.getAverage();

    const auto& pixelsA = A.getPixelVector();
    const auto& pixelsB = B.getPixelVector();
    // split up the parts needed for the Pearson correlation formula
    // my numerator will accumulate the co-variation between A and B
    // the 2 denominators will measure self-variation of both images
    double numerator = 0.0;
    double denominator_A = 0.0;
    double denominator_B = 0.0;


    for ( int r = 0; r < height; ++r) {
        for ( int c = 0; c < width; ++c) {


            double aPix = pixelsA[r][c] - average_A;
            double bPix = pixelsB[r][c]- average_B;

            numerator += aPix * bPix;
            denominator_A += aPix * aPix;
            denominator_B += bPix * bPix;
}
    }
    // zero denominators will always result in an error
    if (denominator_A == 0 || denominator_B == 0) 
         return -2;

    // return correlation 
        /*
                    Σ (Ai - A_) * (Bi - B)
            r = _________________________________
                sqrt(Σ (Ai - A_)^2 * Σ(Bi - B)^2)
        */
    return numerator / (std::sqrt(denominator_A) * std::sqrt(denominator_B));
    
    //1.0 correlated 0.0 no pattern -1.0 mean brightness patterns are opposites
}


}