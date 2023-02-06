#include "BMP_Structs.h"

void BMPFileHeader::WriteToFile(std::ofstream& file) {
    file.write(reinterpret_cast<const char*>(&kFormat), 2);
    file.write(reinterpret_cast<const char*>(&file_size), 4);
    file.write(reinterpret_cast<const char*>(&kReservedBytes), 4);
    file.write(reinterpret_cast<const char*>(&kBitmapDataStart), 4);
}

const uint32_t BMPFileHeader::size = 14;

void BMPInfoHeader::WriteToFile(std::ofstream& file) {
    file.write(reinterpret_cast<const char*>(&header_size), 4);
    file.write(reinterpret_cast<const char*>(&bitmap_width), 4);
    file.write(reinterpret_cast<const char*>(&bitmap_height), 4);
    file.write(reinterpret_cast<const char*>(&kPlanesAmount), 2);
    file.write(reinterpret_cast<const char*>(&kBitsPerPixel), 2);
    file.write(reinterpret_cast<const char*>(&kBiRgb), 4);
    file.write(reinterpret_cast<const char*>(&bitmap_data_size), 4);
    file.write(reinterpret_cast<const char*>(&kHorizontalResolution), 4);
    file.write(reinterpret_cast<const char*>(&kVerticalResolution), 4);
    file.write(reinterpret_cast<const char*>(&kColorsAmount), 4);
    file.write(reinterpret_cast<const char*>(&kImportantColorsAmount), 4);
}

const uint32_t BMPInfoHeader::size = 40;

const Color Color::WHITE = Color(255, 255, 255);
const Color Color::RED = Color(255, 0, 0);
const Color Color::GREEN = Color(0, 255, 0);
const Color Color::BLUE = Color(0, 0, 255);
const Color Color::PURPLE = Color(255, 0, 255);
const Color Color::YELLOW = Color(255, 255, 0);
const Color Color::BLACK = Color(0, 0, 0);

void Color::WriteToFile(std::ofstream& file) {
    file.write(reinterpret_cast<const char*>(&b), 1);
    file.write(reinterpret_cast<const char*>(&g), 1);
    file.write(reinterpret_cast<const char*>(&r), 1);
}

const uint32_t Color::kSize = 3;