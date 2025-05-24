
/*

THINK IT OUT!
IF YOU CAN THINK IT YOU CAN CODE IT!

IDEATION & LOG - BY AMBER FERRELL <3 PROGRAMMING ASSIGMENT 5
*************************************************************
IMAGE CONVERTER     C++     BRUCE DRAPER        SEMESTER 2


Understanding:

3 types of images:      Per Pixel:                  Range:
Color       PPM/P3      3 Values RGB                [0-maxVal]
Grayscale   PGM/P2      1 Value (intensity)         [0-maxVal]
Binary      PBM/P1      1 bit                       (0 or 1)

Each type has width, height and maxVal

**********Tasks & Planning:************

1 > read PPM PGM OR PBM IMAGE
2 > identify image file by header
3 > if:
     (image is color image or grayscale)
     normalize()
     
4 > write it in format specified by output filename::
            if (input format != output format)
                convert the image to a different type!
            if else(input file format == output format)
                normalize and write as is

            if: //gray to color
                (input file == PGM && output file ends in PPM)
                convertToPPM()
                normalize()
                write PPM file

            if else: // gray to binary
                (input file == PGM && output file ends in PBM)
                convertToPBM()
                write PBM file

            if else: // color to gray
                (input file == PPM && output file ends in PGM)
                convertToPGM()
                normalize()
                write PGM file

            if else: //color to binary
                (input file == PPM && output file ends in PBM)
                convertToPBM()
                write PBM file
            
            if else: // binary to gray
                (input file == PBM && output file ends in PGM)
                convertToPGM()
                normalize()
                write PGM file
            if else: // binary to color
                (input file == PBM && output file ends in PPM)
                convertToPPM()
                normalize()
                write PPM file
            
A base class : Image.cpp 

class Image {
public: read bool, write bool, normalize(), Image* convertTo, ~Image() {}
}

then child classes:
    ColorImg : public Image {};
    GrayscaleImg : public Image {};
    BinaryImg : public Image {};


Layout:

|--- main.cpp               << reads input/output file names, controls flow
|--- image.h                << base class + derived class declarations
|--- image_types.cpp              << Implementations of the classes, functions



image.h:

thoughts about using virtual:

if read() is marked virtual it will call the ColorImage version of read()
if read() is not virtual then it will call the Image version even if its actually a color image. 
virtual will tell c++ to use the real objects function even if its called through a base class pointer *


*/


// Main: reads input/output file names, controls flow

#include "image.h"
#include <iostream>
#include <fstream>

int main(int argc, char* argv[]) {

    //1. Parse and check arguments------------------------------------------

        if(argc != 3) { // validate args
            std::cerr << "Usage: " << argv[0] << " <input_file> <output_file>"<< std::endl;
        return -1; }

        std::string inputFile = argv[1];
        std::string outputFile = argv[2];

    
    //2. open input file-------------------------------------------------
        std::ifstream input(inputFile);
        if(!input){
            std::cerr << "ERROR: Cannot open input file. \n ";
            return -1;
        }

    //3. read first line >> decide image type------------------------------
        std::string format;
        input >> format;
        if(format != "P3" && format != "P2" && format != "P3")
            {std::cerr << "ERROR: unsupported image format\n";
            return -1;}
            
    //4. newImage() to create correct object------------------------------
        Image* image = newImage(format);
        if(!image) {
            std::cerr << "ERROR: unsupported image format. \n";
            return -1;
        }

    //5. read()----Loading image data to memory----------------------------
        if(!image->read(input)){
            std::cerr << "ERROR: failed to read input image data. \n";
            delete image;
            return -1;
        }
       

    //6. normalize() if p2 or p3-------------------------------------------
        if(format != "P1") {
            image->normalize();
        }

    //7. convertTo() if file ext differ----------------------------------

        // find the ext
        std::string ext = getExtension(outputFile);

        // use ext to determine conversion
        Image* output = image->convertToExtension(ext);

        if(!output){std::cerr << "ERROR: invalid or unsupported output extension \n";
            delete image;
            return -1;
        }

    //8. write()-------------------------------------------------------
       if(!output->write(outputFile)) {
        std::cerr << "ERROR: failed to write output file\n";
        delete output;
        return -1;
       }
    
    
    //9 clean memory
    if(output != image) delete image;
        delete output;
        return 0;
}