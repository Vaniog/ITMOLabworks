#pragma once
#include "../BMP_Manager/WriteAsBMP.h"
#include "SandData.h"
#include <filesystem>

struct SandHeap {
    uint32_t width;
    uint32_t height;
    uint64_t* field;

    SandData sand_data;
    explicit SandHeap(const SandData& sand_data);
    ~SandHeap();

    void SetPixel(const uint16_t& x, const uint16_t& y, const uint64_t& pixel);
    uint64_t* GetPixelOld(const uint16_t& x, const uint16_t& y) const;
    uint64_t* GetPixelNew(const uint16_t& x, const uint16_t& y) const;

    uint64_t* new_field;
    bool Iteration();

    struct Added {
        uint8_t width_minus = 0;
        uint8_t width_plus = 0;
        uint8_t height_minus = 0;
        uint8_t height_plus = 0;
    };
    Added added;
    Added new_added;

    uint32_t new_width;
    uint32_t new_height;
    void UpdateNewPixel(const uint16_t& x, const uint16_t& y, const int64_t& change);

    void WriteToFile(const std::string& file_name) const {
        WriteSandHeapAsBMP(field, width, height, file_name);
    }

    void Run();
};

