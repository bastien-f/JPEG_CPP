#ifndef __COMPRESSOR_HPP__
#define __COMPRESSOR_HPP__

#include <vector>
#include <iostream>

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

public:
    Compressor(vector<char> data, int width, int height);
    void compress();
};

#endif