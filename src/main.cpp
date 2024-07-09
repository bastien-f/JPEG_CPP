#include <iostream>

#include "BmpReader.hpp"
#include "Compressor.hpp"

using namespace std;

int main()
{
    BmpReader reader;
    reader.readImage("../ressources/landscape.bmp");


    Compressor comp(reader.getImage(), reader.getWidth(), reader.getHeight());
    comp.compress();


    reader.writeImage("../ressources/output.bmp");

    return 0;
}