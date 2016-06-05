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

    if(argc != 1){
        MSG_WARNING("[OnlinePlots]: USAGE is : bin/onlineanalysis");
        return -1;
    } else {
        //First construct the GIF++ infrastructure with its
        //trolleys and RPCs

        //Get the chambers geometry
        IniFile* Dimensions = new IniFile(__dimensions.c_str());
        Dimensions->Read();

        Infrastructure GIF;
        SetInfrastructure(GIF,Dimensions);

        return 0;
    }
}
