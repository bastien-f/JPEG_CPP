#ifndef __BMPREADER_HPP__
#define __BMPREADER_HPP__

#include <vector>
#include <string>
#include <fstream>
#include <iostream>

using namespace std;

#pragma pack(push, 1)
struct FileHeader
{
    short header;
    int total_size;
    short reserved1;
    short reserved2;
    int offset;
};

struct InfoHeader
{
    int header_size;
    int width;
    int height;
    short color_planes;
    short bit_per_pixel;
    int compression;
    int image_size;
    int horizontal_resolution;
    int vertical_resolution;
    int color_numbers;
    int color_used;
};
#pragma pack(pop)

class BmpReader
{
private: 
    vector<char> image;
    FileHeader file_header;
    InfoHeader info_header;


public:
    BmpReader();
    void readImage(string filepath);
    void writeImage(string filepath);

    void changeImage(vector<char> image);

    int getSize(){return info_header.image_size;}
    vector<char> getImage(){return image;}
    int getWidth(){return info_header.width;}
    int getHeight(){return info_header.height;}
};

#endif