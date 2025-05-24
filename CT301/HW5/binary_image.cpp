
#include "image.h"
#include <iostream>
#include <fstream>
#include <cmath>

//-------------------Binary Image Methods--------------------------


bool BinaryImage::read(std::ifstream& file) {

    /*
    header: P1
    width
    height
    max value ignored for p1
    width  * height values of eitehr 0 or 1
    not whitespace-separated, not packed bits*/

    //1 read width height and ignored maxValue

        if(!file){ std::cerr << "ERROR: Input file not opened" << std::endl; return false;}
        int ignored;
        file >> width >> height >> ignored; // ignoring max value

        
    //2 validate width and height are positive

        if(width <= 0 || height <= 0) return false;

    //3 resize pixels as vector <vector><RGB>> using height x width

        pixels.resize(height, std::vector<RGB>(width));

    //4 loop thru each row i and col j
        for(int i = 0; i < height; ++i){
            for (int j = 0; j  < width; ++j){

             // read one int bit      
                int bit;
                if(!(file>>bit))return false;

            // check if bits is 0 or 1
                if(bit != 0 && bit != 1) return false;

             // if valid, store RGB bit (black or white)
                int val = bit == 1 ? 255 : 0;
                pixels[i][j] = {val, val, val};
            }
        }
         return true;

}


// Call the convertTo based on ext
Image* BinaryImage::convertToExtension(const std::string& ext) {
    if(ext == ".ppm") return this;
    if(ext == ".pgm") return convertTo(".pgm");
    if(ext == ".pbm") return convertTo(".pbm");
    return nullptr; // unsupported extension 
}


Image* BinaryImage::convertTo(const std::string& ext) {
    
        
    // binary to color
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
                int val = pixels[i][j].r;
                gray->pixels[i][j] = {val, val, val};
                }
            }
            return gray;
    }
    return nullptr;
    }


    



bool BinaryImage::write(const std::string& filename) {
    std::ofstream file(filename);
    if(!file) return false;
    
    // write header first
    file << "P1\n" << width << " " << height << "\n";

    // write pixel values
    for(int i = 0; i < height; ++i) {
        for(int j = 0; j < width; ++j){
        int bit = pixels[i][j].r == 255 ? 1 : 0;
            file << bit;
        //add line breaks and double spaces to fit formatting specifications
        if(j < width - 1)  file << " ";
        }
        file << "\n";}
    
    
    return true;
    }