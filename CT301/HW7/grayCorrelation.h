#ifndef GRAYCORRELATION_H
#define GRAYCORRELATION_H
#include <vector>
#include <string>


/**
 * @brief Image class will handle grayscale imagese and their correlation */          

class Image {

    public:
        /**
         * @brief Load image from a file
         * @param filename Path to PGM image file
         * @return True if successful, otherwise false
         */
        bool load(const std::string& filename);
        
        
        /**
         * @brief Compute correlation between two images
         * @param A First image
         * @param B Second image
         * @return Correlation score: -1 load error, -2 dimension mismatch
         */
        static double compute(const Image& A, const Image& B); // correlation formula

        
        
        /// @brief Get image width- leaving this here to make code reusable
        int getWidth() const {return width;}

        
        
        /// @brief Get image height - leaving this here to make code reusable
        int getHeight() const{return height;}

        
        
        /// @brief get Pixel value at (row, col)
        double getPixels(int r, int c) const {return static_cast<double>(pixels[r][c]); }

        
        
        /**
         *  @brief get average pixel value
         */
        double getAverage() const; 

        
        
        /**
         * @brief Provides reading access to pixel data
         * @return Constant reference to the 2D pixel vector
         */
        const std::vector<std::vector<unsigned char>>& getPixelVector() const {return pixels;}
        

    private:
    /**
     * @brief 2D vector stores pixel intensity values - Optimized to use unsigned char 
     * improving memory usage and cache performance
     */
        std::vector<std::vector<unsigned char>> pixels;
        int width = 0;
        int height = 0;
       

};

#endif // GRAYCORRELATION_H