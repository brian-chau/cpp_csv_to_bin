#pragma once
#include <chrono>
#include <cstring>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <string>
#include <sstream>
#include "types.h"

class CSVProcessor {
public:
     CSVProcessor(std::string input_filename);
    ~CSVProcessor();
    void generate_bin_file();

private:
    std::time_t time_since_epoch(const std::string& str, bool is_dst = false, const std::string& format = "%Y-%m-%d %H:%M:%S");
    std::string m_input_csv;
};
