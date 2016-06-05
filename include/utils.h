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

//***************************** ENV. VARIABLES

const string __dimensions = "Dimensions/Dimensions_20160526-2016XXXX.ini";
//const string __dimensions = "Dimensions/Dimensions_20150928-20160512.ini";
//const string __logpath = "/var/operation/RUN/log";
const string __logpath = "log";

//***************************** USEFUL FUNCTIONS

int     CharToInt(char& C);
string  CharToString(char& C);
string  intTostring(int value);
string  longTostring(long value);
string  floatTostring(float value);
string  GetLogTimeStamp();

//***************************** STRUCTURES FOR GIF INFRASTRUCTURE
//The RPC structure is more complexe than the one that can be found
//in the GIF_OfflineAnalysis code cause we need to register the rate,
//current and efficiency values for a complete scan. Thus it is needed
//to add some more vectors to get this data in a dynamical way

//Infrastructure inside GIF++
struct RPC{
    string          name;
    unsigned int    nPartitions;
    unsigned int    nGaps;
    vector<string>  gapNames;

    vector< vector<float> > HVeff;
    vector< vector<float> > Rates;
    vector< vector<float> > RatesErr;
    vector< vector<float> > Imon;
    vector< vector<float> > ImonErr;
    vector< vector<float> > Eff;
    vector< vector<float> > EffErr;
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

#endif // UTILS_H
