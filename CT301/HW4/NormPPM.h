/*
PPM Normalizing
-program that reads a PPM image format: P3 
-normalizes its intensity values
-writes normalized image to output file

read ppm image - validate format
determine min and max pix vals in image
normalize pix vals with formula:
value_output = round((value_input - min) * 225/ (max-min))    
write normalized img to output file

prog handles errors:
invalid format, file access issues, improper values: return -1 for all errors
*/

#ifndef NormPPM_H
#define NormPPM_H

#include<vector>
#include<string>

//Read PPM image from file & store pixel data
int readPPM(const 
     std::string& filename,
     std::vector<int>& red,
     std::vector<int>& green,
     std::vector<int>& blue, 
     int& width, 
     int& height, 
     int& maxValue);

//Normalize pixel values to scale between 0 and 255
int normalizeImg(
    std::vector<int>& red, 
    std::vector<int>& green, 
    std::vector<int>& blue);

//Write normalized PPM img to file
int writePPM(
    const std::string& filename,
    const std::vector<int>& red,
    const std::vector<int>& green,
    const std::vector<int>& blue,
    int width,
    int height);

#endif // NormPPM_H