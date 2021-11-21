#pragma once

#define U8  unsigned char
#define U16 unsigned short
#define U32 unsigned long int
#define U64 unsigned long long int

#define S8  signed char
#define S16 signed short
#define S32 signed long int
#define S64 signed long long int

typedef struct Record {
    S64 tpep_pickup_datetime;  // Words 1-4
    S64 tpep_dropoff_datetime; // Words 5-8
    #pragma pack(1)
    struct {                    // Word 9
        U16 passenger_count        : 4;
        U16 VendorID               : 1;
        U16 store_and_forward_flag : 1;
        U16 payment_type           : 2;
        U16 spare1                 : 8;
    };
    #pragma pack()
    #pragma pack(1)
    struct {                    // Word 10
        U16 PULocationID : 9;
        U16 RatecodeID   : 7;
    };
    #pragma pack()
    #pragma pack(1)
    struct {                    // Word 11
        U16 DOLocationID : 9;
        U16 spare2       : 7;
    };
    #pragma pack()
    U16 trip_distance;          // Word 12 Resolution: 0.01
} Record;

#define RECORD_SIZE sizeof(Record)

typedef union Data {
    Record record;
    U8     buffer[RECORD_SIZE];
} Data;



