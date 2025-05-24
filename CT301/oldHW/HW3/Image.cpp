#include "Image.h"
#include <iostream>
#include <fstream>
#include <vector>


//Default Constructor
Image::Image() : rows(0), cols(0), max(0) {pixels.clear();}

//---------- Attempts: Opening Files and Loading Image Data---------------------

bool Image::loadImage(const std::string& filename){
std::ifstream file(filename);
if(!file.is_open()) {
    std::cerr<< "ERROR: unable to open image file" << std::endl;
    return false; 
}

//-------------------P3 header (PPM)---------------------------------

std::string format;
file>> format;

if(format != "P3") 
    {std::cerr << "ERROR: invalid ppm format." << std::endl;
    return false;}

//--------------------Read Image dimensions, max value & validate ------------

file >> cols >> rows >> max;

//Validation
if(rows <=0 || cols <= 0 || max <=0) 
    {std::cerr << "ERROR: Invalid image dimensions or max val" << std::endl;
    return false;}


//resize to hold all pixel vals
pixels.resize(rows * cols * 3);

    // read into vector
    for(int i = 0; i < rows * cols * 3; ++i) 
            {if (! (file >> pixels[i])) 
                {std::cerr << "ERROR: Invalid pixel data in file" << std::endl;
                return false;} }

    return true;

} // end img loading
//------------------------Function to compute checksums--------------------

//return vector of int for each checksum of the row
// set all to 0 to store row-wise checksum values
std::vector<int> Image::calcRowCheckSums() const 
    {std::vector<int> rowSums(rows, 0);
        for(int r = 0; r < rows; ++r) 
            {for(int c = 0; c < cols * 3; ++c) 
                 {rowSums[r] += pixels[r* (cols * 3) + c]; }}
                        return rowSums;}


// Columns
std::vector<int> Image::calcColCheckSums() const 
{std::vector<int> colSums(cols, 0);
   

    for(int c = 0; c < cols; ++c)
        {for(int r = 0; r < rows; ++r)
       
            {colSums[c]+= pixels[(r * cols + c) * 3];
            colSums[c]+= pixels[(r * cols + c) * 3 + 1];
            colSums[c]+= pixels[(r * cols + c) * 3 + 2];}}
                     return colSums;}     
                           
                        

//------------------------------------Saving Checksums to files-------------------------------

bool Image::saveChecksums(const std::string& rowFile, const std::string& colFile) const {

std::vector<int> rowSums = calcRowCheckSums();
std::vector<int> colSums = calcColCheckSums();

// for writing rows
std::ofstream writeRow(rowFile);

if(!writeRow.is_open())    
{std::cerr<<"ERROR: Unable to open file" << rowFile << " for writing row checksums" << std::endl;
return false;}

for(int sum : rowSums) {writeRow << sum << std::endl;}
writeRow << std::endl;
writeRow.close();


// for writing columns
std::ofstream writeCol(colFile);

if(!writeCol.is_open()) 
{std::cerr << "ERROR: Unable to open file" << colFile << " for writing column checksums" << std::endl;
return false;}


for (int sum : colSums) {writeCol << sum << " ";}
writeCol << std::endl;
writeCol.close();

return true;
}
