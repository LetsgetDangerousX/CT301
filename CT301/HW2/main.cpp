#include "Image.h"
#include <iostream>
#include <fstream>

//----------------------MAIN------------------------------
// if not exactly 3 args, print error
int main(int argc, char* argv[]) {
        if (argc != 3) {
            std::cerr << "Error: must enter two file names" << std::endl;
            return -1;
        }

        Image img;
        if(!img.loadImg(argv[1])) {return -1;}
        if(!img.open_checkSum(argv[2])) {return -1;}
        return img.validCSsums();
}
