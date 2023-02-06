#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <iomanip>


struct WordCountFlags {
    bool lines = false;
    bool words = false;
    bool bytes = false;
    bool letters = false;

    void Refactor() { // if all false make all true
        if (!(lines | words | bytes | letters))
            lines = words = bytes = letters = true;
    }
};

struct FileStats {
    std::string file_name;
    u_int64_t lines = 0, words = 0, bytes = 0, letters = 0;
    WordCountFlags flags;
    bool is_open = true;

    explicit FileStats(const std::string &file_name, const WordCountFlags &flags) : flags(flags) {
        this->file_name = file_name;

        CountValues();
    }

    void CountValues(){
        std::ifstream file(file_name);
        is_open = file.is_open();
        if (!is_open) {
            return;
        }
        char c, prev_c;

        file.get(c);
        while (!file.eof()) {
            if (c == '\n')
                lines++;
            if (std::isalpha(c))
                letters++;
            if (isspace(c) && !isspace(prev_c))
                words++;
            prev_c = c;
            file.get(c);
        }
        file.clear();
        bytes = file.tellg();

        max_lines_length = std::max(std::to_string(lines).size(), max_lines_length);
        max_words_length = std::max(std::to_string(words).size(), max_words_length);
        max_bytes_length = std::max(std::to_string(bytes).size(), max_bytes_length);
        max_letters_length = std::max(std::to_string(letters).size(), max_letters_length);
    }

    void write() const {
        if (!is_open) {
            std::cout << "No such file or directory: ";
            std::cout << file_name.c_str() << "\n";
        } else {
            if (flags.lines)
                std::cout << std::setw((int) max_lines_length) << lines << " ";
            if (flags.words)
                std::cout << std::setw((int) max_words_length) << words << " ";
            if (flags.bytes)
                std::cout << std::setw((int) max_bytes_length) << bytes << " ";
            if (flags.letters)
                std::cout << std::setw((int) max_letters_length) << letters << " ";

            std::cout << file_name.c_str();
            std::cout << "\n";
        }
    }

    void add(FileStats &fileStats) {
        lines += fileStats.lines;
        words += fileStats.words;
        bytes += fileStats.bytes;
        letters += fileStats.letters;
    }

    static size_t max_lines_length, max_words_length, max_bytes_length, max_letters_length;
};
size_t FileStats::max_lines_length = 0;
size_t FileStats::max_words_length = 0;
size_t FileStats::max_bytes_length = 0;
size_t FileStats::max_letters_length = 0;

void Run(std::vector<std::string> &file_names, WordCountFlags flags) {
    FileStats total("", flags);

    std::vector<FileStats> files_stats;
    for (std::string &file_name : file_names) {
        files_stats.emplace_back(file_name, flags);
        total.add(files_stats.back());
    }

    for (FileStats &file_stats : files_stats) {
        file_stats.write();
    }
    total.file_name = "total";
    total.is_open = true;
    if (file_names.size() > 1)
        total.write();
}

bool ReadModeInit(const std::vector<std::string> &commands,
                  WordCountFlags &flags) { // return true if commands have right syntax
    for (const std::string &command : commands) {
        if (command == "--lines") {
            flags.lines = true;
            continue;
        }
        if (command == "--words") {
            flags.words = true;
            continue;
        }
        if (command == "--bytes") {
            flags.bytes = true;
            continue;
        }
        if (command == "--letters") {
            flags.letters = true;
            continue;
        }

        if (command.size() == 1) {
            std::cout << "- is not a command\n";
            return false;
        }
        for (int i = 1; i < command.size(); i++) {
            switch (command[i]) {
                case 'l':
                    flags.lines = true;
                    break;
                case 'w':
                    flags.words = true;
                    break;
                case 'c':
                    flags.bytes = true;
                    break;
                case 'm':
                    flags.letters = true;
                    break;
                default:
                    std::cout << "\"" << command[i] << "\" is not a command\n";
                    return false;
            }
        }
    }
    flags.Refactor();
    return true;
}

int main(int argc, char **argv) {
    WordCountFlags flags;
    std::vector<std::string> argv_strings;
    for (int i = 0; i < argc; i++)
        argv_strings.emplace_back(argv[i]);

    if (argc == 1) {
        return -1;
    }

    std::vector<std::string> commands;
    std::vector<std::string> file_names;

    for (int i = 1; i < argc; i++) {
        if (argv_strings[i][0] == '-')
            commands.emplace_back(argv_strings[i]);
        else
            file_names.emplace_back(argv_strings[i]);
    }

    if (!ReadModeInit(commands, flags))
        return 0;

    if (!file_names.empty())
        Run(file_names, flags);
    else
        std::cout << "No files to check\n";

    return 0;
}