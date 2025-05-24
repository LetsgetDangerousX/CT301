#ifndef IMAGE_H
#define IMAGE_H

//base class + derived class declarations

#include <iostream>
#include <vector>
#include <string>
#include <fstream>

/*
FINAL LAYOUT:

|--- main.cpp               << reads input/output file names, controls flow, throws errors
|--- image.h                << base class + derived class declarations + struct for ease of use
|--- colorImage.cpp         << ColorImage method implementations
|--- image_helpers.cpp      << newImage(), getExtension(), etc.
|--- binary_image.cpp		<< Binary Image method implementations
|--- grayscale_Img.cpp		<< grayscale image method implementations

*/

// RGB value storage using struct
struct RGB {
    int r, g, b;


};



// abstract base
class Image {
    public:
    
        int width, height, maxValue;

        //read pixel data in object
        virtual bool read(std::ifstream& file)=0;

         // write image to file, return t/f upon success
        virtual bool write(const std::string& filename) = 0;

        // normalize: change pixel vals, modifies in place
        virtual void normalize();

        // convertTo: dynamically create new converted image and return pointer to an object of type Image
        virtual Image* convertTo(const std::string& ext) =0;

        // helper function for main : decides if conversion is needed
        virtual Image* convertToExtension(const std::string& outputExt){
            return this;
        }
        
        virtual std::string pformat() const = 0; // returns P3 or P2
        
        // virtual destructor
        // memory leak || undefined behavior prevention when deleting derived objects
        virtual ~Image() {}

};

// -----------children------------------
class ColorImage : public Image {
    
    public:

    std::vector<std::vector<RGB>> pixels;
    
    //read
    bool read(std::ifstream& file) override;
    // normalize
    void normalize() override;
    // convertTo
    Image* convertTo(const std::string& ext) override;
    // write
    bool write(const std::string& filename) override;
    // Extension conversion
    Image* convertToExtension(const std::string& outputExt) override;
    // return Pformat
    std::string pformat() const override{return "P3";}
};

// child class graryscale image
class GrayScaleImg : public Image {
    
    public:

    std::vector<std::vector<RGB>> pixels;
    
    //read
    bool read(std::ifstream& file) override;
    // normalize
    void normalize() override;
    // convertTo
    Image* convertTo(const std::string& ext) override;
    // write
    bool write(const std::string& filename) override;
    // Extension conversion
    Image* convertToExtension(const std::string& outputExt) override;
    // return Pformat
    std::string pformat() const override{return "P2";}
};
// child class binary image
class BinaryImage : public Image {
    
    public:

    std::vector<std::vector<RGB>> pixels;
    
    //read
    bool read(std::ifstream& file) override;
    // convertTo
    Image* convertTo(const std::string& ext) override;
    // write
    bool write(const std::string& filename) override;
    // Extension conversion
    Image* convertToExtension(const std::string& outputExt) override;
    // return Pformat
    std::string pformat() const override{return "P1";}
    

};

    // create image
Image* newImage(const std::string& format);

//helper function to get Extension
std::string getExtension(const std::string& filename);
#endif // IMAGE_H