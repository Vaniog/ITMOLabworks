#include "HafArchive.h"
#include "HafMath.h"
#include "HafFileHeader.h"
#include "HafStream.h"

HafArchive::HafArchive() = default;

void HafArchive::AddFiles(std::vector<std::string>& file_names) {
    for (std::string& file_name : file_names) {
        HafFileHeader haf_file_header(file_name);
        file_headers.push_back(haf_file_header);
    }
}

void HafArchive::WriteToFile(HafOFStream& haf_ofstream) {
    for (const HafFileHeader& file_data : file_headers) {
        std::cout << " " << file_data.name << " - start writing\n";
        file_data.WriteToHaf(haf_ofstream);
        std::cout << " " << file_data.name << " - wrote\n";
    }
}

void HafArchive::ReadFromFile(HafIFStream& haf_ifstream) {
    uint32_t cur_pos = 0; // in bytes
    while (cur_pos < haf_ifstream.Size()) {
        HafFileHeader file_header(haf_ifstream, cur_pos);
        file_headers.push_back(file_header);
        cur_pos += HafMath::FromBasicToHafSize(file_header.size_of_header, haf_ifstream.GetWordLengthHaf())
                + HafMath::FromBasicToHafSize(file_header.size_of_data, haf_ifstream.GetWordLengthHaf());
        haf_ifstream.SeekG(cur_pos, std::ios::beg);
    }
}

void HafArchive::Extract(HafIFStream& haf_ifstream, const std::string& arch_dir_name, const std::string& file_name) {
    for (const HafFileHeader& file_data : file_headers) {
        if (file_data.name == file_name || file_name.empty()) {
            std::cout << " " << file_data.name << " - start extracting\n";
            file_data.ExtractToBasic(haf_ifstream, arch_dir_name);
            std::cout << " " << file_data.name << " - extracted\n";
        }
    }
}

void HafArchive::Append(HafOFStream& haf_of_stream, const std::string& file_name) {
    std::cout << " " << file_name << " - start to append\n";
    HafFileHeader file_header(file_name);
    file_header.WriteToHaf(haf_of_stream);
    std::cout << " " << file_name << " - appended\n";
}

void HafArchive::CopyFileToArchive(HafFileHeader& file_header,
                                   HafIFStream& haf_ifstream,
                                   HafOFStream& haf_ofstream) {

    uint32_t word_length_haf = haf_ifstream.GetWordLengthHaf();
    haf_ifstream.SeekG(file_header.start_pos, std::ios::beg);
    uint32_t file_size = HafMath::FromBasicToHafSize(file_header.size_of_data, word_length_haf)
            + HafMath::FromBasicToHafSize(file_header.size_of_header, word_length_haf);

    char* byte = new char;
    for (uint32_t i = 0; i < file_size; i++) {
        haf_ifstream.ReadUnconverted(byte, 1);
        haf_ofstream.WriteUnconverted(byte, 1);
    }
}

void HafArchive::Delete(const std::string& file_name) {
    for (uint32_t i = 0; i < file_headers.size(); i++) {
        if (file_headers[i].name == file_name) {
            file_headers.erase(file_headers.begin() + i);
        }
    }
}

void HafArchive::ConcatenateArchives(HafIFStream& haf_ifstream, HafOFStream& haf_ofstream) {
    char* byte = new char;
    uint32_t pos = 0;
    while (pos != haf_ifstream.Size()) {
        haf_ifstream.ReadUnconverted(byte, 1);
        haf_ofstream.WriteUnconverted(byte, 1);
        pos++;
    }
}

void HafArchive::RewriteArchive(HafIFStream& haf_ifstream, HafOFStream& haf_ofstream) {
    HafArchive haf_archive_in;
    haf_archive_in.ReadFromFile(haf_ifstream);
    haf_archive_in.Extract(haf_ifstream, "temp~");
    HafArchive haf_archive_out;
    for (HafFileHeader &file_header : haf_archive_in.file_headers) {
        haf_archive_out.Append(haf_ofstream, "temp~/" + file_header.name);
    }
}
