#include "SandHeap.h"

SandHeap::SandHeap(const SandData& sand_data) :
        width(sand_data.width), height(sand_data.height), sand_data(sand_data) {
    field = new uint64_t[width * height]{};
}

SandHeap::~SandHeap() {
    delete[] field;
}

uint64_t* SandHeap::GetPixelOld(const uint16_t& x, const uint16_t& y) const {
    return field + width * y + x;
}
uint64_t* SandHeap::GetPixelNew(const uint16_t& x, const uint16_t& y) const {
    return new_field + new_width * static_cast<uint16_t>(y + added.height_minus)
            + static_cast<uint16_t>(x + added.width_minus);
}

void SandHeap::SetPixel(const uint16_t& x, const uint16_t& y, const uint64_t& pixel) {
    field[width * y + x] = pixel;
    if (field[width * y + x] < 4)
        return;
    if (x == 0)
        new_added.width_minus = 1;
    if (x == width - 1)
        new_added.width_plus = 1;
    if (y == 0)
        new_added.height_minus = 1;
    if (y == height - 1)
        new_added.height_plus = 1;
}

bool SandHeap::Iteration() {
    added = new_added;
    new_added = Added();
    new_width = width + added.width_plus + added.width_minus;
    new_height = height + added.height_plus + added.height_minus;
    new_field = new uint64_t[new_width * new_height]{};
    bool was_update = false;

    for (uint32_t x = 0; x < width; x++) {
        for (uint32_t y = 0; y < height; y++) {
            UpdateNewPixel(x, y, static_cast<int64_t>(*GetPixelOld(x, y)));
            if (*GetPixelOld(x, y) < 4) {
                continue;
            }
            UpdateNewPixel(x, y, -4);
            UpdateNewPixel(x + 1, y, 1);
            UpdateNewPixel(x - 1, y, 1);
            UpdateNewPixel(x, y + 1, 1);
            UpdateNewPixel(x, y - 1, 1);
            was_update = true;
        }
    }
    delete[] field;
    field = new_field;
    width = new_width;
    height = new_height;
    return was_update;
}

void SandHeap::UpdateNewPixel(const uint16_t& x, const uint16_t& y, const int64_t& change) {
    *GetPixelNew(x, y) += change;
    if (*GetPixelNew(x, y) < 4)
        return;
    if (x == 0)
        new_added.width_minus = 1;
    if (x == width - 1)
        new_added.width_plus = 1;
    if (y == 0)
        new_added.height_minus = 1;
    if (y == height - 1)
        new_added.height_plus = 1;
}

struct Timer {
    std::chrono::steady_clock::time_point old = std::chrono::steady_clock::now();
    void WriteDeltaTime() const {
        auto cur = std::chrono::steady_clock::now();
        auto diff = cur - old;
        std::cout << std::chrono::duration_cast<std::chrono::hours>(diff).count() << "h ";
        diff -= std::chrono::duration_cast<std::chrono::hours>(diff);
        std::cout << std::chrono::duration_cast<std::chrono::minutes>(diff).count() << "min ";
        diff -= std::chrono::duration_cast<std::chrono::minutes>(diff);
        std::cout << std::chrono::duration_cast<std::chrono::seconds>(diff).count() << "s ";
        diff -= std::chrono::duration_cast<std::chrono::seconds>(diff);
        std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(diff).count() << "ms\n";
    }
};

void SandHeap::Run() {
    Timer timer;
    uint32_t cnt = 0;
    std::ifstream input(sand_data.input);

    if (input.is_open()) { // adding sand from file
        uint32_t x;
        uint32_t y;
        uint32_t amount;
        while (input >> x) {
            input >> y;
            input >> amount;
            SetPixel(x, y, amount);
        }
    } else {
        return;
    }

    std::filesystem::path directory_path = sand_data.output;
    if (!std::filesystem::exists(directory_path)) {
        std::filesystem::create_directory(sand_data.output);
    }
    while (Iteration() && (cnt < sand_data.max_iter || sand_data.max_iter == 0)) {
        cnt++;
        if (sand_data.freq != 0 && cnt % sand_data.freq == 0) {
            std::string file_name = sand_data.output + "/sand-step" + std::to_string(cnt / sand_data.freq) + ".bmp";
            WriteToFile(file_name);
        }
    }
    std::string file_name = sand_data.output + "/sand-final.bmp";
    WriteToFile(file_name);
    if (sand_data.timer_on)
        timer.WriteDeltaTime();
}


