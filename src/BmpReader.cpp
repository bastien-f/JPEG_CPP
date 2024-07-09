#include "BmpReader.hpp"

BmpReader::BmpReader(){}

void BmpReader::readImage(string filepath)
{
    ifstream file(filepath, ios::binary);
    if (!file) 
    {
        cerr << "Can't read image !" << endl;
        exit(-1);
    }

    file.read(reinterpret_cast<char*>(&file_header), sizeof(file_header));
    file.read(reinterpret_cast<char*>(&info_header), sizeof(info_header));

    image.resize(info_header.image_size);
    file.read(&image[0], info_header.image_size);

    file.close();
}

void BmpReader::writeImage(string filepath)
{
    ofstream file(filepath, ios::binary);

    file.write(reinterpret_cast<char*>(&file_header), sizeof(file_header));
    file.write(reinterpret_cast<char*>(&info_header), sizeof(info_header));
    file.write(&image[0], info_header.image_size);

    file.close();
}

// No working
void BmpReader::changeImage(vector<char> new_image)
{
    file_header.total_size = sizeof(file_header) + sizeof(info_header) + new_image.size();
    info_header.bit_per_pixel = 8;// * info_header.image_size / new_image.size();
    info_header.image_size = new_image.size();
    image = new_image;
}