#pragma once
#include "BMP_Structs.h"
#include <fstream>
#include <vector>

void WriteVectorAsBMP(std::vector<std::vector<Color>>& image,
                      const uint32_t& width, const uint32_t& height,
                      const std::string& file_name);
void WriteSandHeapAsBMP(const uint64_t* image,
                        const uint32_t& width, const uint32_t& height,
                        const std::string& file_name); // image'[x][y] == image[y * width + x]
