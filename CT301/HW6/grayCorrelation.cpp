#include "grayCorrelation.h"
#include <fstream>  // read files
#include <sstream>  // for text parsing
#include <cmath>    // sqrt()


/* using the Pearson Correlation Formula this program will 
compare two grayscale images and provide the result

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

    // resize pixels vector
    pixels.resize(height, std::vector<int>(width));
    // pixels will have height outer vectors and
    // an inner vector with width elements
    // default initialized to zero

    // iterate pixel values r and c  to 2D vector
    for(int r = 0; r < height; ++r) {
        for(int c = 0; c < width; ++c) {
            inFile >> pixels[r][c];
        }
    }
return true;}



int Image::getWidth() const {return width;}


int Image::getHeight() const {return height;}


double Image::getPixels(int r, int c) const { return pixels[r][c];}
// using double in case math is needed later

double Image::getAverage() const {
    double sum = 0.0; // initialize sum to double 0.0
    
   for(const std::vector<int>& row : pixels) {
    for (int val : row) {
        sum += val;
    }
   }

    return sum / (width * height);}


double Image::compute(const Image& A, const Image& B){

    /*Understand the formula :: A breakdown 
    r = correlation coefficient (-1 >> 1)
    Ai = pixel value at position i in Image A
    average pixel values of images
    Bi = pixel value at position i in image B
    B = average pixel value of B
    Σ = sum across all pixels (each matching pair of A and B)

    ***(A- A_) =>  A.getPixels(r,c) - average_A and the same for B

        
            Σ (Ai - A_) * (Bi - B)
    r = _________________________________
        sqrt(Σ (Ai - A_)^2 * Σ(Bi - B)^2)

    mean centering:
        subtract average from each pixel val 
        this is how much darker or lighter the pixel is compared to average
        makes avg of all centered values zero
    numerator:
        multiply 2 centered values for each pixel 
        add them all together
            if A and B are bright/dark in same spots => large positive #
            if A is bright where B is dark => negative number
    denominator:
        scales result between -1 & 1
        sqaure each centered value
        add separately
        sqrt of each sum then multiply square roots

        simple

        Final results:
        r =  1.0 =>  perfect positive correlation :: images match in brightness patterns
        r =  0.0 =>  no correlation => no relationship between patterns
        r = -1.0 =>  perfect negative correlation :: images are opposites

        so its going to subtract the average from each pixel it find to see how much it stands out
        then it will multiply those differences between the images
        if they go up and down together its positive
        if one goes up while the other goes down its negative
        the final score tells similarity between patterns

        in my mind, i see it as a set of scales like a seesaw
        
    

    */  

    // check size matches
    if (A.width != B.width || A.height != B.height) return -1;

    // get average of both images using getAverage()
    // correlation is comparing patterns of light/dark 
    // "is the pixel ligher or darker than average"
    double average_A = A.getAverage();
    double average_B = B.getAverage();

    // split up the parts needed for the Pearson correlation formula
    // my numerator will accumulate the co-variation between A and B
    // the 2 denominators will measure self-variation of both images
    double numerator = 0.0;
    double denominator_A = 0.0;
    double denominator_B = 0.0;

    // Double loop through each pixel subtracting average 
    // Iterator in java too complicated in C++ haha
    // using for loop
    // images required same dimensions so I only need to use one image dimension 
    // for the loop limit
    for ( int r = 0; r < A.height; ++r) {
        for ( int c = 0; c < A.width; ++c) {

            // these values a,bPix will house how much A[r][c] & B[r][c]
            // differ from their average brightness
            // apparently this is called mean-centering the data haha :D

            double aPix = A.getPixels(r,c) - average_A;
            double bPix = B.getPixels(r,c) - average_B;
   

    // build numerator and two denominators

        // numerator Σ (Ai - A_) * (Bi - B) => sum of (aPix * bPix)
        /*recap : 
        multiply 2 centered values for each pixel 
        add them all together
            if A and B are bright/dark in same spots => large positive #
            if A is bright where B is dark => negative number*/

        numerator += aPix * bPix;

        // denominator_A:
        /* recap:
        denominator:
        scales result between -1 & 1
        sqaure each centered value
        add separately
        sqrt of each sum then multiply square roots

        sqrt(Σ (Ai - A_)^2 * Σ(Bi - B)^2)
        sqrt(Σ (aPix)^2 * Σ(bPix)^2))
        for denominator_A  : just (aPix)^2
        for debnominator_B : just (bPix)^2

            += in the numerator and += in the denominator
            within the loop should automatically sum the values for both ends
        */

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
}