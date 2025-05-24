#include <iostream>
#include "Image.h"

int main(int argc, char* argv[]){
    if(argc != 4) {
        std::cerr << "ERROR: must provide 3 files" << std:: endl;
         return -1;
    }

    //create image object
    Image img;

    //load image
    if (!img.loadImage(argv[1])) {
        return -1;
    }

    //save row and col sums
   if(!img.saveChecksums(argv[2], argv[3])) {
    return -1;
   }

    std::cout << "Computation of checksum complete; files created." << std::endl;
    return 0;
}