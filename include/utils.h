#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <cstdlib>
#include <sstream>
#include <fstream>
#include <map>
#include <string>
#include <vector>
#include <unistd.h>
#include <iomanip>

#include "IniFile.h"

#define NTROLLEYS      5
#define NSLOTS         4
#define NPARTITIONS    4

using namespace std;

const string __dimensions = "Dimensions/Dimensions_20160526-2016XXXX.ini";
//const string __dimensions = "Dimensions/Dimensions_20150928-20160512.ini";
const string __logpath = "/var/operation/RUN/log";
//const string __logpath = "log";

int     CharToInt(char& C);
string  CharToString(char& C);
string  intTostring(int value);
string  longTostring(long value);
string  floatTostring(float value);
string  GetLogTimeStamp();

//Infrastructure inside GIF++
struct RPC{
    string        name;
    unsigned int  nPartitions;
    unsigned int  strips;
    vector<float> stripGeo;
};

void SetRPC(RPC& rpc, string ID, IniFile* geofile);

struct GIFTrolley {
    unsigned int nSlots;
    string       SlotsID;
    vector<RPC>  RPCs;
};

void SetTrolley(GIFTrolley& trolley, string ID, IniFile* geofile);

struct Infrastructure {
    unsigned int       nTrolleys;
    string             TrolleysID;
    vector<GIFTrolley> Trolleys;
};

void SetInfrastructure(Infrastructure& infra, IniFile* geofile);

//Data in the root file
struct RAWData {
    int             iEvent;     //Event i
    int             TDCNHits;   //Number of hits in event i
    vector<int>    *TDCCh;      //List of channels giving hits per event
    vector<float>  *TDCTS;      //List of the corresponding time stamps
};

void SetIDName(string rpcID, unsigned int partition, char* ID, char* Name, string IDroot, string Nameroot);

#endif // UTILS_H
