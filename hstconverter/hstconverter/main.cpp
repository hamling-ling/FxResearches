//
//  main.cpp
//  hstconverter
//
//  Created by Nobuhiro Kuroiwa on 2014/08/13.
//  Copyright (c) 2014年 Nobuhiro Kuroiwa. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <ctime>
#include <iomanip>

using namespace std;

typedef struct _HistoryHeader
{
    uint32_t    version;
    char        copyright[64];
    char        symbol[12];
    int32_t     period;
    int32_t     digits;
    uint32_t    timesign;
    uint32_t    last_sync;
    uint32_t    reserved[13];
} HistoryHeader;

#pragma pack(push,1)
typedef struct _RateInfo
{
    uint32_t    ctm;
    double      open;
    double      low;
    double      high;
    double      close;
    double      vol;
} RateInfo;
#pragma pack(pop)


void printHistoryHeader(const HistoryHeader& header)
{
    cout << "version   : " << header.version << endl;
    cout << "copyright : " << header.copyright << endl;
    cout << "symbol    : " << header.symbol << endl;
    cout << "period    : " << header.period << endl;
    cout << "digits    : " << header.period << endl;
    time_t tmp = header.timesign;
    cout << "timesign  : " << put_time(localtime(&tmp), "%F %T") << endl;
    tmp = header.last_sync;
    cout << "last_sync : " << put_time(localtime(&tmp), "%F %T") << endl;
}

void printRate(const RateInfo& rate, ostream& ost)
{
    time_t tmp = rate.ctm;
    ost << put_time(std::localtime(&tmp), "%F %T");
    ost << "," << rate.open;
    ost << "," << rate.low;
    ost << "," << rate.high;
    ost << "," << rate.close;
    ost << "," << rate.vol << endl;
}

int main(int argc, const char * argv[])
{
    if(argc < 2) {
        cout << "need to give hst file and output file" << endl;
        cout << "ex) hstconverter [hst file] [output file]" << endl;
        return 1;
    }
    
    ifstream ifs;
    ifs.open(argv[1], ios::in | ios::binary);
    if(!ifs) {
        cout << "file open error" << endl;
        return 1;
    }
    
    HistoryHeader header;
    ifs.read((char*)&header, sizeof(header));
    if(ifs.bad()) {
        cout << "file read error" << endl;
        return 1;
    }
    
    printHistoryHeader(header);
    
    ofstream ofs;
    ofs.open(argv[2], ios::out | ios::trunc);
    if(!ofs.is_open()) {
        cout << "failed to create a file : " << argv[2] << endl;
        return 1;
    }
    
    RateInfo rate;
    while(!ifs.eof()) {
        ifs.read((char*)&rate, 44);
        if(ifs.bad()) {
            cout << "file read error" << endl;
            return 1;
        }
        printRate(rate, ofs);
    }
    
    cout << "done" << endl;
    
    return 0;
}

