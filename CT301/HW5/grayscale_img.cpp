
#include "image.h"
#include <iostream>
#include <fstream>
#include <cmath>

//-------------------GrayScale Image Methods--------------------------


bool GrayScaleImg::read(std::ifstream& file) {
    
    file>>width>>height>>maxValue;
    if( width <= 0 || height <= 0 || maxValue <= 0) return false;

    pixels.resize(height, std::vector<RGB>(width));

for(int i = 0; i < height; ++i){
    for (int j = 0; j  < width; ++j){
        int intensity;
        if(!(file >> intensity)) return false;
        if ( intensity < 0 ||
             intensity > maxValue)
             return false;
        pixels[i][j] = {intensity, intensity, intensity};
    }
}
return true;
}



//------------------------normalization-----------------

void GrayScaleImg::normalize(){

    // initialize min and max values 
    int minVal = 255, maxVal = 0; 
 
    // find min and max RGB values with new RGB struct
    for (int i = 0; i <height; ++i){
        for(int j = 0; j < width; ++j){
        int val = pixels[i][j].r;

        //min
        if(val < minVal) minVal = val;
        
        //max
        if(val > maxVal) maxVal = val;
        
        }
    }

    // check for zero contrast

    if(maxVal == minVal){std::cerr << "ERROR: No contrast in image" << std::endl; return;}
    
    // normalize each pix value
    for (int i = 0; i <height; ++i){
        for(int j = 0; j < width; ++j){

        int val = pixels[i][j].r;

        pixels[i][j].r = pixels[i][j].g = pixels[i][j].b =
        static_cast<int>(std::round((val - minVal) * 255.0 / (maxVal - minVal)));

        // new clamp for 0-255
        int norm = std::max(0, std::min(255, norm));

        pixels[i][j].r = pixels[i][j].g = pixels[i][j].b = norm;
    
        }
    }
}

// Call the convertTo based on ext

Image* GrayScaleImg::convertToExtension(const std::string& ext) {
    if(ext == ".ppm") return this;
    if(ext == ".pgm") return convertTo(".pgm");
    if(ext == ".pbm") return convertTo(".pbm");
    return nullptr; // unsupported extension 

}
Image* GrayScaleImg::convertTo(const std::string& ext) {

 // color image conversion:
 if(ext == ".ppm") {
        
    //create new color image
    ColorImage* color = new ColorImage();
    color->width = width;
    color->height = height;
    color->maxValue = 255;
    color->pixels.resize(height, std::vector<RGB>(width));

    // color .ppm 
    for (int i = 0; i <height; ++i){
        for(int j = 0; j < width; ++j){
        int val = pixels[i][j].r;
        
        color->pixels[i][j] = {val, val, val};
        }
    }
    return color;
}
// binary image conversion
if(ext == "pbm"){
     //create new binary image
    BinaryImage* bin = new BinaryImage();
    bin->width = width;
    bin->height = height;
    bin->maxValue = 1;
    bin->pixels.resize(height, std::vector<RGB>(width));

    // Binary .pbm but = 1 if (r + g + b) > 382.5, else 0
    for (int i = 0; i <height; ++i){
        for(int j = 0; j < width; ++j){
            int intensity = pixels[i][j].r;
            int bit = (intensity > 127) ? 255 : 0;
            bin->pixels[i][j] = {bit, bit, bit};
            }
        }
        return bin;
  
}
return nullptr; // unsupported extension

}

// write the image file
bool GrayScaleImg::write(const std::string& filename) {
    std::ofstream file(filename);
    if(!file) return false;
    
    // write header first
    file << "P2\n" << width << " " << height << "\n255\n";

    // write pixel values
    for(int i = 0; i < height; ++i) {
        for(int j = 0; j < width; ++j){
        int intensity = pixels[i][j].r;
        file << intensity;

        //add line breaks and double spaces to fit formatting specifications
        if(j < width - 1) file << " ";
        }
        file << "\n";
    }
    
    return true;
}


