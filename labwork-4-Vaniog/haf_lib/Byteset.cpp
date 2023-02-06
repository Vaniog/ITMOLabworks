#include "Byteset.h"

bool Byteset::GetBit(const uint32_t& index) {
    uint32_t segment = segments[IndexSegment(index)];
    return (segment >> (kSegmentSize - 1 - IndexIndex(index)) & 1);
}

void Byteset::SetBit(const uint32_t& index, bool bit) {
    uint32_t seg_index = IndexSegment(index);
    while (segments.size() <= seg_index) {
        segments.push_back(0);
    }

    latest_ptr = std::max(latest_ptr, index);

    if (GetBit(index) == bit) {
        return;
    }
    segments[seg_index] ^= (1 << (kSegmentSize - 1 - IndexIndex(index)));
}

void Byteset::PushBit(const bool bit) {
    SetBit(latest_ptr, bit);
    latest_ptr += 1;
}

void Byteset::PopBit() {
    SetBit(0, false);
    start_index = (start_index + 1) % kSegmentSize;
    latest_ptr -= 1;
    if (start_index == 0)
        segments.erase(segments.begin());
}

void Byteset::PushByte(const uint8_t byte) {
    uint32_t seg_index = IndexSegment(latest_ptr);
    uint32_t ind_index = IndexIndex(latest_ptr);
    if (seg_index >= segments.size())
        segments.push_back(0);
    latest_ptr += 8;

    if (ind_index + 8 <= kSegmentSize) {
        segments[seg_index] |= static_cast<uint32_t>(byte) << (kSegmentSize - 8 - ind_index);
        return;
    }

    segments[seg_index] |= static_cast<uint32_t>(byte) >> (8 + ind_index - kSegmentSize);

    if (seg_index + 1 >= segments.size()) {
        segments.push_back(static_cast<uint32_t>(byte) << (2 * kSegmentSize - 8 - ind_index));
        return;
    }
    segments[seg_index + 1] |= static_cast<uint32_t>(byte) << (2 * kSegmentSize - 8 - ind_index);
}

char Byteset::PopByte() {
    uint8_t byte;
    uint32_t ind_index = IndexIndex(0);
    start_index = (start_index + 8) % kSegmentSize;
    latest_ptr -= 8;

    if (ind_index + 8 <= kSegmentSize) {
        byte = static_cast<uint8_t>(segments[0] >> (kSegmentSize - 8 - ind_index));
    } else {
        byte = static_cast<uint8_t>(
                (segments[0] << (8 + ind_index - kSegmentSize)) + (segments[1] >> (2 * kSegmentSize - 8 - ind_index)));
    }

    if (ind_index + 8 <= kSegmentSize) {
        segments[0] &=
                (((1 << ind_index) - 1) << (kSegmentSize - ind_index))
                        + (1 << (kSegmentSize - ind_index - 8)) - 1;
    } else {
        segments[1] &= (uint32_t(0) - 1) >> (8 + ind_index - kSegmentSize);
    }

    if (ind_index + 8 >= kSegmentSize) {
        segments.erase(segments.begin());
    }

    return static_cast<char>(byte);
}

void Byteset::Write(const char* bytes, uint8_t bytes_amount) {
    for (uint32_t iter = 0; iter < bytes_amount; iter++)
        PushByte(*(bytes + iter));
}

void Byteset::Read(char* bytes, uint8_t bytes_amount) {
    for (uint32_t iter = 0; iter < bytes_amount; iter++)
        *(bytes + iter) = static_cast<char>(PopByte());
}

uint32_t Byteset::GetSizeInBits() const {
    return latest_ptr;
}
uint32_t Byteset::IndexSegment(const uint32_t& index) const { // get index segment
    return (start_index + index) / kSegmentSize;
}
uint32_t Byteset::IndexIndex(const uint32_t& index) const { // get index index
    return (start_index + index) % kSegmentSize;
}

[[maybe_unused]] void Byteset::Show() {
    for (uint32_t seg : segments) {
        for (int32_t i = 31; i >= 0; i--) {
            std::cout << ((seg & (1 << i)) >> i);
        }
        std::cout << " ";
    }
    std::cout << "\n";
}

void Byteset::Clear() {
    latest_ptr = 0;
    start_index = 0;
    while (GetSizeInBits() / 8 > 0)
        PopByte();
    while (GetSizeInBits() > 0)
        PopBit();
}