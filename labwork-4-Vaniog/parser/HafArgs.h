#pragma once
#include "Parser.h"

struct HafArgs : public Parser {
    std::string archive_name;
    std::vector<std::string> file_names;
    uint32_t files_amount = 0;

    uint32_t word_length_haf;
    bool create_needed{};

    bool delete_needed{};
    bool concatenate_needed{};
    bool append_needed{};
    bool extract_needed{};
    bool list_needed{};

private:
    bool write_data_needed{};
private:
    void InitSettings() override;
    bool InitValues() override;
    [[nodiscard]] bool IsValid() const; // return validness of arguments
    void WriteData() const;

    void AddToFileNames(const std::string& name, uint32_t recursive_depth = 0);
};