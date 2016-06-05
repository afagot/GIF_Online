#include "../include/OnlinePlots.h"
#include "../include/MsgSvc.h"
#include "../include/utils.h"

#include <sstream>

using namespace std;

int main(int argc ,char *argv[]){
    stringstream converter;

    converter << argv[0];
    string program;
    converter >> program;
    converter.clear();

    if(argc != 3){
        MSG_WARNING("[OnlinePlots]: USAGE is : bin/onlineanalysis /path/to/Offline-Rates.csv /path/to/Offline-Currents.csv");
        return -1;
    } else {
        //First construct the GIF++ infrastructure with its
        //trolleys and RPCs

        //Get the chambers geometry
        IniFile* Dimensions = new IniFile(__dimensions.c_str());
        Dimensions->Read();

        Infrastructure GIF;
        SetInfrastructure(GIF,Dimensions);

        converter << argv[1];
        string rateName;
        converter >> rateName;
        converter.clear();

        MakeRatePlots(GIF,rateName);

        converter << argv[2];
        string currentName;
        converter >> currentName;
        converter.clear();

        MakeCurrentPlots(GIF,currentName);
        return 0;
    }
}
