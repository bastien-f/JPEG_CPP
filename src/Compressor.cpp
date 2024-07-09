#include "Compressor.hpp"

Compressor::Compressor(vector<char> data, int width, int height)
{
    original_image = data;
    this->width = width;
    this->height = height;
    size = data.size();

    vector<char> tmp(size/3, 0);
    lum = tmp;
    chr_r = tmp;
    chr_b = tmp;
}

void Compressor::compress()
{
    YCrCb();
    throwaway();
}


void Compressor::YCrCb()
{
    int i = 0;
    int original_index = 0;

    while (original_index < size)
    {
        int r = original_image[original_index], g = original_image[original_index+1], b = original_image[original_index+2];
        lum[i] = 0.299*r + 0.587*g + 0.114*b;
        chr_r[i] = -0.1687*r - 0.3313*g + 0.5*b + 128;
        chr_b[i] = 0.5*r - 0.4187*g - 0.0813*b + 128;
        original_index += 3;
        i++;
    }
}

void Compressor::throwaway()
{
    vector<char> tmp_r(chr_r.size()/4), tmp_b(chr_b.size()/4);
    int i = 0, j, current_index = 0, original_index = 0, k;

    while (current_index < (chr_r.size()/4) && original_index < size)
    {
        for (k = 0; k < 4; k++)
        {
            tmp_r[current_index] = chr_r[original_index + k];
            tmp_b[current_index] = chr_b[original_index + k];
        }
        tmp_r[current_index] = tmp_r[current_index]/4;
        tmp_b[current_index] = tmp_r[current_index]/4;

        current_index++;
        original_index += 4;
    }

    chr_r.clear();
    chr_b.clear();
    chr_r = tmp_r;
    chr_b = tmp_b;

    cout << chr_r.size() << endl;
}