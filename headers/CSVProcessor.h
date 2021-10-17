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

enum { VENDOR_ID = 0, PU_TIME, DO_TIME, PASSENGER_COUNT, TRIP_DISTANCE, RATE_CODE, STORE_AND_FWD_FLAG, PU_LOCATION, DO_LOCATION, PAYMENT_TYPE };

class CSVProcessor {
public:
     CSVProcessor(std::string input_filename);
    ~CSVProcessor();
    void generate_bin_file();

private:
    std::time_t time_since_epoch(const std::string& str, bool is_dst = false);
    std::string m_input_csv;
};
