
#include <iostream>
#include <string>
//------------------Header-------------------
// declarations, method names, etc. no code!!

/*  reads a PPM image file and a checksum file
    calculates row sums of the image pixels and comapares them
    to the checksums to verify the image 

*/

class Image{
    

    public:
    Image();
    bool loadImg(const std::string& Imgfile);
        //loads image data from the file
        //returns true upon success; false otherwise

    bool open_checkSum(const std::string& CSfile);
        // opens and reads checksum vals from file
        // returns true on success- false otherwise

    int validCsums(const std::string& ImgFile, const std::string& CSfile);
        //Calculates row sums and compares to checksums
        //returns 0 on success or neg otherwise
        //error first row where error occurs

    private:
    int rows, cols, max, checksum, pixValue;
  

};
   