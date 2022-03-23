#include "bitmapfile.h"

BitmapFile::BitmapFile()
{

}

int BitmapFile::read_image(std::string path_to_file) {
    bitmap_file.open(path_to_file, std::ios::binary | std::ios::in | std::ios::out);
    if (!bitmap_file.is_open()) {
        std::cout << "Файл не открыт" << std::endl;
        bitmap_file.clear();
        bitmap_file.close();
        return -1;
    }
    else
        std::cout << "Файл успешно открыт" << std::endl;

    bitmap_file.read((char*)&file_header, sizeof(file_header));
    bitmap_file.read((char*)&info_header, sizeof(info_header));

    if (file_header.type != BMP_TYPE) {
        std::cout << "Формат файла не соответствует bmp" << std::endl;
        return -1;
    }

    if (info_header.bits_per_pixel != 24) {
        std::cout << "Количество бит на пиксель не равно 24" << std::endl;
        return -1;
    }

    if ((info_header.width * info_header.bits_per_pixel/8) % 4 == 0)
        padding_size = 0;
    else
        padding_size = 4 - (info_header.width * info_header.bits_per_pixel/8) % 4;
    std::cout << "Padding size: " << padding_size << std::endl;

    pixel_array = read_pixel_array();

    return 0;
}

std::size_t BitmapFile::get_width() {
    return info_header.width;
}
std::size_t BitmapFile::get_height() {
    return info_header.height;
}

std::vector<uint8_t> BitmapFile::read_pixel_array() {
    std::vector<uint8_t> pixel_array(info_header.width*info_header.height *
                                     info_header.bits_per_pixel/8 +
                                     padding_size*info_header.height);
    bitmap_file.seekg(file_header.offset);
    bitmap_file.read((char*)pixel_array.data(), pixel_array.size());
    return pixel_array;
}

pixel BitmapFile::get_pixel(uint16_t x, uint16_t y) {
    pixel pix;
    uint32_t offset = (info_header.height - 1 - y) *
                      ((info_header.width) * info_header.bits_per_pixel/8 +
                       padding_size) + x * info_header.bits_per_pixel/8;
    pix.B = pixel_array[offset];
    pix.G = pixel_array[offset+1];
    pix.R = pixel_array[offset+2];
    return pix;
}

void BitmapFile::set_pixel_array(uint16_t x, uint16_t y, pixel pix) {
    uint32_t offset = (info_header.height - 1 - y) *
                      ((info_header.width) * info_header.bits_per_pixel/8 +
                       padding_size) + x * info_header.bits_per_pixel/8;
    pixel_array[offset] = pix.B;
    pixel_array[offset+1] = pix.G;
    pixel_array[offset+2] = pix.R;
}

void BitmapFile::set_pixel(uint16_t x, uint16_t y, pixel pix) {
    uint32_t offset = (info_header.height - 1 - y) *
                      ((info_header.width) * info_header.bits_per_pixel/8 +
                       padding_size) + x * info_header.bits_per_pixel/8;
    pixel_array[offset] = pix.B;
    pixel_array[offset+1] = pix.G;
    pixel_array[offset+2] = pix.R;
    bitmap_file.seekp(file_header.offset + offset);
    bitmap_file.write((char*)&pix, sizeof(pix));
}

void BitmapFile::draw_rect(uint16_t x, uint16_t y, uint16_t offset_x, uint16_t offset_y, pixel color) {
    uint32_t x_end = x + offset_x;
    uint32_t y_end = y + offset_y;
    if (x_end > info_header.width)
        x_end = info_header.width;
    if (y_end > info_header.height)
        y_end = info_header.height;

    for (int i = y; i < y_end; i++) {
        for (int j = x; j < x_end; j++) {
            set_pixel_array(j, i, color);
        }
    }

    bitmap_file.seekp(file_header.offset);
    bitmap_file.write((char*)pixel_array.data(), pixel_array.size());
}


void BitmapFile::bitmap_to_yuv420(std::string path_to_yuv420) {
    std::ofstream yuv_file(path_to_yuv420, std::ios::binary | std::ios::out);
    bitmap_to_yuv420();
    yuv_file.write((char*)yuv_array.Y.data(), yuv_array.Y.size());
    yuv_file.write((char*)yuv_array.U.data(), yuv_array.U.size());
    yuv_file.write((char*)yuv_array.V.data(), yuv_array.V.size());
    yuv_file.close();
}

void BitmapFile::bitmap_to_yuv420() {
    pixel pix;
    for (int i = 0; i < info_header.height; i++) {
        for (int j = 0; j < info_header.width; j++) {
            pix = get_pixel(j,i);
            yuv_array.Y.push_back(16 + (66 * pix.R/256 + 129 * pix.G/256 + 25 * pix.B/256));
            if (i % 2 == 0)
                if (j % 2 == 0) {
                    yuv_array.U.push_back(128 + (-38 * pix.R/256 - 74 * pix.G/256 + 112 * pix.B/256));
                    yuv_array.V.push_back(128 + (112 * pix.R/256 - 94 * pix.G/256 - 18 * pix.B/256));
                }
        }
    }

    std::cout << "Y size: " << yuv_array.Y.size() << std::endl;
    std::cout << "U size: " << yuv_array.U.size() << std::endl;
    std::cout << "V size: " << yuv_array.V.size() << std::endl;
}
