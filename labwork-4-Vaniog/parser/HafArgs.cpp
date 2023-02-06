#include "HafArgs.h"

#include <iostream>
#include <filesystem>

void HafArgs::InitSettings() {
    option_convertor = {
            {"-c", "--create"},
            {"-l", "--list"},
            {"-x", "--extract"},
            {"-a", "--append"},
            {"-d", "--delete"},
            {"-A", "--concatenate"},
            {"-f", "--file"},
    };
    are_options = {
            "--create",
            "--list",
            "--extract",
            "--append",
            "--concatenate",
            "--delete",
            "--write-data"
    };
    are_parameters = {
            "--file",
            "--word-length"
    };
}

bool HafArgs::InitValues() {
    archive_name = GetParameter("--file");
    word_length_haf = std::stoi(GetParameter("--word-length"));

    create_needed = std::stoi(GetParameter("--create"));
    delete_needed = std::stoi(GetParameter("--delete"));
    concatenate_needed = std::stoi(GetParameter("--concatenate"));
    append_needed = std::stoi(GetParameter("--append"));
    extract_needed = std::stoi(GetParameter("--extract"));
    list_needed = std::stoi(GetParameter("--list"));
    write_data_needed = std::stoi(GetParameter("--write-data"));

    for (std::string& str : free_args) {
        AddToFileNames(str);
    }

    files_amount = file_names.size();

    if (write_data_needed) {
        WriteData();
    }

    return IsValid();
}

void HafArgs::AddToFileNames(const std::string& name, // NOLINT (it just doesn't understand my recurse)
                             uint32_t recursive_depth) { // here is recurse for parse a directories in directory
    if (std::filesystem::is_regular_file(name)) {
        file_names.push_back(name);
    } else if (std::filesystem::is_directory(name)) {
        for (const auto& iter : std::filesystem::directory_iterator(name)) {
            AddToFileNames(iter.path(), recursive_depth + 1);
        }
    } else {
        std::cout << "file doesn't exists: " << name << "\n";
    }
}

bool HafArgs::IsValid() const {
    if (word_length_haf == 0 && create_needed) {
        std::cout << "set word-length for creating\n";
        return false;
    }
    if (!std::filesystem::exists(archive_name) && !create_needed && !concatenate_needed){
        std::cout << "archive doesn't exists\n";
        return false;
    }
    uint32_t check = create_needed
            + delete_needed
            + concatenate_needed
            + append_needed
            + extract_needed;
    if (check > 1) { // only one flag needs to be set
        std::cout << "too many options\n";
        return false;
    }
    return true;
}

void HafArgs::WriteData() const {
    std::cout << "[Archive name]: " << archive_name << "\n";
    std::cout << "[Files amount]: " << files_amount << "\n";
    for (uint32_t i = 0; i < files_amount; i++) {
        std::cout << i + 1 << " - " << file_names[i] << "\n";
    }
    std::cout << "\n";
    if (create_needed) {
        std::cout << "creating\n";
    } else if (delete_needed) {
        std::cout << "deleting\n";
    } else if (concatenate_needed) {
        std::cout << "concatenating\n";
    } else if (append_needed) {
        std::cout << "appending\n";
    } else if (extract_needed) {
        std::cout << "extracting\n";
    }
    if (list_needed) {
        std::cout << "listing\n";
    }
    std::cout << "\n";
}
