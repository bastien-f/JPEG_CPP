#ifndef __DECODER_HPP__
#define __DECODER_HPP__

class Decoder
{
private:
    int width;
    int height;
    char block_size;

public:
    Decoder(int width, int height, int block_size): width(width), height(height), block_size(block_size) {}

    void decodeImage();    
};

#endif