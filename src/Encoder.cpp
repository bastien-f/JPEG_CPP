#include "Encoder.hpp"
#include "Quantization.hpp"


std::vector<int> Encoder::quantizationBlockZigZag(int start)
{
    int dy = -1;
    int dx = 1;
    int y = 0;
    int x = 0;
    int i = 0;

    std::vector<int> result(block_size * block_size);


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

        result[i] = array[start + y * width + x] / Q50_8[y * block_size + x];

        i++;
        x += dx;
        y += dy;
    }

    return result;
}


void Encoder::encodeImage()
{
    std::vector<int> block;
    int last_zero = 0;
    int last_DC = 0;
    int content = 0;
    int nb_zeros = 0;
    const char end_block = 255;
    
    // Loop through blocks
    for (int i = 0; i < height / block_size; i++)
    {
        for (int j = 0; j < width / block_size; j++)
        {
            // Get zigzag of block
            block = quantizationBlockZigZag(i * width + j);
            
            // Find the last zero pos
            last_zero = block.size();
            while (block[last_zero - 1] == 0)
            {
                last_zero--;
            }

            // Encode DC as [size (8b), value (size bytes)]
            if (i == 0 && j == 0)
            {
                last_DC = block[0];
                file.write(reinterpret_cast<char*>(&last_DC), sizeof(int));
            }
            else 
            {
                writeDC(last_DC - block[0]);
                last_DC = block[0];
            }

            // Encode the rest as [[nb_zeros_before (4b), size (4b)], value (size bytes)]
            // While nb_zeros_before > 14
                // nb_zeros_before - 14
                // add [[14, 1], 0] to file
            // To encode, get value in minimal bytes posible (when decoding they will be treated as ints)
            for (int k = 0; k < block.size() && k < last_zero; k++)
            {
                if (block[k] == 0)
                {
                    if (nb_zeros == 14)
                    {
                        writeVariable(0, 14);
                        nb_zeros = 0;
                    }
                    else
                    {
                        nb_zeros++;
                    }
                }
                else
                {
                    writeVariable(block[k], nb_zeros);
                    nb_zeros = 0;
                }
            }


            // If we are at last zero pos
            file.write(&end_block, sizeof(char));
        }
    }
}


void Encoder::writeVariable(int variable, int8_t zeros)
{
    if (-255 < variable && variable < 255) // 8 bits
    {
        int8_t value = variable;
        int8_t header = zeros << 4;
        header = header | 1; // variable = 1 byte
        this->file.write(reinterpret_cast<char*>(&header), 1);
        this->file.write(reinterpret_cast<char*>(&value), 1);
    }
    else if (-65535 < variable && variable < 65535) // 16 bits
    {
        int16_t value = variable;
        int8_t header = zeros << 4;
        header = header | 2; // variable = 2 byte
        this->file.write(reinterpret_cast<char*>(&header), 1);
        this->file.write(reinterpret_cast<char*>(&value), 2);
    }
    else if (-4294967295 < variable && variable < 4294967295) // 32 bits
    {
        int32_t value = variable;
        int8_t header = zeros << 4;
        header = header | 4; // variable = 4 byte
        this->file.write(reinterpret_cast<char*>(&header), 1);
        this->file.write(reinterpret_cast<char*>(&value), 4);
    }
    else // 64 bits
    {
        int8_t header = zeros << 4;
        header = header | 8; // variable = 8 byte
        this->file.write(reinterpret_cast<char*>(&header), 1);
        this->file.write(reinterpret_cast<char*>(&variable), 8);
    }
}


void Encoder::writeDC(int variable)
{
    if (-255 < variable && variable < 255) // 8 bits
    {
        int8_t value = variable;
        int8_t header = 1;
        this->file.write(reinterpret_cast<char*>(&header), 1);
        this->file.write(reinterpret_cast<char*>(&value), 1);
    }
    else if (-65535 < variable && variable < 65535) // 16 bits
    {
        int16_t value = variable;
        int8_t header = 2;
        this->file.write(reinterpret_cast<char*>(&header), 1);
        this->file.write(reinterpret_cast<char*>(&value), 2);
    }
    else if (-4294967295 < variable && variable < 4294967295) // 32 bits
    {
        int32_t value = variable;
        int8_t header = 4;
        this->file.write(reinterpret_cast<char*>(&header), 1);
        this->file.write(reinterpret_cast<char*>(&value), 4);
    }
    else // 64 bits
    {
        int8_t header = 8;
        this->file.write(reinterpret_cast<char*>(&header), 1);
        this->file.write(reinterpret_cast<char*>(&variable), 8);
    }
}

