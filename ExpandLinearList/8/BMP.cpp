#include <iostream>
#include <fstream>
#include "BMP.h"
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <exception>

// ifstream& operator >> (ifstream& in, BMPFileHeader& fh)
// {
//     in >> fh.bfType >> fh.bfSize >> fh.bfReserved1 
//     >> fh.bfReserved2 >> fh.bfOffBits;
//     return in;
// }

// ifstream& operator >> (ifstream& in, BMPInfoHeader& ih)
// {
//     in >> ih.biSize >> ih.biWidth >> ih.biHeight >> ih.biPlanes
//     >> ih.biBitCount >> ih.biCompression >> ih.biSizeImage
//     >> ih.biXPelsPerMeter >> ih.biYPelsPerMeter >> ih.biClrUsed
//     >> ih.biClrImportant;
//     return in;
// }

ostream& operator << (ostream& out, BMPFileHeader& fh)
{
    out << fh.bfType << fh.bfSize << fh.bfReserved1 
    << fh.bfReserved2 << fh.bfOffBits;
    return out;
}

ostream& operator << (ostream& out, BMPInfoHeader& ih)
{
    out << ih.biSize << ih.biWidth << ih.biHeight << ih.biPlanes
    << ih.biBitCount << ih.biCompression << ih.biSizeImage
    << ih.biXPelsPerMeter << ih.biYPelsPerMeter << ih.biClrUsed
    << ih.biClrImportant;
    return out;
}

coloredPixel operator+(const coloredPixel& a, const coloredPixel& b)
{
    coloredPixel ans;
    ans.rgbBlue = a.rgbBlue+b.rgbBlue;
    ans.rgbRed = a.rgbRed + b.rgbRed;
    ans.rgbGreen = a.rgbGreen + b.rgbGreen;
    return ans;
}

coloredPixel operator/(coloredPixel& a, int n)
{
    coloredPixel ans;
    ans.rgbBlue = a.rgbBlue/n;
    ans.rgbGreen = a.rgbGreen/n;
    ans.rgbRed = a.rgbRed/n;
    return ans; 
}

BMP::BMP(const char* path)
{
    ifstream pic_in;
    pic_in.open("lena.bmp", ios::binary);
    //读取文件头和信息头
    pic_in.read((char*) &bmfh, sizeof(bmfh));
    pic_in.read((char*) &bmih, sizeof(bmih));
    if(bmfh.bfType != 0x4D42)
    {
        throw("NOT a bmp file\n");
    }

    //读取调色板数据
    bmiColors = nullptr;
    if(bmih.biClrUsed)          //如果调色板数不为0
    {
        bmiColors = new RGBQUAD[bmih.biClrUsed];
        pic_in.read((char*)bmiColors, bmih.biClrUsed);
    }

    //先将所有像素信息指针置空
    this->pixels = nullptr;
    pixels = new char[bmfh.bfSize-bmfh.bfOffBits];
    // uint64_t size = bmih.biHeight * bmih.biWidth;
    // size = labs(size);          //总像素数
    pic_in.read((char*)pixels, (bmfh.bfSize-bmfh.bfOffBits));
}



BMP::BMP(const BMPFileHeader& bfh, const BMPInfoHeader& bih, void* pixel_data, RGBQUAD* colors = nullptr)
{
    memcpy(&bmfh, &bfh, sizeof(bfh));
    memcpy(&bmih, &bih, sizeof(bih));
    unsigned long long int size = (uint64_t)bmih.biHeight*bmih.biWidth;
    size = labs(size);
    switch (bmih.biBitCount)
    {
    case 1:
        pixels = new char[size/8];
        break;
    
    case 8:
        pixels = new char[size];
        break;

    case 16:
        pixels = new uint16_t[size];
        break;

    case 24:
        pixels = new char[size*3];
        break;

    default:
        throw("Unsupported bit depth\n");
        break;
    }
    memcpy(pixels, pixel_data, (bmfh.bfSize-bmfh.bfOffBits));
    if(colors != nullptr)
    {
        int map_size = bih.biClrUsed*sizeof(RGBQUAD);
        bmiColors = new RGBQUAD[bih.biClrUsed];
        memcpy(bmiColors, colors, map_size);
    }
}


BMP::BMP(const BMP& pic)
{
    new (this)BMP(pic.getFH(), pic.getIH(), pic.getData(), pic.getColorMap());
}

void BMP::save(const char* path)
{
    ofstream pic_save;
    pic_save.open(path, ios::binary);
    pic_save.write((char*) &bmfh, sizeof(bmfh));
    pic_save.write((char*) &bmih, bmih.biSize);
    if(bmih.biClrUsed)
    {
        pic_save.write((char*) bmiColors, bmih.biClrUsed*sizeof(RGBQUAD));
        cout << "ddd" << endl;
    }
    pic_save.write((char*) pixels, (bmfh.bfSize-bmfh.bfOffBits));
    cout << "saving over" << endl;
    return;
}

BMP::~BMP()
{
    delete[] (char*)pixels;
}

BMP BMP::to8Gray()
{
    if(bmih.biBitCount != 24)
    {
        throw("Type error! Only support 24bit figure\n");
    }
    //调色板构造
    RGBQUAD* color_map = new RGBQUAD[256];
    for(int i = 0; i < 256; i++)
    {
        color_map[i].Reserved = 0;
        color_map[i].rgbBlue = i;
        color_map[i].rgbGreen = i;
        color_map[i].rgbRed = i;
    }

    unsigned long long size = (unsigned long long)abs(bmih.biHeight) * (unsigned long long)bmih.biWidth;
    uint8_t* px_data = new uint8_t[size];
    coloredPixel* it = (coloredPixel*) pixels;
    for(unsigned long long i = 0; i < size; i++)
    {
        double temp = 0.3*(int)it->rgbRed + 0.59*(int)it->rgbGreen + 0.11*(int)it->rgbBlue;
        px_data[i] = (uint8_t)temp;
        it++;
    }
    
    //文件头创建
    BMPFileHeader fh;
    fh.bfType = 0x4D42;
    fh.bfOffBits = sizeof(BMPFileHeader) + sizeof(BMPInfoHeader) + 256*sizeof(RGBQUAD);
    fh.bfSize = fh.bfOffBits + size;
    fh.bfReserved1 = 0;
    fh.bfReserved2 = 0;

    //信息头创建
    BMPInfoHeader ih = bmih;
    ih.biBitCount = 8;
    ih.biClrUsed = 256;
    ih.biClrImportant = 256;
    ih.biSizeImage = 0;

    BMP gray(fh, ih, px_data, color_map);
    delete[] px_data;
    delete[] color_map;
    return gray;
}

BMP BMP::to24Gray()
{
    if(bmih.biBitCount != 24)
    {
        throw("Type error! Only support 24bit figure\n");
    }
    unsigned long long size = (unsigned long long)abs(bmih.biHeight) * (unsigned long long)bmih.biWidth;
    coloredPixel* gray_pixels = new coloredPixel[size];
    coloredPixel* it = (coloredPixel*)pixels;

    for(unsigned long long i = 0; i < size; i++)
    {
        double temp = 0.3*(int)it->rgbRed + 0.59*(int)it->rgbGreen + 0.11*(int)it->rgbBlue;
        gray_pixels[i].rgbBlue = (uint8_t) temp;
        gray_pixels[i].rgbGreen = (uint8_t) temp;
        gray_pixels[i].rgbRed = (uint8_t) temp;
        it++;
    }
    
    BMP gray(bmfh, bmih, gray_pixels);
    delete[] gray_pixels;
    return gray;
}

BMP BMP::smooth()
{
    BMPFileHeader smoothed_fh = bmfh;
    BMPInfoHeader smoothed_ih = bmih;
    if(bmih.biBitCount == 24)
    {
        uint64_t size = (uint64_t)bmih.biHeight * (uint64_t)bmih.biWidth;
        size = labs(size);
        coloredPixel* smoothed_pixels = new coloredPixel[size];
        coloredPixel** px_matrix = new coloredPixel*[bmih.biWidth];
        for(int i = 0; i < abs(bmih.biHeight); i++)
        {
            px_matrix[i] = (coloredPixel*)pixels+i*bmih.biWidth;
        }

        cout << "matrix constructed" << endl;

        coloredPixel* it = smoothed_pixels;
        it[0] = px_matrix[0][0]/4 + px_matrix[0][1]/4 + px_matrix[1][0]/4 + px_matrix[1][1]/4;
        uint64_t counter = 1;
        for(int j = 1; j < bmih.biWidth-1; j++)
        {
            it[counter] = px_matrix[0][j]/6 + px_matrix[0][j-1]/6 +px_matrix[0][j+1]/6 
            + px_matrix[1][j]/6 + px_matrix[1][j-1]/6 + px_matrix[1][j+1]/6;
            counter++;
        }

        it[counter] = px_matrix[0][bmih.biWidth-2]/4 + px_matrix[0][bmih.biWidth-1]/4 + 
        px_matrix[1][bmih.biWidth-2]/4 + px_matrix[1][bmih.biWidth-1]/4;
        counter++;

        for(int i = 1; i < bmih.biHeight-1; i++)
        {
            it[counter] = px_matrix[i][0]/6 + px_matrix[i][1]/6 + px_matrix[i-1][0]/6 
            + px_matrix[i-1][1]/6 + px_matrix[i+1][0]/6 + px_matrix[i+1][1]/6;
            counter++;

            for(int j = 1; j < bmih.biWidth-1; j++)
            {
                it[counter] = px_matrix[i-1][j-1]/9 + px_matrix[i-1][j]/9 + px_matrix[i-1][j+1]/9 +
                px_matrix[i][j-1]/9 + px_matrix[i][j]/9 + px_matrix[i][j+1]/9 +
                px_matrix[i+1][j-1]/9 + px_matrix[i+1][j]/9 + px_matrix[i+1][j+1]/9;
                counter++;
            }

            it[counter] = px_matrix[i-1][bmih.biWidth-2]/6 + px_matrix[i-1][bmih.biWidth-1]/6 + 
            px_matrix[i][bmih.biWidth-2]/6 + px_matrix[i][bmih.biWidth-1]/6 + 
            px_matrix[i+1][bmih.biWidth-2]/6 + px_matrix[i+1][bmih.biWidth-1]/6;
            counter++;
        }

        it[counter] = px_matrix[bmih.biHeight-1][0]/4 + px_matrix[bmih.biHeight-1][1]/4 + 
        px_matrix[bmih.biHeight-2][0]/4 + px_matrix[bmih.biHeight-2][1]/4;
        counter++;

        for(int j = 1; j < bmih.biWidth-1; j++)
        {
            it[counter] = px_matrix[bmih.biHeight-2][j-1]/6 + px_matrix[bmih.biHeight-2][j]/6 + px_matrix[bmih.biHeight-2][j+1]/6 +
            px_matrix[bmih.biHeight-1][j-1]/6 + px_matrix[bmih.biHeight-1][j]/6 + px_matrix[bmih.biHeight-1][j+1]/6;
            counter++;
        }

        it[counter] = px_matrix[bmih.biHeight-2][bmih.biWidth-2]/4 + px_matrix[bmih.biHeight-2][bmih.biWidth-1]/4 +
        px_matrix[bmih.biHeight-1][bmih.biWidth-2]/4 + px_matrix[bmih.biHeight-1][bmih.biWidth-1]/4;
        counter++;
        BMP ans(smoothed_fh, smoothed_ih, smoothed_pixels);
        delete[] smoothed_pixels;
        delete[] px_matrix;
        return ans;
    }
    else if(bmih.biBitCount == 8)
    {
        uint64_t size = (uint64_t)bmih.biHeight * (uint64_t)bmih.biWidth;
        size = labs(size);
        uint8_t* smoothed_pixels = new uint8_t[size];
        uint8_t** px_matrix = new uint8_t*[bmih.biWidth];
        for(int i = 0; i < abs(bmih.biHeight); i++)
        {
            px_matrix[i] = (uint8_t*)pixels+i*bmih.biWidth;
        }
        uint8_t* it = smoothed_pixels;
        it[0] = px_matrix[0][0]/4 + px_matrix[0][1]/4 + px_matrix[1][0]/4 + px_matrix[1][1]/4;
        uint64_t counter = 1;
        for(int j = 1; j < bmih.biWidth-1; j++)
        {
            it[counter] = px_matrix[0][j]/6 + px_matrix[0][j-1]/6 +px_matrix[0][j+1]/6 
            + px_matrix[1][j]/6 + px_matrix[1][j-1]/6 + px_matrix[1][j+1]/6;
            counter++;
        }

        it[counter] = px_matrix[0][bmih.biWidth-2]/4 + px_matrix[0][bmih.biWidth-1]/4 + 
        px_matrix[1][bmih.biWidth-2]/4 + px_matrix[1][bmih.biWidth-1]/4;
        counter++;

        for(int i = 1; i < bmih.biHeight-1; i++)
        {
            it[counter] = px_matrix[i][0]/6 + px_matrix[i][1]/6 + px_matrix[i-1][0]/6 
            + px_matrix[i-1][1]/6 + px_matrix[i+1][0]/6 + px_matrix[i+1][1]/6;
            counter++;

            for(int j = 1; j < bmih.biWidth-1; j++)
            {
                it[counter] = px_matrix[i-1][j-1]/9 + px_matrix[i-1][j]/9 + px_matrix[i-1][j+1]/9 +
                px_matrix[i][j-1]/9 + px_matrix[i][j]/9 + px_matrix[i][j+1]/9 +
                px_matrix[i+1][j-1]/9 + px_matrix[i+1][j]/9 + px_matrix[i+1][j+1]/9;
                counter++;
            }

            it[counter] = px_matrix[i-1][bmih.biWidth-2]/6 + px_matrix[i-1][bmih.biWidth-1]/6 + 
            px_matrix[i][bmih.biWidth-2]/6 + px_matrix[i][bmih.biWidth-1]/6 + 
            px_matrix[i+1][bmih.biWidth-2]/6 + px_matrix[i+1][bmih.biWidth-1]/6;
            counter++;
        }

        it[counter] = px_matrix[bmih.biHeight-1][0]/4 + px_matrix[bmih.biHeight-1][1]/4 + 
        px_matrix[bmih.biHeight-2][0]/4 + px_matrix[bmih.biHeight-2][1]/4;
        counter++;

        for(int j = 1; j < bmih.biWidth-1; j++)
        {
            it[counter] = px_matrix[bmih.biHeight-2][j-1]/6 + px_matrix[bmih.biHeight-2][j]/6 + px_matrix[bmih.biHeight-2][j+1]/6 +
            px_matrix[bmih.biHeight-1][j-1]/6 + px_matrix[bmih.biHeight-1][j]/6 + px_matrix[bmih.biHeight-1][j+1]/6;
            counter++;
        }

        it[counter] = px_matrix[bmih.biHeight-2][bmih.biWidth-2]/4 + px_matrix[bmih.biHeight-2][bmih.biWidth-1]/4 +
        px_matrix[bmih.biHeight-1][bmih.biWidth-2]/4 + px_matrix[bmih.biHeight-1][bmih.biWidth-1]/4;
        counter++;
        BMP ans(smoothed_fh, smoothed_ih, smoothed_pixels, bmiColors);
        cout << "construct over" << endl;
        delete[] smoothed_pixels;
        delete[] px_matrix;
        return ans;
    }
    else
    {
        throw("unsupported type\n");
    }
}

inline coloredPixel lumAdjust24(const coloredPixel& in, const double& a)
{
    if(a < 0 || a > 1)throw("arguement range error\n");
    coloredPixel ans;
    double r_d, g_d, b_d;
    r_d = pow((double)in.rgbRed, a) * pow(255.0, 1-a);
    g_d = pow((double)in.rgbGreen, a) * pow(255.0, 1-a);
    b_d = pow((double)in.rgbBlue, a) * pow(255.0, 1-a);
    int r, g, b;
    r = (int)r_d; g = (int)g_d; b = (int)b_d;
    if(r > 255){r = 255;}
    if(g > 255){g = 255;}
    if(b > 255){b = 255;}
    ans.rgbBlue = (uint8_t)b;
    ans.rgbGreen = (uint8_t)g;
    ans.rgbRed = (uint8_t)r;
    return ans;
}

inline uint8_t lumAdjust8(const uint8_t& in, const double& a)
{
    if(a < 0 || a > 1)throw("arguement range error\n");
    double ans_d = pow((double) in, a) * pow(255.0, 1-a);
    int ans = (int)ans_d;
    if(ans > 255) ans = 255;
    return (uint8_t)ans;
}
BMP BMP::luminance(const double& a)
{
    if(bmih.biBitCount == 24)
    {
        uint64_t size = (uint64_t)bmih.biHeight * (uint64_t)bmih.biWidth;
        size = labs(size);
        coloredPixel* lightened_pixels = new coloredPixel[size];

        for(uint64_t i = 0; i < size; i++)
        {
            lightened_pixels[i] = lumAdjust24(((coloredPixel*)pixels)[i], a);
        }
        BMP ans(bmfh, bmih, lightened_pixels);        
        delete[] lightened_pixels;
        return ans;
    }
    else if(bmih.biBitCount == 8)
    {
        uint64_t size = (uint64_t)bmih.biHeight * (uint64_t)bmih.biWidth;
        size = labs(size);
        uint8_t* lightened_pixels = new uint8_t[size];

        for(uint64_t i = 0; i < size; i++)
        {
            lightened_pixels[i] = lumAdjust8(((uint8_t*)pixels)[i], a);
        }
        BMP ans(bmfh, bmih, lightened_pixels, bmiColors);
        delete[] lightened_pixels;
        return ans;
    }
    else
    {
        throw("unsupported figure type\n");
    }
}


inline uint8_t conAdjust8(const uint8_t& in, const double& a)
{
    double in_d = (double)in;
    double ans_d = 128.0 + (in_d-128.0)*(1+a);
    int ans = (int)ans_d;
    if(ans > 255) ans = 255;
    else if(ans < 0) ans = 0;
    return (uint8_t)ans;
}

inline coloredPixel conAdjust24(const coloredPixel& in, const double& a)
{
    coloredPixel ans;
    ans.rgbBlue = conAdjust8(in.rgbBlue, a);
    ans.rgbGreen = conAdjust8(in.rgbGreen, a);
    ans.rgbRed = conAdjust8(in.rgbRed, a);
    return ans;
}
BMP BMP::contrast(const double& a)
{
    if(a > 1 || a < -1){throw("arguement range error\n");}
    if(bmih.biBitCount == 24)
    {
        uint64_t size = (uint64_t)bmih.biHeight * (uint64_t)bmih.biWidth;
        size = labs(size);
        coloredPixel* con_pixels = new coloredPixel[size];
        for(int i = 0; i < size; i++)
        {
            con_pixels[i] = conAdjust24(((coloredPixel*)pixels)[i], a);
        }
        BMP ans(bmfh, bmih, con_pixels);
        delete[] con_pixels;
        return ans;
    }
    else if(bmih.biBitCount == 8)
    {
        uint64_t size = (uint64_t)bmih.biHeight * (uint64_t)bmih.biWidth;
        size = labs(size);
        uint8_t* con_pixels = new uint8_t[size];
        for(int i = 0; i < size; i++)
        {
            con_pixels[i] = conAdjust8(((uint8_t*)pixels)[i], a);
        }
        BMP ans(bmfh, bmih, con_pixels, bmiColors);
        delete[] con_pixels;
        return ans;
    }
    else
    {
        throw("unsupported figure type\n");
    }
}


BMP BMP::gray2BnW()
{
    if(bmih.biBitCount != 8){throw("unsupported figure type\n");}
    uint64_t size = (uint64_t)bmih.biHeight * (uint64_t)bmih.biWidth;
    size = labs(size);
    uint8_t* BnW_pixels = new uint8_t[size];
    for(uint64_t i = 0; i < size; i++)
    {
        BnW_pixels[i] = ((uint8_t*)pixels)[i] < 128? 0:255;
    }
    BMP ans(bmfh, bmih, BnW_pixels, bmiColors);
    delete[] BnW_pixels;
    return ans;
}