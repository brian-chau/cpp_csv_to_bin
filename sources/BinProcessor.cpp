#include <sys/stat.h>

#include "BinProcessor.h"

BinProcessor *  itsBinProc;
BinProcessor::BinProcessor(std::string filename)
    : m_input_filename(filename)
    , m_pu_total(0)
    , m_do_total(0)
{
    struct stat st;
    if (stat(m_input_filename.c_str(), &st) != 0) {
        m_file_size = 0;
    }
    m_file_size              = st.st_size;
    m_num_records_to_process = m_file_size / RECORD_SIZE;
}

BinProcessor::~BinProcessor() {

}

typedef struct threadParam {
    int         t_id;
    std::string t_filename;
} threadParam;

void BinProcessor::StoreResults(int pu_count, int do_count) {
    const std::lock_guard<std::mutex> lock(mut);
    m_pu_total += pu_count;
    m_do_total += do_count;
}

static void * ReadFile(void * param) {
    threadParam  *params = (threadParam*) param;

    std::string   line;
    std::ifstream file(params->t_filename);

    if (file.is_open()) {
        // Seek to record
        int init_pos = params->t_id * itsBinProc->m_num_records_to_process * RECORD_SIZE;
        file.seekg(init_pos, std::ios::beg);

        // Start counting records
        int pu_count(0), do_count(0);
        Data data;
        for (int i=0; i < itsBinProc->m_num_records_to_process; i++) {
            memset(&data, 0, RECORD_SIZE);
            file.read((char*)data.buffer, RECORD_SIZE);
            if (data.record.payment_type == 3) {
                if (data.record.PULocationID == 170) {
                    pu_count++;
                } else if (data.record.DOLocationID == 170) {
                    do_count++;
                }
            }
        }
        itsBinProc->StoreResults(pu_count, do_count);

        file.close();
    } else {
        std::cout << "Cannot open file" << std::endl;
    }
    pthread_exit(NULL);
}

void BinProcessor::count_conditions(){
    pthread_t   threads[MAX_THREADS];
    threadParam mTD[MAX_THREADS];

    for( int j=0, rc(0); j<MAX_THREADS; j++ ) {
        mTD[j].t_id       = j;
        mTD[j].t_filename = m_input_filename;
        rc = pthread_create(&threads[j], NULL, ReadFile, (void*)&mTD[j]);
        if (rc) {
            std::cout << "Unable to create thread: " << rc << std::endl;
            return;
        }
        pthread_join(threads[j], NULL);
    }

    std::cout << "PU Count: " << m_pu_total << std::endl;
    std::cout << "DO Count: " << m_do_total << std::endl;

    pthread_exit(NULL);
}
