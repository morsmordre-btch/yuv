#ifndef BITMAPFILE_H
#define BITMAPFILE_H

#include <iostream>
#include <fstream>
#include <vector>

#define BMP_TYPE 0x4D42

#define KR 0.299
#define KB 0.114

#pragma pack(push, 2)
struct bitmap_file_header {
    uint16_t type;
    uint32_t size;
    uint16_t reserv_0;
    uint16_t reserv_1;
    uint32_t offset;
};
#pragma pack(pop)

struct bitmap_info_header {
    uint32_t size_bi_header;
    uint32_t width;
    uint32_t height;
    uint16_t number_planes;
    uint16_t bits_per_pixel;
    uint32_t compression;
    uint32_t raw_size;
    uint32_t horizontal_resolution;
    uint32_t vertical_resolution;
    uint32_t colors_used;
    uint32_t important_colors;
};

struct pixel{
    uint8_t R;
    uint8_t G;
    uint8_t B;
};

struct yuv{
    std::vector<uint8_t> Y;
    std::vector<uint8_t> U;
    std::vector<uint8_t> V;
};

class BitmapFile
{
    std::fstream bitmap_file;

    bitmap_file_header file_header;
    bitmap_info_header info_header;


    std::vector<uint8_t> pixel_array;

    uint32_t padding_size;


public:
    yuv yuv_array;
    BitmapFile();
    int read_image(std::string path_to_file);
    void bitmap_to_yuv420(std::string path_to_yuv420);
    void bitmap_to_yuv420();

    pixel get_pixel(uint16_t x, uint16_t y);
    void set_pixel(uint16_t x, uint16_t y, pixel pix);
    void draw_rect(uint16_t x, uint16_t y,
                   uint16_t offset_x, uint16_t offset_y,
                   pixel color);

    std::size_t get_width();
    std::size_t get_height();
private:
    yuv bgr_to_yuv420(int B, int G, int R);
    std::vector<uint8_t> read_pixel_array();
    void set_pixel_array(uint16_t x, uint16_t y, pixel pix);
};

#endif // BITMAPFILE_H
