#include <iostream>

#include "CSVProcessor.h"
#include "BinProcessor.h"

int main(int argc, char** argv) {
    if (argc != 3) {
        std::cout << "Usage: " << std::endl;
        std::cout << "    ./" << argv[0] << " file.csv convert" << std::endl;
        std::cout << "    ./" << argv[0] << " file.bin count" << std::endl;
        return -1;
    }

    std::string option_arg(argv[2]);
    if (option_arg == "convert") {
        CSVProcessor* csvProcessor = new CSVProcessor(std::string(argv[1]));
        csvProcessor->generate_bin_file();
        delete csvProcessor;
    } else if (option_arg == "count") {
        itsBinProc = new BinProcessor(std::string(argv[1]));
        itsBinProc->count_conditions();
        delete itsBinProc;
    }
    return 0;
}
