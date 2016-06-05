#include "../include/utils.h"

using namespace std;

int CharToInt(char &C){
    stringstream ss;
    ss << C;

    int I;
    ss >> I;
    return I;
}

// ****************************************************************************************************


string CharToString(char& C){
    stringstream ss;
    ss << C;

    string S;
    ss >> S;
    return S;
}

// ****************************************************************************************************

string intTostring(int value){
    string word;
    stringstream ss;
    ss << value;
    ss>> word;

    return word;
}

// ****************************************************************************************************

string longTostring(long value){
    string word;
    stringstream ss;
    ss << value;
    ss>> word;

    return word;
}

// ****************************************************************************************************

string floatTostring(float value){
    string word;
    stringstream ss;
    ss << value;
    ss>> word;

    return word;
}

// ****************************************************************************************************

string GetLogTimeStamp(){
    stringstream stream;

    //Get time information
    time_t t = time(0);
    struct tm *Time = localtime(&t);
    int Y = Time->tm_year + 1900;
    int M = Time->tm_mon + 1;
    int D = Time->tm_mday;
    int h = Time->tm_hour;
    int m = Time->tm_min;
    int s = Time->tm_sec;

    //Set the Date
    string Date;

    stream << setfill('0') << setw(4) << Y << "-"
           << setfill('0') << setw(2) << M << "-"
           << setfill('0') << setw(2) << D << "."
           << setfill('0') << setw(2) << h << ":"
           << setfill('0') << setw(2) << m << ":"
           << setfill('0') << setw(2) << s << ".";

    stream >> Date;
    stream.clear();

    return Date;
}

// ****************************************************************************************************


//Functions to set up the structures needed to define the GIF++ infrastructure
void SetRPC(RPC &rpc, string ID, IniFile *geofile){
    rpc.name        = geofile->stringType(ID,"Name","");
    rpc.nPartitions = geofile->intType(ID,"Partitions",NPARTITIONS);
    rpc.nGaps       = geofile->intType(ID,"Gaps",0);

    for(unsigned int g = 0; g < rpc.nGaps; g++){
        string gapID = "Gap" + intTostring(g+1);
        string gapType = geofile->stringType(ID,gapID,"empty");

        string tmpgapname;
        if(gapType == "empty") tmpgapname = rpc.name;
        else tmpgapname = rpc.name + "-" + gapType;

        rpc.gapNames.push_back(tmpgapname);
    }

    rpc.HVeff.clear();
    rpc.Rates.clear();
    rpc.RatesErr.clear();
    rpc.Imon.clear();
    rpc.ImonErr.clear();
    rpc.Eff.clear();
    rpc.EffErr.clear();
}

// ****************************************************************************************************


void SetTrolley(GIFTrolley &trolley, string ID, IniFile *geofile){
    trolley.nSlots = geofile->intType(ID,"nSlots",NSLOTS);
    trolley.SlotsID = geofile->stringType(ID,"SlotsID","");

    for(unsigned int s = 0; s < trolley.nSlots; s++){
        string rpcID = ID + "S" + CharToString(trolley.SlotsID[s]);

        RPC temprpc;
        SetRPC(temprpc,rpcID,geofile);
        trolley.RPCs.push_back(temprpc);
    }
}

// ****************************************************************************************************


void SetInfrastructure(Infrastructure &infra, IniFile *geofile){
    infra.nTrolleys = geofile->intType("General","nTrolleys",NTROLLEYS);
    infra.TrolleysID = geofile->stringType("General","TrolleysID","");
    infra.Trolleys.clear();

    for(unsigned int t = 0; t < infra.nTrolleys; t++){
        string trolleyID = "T" + CharToString(infra.TrolleysID[t]);

        GIFTrolley tempTrolley;
        SetTrolley(tempTrolley, trolleyID, geofile);
        infra.Trolleys.push_back(tempTrolley);
    }
}

// ****************************************************************************************************


//Name of histograms
void SetIDName(string rpcID, unsigned int partition, char* ID, char* Name, string IDroot, string Nameroot){
    string P[4] = {"A","B","C","D"};
    sprintf(ID,"%s_%s_%s",IDroot.c_str(),rpcID.c_str(),P[partition].c_str());
    sprintf(Name,"%s %s_%s",Nameroot.c_str(),rpcID.c_str(),P[partition].c_str());
}
