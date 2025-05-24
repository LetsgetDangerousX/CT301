#include <vector>
#include <iostream>

//------------------Header-------------------
// declarations, method names, etc. no code!!

/*  reads a PPM image file and a checksum file
    calculates row sums of the image pixels and comapares them
    to the checksums to verify the image 

*/

class Image{
    
    public:
    bool loadImg(const char* Imgfile);
        //loads image data from the file
        //returns true upon success; false otherwise

    bool open_checkSum(const char* CSfile);
        // opens and reads checksum vals from file
        // returns true on success- false otherwise

    int validCSsums();
        //Calculates row sums and compares to checksums
        //returns 0 on success or neg otherwise
        //error first row where error occurs

    private:
    int rows;   
    int cols;       
    int max;        // max value
    int pixVal;     // temp var to store pixel values

    int sumValue;  // temp var for checksum value
   
    int csCount=0; // this should fix the error from autograder

 
};