#include "NormPPM.h"
#include <iostream>
#include <fstream>
#include <cmath> 


/***********************************************
 * FILE READING AND VALIDATION
 * this section will handle all input operations:
 * file opening and validation
 * header parsing and verification
 * pixel data reading with error checking
 * memory management for pixel storage 
 * *********************************************
 */

int readPPM(const 
    std::string& filename,
    std::vector<int>& red,
    std::vector<int>& green,
    std::vector<int>& blue, 
    int& width, 
    int& height, 
    int& maxValue) {
        
        // open file
    std::ifstream file(filename);

    // file open error check
    if(!file) {std::cerr << "ERROR: Input file not opened."<<std::endl; return -1;}

    // file format error check
    std::string format;
    file >> format;
    if ( format != "P3") {std::cerr << "ERROR: Invalid PPM Format." << std::endl; return -1;}
    
    // error reading values
    if(!(file >> width >> height >> maxValue)) {
        std::cerr << "ERROR: Problem reading dimension or max value" << std::endl; return -1;}


    // image dimension/max val error check
    if(width <=0 || height <=0 || maxValue <= 0 || maxValue > 255) 
    {std::cerr << "ERROR: Invalid image dimension or max value," << std::endl; return -1;}


    int dim = width * height;
    // resize vector to store pix data
    red.resize(dim);
    green.resize(dim);
    blue.resize(dim);
  
/***********************************************
 * PROCESSING OF PIXEL DATA
 * this block will handle pixel reading logic using:
 * Sequential reading of red green blue 
 * Per pixel value validation
 * memory cleanup on error conditions ( that tripped me up hard!)
 * *********************************************
 */
   int pixelCount = 0;
   while(pixelCount < dim) {
    if(!(file >> red[pixelCount] >>
       green[pixelCount] >>
       blue[pixelCount]))
       {std::cerr << "ERROR: Problem reading pixel data" << std::endl; 
        red.clear(); green.clear(); blue.clear(); return -1;}

    // max exceeded error check
    if(red[pixelCount] > maxValue ||
       green[pixelCount] > maxValue ||
       blue[pixelCount] > maxValue ||
       red[pixelCount] < 0 || 
       green[pixelCount] < 0 || 
       blue[pixelCount] < 0)
        {std::cerr << "ERROR: Pixel Value exceeds max or is negative" << std::endl; 
        red.clear(); green.clear(); blue.clear(); return -1;}
    
        pixelCount++;
    } 

    // Error if excess pixels found
    int excess;
    if(file >>excess) { std::cerr << "ERROR: Excessive pixel data detected" << std::endl;
        //clean up on error
        red.clear();
        green.clear();
        blue.clear();
        return -1;}

    //If pixelcount doesnt meet expectations
    if(pixelCount != dim) {std::cerr<<"ERROR: Amount of data read: " << pixelCount << " expected value: " << dim << std::endl; 
        return -1;}

return 0; // success!!
    }

/***********************************************
 * IMAGE NORMALIZATION 
 * normalization algorithm that will:
 * Find actual data range in image
 * Apply normalization to data
 * Handle edge cases 
 * *********************************************
 */

int normalizeImg(
    std::vector<int>& red, 
    std::vector<int>& green, 
    std::vector<int>& blue){

// initialize min and max values 
int minVal = 255, maxVal = 0; 
/* looking for highest max and min values, 
setting min to 255 and max to 0 ensures any encountered 
pixel val will replace it if its greater or lesser 
than the intialized values*/

/***********************************************
 * NORMALIZATION  TRANSFORMATION
 * Apply the normalization formula
 * new value = round(oldval - min) * 255 / (max - min))
 * using std::round() returns double and we need to store as int
 *  using static_cast makes conversion explicit and checked at compile time
 * 
 * added clamping to ensure 0-255 range
 * *********************************************
 */
// find min and max values
for (size_t i = 0; i <red.size(); i++){
    //min:
    if(red[i] < minVal) minVal = red[i];
    if(green[i] < minVal) minVal = green[i];
    if(blue[i] < minVal) minVal = blue[i];
    //max
    if(red[i] > maxVal) maxVal = red[i];
    if(green[i] > maxVal) maxVal = green[i];
    if(blue[i] > maxVal) maxVal = blue[i];
}
// check for zero contrast
 if(maxVal == minVal){std::cerr << "ERROR: No contrast in image" << std::endl; return -1;}

// normalize each pix value
for (size_t i =0; i < red.size(); i++) {
    red[i] =   static_cast<int>(std::round((red[i] - minVal) * 255.0 / (maxVal - minVal)));
    green[i] = static_cast<int>(std::round((green[i] - minVal) * 255.0 / (maxVal - minVal)));
    blue[i] =  static_cast<int>(std::round((blue[i] - minVal) * 255.0 / (maxVal - minVal)));

    // keeping values in range:
    if(red[i] < 0) red[i] =0;
    if(red[i] >255) red[i] = 255;

    if(green[i] < 0) green[i] =0;
    if(green[i] >255) green[i] = 255;

    if(blue[i] < 0) blue[i] =0;
    if(blue[i] >255) blue[i] = 255;
}
return 0; // success!
    }
/***********************************************
 * OUTPUT FILE CREATION
 * Handle writing the final PPM File
 * file creation and validation
 * Proper PPM header formatting
 * Organized pixel data output
 * *********************************************
 */

int writePPM(
    const std::string& filename, 
    const std::vector<int>& red, 
    const std::vector<int>& green,
    const std::vector<int>& blue,
    int width,
    int height){

    
// open output file
std::ofstream file(filename);

// check if open with error msg
if(!file) { std::cerr << "ERROR: Output File not opened" << std::endl; return -1;}


/***********************************************
 * PIXEL DATA FORMAT
 * formats pixel data 
 * spacing between values and new lines:
 *  (i+1) % width !=0 
    *  if it reaches zero, weve reached the end of a row
    *  if it doesnt equal zero then we provide a space
    *  example:
    *  width = 5 i = 4 (5th pixel)
    * (4+1) % 5 = 0 => newline
    * (3+1) % 5 = 4 => space (at 4th pixel)
 * line breaks at row boundaries
 * Streaming output
 * *********************************************
 */
// write header first
file << "P3\n" << width << " " << height << " " << "255\n";

// write pixel values
for(size_t i = 0; i < red.size(); i++) {
    file << red[i] << " " << green[i] << " " << blue[i];

    //add line breaks and double spaces to fit formatting specifications
    if((i + 1) % width != 0) {
        file << " ";
    }else{file << "\n";}
}


return 0; 
// success!!
    }
