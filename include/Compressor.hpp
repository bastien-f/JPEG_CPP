#ifndef __COMPRESSOR_HPP__
#define __COMPRESSOR_HPP__

#include <vector>
#include <iostream>
#include <cmath>
#include <fstream>
#include "Fourier.hpp"
#include "Encoder.hpp"


class Compressor
{
private:
    std::vector<char> original_image; // 3 channels [r1, g1, b1, r2, g2, b2, ...]
    std::vector<char> lum;
    std::vector<char> chr_b;
    std::vector<char> chr_r;

    int size, width, height;
    char division_coef;
    char block_size;

    std::string file_path;

    void YCrCb();
    void throwaway();
    
    
    public:
    Compressor(std::vector<char> data, int width, int height, std::string path);
    void compress();
    void RGBExtend();
    
    std::vector<char> getImage() {return original_image;}
};

#endif