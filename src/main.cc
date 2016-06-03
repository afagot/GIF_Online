#include "../include/OnlinePlots.h"
#include "../include/MsgSvc.h"

#include <sstream>

using namespace std;

int main(int argc ,char *argv[]){
    stringstream converter;

    converter << argv[0];
    string program;
    converter >> program;
    converter.clear();

    if(argc != 2){
        MSG_WARNING("[OnlinePlots]: USAGE is :" + program + " RateSummaryFile");
        return -1;
    } else {
        converter << argv[1];
        string resultsfile;
        converter >> resultsfile;
        converter.clear();

        return 0;
    }
}
