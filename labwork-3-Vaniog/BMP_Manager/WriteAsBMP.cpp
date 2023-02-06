#include "WriteAsBMP.h"

void WriteSandHeapAsBMP(const uint64_t* image,
                        const uint32_t& width, const uint32_t& height,
                        const std::string& file_name) { // image'[x][y] == image[y * width + x]
    struct BMPFileHeader file_header;
    struct BMPInfoHeader info_header;

    uint32_t bits_per_pixel = Color::kSize * 8;
    uint32_t bitmap_row_size = (bits_per_pixel * width + 31) / 32 * 4;
    uint32_t bitmap_data_size = bitmap_row_size * height;
    file_header.file_size = BMPFileHeader::size + BMPInfoHeader::size + bitmap_data_size;
    info_header.bitmap_data_size = bitmap_data_size;
    info_header.bitmap_width = width;
    info_header.bitmap_height = height;

    uint32_t padding_size = bitmap_row_size - bits_per_pixel / 8 * width;
    std::ofstream file(file_name, std::ios::binary | std::ios::out);
    file_header.WriteToFile(file);
    info_header.WriteToFile(file);
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            uint64_t pixel = image[y * width + x];
            if (pixel == 0) {
                Color(Color::WHITE).WriteToFile(file);
            } else if (pixel == 1) {
                Color(2, 50, 32).WriteToFile(file); // Green
            } else if (pixel == 2) {
                Color(138, 43, 226).WriteToFile(file); // Purple
            } else if (pixel == 3) {
                Color(255, 215, 0).WriteToFile(file); // Yellow
            } else {
                Color(Color::BLACK).WriteToFile(file);
            }
        }
        file.write("0", padding_size);
    }
}

void WriteVectorAsBMP(std::vector<std::vector<Color>>& image,
                      const uint32_t& width, const uint32_t& height,
                      const std::string& file_name) {
    struct BMPFileHeader file_header;
    struct BMPInfoHeader info_header;

    uint32_t bits_per_pixel = Color::kSize * 8;
    uint32_t bitmap_row_size = (bits_per_pixel * width + 31) / 32 * 4;
    uint32_t bitmap_data_size = bitmap_row_size * height;
    file_header.file_size = BMPFileHeader::size + BMPInfoHeader::size + bitmap_data_size;
    info_header.bitmap_data_size = bitmap_data_size;
    info_header.bitmap_width = width;
    info_header.bitmap_height = height;

    uint32_t padding_size = bitmap_row_size - bits_per_pixel / 8 * width;
    std::ofstream file(file_name, std::ios::binary | std::ios::out);
    file_header.WriteToFile(file);
    info_header.WriteToFile(file);
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            image[x][y].WriteToFile(file);
        }
        file.write("0", padding_size);
    }
}
