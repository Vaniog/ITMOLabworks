#pragma once
#include <iostream>
#include <string>

struct SandData {
    uint32_t width;
    uint32_t height;
    std::string input;
    std::string output;
    uint32_t max_iter = 0;
    uint32_t freq;
    bool write_data = false;
    bool smart_input = false;
    bool timer_on = false;

    void Parse(int argc, char** argv) {
        SetDefault();
        for (int i = 1; i < argc; i++) {
            std::string command = argv[i];
            if (command == "--width" || command == "-w") {
                height = std::stoi(argv[i + 1]);
                i++;
            } else if (command == "--length" || command == "-l") {
                width = std::stoi(argv[i + 1]);
                i++;
            } else if (command == "--input" || command == "-i") {
                input = argv[i + 1];
                i++;
            } else if (command == "--output" || command == "-o") {
                output = argv[i + 1];
                i++;
            } else if (command == "--max-iter" || command == "-m") {
                max_iter = std::stoi(argv[i + 1]);
                i++;
            } else if (command == "--freq" || command == "-f") {
                freq = std::stoi(argv[i + 1]);
                i++;
            } else if (command == "--write-data") {
                write_data = true;
            } else if (command == "--smart"){
                smart_input = true;
            } else if (command == "--timer"){
                timer_on = true;
            }
        }
        if (smart_input){
            SmartInput();
        }
        if (write_data) {
            WriteData();
        }
        UpdateDefault();
    }

    void SetDefault() {
        std::ifstream default_file("default_sand_data.txt");
        if (!default_file.is_open()) {
            return;
        }
        default_file >> width;
        default_file >> height;
        default_file >> input;
        default_file >> output;
        default_file >> max_iter;
        default_file >> freq;
    }

    void UpdateDefault() const {
        std::ofstream default_file("default_sand_data.txt");
        default_file << width << "\n";
        default_file << height << "\n";
        default_file << input << "\n";
        default_file << output << "\n";
        default_file << max_iter << "\n";
        default_file << freq << "\n";
    }

    void WriteData() const {
        std::cout << "width = " << width << "\n";
        std::cout << "height = " << height << "\n";
        std::cout << "input = " << input << "\n";
        std::cout << "output = " << output << "\n";
        std::cout << "max_iter = " << max_iter << "\n";
        std::cout << "freq = " << freq << "\n";
    }

    void SmartInput() {
        std::cout << "width [def=" << width << "]: ";
        GetNumber(width);
        std::cout << "height [def=" << height << "]: ";
        GetNumber(height);
        std::cout << "input [def=" << input << "]: ";
        GetString(input);
        std::cout << "output [def=" << output << "]: ";
        GetString(output);
        std::cout << "max_iter [def=" << max_iter << "]: ";
        GetNumber(max_iter);
        std::cout << "freq [def=" << freq << "]: ";
        GetNumber(freq);
    }

    static void GetString(std::string &str){
        std::string input_str;
        std::getline(std::cin, input_str);
        if (!input_str.empty()){
            str = input_str;
        }
    }
    static void GetNumber(uint32_t &number){
        std::string input_str;
        std::getline(std::cin, input_str);
        if (!input_str.empty()){
            number = std::stoi(input_str);
        }
    }
};
