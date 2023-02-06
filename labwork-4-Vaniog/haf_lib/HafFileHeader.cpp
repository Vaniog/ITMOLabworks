#include "HafFileHeader.h"
#include "HafMath.h"

#include <filesystem>

HafFileHeader::HafFileHeader(std::string file_name)
        : name(std::move(file_name)) {
    name_without_slashes = ConvertName(name);
    name_length = this->name_without_slashes.length();
    size_of_data = std::filesystem::file_size(this->name);
    size_of_header = sizeof(name_length) + sizeof(size_of_header) + name_length * sizeof(char);
}

HafFileHeader::HafFileHeader(HafIFStream& haf_ifstream, uint32_t start_pos)
        : start_pos(start_pos) {
    ReadFromHaf(haf_ifstream);
    size_of_header = sizeof(name_length) + sizeof(size_of_header) + name_length * sizeof(char);
}

void HafFileHeader::WriteToHaf(HafOFStream& haf_ofstream) const {
    haf_ofstream.Write(reinterpret_cast<const char*>(&name_length), 1);
    haf_ofstream.Write(&name_without_slashes[0], name_length);
    haf_ofstream.Write(reinterpret_cast<const char*>(&size_of_data), 4);
    haf_ofstream.FreeData();

    std::ifstream basic_file_in(name);
    uint8_t byte;
    for (uint32_t i = 0; i < size_of_data; i++) {
        basic_file_in.read(reinterpret_cast<char*>(&byte), 1);
        haf_ofstream.Write(reinterpret_cast<const char*>(&byte), 1);
    }
    haf_ofstream.FreeData();
}

void HafFileHeader::ReadFromHaf(HafIFStream& haf_ifstream) {
    haf_ifstream.Read(reinterpret_cast<char*>(&name_length), 1);
    name.resize(name_length);
    haf_ifstream.Read(&name[0], name_length);
    haf_ifstream.Read(reinterpret_cast<char*>(&size_of_data), 4);
    haf_ifstream.CleanTrash();
}

void HafFileHeader::ExtractToBasic(HafIFStream& haf_ifstream, const std::string& arch_dir_name) const {
    if (!std::filesystem::is_directory(arch_dir_name)) {
        std::filesystem::create_directory(arch_dir_name);
    }
    std::ofstream basic_file_out(arch_dir_name + "/" + name, std::ios::binary);

    haf_ifstream.SeekG(start_pos + HafMath::FromBasicToHafSize(size_of_header, haf_ifstream.GetWordLengthHaf()),
                       std::ios::beg);
    uint8_t byte;
    for (uint32_t i = 0; i < size_of_data; i++) {
        haf_ifstream.Read(reinterpret_cast<char*>(&byte), 1);
        basic_file_out.write(reinterpret_cast<const char*>(&byte), 1);
    }
}
std::string HafFileHeader::ConvertName(const std::string& file_name) {
    std::string new_name;
    for (const char c : file_name) {
        new_name += c;
        if (c == '/') {
            new_name = "";
        }
    }
    return new_name;
}
