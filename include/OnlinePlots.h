#ifndef ONLINEPLOTS_H
#define ONLINEPLOTS_H

#include "../include/utils.h"

#include <string>

using namespace std;

void MakeRatePlots(Infrastructure Infra, string fName);
void MakeCurrentPlots(Infrastructure Infra, string fName);
void MakeDIPPlots(string fName);

#endif // ONLINEPLOTS_H
