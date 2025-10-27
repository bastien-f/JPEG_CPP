#ifndef __FOURIER_HPP__
#define __FOURIER_HPP__


#include <vector>
#include <cmath>


class Fourier
{   
private:
    int block_size;
    std::vector<float> array;
    int width;
    int height;

    float c(int i, int j);

public:
    Fourier(std::vector<float> &arr, int dct_block_size, int width, int height): block_size(dct_block_size), array(arr), width(width), height(height) {}

    Fourier(std::vector<char> &arr, int dct_block_size, int width, int height): block_size(dct_block_size), width(width), height(height)
    {
        array.resize(arr.size());
        for (int i = 0; i < arr.size(); i++)
        {
            array[i] = arr[i];
        }
    }

    ~Fourier()
    {
        array.clear();
    }

    std::vector<float> &getArray() {return array;}
    void setArray(std::vector<float> &arr, int width, int height) {
        array = arr;
        this->width = width;
        this->height = height;
    }

    void setArray(std::vector<char> &arr, int width, int height) {
        array.resize(arr.size());
        for (int i = 0; i < arr.size(); i++)
        {
            array[i] = arr[i];
        }
        this->width = width;
        this->height = height;
    }

    void dctBlock(int start);
    void dct();

    void reverseDctBlock(int start);
    void reverseDct();
};


#endif