#include <iostream>
#include "bitmapfile.h"
#include "yuvfile.h"

int main()
{
    BitmapFile *a = new BitmapFile;
    a->read_image("bitmap.bmp");

    a->draw_rect(90,90,20,20,pixel{0xFF, 0x00, 0xFF});


    a->bitmap_to_yuv420();

    YuvFile yuv_file;
    yuv_file.read_yuv("tt_352x240.yuv", 352, 240);

    yuv_file.insert_image(100,30, a);

    return 0;
}
