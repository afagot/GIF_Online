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

    if(argc != 3 && argc != 4){
        MSG_WARNING("[OnlinePlots]: USAGE is : bin/onlineanalysis /path/to/Offline-Rates.csv /path/to/Offline-Currents.csv /path/to/Offline-DIP.csv");
        MSG_WARNING("or if there is no rate file");
        MSG_WARNING("[OnlinePlots]: USAGE is : bin/onlineanalysis /path/to/Offline-Currents.csv /path/to/Offline-DIP.csv");
        return -1;
    } else if (argc == 3){
        //First construct the GIF++ infrastructure with its
        //trolleys and RPCs

        //Get the chambers geometry
        IniFile* Dimensions = new IniFile(__dimensions.c_str());
        Dimensions->Read();

        Infrastructure GIF;
        SetInfrastructure(GIF,Dimensions);

        converter << argv[1];
        string currentName;
        converter >> currentName;
        converter.clear();

        converter << argv[2];
        string dipName;
        converter >> dipName;
        converter.clear();

        if(!IsReRunning(currentName)) MSG_WARNING("[OnlinePlots] Update online plots");

        MakeCurrentPlots(GIF,currentName);
        MakeDIPPlots(dipName);

        return 0;
    } else if (argc == 4){
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

        converter << argv[2];
        string currentName;
        converter >> currentName;
        converter.clear();

        converter << argv[3];
        string dipName;
        converter >> dipName;
        converter.clear();

        if(!IsReRunning(currentName)) MSG_WARNING("[OnlinePlots] Update online plots");

        MakeRatePlots(GIF,rateName);
        MakeCurrentPlots(GIF,currentName);
        MakeDIPPlots(dipName);

        return 0;
    }
}
