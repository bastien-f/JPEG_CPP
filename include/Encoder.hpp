#ifndef __ENCODER_HPP__
#define __ENCODER_HPP__

#include <vector>
#include <fstream>
#include <string>


class Encoder
{
private:
    int block_size;
    std::vector<float> array;
    int width;
    int height;
    std::ofstream file;

public:
    Encoder(std::vector<float> &arr, int dct_block_size, int width, int height, std::string file_path): block_size(dct_block_size), array(arr), width(width), height(height)
    {
        this->file.open(file_path, std::ios::binary);
    }

    Encoder(std::vector<char> &arr, int dct_block_size, int width, int height, std::string file_path): block_size(dct_block_size), width(width), height(height)
    {
        array.resize(arr.size());
        for (int i = 0; i < arr.size(); i++)
        {
            array[i] = arr[i];
        }

        this->file.open(file_path, std::ios::binary);
    }

    ~Encoder()
    {
        file.close();
        array.clear();
    }

    void setArray(std::vector<float> array)
    {
        this->array = array;
    }

    void setArray(std::vector<float> array, int width, int height)
    {
        this->array = array;
        this->width = width;
        this->height = height;
    }

    void closeFile() {file.close();}


    std::vector<int> quantizationBlockZigZag(int start);

    void encodeImage();

    void writeDC(int variable);
    void writeVariable(int variable, int8_t zeros);
};




// Quantization Matrices ------------------------------------------------------------------------

// 50% Quality with blocks of size 8
extern std::vector<int> Q50_8;


#endif