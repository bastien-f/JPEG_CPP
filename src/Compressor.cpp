#include "Compressor.hpp"

Compressor::Compressor(vector<char> data, int width, int height)
{
    // Init original image attribute (copy to not modify the original)
    this->width = width;
    this->height = height;
    size = data.size();
    original_image.resize(size);
    for (int i = 0; i < size; i++)
    {
        original_image[i] = static_cast<unsigned char>(data[i]);
    }

    // Init luminance & chrominances (all a third of the size of the original image, because only one channel)
    vector<unsigned char> tmp(size/3, 0);
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

    std::vector<unsigned char> test = {
        static_cast<unsigned char>(139), static_cast<unsigned char>(144), static_cast<unsigned char>(149), static_cast<unsigned char>(153), static_cast<unsigned char>(155), static_cast<unsigned char>(155), static_cast<unsigned char>(155), static_cast<unsigned char>(155),
        static_cast<unsigned char>(144), static_cast<unsigned char>(151), static_cast<unsigned char>(153), static_cast<unsigned char>(156), static_cast<unsigned char>(159), static_cast<unsigned char>(156), static_cast<unsigned char>(156), static_cast<unsigned char>(156),
        static_cast<unsigned char>(150), static_cast<unsigned char>(155), static_cast<unsigned char>(160), static_cast<unsigned char>(163), static_cast<unsigned char>(158), static_cast<unsigned char>(156), static_cast<unsigned char>(156), static_cast<unsigned char>(156),
        static_cast<unsigned char>(159), static_cast<unsigned char>(161), static_cast<unsigned char>(162), static_cast<unsigned char>(160), static_cast<unsigned char>(160), static_cast<unsigned char>(159), static_cast<unsigned char>(159), static_cast<unsigned char>(159),
        static_cast<unsigned char>(159), static_cast<unsigned char>(160), static_cast<unsigned char>(161), static_cast<unsigned char>(162), static_cast<unsigned char>(162), static_cast<unsigned char>(155), static_cast<unsigned char>(155), static_cast<unsigned char>(155),
        static_cast<unsigned char>(161), static_cast<unsigned char>(161), static_cast<unsigned char>(161), static_cast<unsigned char>(161), static_cast<unsigned char>(160), static_cast<unsigned char>(157), static_cast<unsigned char>(157), static_cast<unsigned char>(157),
        static_cast<unsigned char>(162), static_cast<unsigned char>(162), static_cast<unsigned char>(161), static_cast<unsigned char>(163), static_cast<unsigned char>(162), static_cast<unsigned char>(157), static_cast<unsigned char>(157), static_cast<unsigned char>(157),
        static_cast<unsigned char>(162), static_cast<unsigned char>(162), static_cast<unsigned char>(161), static_cast<unsigned char>(161), static_cast<unsigned char>(163), static_cast<unsigned char>(158), static_cast<unsigned char>(158), static_cast<unsigned char>(158)
    };
    
    std::vector<unsigned char> test2 = {
        static_cast<unsigned char>(140), static_cast<unsigned char>(144), static_cast<unsigned char>(147), static_cast<unsigned char>(140), static_cast<unsigned char>(140), static_cast<unsigned char>(155), static_cast<unsigned char>(179), static_cast<unsigned char>(175),
        static_cast<unsigned char>(144), static_cast<unsigned char>(152), static_cast<unsigned char>(140), static_cast<unsigned char>(147), static_cast<unsigned char>(140), static_cast<unsigned char>(148), static_cast<unsigned char>(167), static_cast<unsigned char>(179),
        static_cast<unsigned char>(152), static_cast<unsigned char>(155), static_cast<unsigned char>(136), static_cast<unsigned char>(167), static_cast<unsigned char>(163), static_cast<unsigned char>(162), static_cast<unsigned char>(152), static_cast<unsigned char>(172),
        static_cast<unsigned char>(168), static_cast<unsigned char>(145), static_cast<unsigned char>(156), static_cast<unsigned char>(160), static_cast<unsigned char>(152), static_cast<unsigned char>(155), static_cast<unsigned char>(136), static_cast<unsigned char>(160),
        static_cast<unsigned char>(162), static_cast<unsigned char>(148), static_cast<unsigned char>(156), static_cast<unsigned char>(148), static_cast<unsigned char>(140), static_cast<unsigned char>(136), static_cast<unsigned char>(147), static_cast<unsigned char>(162),
        static_cast<unsigned char>(146), static_cast<unsigned char>(167), static_cast<unsigned char>(140), static_cast<unsigned char>(155), static_cast<unsigned char>(155), static_cast<unsigned char>(140), static_cast<unsigned char>(136), static_cast<unsigned char>(162),
        static_cast<unsigned char>(136), static_cast<unsigned char>(156), static_cast<unsigned char>(123), static_cast<unsigned char>(167), static_cast<unsigned char>(162), static_cast<unsigned char>(144), static_cast<unsigned char>(140), static_cast<unsigned char>(147),
        static_cast<unsigned char>(148), static_cast<unsigned char>(155), static_cast<unsigned char>(136), static_cast<unsigned char>(155), static_cast<unsigned char>(152), static_cast<unsigned char>(147), static_cast<unsigned char>(147), static_cast<unsigned char>(136)
    };

    vector<int> res = dct(test, 8, 8);
    quantization(res);

    displayArray(res, 8, 8);

    // dct();
}


void Compressor::YCrCb()
{
    int i = 0;
    int original_index = 0;

    while (original_index < size)
    {
        // Get the red, blue and green values at this pixel
        int r = original_image[original_index], g = original_image[original_index+1], b = original_image[original_index+2];

        // Compute YCrCb at this pixel
        lum[i] = 0.299*r + 0.587*g + 0.114*b;
        chr_r[i] = -0.1687*r - 0.3313*g + 0.5*b + 128;
        chr_b[i] = 0.5*r - 0.4187*g - 0.0813*b + 128;

        original_index += 3;
        i++;
    }
}

void Compressor::throwaway()
{
    vector<unsigned char> tmp_r(chr_r.size()/4), tmp_b(chr_b.size()/4); // result vectors
    int i = 0, j, current_index = 0, original_index = 0, k;

    while (current_index < (chr_r.size()/4) && original_index < size)
    {
        // Look at the pixels around and make the average of their values
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

    // Replace the chrominance vectors by the new ones
    chr_r.clear();
    chr_b.clear();
    chr_r = tmp_r;
    chr_b = tmp_b;
}

void Compressor::dctBloc(vector<int> &dct_res, int x_start, int y_start, vector<unsigned char> &image, int max_width, int max_height)
{
    double value = 0.0;

    // Compute for each pixel of the bloc
    for (int i = 0; i < DCT_BLOC_SIZE; i++)
    {
        for (int j = 0; j < DCT_BLOC_SIZE; j++)
        {
            // Take into acount every pixel of the bloc
            value = 0.0;
            for (int x = 0; x < DCT_BLOC_SIZE; x++)
            {
                for (int y = 0; y < DCT_BLOC_SIZE; y++)
                {
                    // DCT
                    value += static_cast<int>((image[(y+y_start)*max_width + x+x_start])) *
                        cos((j+x_start)*M_PI*(2*(x+x_start)+1)/(2*DCT_BLOC_SIZE)) *
                        cos((i+y_start)*M_PI*(2*(y+y_start)+1)/(2*DCT_BLOC_SIZE));
                }
            }
            value *= (2/(float)DCT_BLOC_SIZE)*getDctConstant(i)*getDctConstant(j);
            
            dct_res[(i+y_start)*max_width + j+x_start] = floor(value + 0.5); // Round value
        }
    }
}


float Compressor::getDctConstant(int x)
{
    if (x == 0) return (1/sqrt(2));
    else return 1;    
}


vector<int> Compressor::dct(vector<unsigned char> &image, int max_width, int max_height)
{
    vector<int> dct_res(image.size());

    // Do the dct on blocs of DCT_BLOC_SIZE size
    for (int j = 0; j <= max_width - DCT_BLOC_SIZE; j += DCT_BLOC_SIZE)
    {
        for (int i = 0; i <= max_height - DCT_BLOC_SIZE; i += DCT_BLOC_SIZE)
        {
            dctBloc(dct_res, j, i, image, max_width, max_height);
        }
    }

    return dct_res;
}

void Compressor::quantization(vector<int> &dct_res)
{
    // Check quantization table
    if (Q.size() != DCT_BLOC_SIZE * DCT_BLOC_SIZE) {std::cerr << "Error, Quantization table of wrong size" << std::endl; exit(-1);}

    for (int index = 0; index < dct_res.size(); index++)
    {
        dct_res[index] = floor((float)(dct_res[index])/Q[index % Q.size()] + 0.5);
    }
}


void Compressor::displayArray(vector<char> &array, int lines, int columns)
{
    for (int i = 0; i < lines; i++)
    {
        for (int j = 0; j < columns; j++)
        {
            cout << static_cast<int>(/*static_cast<unsigned char>*/(array[i * lines + j])) << " ";
        }
        cout << endl;
    }
}

void Compressor::displayArray(vector<int> &array, int lines, int columns)
{
    for (int i = 0; i < lines; i++)
    {
        for (int j = 0; j < columns; j++)
        {
            cout << array[i * lines + j] << "\t";
        }
        cout << endl;
    }
}