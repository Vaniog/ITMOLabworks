#pragma once
#include <iostream>

struct HafMath{
    static uint32_t FromHafToBasicLength(uint32_t haf_length);
    static uint32_t FromBasicToHafSize(uint32_t basic_size, uint32_t word_length_haf);
    static uint32_t GetWordLengthFromFile(std::string& file_name);
};