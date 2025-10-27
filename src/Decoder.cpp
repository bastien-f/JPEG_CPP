#include "Decoder.hpp"
#include "Quantization.hpp"

void Decoder::decodeImage()
{
    int block_x = width / block_size;
    int block_y = height / block_size;
    const char end_block = 255;
    char DC_value;
    char previous_DC;
    std::vector<int> block(block_size * block_size);

    for (int i = 0; i < block_y; i++)
    {
        for (int j = 0; j < block_x; j++)
        {
            // Analyse block -----

            // Get DC
            file.read(&DC_value, sizeof(char));
            if (i == 0 && j == 0)
            {
                previous_DC = DC_value;
                block[0] = DC_value;
            }
            else
            {
                block[0] = previous_DC - DC_value;
                previous_DC = previous_DC - DC_value;
            }
                     
            // Loop on other numbers while header != 255
            char header, zeros, size;
            int value;
            int k = 1;
            int l;
            file.read(&header, 1);
            while (header != 255 && k < block.size())
            {
                size = header & 0b00001111;
                zeros = header >> 4;
                file.read(reinterpret_cast<char*>(&value), size);
                value >> ((sizeof(int) - size) * 8); // Convert to int
                
                // Add zeros
                l = 0;
                while (k < block.size() && l < zeros)
                {
                    block[k] = 0;                    
                    k++;
                    l++;
                }

                // Add value
                block[k] = value;

                k++;
                file.read(&header, 1);
            }
            


            // Unwrap values in image
            unwrapBlock(block, i * block_size * width + j * block_size);
        }
    }
}



void Decoder::unwrapBlock(std::vector<int> &block, int image_position)
{
    int dy = -1;
    int dx = 1;
    int y = 0;
    int x = 0;
    int i = 0;


    while (x < block_size && y < block_size)
    {
        if (y < 0)
        {
            y = 0;
            dy *= -1;
            dx *= -1;
        }
        else if (x < 0)
        {
            x = 0;
            dy *= -1;
            dx *= -1;
        }
        else if (y == block_size)
        {
            y = block_size - 1;
            x += 2;
            dy *= 1;
            dx *= 1;
        }
        else if (x == block_size)
        {
            x = block_size - 1;
            y += 2;
            dy *= 1;
            dx *= 1;
        }

        image[image_position + y * width + x] = block[i] * Q50_8[y * block_size + x];

        i++;
        x += dx;
        y += dy;
    }
}
