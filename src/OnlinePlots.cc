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
#include "TStyle.h"
#include "TGraphErrors.h"
#include "TGraphPainter.h"
#include "TMultiGraph.h"
#include "TCanvas.h"
#include "TAxis.h"
#include "TLegend.h"

using namespace std;

void MakeRatePlots(Infrastructure Infra, string fName){
    //We need a vector of vectors of vectors... to keep
    //all the data in a dynamic way. This vector will
    //contain 4 vector<vector<float>> : 1 for the HVeff,
    //1 filled with 0 for the HVeffError, 1 for the Rates
    //and 1 for the RatesErr.
    //The vector filled with 0s will be important to use
    //the TGraphError objects
    vector< vector<float> > Data[4];

    for(unsigned int i = 0; i < 4; i++)
        Data[i].clear();

    //To be able to read the file in a dynamical way, we
    //also need to exactly know the number of RPCs and of
    //partitions
    unsigned int TotRPCs = 0;
    unsigned int TotPartitions = 0;
    vector<unsigned int> RPCPartitions;
    RPCPartitions.clear();

    //Make another vector to get the names of the chambers
    vector<string> RPCNames;
    RPCNames.clear();

    for(unsigned int t = 0; t < Infra.nTrolleys; t++){
        TotRPCs += Infra.Trolleys[t].nSlots;

        for(unsigned int s = 0; s < Infra.Trolleys[t].nSlots; s++){
            TotPartitions += Infra.Trolleys[t].RPCs[s].nPartitions;
            RPCPartitions.push_back(Infra.Trolleys[t].RPCs[s].nPartitions);
            RPCNames.push_back(Infra.Trolleys[t].RPCs[s].name);
        }
    }

    //Resize Data vector content accordingly what we have
    //got from the infracstructure
    Data[0].resize(TotRPCs);
    Data[1].resize(TotPartitions);
    Data[2].resize(TotPartitions);
    Data[3].resize(TotPartitions);

    //To save and access to  the rates and their errors in
    //the right vector, we need to now the partition index
    //among all the partitions
    unsigned int pi = 0;

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
                //Then in the rest of the line, we will find
                //a collection of HVeff, Rates and RatesErr
                //built like:
                //HVeff1 Rate11 RateErr11 Rate12 RateErr12 ... HVeff2 ...
                //This means we first need to read the HVeff
                //value for 1 rpc then to read all the rates
                //and errors for each of its partitions.

                //Reinitialise pi before the next line
                pi = 0;

                //Thus, start by looping other the number of
                //rpcs.
                for(unsigned int r = 0; r < TotRPCs; r++){
                    float tmphv = 0;
                    RateFile >> tmphv;
                    Data[0][r].push_back(tmphv);
                    Data[1][r].push_back(0.);

                    //For each rpc, loop over its number of
                    //partitions
                    for(unsigned int p = 0; p < RPCPartitions[r]; p++){
                        float tmprate = 0;
                        float tmperr = 0;
                        RateFile >> tmprate >> tmperr;
                        Data[2][pi].push_back(tmprate);
                        Data[3][pi].push_back(tmperr);

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
    string fNameROOT = fName.erase(fName.find_last_of(".")) + ".root";
    TFile fROOT(fNameROOT.c_str(),"RECREATE");

    //re-initialise the partition index pi
    pi = 0;

    for(unsigned int r = 0; r < TotRPCs; r++){
        TMultiGraph* ChamberRatesPlot = new TMultiGraph();

        for(unsigned int p = 0; p < RPCPartitions[r]; p++){
            string partitionID = "ABCD";
            string Graphtitle = RPCNames[r] + " Partition " + partitionID[p];
            string Canvastitle = "Rate-" + RPCNames[r] + "-Partition-" + partitionID[p];

            TGraphErrors* PartitionRatePlot = new TGraphErrors(Data[0][r].size(),&(Data[0][r][0]),&(Data[2][pi][0]),&(Data[1][r][0]),&(Data[3][pi][0]));
            PartitionRatePlot->SetTitle(Graphtitle.c_str());
            PartitionRatePlot->GetXaxis()->SetTitle("HV_{eff}(V)");
            PartitionRatePlot->GetYaxis()->SetTitle("Mean hit rate(Hz/cm^{2})");
            PartitionRatePlot->GetXaxis()->SetRangeUser(Data[0][r].front(),Data[0][r].back());
            PartitionRatePlot->SetLineColor(p+2);
            PartitionRatePlot->SetMarkerColor(p+2);
            PartitionRatePlot->SetMarkerStyle(p+20);

            TCanvas* cPart = new TCanvas(Canvastitle.c_str());
            cPart->cd(0);
            ChamberRatesPlot->Draw("ap");
            cPart->BuildLegend();
            cPart->Write();

            ChamberRatesPlot->Add(PartitionRatePlot);
            pi++;
        }

        string MultigraphTitle = "Mean hit rate evolution in " + RPCNames[r];
        string MultiCanvasTitle = "Rate-" + RPCNames[r];

        string DQMFolder = fName.substr(0,fName.find_last_of("/")) + "/Online/";
        string mkdirDQMFolder = "mkdir -p " + DQMFolder;
        system(mkdirDQMFolder.c_str());

        TCanvas* cMulti = new TCanvas(MultiCanvasTitle.c_str());
        cMulti->cd(0);
        ChamberRatesPlot->Draw("ap");
        ChamberRatesPlot->SetTitle(MultigraphTitle.c_str());
        ChamberRatesPlot->GetXaxis()->SetTitle("HV_{eff}(V)");
        ChamberRatesPlot->GetYaxis()->SetTitle("Mean hit rate(Hz/cm^{2})");
        cMulti->BuildLegend();
        cMulti->Update();
        string PDF = DQMFolder + cMulti->GetName() + ".pdf";
        string PNG = DQMFolder + cMulti->GetName() + ".png";
        cMulti->SaveAs(PDF.c_str());
        cMulti->SaveAs(PNG.c_str());
        cMulti->Write();
    }

    fROOT.Close();
}

//*****************************************************************************************

void MakeCurrentPlots(Infrastructure Infra, string fName){
    //We need a vector of vectors of vectors... to keep
    //all the data in a dynamic way. This vector will
    //contain 4 vector<vector<float>> : 1 for the HVeff,
    //1 filled with 0 for the HVeffError, 1 for the Imon
    //and 1 for the ImonErr.
    //The vector filled with 0s will be important to use
    //the TGraphError objects
    vector< vector<float> > Data[4];

    for(unsigned int i = 0; i < 4; i++)
        Data[i].clear();

    //To be able to read the file in a dynamical way, we
    //also need to exactly know the number of RPCs and of
    //partitions
    unsigned int TotRPCs = 0;
    unsigned int TotPartitions = 0;
    vector<unsigned int> RPCPartitions;
    RPCPartitions.clear();

    //Make another vector to get the names of the chambers
    vector<string> RPCNames;
    RPCNames.clear();

    for(unsigned int t = 0; t < Infra.nTrolleys; t++){
        TotRPCs += Infra.Trolleys[t].nSlots;

        for(unsigned int s = 0; s < Infra.Trolleys[t].nSlots; s++){
            TotPartitions += Infra.Trolleys[t].RPCs[s].nPartitions;
            RPCPartitions.push_back(Infra.Trolleys[t].RPCs[s].nPartitions);
            RPCNames.push_back(Infra.Trolleys[t].RPCs[s].name);
        }
    }

    //Resize Data vector content accordingly what we have
    //got from the infracstructure
    Data[0].resize(TotPartitions);
    Data[1].resize(TotPartitions);
    Data[2].resize(TotPartitions);
    Data[3].resize(TotPartitions);

    //Open in reading mode the rate file
    ifstream CurrentFile(fName.c_str(), ios::in);

    if(CurrentFile){
        while(CurrentFile.good()){
            //The first column is always the HVstep that
            //is not really useful to us as data. But we
            //will use it to know that we reached the last
            //line of the rate file. Indeed, each line
            //ending with a newline character, we won't
            //reach the end of the file before trying to
            //read a very last time the HVstep
            unsigned int HVstep = 0;
            CurrentFile >> HVstep;

            if(HVstep != 0){
                //Then in the rest of the line, we will find
                //a collection of HVeff, Rates and RatesErr
                //built like:
                //HVeff1 Rate11 RateErr11 Rate12 RateErr12 ... HVeff2 ...
                //This means we first need to read the HVeff
                //value for 1 rpc then to read all the rates
                //and errors for each of its partitions.

                //Thus, start by looping other the number of
                //rpcs.
                for(unsigned int p = 0; p < TotPartitions; p++){
                    float tmphv = 0;
                    float tmpimon = 0;
                    float tmperr = 0;
                    CurrentFile >> tmphv >> tmpimon >> tmperr;
                    Data[0][p].push_back(tmphv);
                    Data[1][p].push_back(0.);
                    Data[2][p].push_back(tmpimon);
                    Data[3][p].push_back(tmperr);
                }
            }
        }
    } else {
        MSG_ERROR("[Online] Currentfile " + fName + " could not be opened");
        return;
    }

    CurrentFile.close();

    //Now that all the data is contained into our big vector
    //we will be able to make some plots
    string fNameROOT = fName.erase(fName.find_last_of(".")) + ".root";
    TFile fROOT(fNameROOT.c_str(),"RECREATE");

    //re-initialise the partition index pi
    unsigned int pi = 0;

    for(unsigned int r = 0; r < TotRPCs; r++){
        TMultiGraph* ChamberCurrentsPlot = new TMultiGraph();

        for(unsigned int p = 0; p < RPCPartitions[r]; p++){
            string partitionID = "ABCD";
            string Graphtitle = RPCNames[r] + " Partition " + partitionID[p];
            string Canvastitle = "Current-" + RPCNames[r] + "-Partition-" + partitionID[p];

            TGraphErrors* PartitionCurrentPlot = new TGraphErrors(Data[0][pi].size(),&(Data[0][pi][0]),&(Data[2][pi][0]),&(Data[1][pi][0]),&(Data[3][pi][0]));
            PartitionCurrentPlot->SetTitle(Graphtitle.c_str());
            PartitionCurrentPlot->GetXaxis()->SetTitle("HV_{eff}(V)");
            PartitionCurrentPlot->GetYaxis()->SetTitle("Mean current (µA/cm^{2})");
            PartitionCurrentPlot->GetXaxis()->SetRangeUser(Data[0][pi].front(),Data[0][pi].back());
            PartitionCurrentPlot->SetLineColor(p+2);
            PartitionCurrentPlot->SetMarkerColor(p+2);
            PartitionCurrentPlot->SetMarkerStyle(p+20);

            TCanvas* cPart = new TCanvas(Canvastitle.c_str());
            cPart->cd(0);
            ChamberCurrentsPlot->Draw("ap");
            cPart->BuildLegend();
            cPart->Write();

            ChamberCurrentsPlot->Add(PartitionCurrentPlot);
            pi++;
        }

        string MultigraphTitle = "Mean monitored current evolution in " + RPCNames[r];
        string MultiCanvasTitle = "Current-" + RPCNames[r];

        string DQMFolder = fName.substr(0,fName.find_last_of("/")) + "/Online/";
        string mkdirDQMFolder = "mkdir -p " + DQMFolder;
        system(mkdirDQMFolder.c_str());

        TCanvas* cMulti = new TCanvas(MultiCanvasTitle.c_str());
        cMulti->cd(0);
        ChamberCurrentsPlot->Draw("ap");
        ChamberCurrentsPlot->SetTitle(MultigraphTitle.c_str());
        ChamberCurrentsPlot->GetXaxis()->SetTitle("HV_{eff}(V)");
        ChamberCurrentsPlot->GetYaxis()->SetTitle("Mean current (µA/cm^{2})");
        cMulti->BuildLegend();
        cMulti->Update();
        string PDF = DQMFolder + cMulti->GetName() + ".pdf";
        string PNG = DQMFolder + cMulti->GetName() + ".png";
        cMulti->SaveAs(PDF.c_str());
        cMulti->SaveAs(PNG.c_str());
        cMulti->Write();
    }

    fROOT.Close();
}
