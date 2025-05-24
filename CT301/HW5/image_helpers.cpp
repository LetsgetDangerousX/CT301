#include "image.h"

//-----------------------------------HELPER FUNCTIONS---------------------------------
// helper function to assist in the P format and creation of new image,
 // cleaner than making new objects in main


// determine which image were working with and make it
Image* newImage(const std::string& format){
    if(format == "P3") return new ColorImage();
    if(format == "P2") return new GrayScaleImg();
    if(format == "P1") return new BinaryImage();

    return nullptr; 
}

 std::string getExtension(const std::string& filename){
    size_t position = filename.find_last_of('.');
    if(position == std::string::npos){return "";} //no ext found
            return filename.substr(position);
}
        
void Image::normalize(){}