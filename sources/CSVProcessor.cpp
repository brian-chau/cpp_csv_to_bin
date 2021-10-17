#include <vector>

#include "CSVProcessor.h"


CSVProcessor::CSVProcessor(std::string input_csv)
    : m_input_csv(input_csv)
{

}

CSVProcessor::~CSVProcessor() {
}

std::time_t CSVProcessor::time_since_epoch(const std::string& str, bool is_dst) {
    const std::string& format = "%Y-%m-%d %H:%M:%S";
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
                    //case PU_TIME:            data.record.tpep_pickup_datetime   = time_since_epoch(token);      break; // TODO: Find out why this takes so long?
                    //case DO_TIME:            data.record.tpep_dropoff_datetime  = time_since_epoch(token);      break;
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
