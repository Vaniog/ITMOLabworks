#include "HafStream.h"
#include "HafMath.h"

#include <filesystem>

HafIFStream::HafIFStream(const std::string& file_name, // NOLINT (it says I don't init word_length_haf, it is false)
                         std::ios::openmode open_mode) {
    haf_file_in.open(file_name, open_mode);
    start_byte = HafMath::FromBasicToHafSize(4, default_word_length);
    ReadWordLength();
    size = std::filesystem::file_size(file_name);
}

void HafIFStream::ReadWordLength() {
    uint32_t tmp_length;
    SetWordLength(default_word_length);
    Read(reinterpret_cast<char*>(&tmp_length), 4);
    SetWordLength(tmp_length);
    SeekG(0, std::ios::beg);
}

void HafIFStream::SetWordLength(uint32_t length_haf) {
    word_length_haf = length_haf;
}

uint32_t HafIFStream::GetWordLengthHaf() const {
    return word_length_haf;
}

void HafIFStream::Read(char* bytes_out, uint32_t bytes_amount) {
    char byte;
    while (basic_data_byteset.GetSizeInBits() < bytes_amount * 8) {
        haf_file_in.read(&byte, 1);
        haf_data_byteset.PushByte(byte);
        while (haf_data_byteset.GetSizeInBits() >= word_length_haf) {
            Unconvert();
        }
    }
    basic_data_byteset.Read(bytes_out, bytes_amount);
}

void HafIFStream::ReadUnconverted(char* bytes_out, uint32_t bytes_amount) {
    haf_file_in.read(bytes_out, bytes_amount);
}

void HafIFStream::SeekG(uint32_t pos, std::ios::seekdir seek) {
    CleanTrash();
    haf_file_in.seekg(start_byte + pos, seek);
}

void HafIFStream::CleanTrash() {
    basic_data_byteset.Clear();
    haf_data_byteset.Clear();
}

uint32_t HafIFStream::Size() const {
    return size - start_byte;
}

void HafIFStream::Close() {
    haf_file_in.close();
}

HafOFStream::HafOFStream(const std::string& file_name,
                         uint32_t word_length_haf,
                         std::ios::openmode open_mode) : word_length_haf(word_length_haf) {
    haf_file_out.open(file_name, open_mode);
    word_length_basic = HafMath::FromHafToBasicLength(word_length_haf);
    if (open_mode != std::ios::app)
        WriteWordLength();
}

void HafOFStream::WriteWordLength() {
    uint32_t tmp_length = word_length_haf;
    SetWordLength(default_word_length);
    Write(reinterpret_cast<const char*>(&tmp_length), 4);
    FreeData();
    SetWordLength(tmp_length);
}

void HafOFStream::SetWordLength(uint32_t length_haf) {
    word_length_haf = length_haf;
    word_length_basic = HafMath::FromHafToBasicLength(word_length_haf);
}
void HafOFStream::Write(const char* bytes_out, uint32_t bytes_amount) {
    basic_data_byteset.Write(bytes_out, bytes_amount);
    while (basic_data_byteset.GetSizeInBits() >= word_length_basic) {
        Convert();
    }

    char byte;
    while (haf_data_byteset.GetSizeInBits() >= 8) {
        haf_data_byteset.Read(&byte, 1);
        haf_file_out.write(&byte, 1);
    }
}

void HafOFStream::WriteUnconverted(const char* bytes_out, uint32_t bytes_amount) {
    haf_file_out.write(bytes_out, bytes_amount);
}

void HafOFStream::FreeData() {
    if (basic_data_byteset.GetSizeInBits() == 0) {
        return;
    }
    while (basic_data_byteset.GetSizeInBits() < word_length_basic) {
        basic_data_byteset.PushBit(false); // padding
    }
    Convert();
    while (haf_data_byteset.GetSizeInBits() % 8 != 0)
        haf_data_byteset.PushBit(false); // padding

    char byte;
    while (haf_data_byteset.GetSizeInBits() != 0) {
        haf_data_byteset.Read(&byte, 1);
        haf_file_out.write(&byte, 1);
    }
}

void HafOFStream::Close() {
    haf_file_out.close();
}

HafOFStream::~HafOFStream() {
    FreeData();
}

void HafOFStream::Convert() {
    bool bits[word_length_haf];
    uint32_t fill_iter;
    for (fill_iter = 0; fill_iter < word_length_haf; fill_iter++) {
        bits[fill_iter] = true;
    }

    fill_iter = 1;
    while (fill_iter <= word_length_haf) {
        bits[fill_iter - 1] = false;
        fill_iter <<= 1;
    }

    // now in bits: ones at data bits, zeros at controlling bits

    fill_iter = 1;
    uint32_t fill_iter_copy;
    uint32_t control_iter;
    while (fill_iter <= word_length_haf) {
        if (bits[fill_iter - 1]) { // if bit isn't controlling
            bits[fill_iter - 1] = basic_data_byteset.GetBit(0);
            basic_data_byteset.PopBit();
        }
        control_iter = 1;
        fill_iter_copy = fill_iter;
        while (fill_iter_copy) { // xor with bit's controlling bits
            if (fill_iter_copy & 1) {
                bits[control_iter - 1] ^= bits[fill_iter - 1];
            }
            fill_iter_copy >>= 1;
            control_iter <<= 1;
        }
        fill_iter++;
    }

    for (fill_iter = 0; fill_iter < word_length_haf; fill_iter++) {
        haf_data_byteset.PushBit(bits[fill_iter]);
    }
}

void HafIFStream::Unconvert() {
    bool bits[word_length_haf];
    uint32_t fill_iter;
    for (fill_iter = 0; fill_iter < word_length_haf; fill_iter++) {
        bits[fill_iter] = haf_data_byteset.GetBit(0);
        // std::cout << bits[fill_iter];
        haf_data_byteset.PopBit();
    }
    fill_iter = 1;
    uint32_t fill_iter_copy;
    uint32_t control_iter;

    // std::cout << "->";

    while (fill_iter <= word_length_haf) {
        control_iter = 1;
        fill_iter_copy = fill_iter;
        while (fill_iter_copy) { // xor with bit's controlling bits
            if (fill_iter != control_iter && fill_iter_copy & 1) {
                bits[control_iter - 1] ^= bits[fill_iter - 1];
            }
            fill_iter_copy >>= 1;
            control_iter <<= 1;
        }
        fill_iter++;
    }

    // now in right controlling bits - zeros

    uint32_t wrong_bit = 0;

    fill_iter = 1;
    while (fill_iter <= word_length_haf) {
        if (bits[fill_iter - 1]) {
            wrong_bit += fill_iter;
        }
        fill_iter <<= 1;
    }

    if (wrong_bit != 0) {
        std::cout << "Error fixed\n";
        bits[wrong_bit - 1] = !bits[wrong_bit - 1];
    }

    control_iter = 4;
    for (fill_iter = 3; fill_iter <= word_length_haf; fill_iter++) {
        if (fill_iter == control_iter) {
            control_iter <<= 1;
            continue;
        }
        basic_data_byteset.PushBit(bits[fill_iter - 1]);
    }
}
