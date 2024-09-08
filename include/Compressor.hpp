#ifndef __COMPRESSOR_HPP__
#define __COMPRESSOR_HPP__

#include <vector>
#include <iostream>
#include <cmath>

#define DCT_BLOC_SIZE 8

using namespace std;

class Compressor
{
private:
    vector<unsigned char> original_image;
    vector<unsigned char> lum;
    vector<unsigned char> chr_r;
    vector<unsigned char> chr_b;

    int size, width, height;


    vector<int> Q = {
        16, 11, 10, 16, 24, 40, 51, 61,
        12, 12, 14, 19, 26, 58, 60, 55,
        14, 13, 16, 24, 40, 57, 69, 56,
        14, 17, 22, 29, 51, 87, 80, 62,
        18, 22, 37, 56, 68, 109, 103, 77,
        24, 35, 55, 64, 81, 104, 113, 92,
        49, 64, 78, 87, 103, 121, 120, 101,
        72, 92, 95, 98, 112, 100, 103, 99
    };

    /**
     * Divides the image in 3 channels of a different base: RGB -> YCrCb
     * These channels are put in the lum, chr_r and chr_b attributes vectors
     */
    void YCrCb();

    /**
     * Throws away information: half the chrominance information, since the human eye is less sensible to color than to luminance
     */
    void throwaway();

    float getDctConstant(int x);
    void dctBloc(vector<int> &dct_res, int x_start, int y_start, vector<unsigned char> &image, int max_width, int max_height);

    /**
     * Does the dct for a specific image
     */
    vector<int> dct(vector<unsigned char> &image, int max_width, int max_height);

    void quantization(vector<int> &dct_res);

    void displayArray(vector<char> &array, int lines, int columns);
    void displayArray(vector<int> &array, int lines, int columns);

public:
    Compressor(vector<char> data, int width, int height);
    void compress();
};

#endif