#include "yuvfile.h"

YuvFile::YuvFile()
{

}

int YuvFile::read_yuv(std::string path_to_file, std::size_t width, std::size_t height) {
    yuv_video.open(path_to_file, std::ios::binary | std::ios::in | std::ios::out);
    if (!yuv_video.is_open()) {
        std::cout << "Файл не открыт" << std::endl;
        yuv_video.clear();
        yuv_video.close();
        return -1;
    }
    else
        std::cout << "Файл успешно открыт" << std::endl;
    this->width = width;
    this->height = height;
    return 0;
}

yuv YuvFile::read_frame() {
    yuv frame;
    frame.Y.resize(width*height);
    frame.U.resize(width*height/4);
    frame.V.resize(width*height/4);
    yuv_video.read((char*)frame.Y.data(), width*height);
    yuv_video.read((char*)frame.U.data(), width*height/4);
    yuv_video.read((char*)frame.V.data(), width*height/4);
    return frame;
}

int YuvFile::insert_image(std::size_t x_pos, std::size_t y_pos, BitmapFile *bmp_file) {
    yuv yuv_image = bmp_file->yuv_array;
    std::size_t width_image = bmp_file->get_width();
    std::size_t height_image = bmp_file->get_height();
    std::ofstream yuv_file("out.yuv", std::ios::binary | std::ios::out);
    while(!yuv_video.eof()) {
        frame = read_frame();

        for (std::size_t y = 0; y < height_image; y++) {
            for (std::size_t x = 0; x < width_image; x++) {
                frame.Y[get_Y_pos(x+x_pos,y+y_pos,width,height)] = yuv_image.Y[get_Y_pos(x,y,width_image,height_image)];
            }
        }
        for (std::size_t y = 0; y < height_image/2; y++) {
            for (std::size_t x = 0; x < width_image/2; x++) {
                frame.U[get_U_V_pos(x+x_pos/2,y+y_pos/2,width,height)] = yuv_image.U[get_U_V_pos(x,y,width_image,height_image)];
            }
        }
        for (std::size_t y = 0; y < height_image/2; y++) {
            for (std::size_t x = 0; x < width_image/2; x++) {
                frame.V[get_U_V_pos(x+x_pos/2,y+y_pos/2,width,height)] = yuv_image.V[get_U_V_pos(x,y,width_image,height_image)];
            }
        }
        yuv_file.write((char*)frame.Y.data(), frame.Y.size());
        yuv_file.write((char*)frame.U.data(), frame.U.size());
        yuv_file.write((char*)frame.V.data(), frame.V.size());
    }
    return 0;
}

std::size_t YuvFile::get_Y_pos(std::size_t x, std::size_t y, std::size_t width, std::size_t height) {
    return (y * width + x);
}

std::size_t YuvFile::get_U_V_pos(std::size_t x, std::size_t y, std::size_t width, std::size_t height) {
    return (y * width/2 + x);
}

