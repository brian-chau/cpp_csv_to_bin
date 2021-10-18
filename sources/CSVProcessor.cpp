#include <vector>

#include "CSVProcessor.h"


CSVProcessor::CSVProcessor(std::string input_csv)
    : m_input_csv(input_csv)
{

}

CSVProcessor::~CSVProcessor() {
}

S64 CSVProcessor::time_since_epoch(const std::string& str) {
    std::string format("%d-%d-%d %d:%d:%d");
    std::tm     t = {0};
    sscanf(str.c_str(), format.c_str(), &t.tm_year, &t.tm_mon, &t.tm_mday, &t.tm_hour, &t.tm_min, &t.tm_sec);

    // Note: Could have used mktime here, but with such a large file, it would have been taken 2m30s or longer for 800MB
    //       As an alternative, I found this code snippet somewhere online that supposedly does the same thing,
    //         which reduces the runtime down to ~9s
    //return mktime(&t);
    S64         year;
    std::time_t result;
    #define MONTHSPERYEAR   12      /* months per calendar year */
    static const int cumdays[MONTHSPERYEAR] = { 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334 };

    year    = 1900 + t.tm_year + t.tm_mon / MONTHSPERYEAR;
    result  = (year - 1970) * 365 + cumdays[t.tm_mon % MONTHSPERYEAR];
    result += (year - 1968) / 4;
    result -= (year - 1900) / 100;
    result += (year - 1600) / 400;
    if ((year % 4) == 0 && ((year % 100) != 0 || (year % 400) == 0) && (t.tm_mon % MONTHSPERYEAR) < 2) {
        result--;
    }
    result += t.tm_mday - 1;
    result *= 24;
    result += t.tm_hour;
    result *= 60;
    result += t.tm_min;
    result *= 60;
    result += t.tm_sec;
    if (t.tm_isdst == 1) {
        result -= 3600;
    }
    return (result);
}

void CSVProcessor::generate_bin_file() {
    std::ifstream iss(m_input_csv.c_str());
    std::ofstream oss("out.bin", std::ios::binary | std::ios::out);
    std::string   line("");

    if (iss.is_open()) {
        // Skip the first two lines
        std::getline(iss, line);
        std::getline(iss, line);
        std::string       token;
        Data data;
        for (int i=0; std::getline(iss, line); i++) {
            memset(&data, 0, RECORD_SIZE);
            char * line_char = (char*) line.c_str();
            char * token     = std::strtok(line_char, ",");
            for(int j=0; token && j <= PAYMENT_TYPE; j++) {
                switch(j) {
                    case VENDOR_ID:          data.record.VendorID               = atoi(token);                  break;
                    case PU_TIME:            data.record.tpep_pickup_datetime   = time_since_epoch(token);      break;
                    case DO_TIME:            data.record.tpep_dropoff_datetime  = time_since_epoch(token);      break;
                    case PASSENGER_COUNT:    data.record.passenger_count        = atoi(token);                  break;
                    case TRIP_DISTANCE:      data.record.trip_distance          = (U16) std::stof(token) * 100; break;
                    case RATE_CODE:          data.record.RatecodeID             = atoi(token);                  break;
                    case STORE_AND_FWD_FLAG: data.record.store_and_forward_flag = atoi(token);                  break;
                    case PU_LOCATION:        data.record.PULocationID           = atoi(token);                  break;
                    case DO_LOCATION:        data.record.DOLocationID           = atoi(token);                  break;
                    case PAYMENT_TYPE:       data.record.payment_type           = atoi(token);                  break;
                }
                token = std::strtok(NULL, ",");
            }
            oss.write((const char*)&data.buffer, RECORD_SIZE);
        }
        iss.close();
    } else {
        std::cout << "Cannot open file" << std::endl;
    }
    oss.close();
}
