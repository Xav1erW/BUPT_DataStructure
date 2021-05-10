#include <iostream>
#include <memory>
using namespace std;
#pragma once

#pragma pack(1)
struct BMPFileHeader
{
    uint16_t bfType;        //2Bytes，必须为"BM"，即0x424D 才是Windows位图文件
    uint32_t bfSize;         //4Bytes，整个BMP文件的大小
    uint16_t bfReserved1;  //2Bytes，保留，为0
    uint16_t bfReserved2;  //2Bytes，保留，为0
    uint32_t bfOffBits;     //4Bytes，文件起始位置到图像像素数据的字节偏移量
};

#pragma pack(1)
struct BMPInfoHeader
{
    uint32_t biSize;     //4Bytes，信息头结构体大小，存在其他版本信息头，用作区分
    uint32_t biWidth;    //4Bytes，图像宽度（以像素为单位）
    int32_t biHeight;    //4Bytes，图像高度，+：图像存储顺序为Bottom2Top，-：Top2Bottom
    uint16_t biPlanes;    //2Bytes，图像数据平面，BMP存储RGB数据，因此总为1
    uint16_t biBitCount;         //2Bytes，图像像素位数
    uint32_t biCompression;     //4Bytes，0：不压缩，1：RLE8，2：RLE4
    uint32_t biSizeImage;       //4Bytes，4字节对齐的图像数据大小
    uint32_t biXPelsPerMeter;   //4 Bytes，用象素/米表示的水平分辨率
    uint32_t biYPelsPerMeter;   //4 Bytes，用象素/米表示的垂直分辨率
    uint32_t biClrUsed;          //4 Bytes，实际使用的调色板索引数，0：使用所有的调色板索引
    uint32_t biClrImportant;     //4 Bytes，重要的调色板索引数，0：所有的调色板索引都重要
};
// istream& operator >> (istream& in, BMPFileHeader& fh);
// istream& operator >> (istream& in, BMPInfoHeader& ih);
ostream& operator << (istream& out, BMPFileHeader& fh);
ostream& operator << (istream& out, BMPFileHeader& ih);

#pragma pack(1)
struct RGBQUAD                  //调色板结构体
{
    uint8_t rgbRed;
    uint8_t rgbGreen;
    uint8_t rgbBlue;
    uint8_t Reserved;
};


#pragma pack(1)
template<typename T>
struct Pixel                //像素数据类型，T类型具体取决于BMP文件信息头给出的像素位深
{
    T data;
};

#pragma pack(1)
struct coloredPixel         //24位深图像像素
{
    uint8_t rgbBlue;
    uint8_t rgbGreen;
    uint8_t rgbRed;
};

coloredPixel operator+(const coloredPixel&, const coloredPixel&);
coloredPixel operator/(coloredPixel&, int);

class BMP
{
private:
    BMPFileHeader bmfh;             //BMP文件头
    BMPInfoHeader bmih;             //BMP信息头
    RGBQUAD *bmiColors;             //调色板
    void* pixels;                     //存放像素数据
public:
    BMP(const char*);          //读取BMP文件
    BMP(const BMPFileHeader&, const BMPInfoHeader&, void*, RGBQUAD*);    //从数据创建
    BMP(const BMP&);            //拷贝构造
    void save(const char*);        //保存BMP到文件
    BMP to8Gray();                  //转8位灰度图
    BMP to24Gray();                 //转24位灰度图
    BMP smooth();                   //平滑化
    BMP luminance(const double&);  //调整亮度
    BMP contrast(const double&);    //调整对比度
    BMP gray2BnW();       //灰度图转黑白
    BMPFileHeader getFH()const{return bmfh;};
    BMPInfoHeader getIH()const{return bmih;};
    void* getData()const{return pixels;};
    RGBQUAD* getColorMap()const{return bmiColors;};
    ~BMP();
};

