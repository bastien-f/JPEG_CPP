#include "Decoder.hpp"

void Decoder::decodeImage()
{
    int block_x = width / block_size;
    int block_y = height / block_size;
    const char end_block = 255;

    for (int i = 0; i < block_y; i++)
    {
        for (int j = 0; j < block_x; j++)
        {
            // Analyse block

            // Get DC
                     
            // Loop on other numbers while header != 255
        }
    }
}