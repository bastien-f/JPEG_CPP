#include "Fourier.hpp"

void Fourier::dctBlock(int start)
{
    int start_i = start / width;
    int start_j = start % width;
    int block_area = block_size * block_size;
    int stop = start + block_size + width * (block_size - 1);

    std::vector<float> result(block_area);

    float total;

    for (int i = 0; i < block_size; i++)
    {
        for (int j = 0; j < block_size; j++)
        {
            total = 0;
            for (int y = 0; y < block_size; y++)
            {
                for (int x = 0; x < block_size; x++)
                {
                    total += array[start + y * width + x] 
                        * std::cos((2 * y + 1) * i * M_PI / (2 * block_size))
                        * std::cos((2 * x + 1) * j * M_PI / (2 * block_size));
                }
            }
            total *= c(i, j) / std::sqrt(2 * block_size);
            result[i * block_size + j] = total;
        }
    }

    for (int i = 0; i < block_size; i++)
    {
        for (int j = 0; j < block_size; j++)
        {
            array[start + i * width + j] = result[i * block_size + j];
        }
    }
}

float Fourier::c(int i, int j)
{
    if (i == 0 && j == 0) return 0.5;
    if (i > 0 && j > 0) return 1;
    return 1 / std::sqrt(2);
}



  
void Fourier::dct()
{
    int block_size = 8;

    if (width * height != array.size()) throw "DCT : mauvaise largeur et hauteur";
    if (width % block_size != 0 || height % block_size != 0) throw "DCT : hauteur ou largeur de taille non compatible avec le bloc";

    int nb_col = width / block_size, nb_lines = height / block_size;

    for (int i = 0; i < nb_lines; i++)
    {
        for (int j = 0; j < nb_col; j++)
        {
            dctBlock(i * block_size * width + j * block_size);
        }
    }
}





void Fourier::reverseDctBlock(int start)
{

}


void Fourier::reverseDct()
{
    int block_size = 8;

    if (width * height != array.size()) throw "DCT : mauvaise largeur et hauteur";
    if (width % block_size != 0 || height % block_size != 0) throw "DCT : hauteur ou largeur de taille non compatible avec le bloc";

    int nb_col = width / block_size, nb_lines = height / block_size;

    for (int i = 0; i < nb_lines; i++)
    {
        for (int j = 0; j < nb_col; j++)
        {
            dctBlock(i * block_size * width + j * block_size);
        }
    }
}