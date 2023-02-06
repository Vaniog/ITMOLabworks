#include "HafManager.h"
#include "HafMath.h"
#include <filesystem>

void HafManager::DoJob() {
    if (haf_args.create_needed) {
        Create();
    } else if (haf_args.extract_needed) {
        Extract();
    } else if (haf_args.append_needed) {
        Append();
    } else if (haf_args.delete_needed) {
        Delete();
    } else if (haf_args.concatenate_needed) {
        Concatenate();
    }

    if (haf_args.list_needed) {
        List();
    }
}

void HafManager::Create() {
    HafOFStream haf_ofstream(haf_args.archive_name, haf_args.word_length_haf);
    HafArchive haf_archive;
    haf_archive.AddFiles(haf_args.file_names);
    haf_archive.WriteToFile(haf_ofstream);
}

void HafManager::Extract() {
    HafIFStream haf_ifstream(haf_args.archive_name);
    HafArchive haf_archive;
    haf_archive.ReadFromFile(haf_ifstream);

    std::string arch_dir_name;
    for (uint32_t i = 0; haf_args.archive_name[i] != '.'; i++) {
        arch_dir_name.push_back(haf_args.archive_name[i]);
    }

    if (haf_args.file_names.empty()) { // Extracting all
        haf_archive.Extract(haf_ifstream, arch_dir_name);
    } else {
        for (std::string& file_name : haf_args.file_names) {
            haf_archive.Extract(haf_ifstream, arch_dir_name, file_name);
        }
    }
}

void HafManager::Append() {
    uint32_t word_length_haf = HafMath::GetWordLengthFromFile(haf_args.archive_name);
    HafOFStream haf_ofstream(haf_args.archive_name, word_length_haf, std::ios::app);

    for (std::string& file_name : haf_args.file_names) {
        HafArchive::Append(haf_ofstream, file_name);
    }
}

void HafManager::List() const {
    HafIFStream haf_ifstream(haf_args.archive_name);
    HafArchive haf_archive;
    haf_archive.ReadFromFile(haf_ifstream);

    std::cout << "[List]:\n";
    for (auto& file_header : haf_archive.file_headers)
        std::cout << "* " << file_header.name << "\n";
    std::cout << "\n";
}

void HafManager::Delete() {
    HafIFStream haf_ifstream(haf_args.archive_name);
    HafArchive haf_archive_in;
    haf_archive_in.ReadFromFile(haf_ifstream);

    uint32_t word_length_haf = haf_ifstream.GetWordLengthHaf();
    HafOFStream haf_ofstream(haf_args.archive_name + "~", word_length_haf);

    for (uint32_t i = 0; i < haf_args.files_amount; i++) {
        haf_archive_in.Delete(haf_args.file_names[i]);
    }

    for (HafFileHeader& file_header : haf_archive_in.file_headers) {
        HafArchive::CopyFileToArchive(file_header, haf_ifstream, haf_ofstream);
    }

    std::filesystem::remove(haf_args.archive_name);
    std::filesystem::rename(haf_args.archive_name + "~", haf_args.archive_name);
}

void HafManager::Concatenate() {
    if (haf_args.file_names.empty()) {
        std::cout << "nothing to concatenate";
    }

    uint32_t word_length_haf = HafMath::GetWordLengthFromFile(haf_args.file_names[0]);
    HafOFStream haf_ofstream(haf_args.archive_name, word_length_haf);

    for (std::string& file_name : haf_args.file_names) {
        if (HafMath::GetWordLengthFromFile(file_name) != word_length_haf) {
            HafIFStream haf_ifstream(file_name);
            HafOFStream haf_ofstream_temp(file_name + '~', word_length_haf);

            std::cout << "need to rewrite " << file_name << " because it has different word_length\n";
            HafArchive::RewriteArchive(haf_ifstream, haf_ofstream_temp);
            haf_ofstream_temp.Close();
            std::cout << "rewrite finished\n";

            HafIFStream haf_ifstream_temp(file_name + "~");
            HafArchive::ConcatenateArchives(haf_ifstream_temp, haf_ofstream);
            std::filesystem::remove(file_name + "~");
        } else {
            HafIFStream haf_ifstream(file_name);
            HafArchive::ConcatenateArchives(haf_ifstream, haf_ofstream);
        }
    }
}
