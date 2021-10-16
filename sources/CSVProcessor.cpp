#include <regex>

#include "CSVProcessor.h"


CSVProcessor::CSVProcessor(std::string input_csv)
    : m_input_csv(input_csv)
{

}

CSVProcessor::~CSVProcessor() {
    
}

std::time_t CSVProcessor::time_since_epoch(const std::string& str, bool is_dst, const std::string& format) {
    std::tm t = {0};
    t.tm_isdst = is_dst ? 1 : 0;
    std::istringstream ss(str);
    ss >> std::get_time(&t, format.c_str());

    return mktime(&t);
}

void CSVProcessor::generate_bin_file() {
    std::ifstream iss(m_input_csv.c_str());
    std::ofstream oss("out.bin", std::ios::binary | std::ios::out);
    std::string   line("");

    Data data;
    if (iss.is_open()) {
        std::regex pattern("([^,]+),([^,]+),([^,]+),([^,]+),([^,]+),([^,]+),([^,]+),([^,]+),([^,]+),([^,]+)");
        std::smatch match;
        for(int i=0; std::getline(iss, line); i++) {
            if (i < 2)
                continue;

            memset(&data, 0, sizeof(Data));
            if (std::regex_search(line, match, pattern)) {
                data.record.VendorID               = atoi(match.str(1).c_str());
                data.record.tpep_pickup_datetime   = time_since_epoch(match.str(2).c_str());
                data.record.tpep_dropoff_datetime  = time_since_epoch(match.str(3).c_str());
                data.record.passenger_count        = atoi(match.str(4).c_str());
                data.record.trip_distance          = (U16) std::stof(match.str(5).c_str()) * 100;
                data.record.RatecodeID             = atoi(match.str(6).c_str());
                data.record.store_and_forward_flag = atoi(match.str(7).c_str());
                data.record.PULocationID           = atoi(match.str(8).c_str());
                data.record.DOLocationID           = atoi(match.str(9).c_str());
                data.record.payment_type           = atoi(match.str(10).c_str());
            }
            oss.write((const char *)&data.buffer, RECORD_SIZE);
        }
    }
    iss.close();
    oss.close();
}
