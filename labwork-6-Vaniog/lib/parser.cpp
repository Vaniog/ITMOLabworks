#include "parser.h"
#include "IstreamFuncs.h"

#include <sstream>
#include <fstream>

namespace omfl {

void OMFLData::Parse(std::istream& is) {
    std::string key;
    std::string value;
    std::string section_name;

    while (is) {
        SkipSpacesAndComments(is);
        if (!is) {
            return;
        }

        if (is.peek() == '[') {
            section_name = GetSection(is);
            if (section_name.empty()){
                is_valid_ = false;
                return;
            }
            root_section_->AddSection(section_name);
            cur_section_ = &root_section_->Get(section_name);
        } else {
            key = GetKey(is);
            if (key.empty()){
                is_valid_ = false;
                return;
            }
            auto new_param = std::make_shared<Param>(is);
            if (!new_param->IsValid()) {
                is_valid_ = false;
                return;
            }

            if (!cur_section_->AddParam(key, new_param)) {
                is_valid_ = false;
                return;
            }
        }
    }
}

OMFLData::OMFLData(const std::string& str) {
    cur_section_ = new Section;
    root_section_ = std::shared_ptr<Section>(cur_section_);
    std::stringstream iss(str);
    Parse(iss);
}

OMFLData::OMFLData(const std::filesystem::path& path) {
    cur_section_ = new Section;
    root_section_ = std::shared_ptr<Section>(cur_section_);
    std::ifstream file(path);
    if (!file.is_open())
        return;
    Parse(file);
}

bool OMFLData::valid() const {
    return is_valid_;
}

OMFLData parse(const std::filesystem::path& path){
    return OMFLData(path);
}
OMFLData parse(const std::string& str) {
    return OMFLData(str);
}

Section& OMFLData::Get(const std::string& str) const {
    return root_section_->Get(str);
}

void OMFLData::SkipSpacesAndComments(std::istream& is) {
    is >> std::ws;
    while (is.peek() == '#') {
        isf::IgnoreWhile(is, [](uint8_t c){return c != '\n';});
        is >> std::ws;
    }
}

std::string OMFLData::GetSection(std::istream& is) {
    is.ignore(); // to skip '['
    std::string section_name = isf::GetWhile(is, [&](uint8_t c){return IsCharValid(c) || c == '.';});

    if (section_name.empty() || section_name[0] == '.' || section_name.back() == '.') {
        return "";
    }
    if (is.peek() == ']') {
        is.ignore();
        return section_name;
    }
    return "";
}
std::string OMFLData::GetKey(std::istream& is) {
    std::string key = isf::GetWhile(is, IsCharValid);
    SkipSpacesAndComments(is);
    if (is.peek() == '=') {
        is.ignore();
        return key;
    }
    return "";
}
bool OMFLData::IsCharValid(uint8_t c) {
    return std::isalpha(c) ||
            std::isdigit(c) ||
            c == '-' ||
            c == '_';
}

}