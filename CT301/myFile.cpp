#include "NormPPM.h" // Include the header file for function declarations and required types
#include <iostream>   // For standard I/O operations
#include <fstream>    // For file stream operations
#include <cmath>      // For math functions like std::round

// Reads a PPM (P3 format) image file and stores the RGB pixel data in separate vectors
int readPPM(const std::string& filename, // Input file name
            std::vector<int>& red,       // Vector to store red channel values
            std::vector<int>& green,     // Vector to store green channel values
            std::vector<int>& blue,      // Vector to store blue channel values
            int& width,                  // Output image width
            int& height,                 // Output image height
            int& maxValue) {             // Output max value allowed per channel

    std::ifstream file(filename); // Attempt to open the input file

    if (!file) { // Check if file failed to open
        std::cerr << "ERROR: Input file not opened." << std::endl;
        return -1; // Return -1 for failure
    }

    std::string format; // Variable to store format header (should be P3)
    file >> format; // Read first word from file
    if (format != "P3") { // Validate the format
        std::cerr << "ERROR: Invalid PPM Format." << std::endl;
        return -1; // Return -1 if not P3
    }

    if (!(file >> width >> height >> maxValue)) { // Attempt to read width, height, and max color value
        std::cerr << "ERROR: Problem reading dimension or max value" << std::endl;
        return -1; // Return -1 on read error
    }

    if (width <= 0 || height <= 0 || maxValue <= 0 || maxValue > 255) { // Validate values are in bounds
        std::cerr << "ERROR: Invalid image dimension or max value," << std::endl;
        return -1; // Return -1 if values are invalid
    }

    int dim = width * height; // Compute total number of pixels

    red.resize(dim);   // Allocate memory for red vector
    green.resize(dim); // Allocate memory for green vector
    blue.resize(dim);  // Allocate memory for blue vector

    int pixelCount = 0; // Track number of successfully read pixels

    while (pixelCount < dim) { // Loop through expected number of pixels
        if (!(file >> red[pixelCount] >> green[pixelCount] >> blue[pixelCount])) { // Attempt to read pixel
            std::cerr << "ERROR: Problem reading pixel data" << std::endl;
            red.clear(); green.clear(); blue.clear(); // Clear all vectors on failure to release memory and prevent invalid usage
            return -1; // Return failure
        }

        // Validate RGB values are within bounds (0 to maxValue)
        if (red[pixelCount] > maxValue || red[pixelCount] < 0 ||
            green[pixelCount] > maxValue || green[pixelCount] < 0 ||
            blue[pixelCount] > maxValue || blue[pixelCount] < 0) {
            std::cerr << "ERROR: Pixel Value exceeds max or is negative" << std::endl;
            red.clear(); green.clear(); blue.clear(); // Clear data to avoid partial or corrupted usage
            return -1; // Return failure
        }

        pixelCount++; // Move to next pixel
    }

    int excess; // Variable to detect excessive data
    if (file >> excess) { // Try reading more data than expected
        std::cerr << "ERROR: Excessive pixel data detected" << std::endl;
        red.clear(); green.clear(); blue.clear(); // Clear memory as data isn't valid
        return -1; // Return error
    }

    if (pixelCount != dim) { // Final check to make sure we read exactly the expected amount
        std::cerr << "ERROR: Amount of data read: " << pixelCount << " expected value: " << dim << std::endl;
        return -1; // Error if mismatch
    }

    return 0; // Success
}

// Normalizes RGB values of the image to full range [0, 255]
int normalizeImg(std::vector<int>& red, std::vector<int>& green, std::vector<int>& blue) {
    int minVal = 255, maxVal = 0; // Start with widest possible bounds to find real min/max

    for (size_t i = 0; i < red.size(); i++) { // Loop through all pixels (size_t is used because vector::size() returns size_t, which is an unsigned type suited for array indexing)
        if (red[i] < minVal) minVal = red[i];   // Update min if red value is lower
        if (green[i] < minVal) minVal = green[i]; // Update min if green value is lower
        if (blue[i] < minVal) minVal = blue[i]; // Update min if blue value is lower

        if (red[i] > maxVal) maxVal = red[i];   // Update max if red value is higher
        if (green[i] > maxVal) maxVal = green[i]; // Update max if green value is higher
        if (blue[i] > maxVal) maxVal = blue[i]; // Update max if blue value is higher
    }

    if (maxVal == minVal) { // If image has no contrast, normalization will divide by zero
        std::cerr << "ERROR: No contrast in image" << std::endl;
        return -1; // Return failure
    }

    for (size_t i = 0; i < red.size(); i++) { // Normalize all pixels (size_t is used for safe indexing and to match vector::size() return type) (again using size_t for consistency and to avoid signed/unsigned mismatch warnings)
        // Apply normalization formula and cast to int
        red[i] = static_cast<int>(std::round((red[i] - minVal) * 255.0 / (maxVal - minVal))); // Normalize red by stretching value to 0–255 range and rounding
        green[i] = static_cast<int>(std::round((green[i] - minVal) * 255.0 / (maxVal - minVal))); // Normalize green channel value
        blue[i] = static_cast<int>(std::round((blue[i] - minVal) * 255.0 / (maxVal - minVal))); // Normalize blue channel value

        // Clamp each result to valid range [0, 255] in case of floating-point issues
        if (red[i] < 0) red[i] = 0; // Clamp red to minimum value of 0
        if (red[i] > 255) red[i] = 255; // Clamp red to maximum value of 255

        if (green[i] < 0) green[i] = 0; // Clamp green to minimum value of 0
        if (green[i] > 255) green[i] = 255; // Clamp green to maximum value of 255

        if (blue[i] < 0) blue[i] = 0; // Clamp blue to minimum value of 0
        if (blue[i] > 255) blue[i] = 255; // Clamp blue to maximum value of 255
    }

    return 0; // Success
}

// Writes RGB data to a PPM (P3 format) output file
int writePPM(const std::string& filename, // Output file name
             const std::vector<int>& red, // Red channel data
             const std::vector<int>& green, // Green channel data
             const std::vector<int>& blue, // Blue channel data
             int width, // Image width
             int height) { // Image height

    std::ofstream file(filename); // Open the output file for writing

    if (!file) { // Validate output file opened properly
        std::cerr << "ERROR: Output File not opened" << std::endl;
        return -1; // Return failure if file can't open
    }

    // PPM format expects max color value to be 255, even if original max was lower
    // This is intentional: we normalize values to span 0–255, so we hardcode it
    file << "P3\n" << width << " " << height << " 255\n"; // Write header in P3 format

    for (size_t i = 0; i < red.size(); i++) { // Loop through each pixel (size_t again for compatibility with vector::size())
        file << red[i] << " " << green[i] << " " << blue[i]; // Write RGB values for pixel

        if ((i + 1) % width != 0) { // Check if not end of row
            file << " "; // Add space after pixel
        } else {
            file << "\n"; // End row with newline
        }
    }

    return 0; // Success
}
