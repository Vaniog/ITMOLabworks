#pragma once
#include "Param.h"
#include "Section.h"

#include <filesystem>
#include <istream>
#include <sstream>
#include <map>
#include <memory>
#include <variant>
#include <vector>
#include <functional>

namespace omfl {

class OMFLData {
public:
    explicit OMFLData(const std::string& str);
    explicit OMFLData(const std::filesystem::path& path);
    void Parse(std::istream& is);
    [[nodiscard]] bool valid() const;
    [[nodiscard]] Section& Get(const std::string& str) const;
private:
    bool is_valid_ = true;
    std::shared_ptr<Section> root_section_;
    Section* cur_section_;
    std::map<std::string, std::shared_ptr<Param>> data_;
    static void SkipSpacesAndComments(std::istream& is);

    static std::string GetSection(std::istream& is);
    static std::string GetKey(std::istream& is);
    static bool IsCharValid(uint8_t c);
};
OMFLData parse(const std::filesystem::path& path);
OMFLData parse(const std::string& str);

}// namespace