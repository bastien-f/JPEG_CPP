#include "Compressor.hpp"

Compressor::Compressor(std::vector<char> data, int width, int height, std::string path)
{
    original_image = data;
    this->width = width;
    this->height = height;
    size = data.size();

    division_coef = 4;
    block_size = 8;

    lum.resize(size/3);
    chr_b.resize(size/3);
    chr_r.resize(size/3);

    file_path = path;
}

void Compressor::compress()
{
    YCrCb();
    throwaway();
    
    
    // Encode header of file
    std::ofstream file(file_path, std::ios::binary);
    file.write(reinterpret_cast<char*>(&width), sizeof(int));
    file.write(reinterpret_cast<char*>(&height), sizeof(int));
    file.write(&division_coef, sizeof(char));
    file.write(&block_size, sizeof(char));
    file.close();
    
    // Encode each channel
    // Lum
    Fourier fourier(lum, this->block_size, this->width, this->height);
    fourier.dct();
    Encoder encoder(fourier.getArray(), this->block_size, this->width, this->height, file_path);
    encoder.encodeImage();

    // Chr blue
    fourier.setArray(chr_b, width/2, height/2);
    fourier.dct();
    encoder.setArray(fourier.getArray(), width/2, height/2);
    encoder.encodeImage();

    // Chr red
    fourier.setArray(chr_r, width/2, height/2);
    fourier.dct();
    encoder.setArray(fourier.getArray(), width/2, height/2);
    encoder.encodeImage();
}


void Compressor::YCrCb()
{
    int i = 0;
    int original_index = 0;

    while (original_index < size)
    {
        int r = original_image[original_index], 
            g = original_image[original_index+1], 
            b = original_image[original_index+2];
        
        lum[i] = 0.299*r + 0.587*g + 0.114*b;
        chr_b[i] = -0.1687*r - 0.3313*g + 0.5*b;
        chr_r[i] = 0.5*r - 0.4187*g - 0.0813*b;
        original_index += 3;
        i++;
    }
}

void Compressor::throwaway()
{
    std::vector<char> tmp_r(chr_r.size()/4), tmp_b(chr_b.size()/4);
    int i = 0, j, current_index = 0, original_index = 0, k;

    // REDO FOR 4x4 INSTEAD OF LINE OF 4


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

    chr_b.clear();
    chr_r.clear();
    chr_b = tmp_b;
    chr_r = tmp_r;
}

  
void Compressor::RGBExtend() 
{
    int original_index = 0, i = 0;

    while (original_index < original_image.size())
    {
        original_image[original_index] = lum[i] + 1.402 * chr_r[i];
        original_image[original_index + 1] = lum[i] - 0.34414 * chr_b[i] - 0.71414 * chr_r[i];
        original_image[original_index + 2] = lum[i] + 1.772 * chr_b[i];   
        original_index += 3;     
        i++;
    }
}