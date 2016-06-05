#include "../include/IniFile.h"
#include "../include/MsgSvc.h"
#include "../include/utils.h"
#include "../include/OnlinePlots.h"

#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <map>
#include "TMath.h"
#include "TFile.h"
#include "TTree.h"
#include "TStyle.h"
#include "TF1.h"
#include "TH1S.h"
#include "TH1F.h"
#include "TH3S.h"
#include "TH3F.h"
#include "THistPainter.h"
#include "TF1.h"
#include "TGraphErrors.h"
#include "TGraphPainter.h"
#include "TMultiGraph.h"
#include "TCanvas.h"
#include "TLegend.h"

using namespace std;

void MakeRatePlots(Infrastructure Infra, string fName){
    //We need a vector of vectors of vectors... to keep
    //all the data in a dynamic way. This vector will
    //contain 3 vector<vector<float>> : 1 for the HVeff,
    //1 for the Rates and 1 for the RatesErr
    vector< vector<float> > Data[3];

    for(unsigned int i = 0; i < 3; i++)
        Data[i].clear();

    //To be able to read the file in a dynamical way, we
    //also need to exactly know the number of RPCs and of
    //partitions
    unsigned int TotRPCs = 0;
    unsigned int TotPartitions = 0;
    vector<unsigned int> RPCPartitions;
    RPCPartitions.clear();

    for(unsigned int t = 0; t < Infra.nTrolleys; t++){
        TotRPCs += Infra.Trolleys[t].nSlots;

        for(unsigned int s = 0; s < Infra.Trolleys[t].nSlots; s++){
            TotPartitions += Infra.Trolleys[t].RPCs[s].nPartitions;
            RPCPartitions.push_back(Infra.Trolleys[t].RPCs[s].nPartitions);
        }
    }

    //Resize Data vector content accordingly what we have
    //got from the infracstructure
    Data[0].resize(TotRPCs);
    Data[1].resize(TotPartitions);
    Data[2].resize(TotPartitions);

    //Open in reading mode the rate file
    ifstream RateFile(fName.c_str(), ios::in);

    if(RateFile){
        while(RateFile.good()){
            //The first column is always the HVstep that
            //is not really useful to us as data. But we
            //will use it to know that we reached the last
            //line of the rate file. Indeed, each line
            //ending with a newline character, we won't
            //reach the end of the file before trying to
            //read a very last time the HVstep
            unsigned int HVstep = 0;
            RateFile >> HVstep;

            if(HVstep != 0){
                //To save the rates and their errors in the
                //right vector, we need to now the partition
                //index among all the partitions
                unsigned int pi = 0;

                //Then in the rest of the line, we will find
                //a collection of HVeff, Rates and RatesErr
                //built like:
                //HVeff1 Rate11 RateErr11 Rate12 RateErr12 ... HVeff2 ...
                //This means we first need to read the HVeff
                //value for 1 rpc then to read all the rates
                //and errors for each of its partitions.

                //This, start by looping other the number of
                //rpcs.
                for(unsigned int r = 0; r < TotRPCs; r++){
                    unsigned int tmphv = 0;
                    RateFile >> tmphv;
                    Data[0][r].push_back(tmphv);

                    //For each rpc, loop over its number of
                    //partitions
                    for(unsigned int p = 0; p < RPCPartitions[r]; p++){
                        unsigned int tmprate = 0;
                        unsigned int tmperr = 0;
                        RateFile >> tmprate >> tmperr;
                        Data[1][pi].push_back(tmprate);
                        Data[2][pi].push_back(tmperr);

                        pi++;
                    }
                }
            }
        }
    } else {
        MSG_ERROR("[Online] Ratefile " + fName + " could not be opened");
        return;
    }

    RateFile.close();

    //Now that all the data is contained into our big vector
    //we will be able to make some plots


}
