#pragma once
#include <iostream>

#include "HafStream.h"

/*
 * structure of .haf file
 *
 * word_length_haf
 *
 * file1->name_length
 * file1->name
 * file1->size_of_header
 * file1 copy
 *
 * file2->name_length
 * ...
 *
 */

struct HafFileHeader {
    std::string name;
    std::string name_without_slashes; // will be written into archive
    static std::string ConvertName(const std::string& file_name); // convert path_name to name, delete slashes
    uint8_t name_length{};
    uint32_t size_of_header{}; // header in bytes
    uint32_t size_of_data{}; // without header in bytes
    uint32_t start_pos{}; // in bytes

    explicit HafFileHeader(std::string name);
    explicit HafFileHeader(HafIFStream& haf_ifstream, uint32_t start_pos);

    void WriteToHaf(HafOFStream& haf_ofstream) const;
    void ReadFromHaf(HafIFStream& haf_ifstream);
    void ExtractToBasic(HafIFStream& haf_ifstream, const std::string& arch_dir_name) const;

    bool operator<(const HafFileHeader& file_header) const { // for using in sets
        return size_of_data < file_header.size_of_data;
    }
};