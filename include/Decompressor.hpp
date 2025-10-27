#ifndef __DECOMPRESSOR_HPP__
#define __DECOMPRESSOR_HPP__


#include <vector>
#include <fstream>
#include <string>


class Decompressor
{
private:
    int width;
    int height;
    char division_size;
    char block_size;
    int nb_blocks;

    std::ifstream file;

public:
    Decompressor(std::string file_path)
    {
        file.open(file_path, std::ios::binary);

        file.read(reinterpret_cast<char*>(&width), sizeof(int));
        file.read(reinterpret_cast<char*>(&height), sizeof(int));

        file.read(&division_size, sizeof(char));
        file.read(&block_size, sizeof(char));
        
        file.close();

        nb_blocks = width * height / (block_size * block_size);
    }

    ~Decompressor()
    {
        file.close();
    }

    void decompressImage(std::vector<int> array, bool divided);
};


#endif