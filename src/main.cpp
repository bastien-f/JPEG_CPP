#include <iostream>

#include "BmpReader.hpp"
#include "Compressor.hpp"
#include "Decoder.hpp"

using namespace std;

int main()
{
    std::string output_path = "../ressources/output.bin";
    BmpReader reader;
    reader.readImage("../ressources/landscape.bmp");


    // Compressor comp(reader.getImage(), reader.getWidth(), reader.getHeight(), output_path);
    // comp.compress();


    return 0;
}