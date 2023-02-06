#pragma once
#include "HafStream.h"
#include "../parser/HafArgs.h"
#include "HafFileHeader.h"

#include <vector>

class HafArchive {
public:
    std::vector<HafFileHeader> file_headers;
    explicit HafArchive();
    void AddFiles(std::vector<std::string>& file_names);

    void WriteToFile(HafOFStream& haf_ofstream);
    void ReadFromFile(HafIFStream& haf_ifstream);

    void Extract(HafIFStream& haf_ifstream, const std::string& arch_dir_name, const std::string& file_name = "");
    static void Append(HafOFStream& haf_of_stream, const std::string& file_name);

    static void CopyFileToArchive(HafFileHeader& file_header,
                                  HafIFStream& haf_ifstream, HafOFStream& haf_ofstream);
    static void ConcatenateArchives(HafIFStream& haf_ifstream, HafOFStream& haf_ofstream);
    static void RewriteArchive(HafIFStream& haf_ifstream, HafOFStream& haf_ofstream);
    void Delete(const std::string& file_name);
};