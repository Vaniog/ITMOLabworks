#pragma once

#include <iostream>
#include <fstream>

struct BMPFileHeader {
private:
    uint16_t kFormat = 0x4d42; // BM kFormat
public:
    uint32_t file_size; // in bytes
private:
    uint32_t kReservedBytes = 0;
    uint32_t kBitmapDataStart = 54; // in bytes
public:
    void WriteToFile(std::ofstream& file);
public:
    static const uint32_t size; // in bytes
};

struct BMPInfoHeader {
private:
    const uint32_t header_size = size; // in bytes
public:
    uint32_t bitmap_width;
    uint32_t bitmap_height;
private:
    const uint16_t kPlanesAmount = 1;
    const uint16_t kBitsPerPixel = 24;
    const uint32_t kBiRgb = 0; // to save without compression, BI_RGB bitmap
public:
    uint32_t bitmap_data_size;
private:
    const uint32_t kHorizontalResolution = 1; // I don't know what is it
    const uint32_t kVerticalResolution = 1; // I don't know what is it
    const uint32_t kColorsAmount = 0; // to use all colors
    const uint32_t kImportantColorsAmount = 0; // all colors matter, even black
public:
    void WriteToFile(std::ofstream& file);
    static const uint32_t size; // in bytes
};

struct Color {
    uint8_t r;
    uint8_t g;
    uint8_t b;
    Color() {
        r = 255, g = 255, b = 255;
    }
    Color(uint8_t r, uint8_t g, uint8_t b) :
            r(r), g(g), b(b) {};
    static const Color WHITE;
    static const Color RED;
    static const Color GREEN;
    static const Color BLUE;
    static const Color PURPLE;
    static const Color YELLOW;
    static const Color BLACK;

    void WriteToFile(std::ofstream& file);
    static const uint32_t kSize; // in bytes
};
