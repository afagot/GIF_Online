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

    if(argc != 2){
        MSG_WARNING("[Online] expects to have 2 parameters");
        MSG_WARNING("[Online] USAGE is : " + program + " /path/to/Offline/files");

        return -1;
    } else if (argc == 2){
        //First construct the GIF++ infrastructure with its
        //trolleys and RPCs

        //Get the chambers geometry
        IniFile* Dimensions = new IniFile(__dimensions.c_str());
        Dimensions->Read();

        Infrastructure GIF;
        SetInfrastructure(GIF,Dimensions);

        string path;

        converter << argv[1];
        converter >> path;
        converter.clear();

        string rateName     = path + "/Offline-Rate.csv";
        string currentName  = path + "/Offline-Current.csv";
        string dipName      = path + "/Offline-DIP.csv";

        if(existFile(rateName)) MakeRatePlots(GIF,rateName);
        if(existFile(currentName)) MakeCurrentPlots(GIF,currentName);
        if(existFile(dipName)) MakeDIPPlots(dipName);

        MSG_WARNING("[Online] Online plots updated");

        return 0;
    }
}
