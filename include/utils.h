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

const string __logpath = "/var/operation/RUN/log-online";
const string __dipinfo = "/var/operation/RUN/DIP_PUBLICATIONS";

//***************************** USEFUL FUNCTIONS

bool    existFile(string filename);
int     CharToInt(char& C);
string  CharToString(char& C);
string  intTostring(int value);
string  longTostring(long value);
string  floatTostring(float value);
string  GetLogTimeStamp();
void    getDIPParamUnit(string paramID, string &paramUnit) ;

//***************************** STRUCTURES FOR GIF INFRASTRUCTURE

//Infrastructure inside GIF++
struct RPC{
    string          name;
    unsigned int    nPartitions;
    unsigned int    nGaps;
    vector<string>  gapNames;
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
