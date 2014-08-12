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

void printRate(const RateInfo& rate)
{
    time_t tmp = rate.ctm;
    cout << put_time(std::localtime(&tmp), "%F %T");
    cout << "," << rate.open;
    cout << "," << rate.low;
    cout << "," << rate.high;
    cout << "," << rate.close;
    cout << "," << rate.vol << endl;
}

int main(int argc, const char * argv[])
{
    ifstream ifs;
    ifs.open("USDJPY.hst", ios::in | ios::binary);
    if(!ifs) {
        cout << "file open error" << endl;
        return 1;
    }
    
    HistoryHeader header;
    ifs.read((char*)&header, sizeof(header));
    if(ifs.bad()) {
        cout << "file read error" << endl;
        ifs.close();
        return 1;
    }
    
    printHistoryHeader(header);
    
    RateInfo rate;
    while(!ifs.eof()) {
        ifs.read((char*)&rate, 44);
        if(ifs.bad()) {
            cout << "file read error" << endl;
            ifs.close();
            return 1;
        }
        printRate(rate);
    }
    
    ifs.close();
    
    return 0;
}

