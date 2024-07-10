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

    void YCrCb();
    void throwaway();
    void dctBloc(vector<char> &dct_res, int x_start, int y_start, vector<char> &image, int max_width);
    float getDctConstant(int x);
    vector<char> dct(vector<char> &image);
    void displayArray(vector<char> &array, int lines, int columns);

public:
    Compressor(vector<char> data, int width, int height);
    void compress();
};

#endif