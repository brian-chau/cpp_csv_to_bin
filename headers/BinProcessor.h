#pragma once
#include <chrono>
#include <cstring>
#include <fstream>
#include <iostream>
#include <limits>
#include <mutex>
#include <numeric>
#include <pthread.h>
#include <sstream>
#include <string>

#include "types.h"

#define MAX_THREADS 2

class BinProcessor {
public:
     BinProcessor(std::string filename);
    ~BinProcessor();

    void        count_conditions();
    void        StoreResults(int pu_count, int do_count);

    int         m_num_records_to_process;

private:
    // Private methods


    // Private members
    std::string m_input_filename;
    std::mutex  mut;
    int         m_pu_total;
    int         m_do_total;
    int         m_file_size;

};

extern BinProcessor *  itsBinProc;
