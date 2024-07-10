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

    std::vector<int> numbers = {
        139, 144, 149, 153, 155, 155, 155, 155,
        144, 151, 153, 156, 159, 156, 156, 156,
        150, 155, 160, 163, 158, 156, 156, 156,
        159, 161, 162, 160, 160, 159, 159, 159,
        159, 160, 161, 162, 162, 155, 155, 155,
        161, 161, 161, 161, 160, 157, 157, 157,
        162, 162, 161, 163, 162, 157, 157, 157,
        162, 162, 161, 161, 163, 158, 158, 158
    };

    std::vector<char> test = {
        static_cast<char>(139), static_cast<char>(144), static_cast<char>(149), static_cast<char>(153), static_cast<char>(155), static_cast<char>(155), static_cast<char>(155), static_cast<char>(155),
        static_cast<char>(144), static_cast<char>(151), static_cast<char>(153), static_cast<char>(156), static_cast<char>(159), static_cast<char>(156), static_cast<char>(156), static_cast<char>(156),
        static_cast<char>(150), static_cast<char>(155), static_cast<char>(160), static_cast<char>(163), static_cast<char>(158), static_cast<char>(156), static_cast<char>(156), static_cast<char>(156),
        static_cast<char>(159), static_cast<char>(161), static_cast<char>(162), static_cast<char>(160), static_cast<char>(160), static_cast<char>(159), static_cast<char>(159), static_cast<char>(159),
        static_cast<char>(159), static_cast<char>(160), static_cast<char>(161), static_cast<char>(162), static_cast<char>(162), static_cast<char>(155), static_cast<char>(155), static_cast<char>(155),
        static_cast<char>(161), static_cast<char>(161), static_cast<char>(161), static_cast<char>(161), static_cast<char>(160), static_cast<char>(157), static_cast<char>(157), static_cast<char>(157),
        static_cast<char>(162), static_cast<char>(162), static_cast<char>(161), static_cast<char>(163), static_cast<char>(162), static_cast<char>(157), static_cast<char>(157), static_cast<char>(157),
        static_cast<char>(162), static_cast<char>(162), static_cast<char>(161), static_cast<char>(161), static_cast<char>(163), static_cast<char>(158), static_cast<char>(158), static_cast<char>(158)
    };

    vector<char> res = dct(chr_r);
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
}

void Compressor::dctBloc(vector<char> &dct_res, int x_start, int y_start, vector<char> &image, int max_width)
{
    double value = 0.0;

    for (int i = 0; i < DCT_BLOC_SIZE; i++)
    {
        for (int j = 0; j < DCT_BLOC_SIZE; j++)
        {
            value = 0.0;
            for (int x = 0; x < DCT_BLOC_SIZE; x++)
            {
                for (int y = 0; y < DCT_BLOC_SIZE; y++)
                {
                    value += static_cast<int>(static_cast<unsigned char>(image[(y+y_start)*max_width + x+x_start])) *
                        cos((j+x_start)*M_PI*(2*x+1)/(2*DCT_BLOC_SIZE)) *
                        cos((i+y_start)*M_PI*(2*y+1)/(2*DCT_BLOC_SIZE));
                }
            }
            value *= (2/(float)DCT_BLOC_SIZE)*getDctConstant(i)*getDctConstant(j);
            
            dct_res[(i+y_start)*max_width + j+x_start] = floor(value + 0.5);
        }
    }
}

float Compressor::getDctConstant(int x)
{
    if (x == 0) return (1/sqrt(2));
    else return 1;    
}


vector<char> Compressor::dct(vector<char> &image)
{
    vector<char> dct_res(image.size());
    int max_width, max_height;

    if (image.size() < size) {max_width = width/2; max_height = height/2;}

    for (int j = 0; j < max_width - DCT_BLOC_SIZE; j += DCT_BLOC_SIZE)
    {
        for (int i = 0; i < max_height - DCT_BLOC_SIZE; i += DCT_BLOC_SIZE)
        {
            dctBloc(dct_res, j, i, image, max_width);
        }
    }

    return dct_res;
}

void Compressor::displayArray(vector<char> &array, int lines, int columns)
{
    for (int i = 0; i < lines; i++)
    {
        for (int j = 0; j < columns; j++)
        {
            cout << static_cast<int>(static_cast<unsigned char>(array[i * lines + j])) << " ";
        }
        cout << endl;
    }
}