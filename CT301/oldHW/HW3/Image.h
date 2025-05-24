#ifndef IMAGE_H
#define IMAGE_H

#include <vector>
#include <string>

/**Goals:
Read ASCII PPM Image P3 *
Reject inalvid files    *
    wrong format        *
    wrong dimensions    *
Compute row-wise checksum   *
Compute col-wise checksum   *
Handle multi-channel pizel rgb  *
save rowwise checksum           *
save colwise checksum           *
handle invalid cmd line args or file i/o errors *
return -1 on failure and 0 on success           *
*/



class Image{


    public:
    int rows, cols, max;
    std::vector<int> pixels; // 1 vector to store pixel data

    //default constructor
    Image();

    //Function to load image PPM file
    bool loadImage(const std::string& filename);

    //Function to comput and write row checksums to file
    std::vector<int> calcRowCheckSums() const;

    //Function to compute and write column checksum to file
    std::vector<int> calcColCheckSums() const;

    //Function to save checksum values for both rows and cols
    bool saveChecksums(const std::string& rowFile, const std::string& colFile) const;
};


#endif // Image_H