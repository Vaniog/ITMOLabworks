#pragma once
#include "Byteset.h"

#include <fstream>

struct HafIFStream {
    explicit HafIFStream(const std::string& file_name,
                         std::ios::openmode open_mode = std::ios::in);

    static const uint32_t default_word_length = 8; // for word_length_haf in header
    [[nodiscard]] uint32_t GetWordLengthHaf() const;
private:
    uint32_t word_length_haf;
    void ReadWordLength(); // for word_length_haf in header
public:
    void SetWordLength(uint32_t length_haf);

    uint32_t start_byte; // without word_length_haf in header
    void Read(char* bytes_out, uint32_t bytes_amount);
    void ReadUnconverted(char* bytes_out, uint32_t bytes_amount); // without converting
    void SeekG(uint32_t pos, std::ios::seekdir seek);
    void CleanTrash(); // free the basic_data_byteset, to clean padding bits from the basic_data_byteset
    uint32_t Size() const; // without word_length_haf in header
    void Close();

private:
    Byteset basic_data_byteset; // just data
    Byteset haf_data_byteset; // in hamming code
    uint32_t size; // in bytes
    std::ifstream haf_file_in;
    void Unconvert();
};

struct HafOFStream {
    explicit HafOFStream(const std::string& file_name,
                         uint32_t word_length_haf,
                         std::ios::openmode open_mode = std::ios::out);
    static const uint32_t default_word_length = 8; // for word_length_haf in header
private:
    uint32_t word_length_haf;
    uint32_t word_length_basic;
    void WriteWordLength(); // for word_length_haf in header
public:
    void SetWordLength(uint32_t length_haf); // for word_length_haf in header

    void Write(const char* bytes_out, uint32_t bytes_amount);
    void WriteUnconverted(const char* bytes_out, uint32_t bytes_amount); // without converting
    void FreeData(); /* writes padding to the basic_data_byteset while it isn't convertable, then convert it
    adds padding bits to the haf_file
    so the next data will be written from an integer position of bytes */

    void Close();
    ~HafOFStream();

private:
    Byteset basic_data_byteset; // just data
    Byteset haf_data_byteset; // in hamming code
    std::ofstream haf_file_out;
    void Convert();
};