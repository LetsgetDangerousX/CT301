#include "image.h"
#include <iostream>
#include <fstream>
#include <cmath>

/*
    ColorImage Class:
    read width height and maxvalue
    parse all R G B pixel values
    store in a vector<vector<RBG>>

    boolean => main will check true/false and return -1 as required 
    for any error caught in class
*/


//--------------------------Color Image Methods--------------------------
bool ColorImage::read(std::ifstream& file) {
file >> width >> height >> maxValue;

if( width <= 0 || height <= 0 || maxValue <= 0) {return false;}

pixels.resize(height, std::vector<RGB>(width));

for(int i = 0; i < height; ++i){
    for (int j = 0; j  < width; ++j){
        int r, g, b;
        if(!(file >> r >> g >> b)) return false;
        if ( r < 0 || g < 0 || b < 0 ||
             r > maxValue || g > maxValue || b > maxValue)
             return false;
        pixels[i][j] = {r, g, b};
    }
}
return true;
}

//------------------------normalization-----------------

void ColorImage::normalize(){
 
    
    // initialize min and max values 
    int minVal = 255, maxVal = 0; 
 
    // find min and max RGB values with new RGB struct
    for (int i = 0; i <height; ++i){
        for(int j = 0; j < width; ++j){
        RGB& p = pixels[i][j];
        if(p.r < minVal) minVal = p.r;
        if(p.g < minVal) minVal = p.g;
        if(p.b < minVal) minVal = p.b;
        //max
        if(p.r > maxVal) maxVal = p.r;
        if(p.g > maxVal) maxVal = p.g;
        if(p.b > maxVal) maxVal = p.b;
    }
}
   
     // check for zero contrast

     if(maxVal == minVal){std::cerr << "ERROR: No contrast in image" << std::endl; return;}

    // normalize each pix value
    for (int i = 0; i <height; ++i){
        for(int j = 0; j < width; ++j){
        RGB& p = pixels[i][j];
        p.r =   static_cast<int>(std::round((p.r - minVal) * 255.0 / (maxVal - minVal)));
        p.g = static_cast<int>(std::round((p.g - minVal) * 255.0 / (maxVal - minVal)));
        p.b =  static_cast<int>(std::round((p.b - minVal) * 255.0 / (maxVal - minVal)));
    
        // keeping values in range:
        if(p.r < 0) p.r =0;
        if(p.r >255) p.r = 255;
    
        if(p.g < 0) p.g =0;
        if(p.g >255) p.g = 255;
    
        if(p.b < 0) p.b =0;
        if(p.b > 255) p.b = 255;
        }
    }
}


// -------------convert to correct output file extension--------------------------------

// Calling function
Image* ColorImage::convertToExtension(const std::string& ext) {
    if(ext == ".ppm") return this;
    if(ext == ".pgm") return convertTo(".pgm");
    if(ext == ".pbm") return convertTo(".pbm");
    return nullptr; // unsupported extension 
}

// conversion function
Image* ColorImage::convertTo(const std::string& ext) {
    
    // grayscale image conversion:
    if(ext == ".pgm") {
        
        //create new grayscale image
        GrayScaleImg* gray = new GrayScaleImg();
        gray->width = width;
        gray->height = height;
        gray->maxValue = 255;
        gray->pixels.resize(height, std::vector<RGB>(width));

        // grayscale .pgm intensity = round((r + g + b)/3)
        for (int i = 0; i <height; ++i){
            for(int j = 0; j < width; ++j){
            RGB& p = pixels[i][j];
            int average = static_cast<int>(std::round((p.r + p.g + p.b)/3.0));
            gray->pixels[i][j] = {average, average, average};
            }
        }
        return gray;
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
            RGB& p = pixels[i][j];
            int total = p.r + p.g + p.b;
            int value = (total > 382.5) ? 255 : 0;
            bin->pixels[i][j] = {value, value, value};
            }
        }
        return bin;
      
    }
    return nullptr; // unsupported extension
}

// write the color image to output file------------------------------
bool ColorImage::write(const std::string& filename) {

    std::ofstream file(filename);
    if(!file) return false;
    
    // write header first
    file << "P3\n"; 
    file << width << " " << height << "\n";
    file << "255\n";

    // write pixel values
    for(int i = 0; i < height; ++i) {
        for(int j = 0; j < width; ++j){
        file 
        << pixels[i][j].r << " " 
        << pixels[i][j].g << " " 
        << pixels[i][j].b;

        //add line breaks and double spaces to fit formatting specifications
        if(j < width - 1) {
            file << " ";
        }else{file << "\n";}
    }
    }
    return true;
}






