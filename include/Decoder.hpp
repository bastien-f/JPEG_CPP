#ifndef __DECODER_HPP__
#define __DECODER_HPP__

#include <vector>
#include <fstream>
#include <string>

class Decoder
{
private:
    int width;
    int height;
    char block_size;
    std::vector<float> image;
    std::ifstream file;

public:
    Decoder(int width, int height, int block_size, std::string file_path): width(width), height(height), block_size(block_size) 
    {
        image.resize(width * height);
        file.open(file_path, std::ios::binary);
    }

    std::vector<float> getImage() {return image;}
    
    void setSize(int width, int height)
    {
        this->width = width;
        this->height = height;
    }
    void setWidth(int width) {this->width = width;}
    void setHeight(int height) {this->height = height;}


    void decodeImage();    
    void unwrapBlock(std::vector<int> &block, int image_position);
};

#endif