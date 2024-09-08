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
    vector<char> original_image;
    vector<char> lum;
    vector<char> chr_r;
    vector<char> chr_b;

    int size, width, height;

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
    void dctBloc(vector<char> &dct_res, int x_start, int y_start, vector<char> &image, int max_width);

    /**
     * Does the dct for a specific image
     */
    vector<char> dctImage(vector<char> &image, int max_width, int max_height);
    void dct();

    void displayArray(vector<char> &array, int lines, int columns);

public:
    Compressor(vector<char> data, int width, int height);
    void compress();
};

#endif