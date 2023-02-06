#pragma once
#include <iostream>
#include <vector>
#include <deque>

class Byteset {
    const uint32_t kSegmentSize = 32;
    std::vector<uint32_t> segments;
    uint32_t start_index = 0;
    uint32_t latest_ptr = 0;
public:
    bool GetBit(const uint32_t& index);
    void SetBit(const uint32_t& index, bool bit);

    void PushBit(bool bit);
    void PopBit();

    void PushByte(uint8_t byte);
    char PopByte();

    void Clear();

    void Write(const char* bytes, uint8_t bytes_amount);
    void Read(char* bytes, uint8_t bytes_amount);

    [[maybe_unused]] void Show();

    [[nodiscard]] uint32_t GetSizeInBits() const;
private:

    [[nodiscard]] inline uint32_t IndexSegment(const uint32_t& index) const;
    [[nodiscard]] inline uint32_t IndexIndex(const uint32_t& index) const;
};