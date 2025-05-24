#ifndef GRAYCORRELATION_H
#define GRAYCORRELATION_H
#include <vector>
#include <string>


// Load the filenames
// create Image objects to hold them
// get width/height - const
// get pixels - const
// get the average - const
// compare the images:
     // belongs to class itself: static
     // double for decimals
     // const - 2 Images as input wont change, passing ref & so no slow copying

// private variables
// width integer
// height integer
// vector of pixels                 

class Image {

    public:
        bool load(const std::string& filename);
        static double compute(const Image& A, const Image& B); // correlation formula


        int getWidth() const;
        int getHeight() const;
        double getPixels(int r, int c) const;
        double getAverage() const;
        

    private:
        std::vector<std::vector<int>> pixels;
        int width;
        int height;
       

};

#endif // GRAYCORRELATION_H