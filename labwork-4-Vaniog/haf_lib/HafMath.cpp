#include "HafMath.h"
#include "HafStream.h"
#include <cmath>

uint32_t HafMath::FromHafToBasicLength(uint32_t haf_length) {
    return haf_length - static_cast<uint32_t>(ceil(log2(static_cast<double>(haf_length) + 0.001)));
}

uint32_t HafMath::FromBasicToHafSize(uint32_t basic_size, uint32_t word_length_haf) {
    uint32_t word_length_basic = FromHafToBasicLength(word_length_haf);
    uint32_t haf_size_bits =
            static_cast<uint32_t>(ceil(static_cast<double>(basic_size * 8) / word_length_basic))
                    * word_length_haf;
    if (haf_size_bits % 8 != 0)
        return haf_size_bits / 8 + 1; // for padding
    return haf_size_bits / 8;
}

uint32_t HafMath::GetWordLengthFromFile(std::string& file_name) {
    HafIFStream haf_ifstream(file_name);
    return haf_ifstream.GetWordLengthHaf();
}
