#include <iostream>

#include "CSVProcessor.h"
#include "BinProcessor.h"

bool ends_with(const char * str, const char * suffix) {
    if (NULL == str || NULL == suffix) {
        return false;
    }

    size_t str_len    = strlen(str);
    size_t suffix_len = strlen(suffix);
    if (suffix_len > str_len) {
        return false;
    } else {
        return 0 == strncmp( str + str_len - suffix_len, suffix, suffix_len );
    }
}

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cout << "Usage: " << std::endl;
        std::cout << "    ./" << argv[0] << " file.csv" << std::endl;
        std::cout << "    ./" << argv[0] << " file.bin" << std::endl;
        return -1;
    }

    if (ends_with(argv[1], ".csv")) {
        CSVProcessor* csvProcessor = new CSVProcessor(std::string(argv[1]));
        csvProcessor->generate_bin_file();
        delete csvProcessor;
    } else if (ends_with(argv[1], ".bin")) {
        itsBinProc = new BinProcessor(std::string(argv[1]));
        itsBinProc->count_conditions();
        delete itsBinProc;
    }
    return 0;
}
