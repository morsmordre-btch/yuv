#ifndef YUVFILE_H
#define YUVFILE_H

#include <iostream>
#include <fstream>
#include <vector>
#include "bitmapfile.h"



class YuvFile
{
    yuv frame;
    std::size_t width;
    std::size_t height;
    std::fstream yuv_video;

public:
    YuvFile();
    int read_yuv(std::string path_to_file, std::size_t width, std::size_t height);

    int insert_image(std::size_t x, std::size_t y, BitmapFile *bmp_file);

private:
    yuv read_frame();

    std::size_t get_Y_pos(std::size_t x, std::size_t y, std::size_t width, std::size_t height);
    std::size_t get_U_V_pos(std::size_t x, std::size_t y, std::size_t width, std::size_t height);
};

#endif // YUVFILE_H
