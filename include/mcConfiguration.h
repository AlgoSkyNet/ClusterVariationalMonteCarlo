/* 
 * File:   mcConfigurationCluster.h
 * Author: chris
 *
 * Created on August 3, 2011, 3:20 PM
 */

#include<iostream>
#include<fstream>
#include "mc.h"

#ifndef MCCONFIGURATION_H
#define	MCCONFIGURATION_H

using namespace std;
// Using the configuration file:
//
class mcConfiguration {
public:

    static mcConfiguration* readFromFile(char* ifilename) {
        mcConfiguration* ret = new mcConfiguration();
        ret->filename=ifilename;
        ifstream config(ret->filename);
        config >> ret->nClusterSites;
        config >> ret->nElectronsUp;
        config >> ret->nElectronsDown;
        config >> ret->nHoppings;
        //allocate all fields
        ret->hoppings = new int*[ret->nHoppings];
        for (int i = 0; i < ret->nHoppings; i++) {
            ret->hoppings[i] = new int[2];
        }
        ret->hoppingValues = new double[ret->nHoppings];
        ret->correlationValues = new double[ret->nClusterSites];
        ret->chemicalPotentialUp = new double[ret->nClusterSites];
        ret->chemicalPotentialDown = new double[ret->nClusterSites];
        for (int i = 0; i < ret->nHoppings; i++) {
            config >> ret->hoppings[i][0] >> ret->hoppings[i][1] >> ret->hoppingValues[i];
        }
        for (int i = 0; i < ret->nClusterSites; i++) {
            config >> ret->correlationValues[i] >> ret->chemicalPotentialUp[i] >> ret->chemicalPotentialDown[i];
        }
        // the cluser interconnects
        config >> ret->nClusters;
        config >> ret->nPerturbations;
        //allocate all fields
        ret->perturbations = new int*[ret->nPerturbations];
        for (int i = 0; i < ret->nPerturbations; i++) {
            ret->perturbations[i] = new int[4];
        }
        ret->perturbationValues = new double[ret->nPerturbations];
        for (int i = 0; i < ret->nPerturbations; i++) {
            config >> ret->perturbations[i][0] >> ret->perturbations[i][1]>> ret->perturbations[i][2] >> ret->perturbations[i][3] >> ret->perturbationValues[i];
        }
        
        config>>ret->nWarmupSteps; 
        config>>ret->nMeasurements; 
        config>>ret->nSkipSteps; 
        config>>ret->gammaStart;
        config>>ret->gammaStop;
        config>>ret->gammaSteps;
        ret->nTotalSites=ret->nClusterSites*ret->nClusters;
        ret->nTotalElectronsUp=ret->nClusters*ret->nElectronsUp;
        ret->nTotalElectronsDown=ret->nClusters*ret->nElectronsDown;
        
        mc_state single = (1 << ret->nClusterSites) - 1;
        for (int i = 0; i < ret->nClusters; i++) {
            int nd = (2 * i) * ret->nClusterSites;
            int nu = (2 * i + 1) * ret->nClusterSites;
            ret->downMask += single << (2*i*ret->nClusterSites);
            ret->upMask += single <<((2*i+1)*ret->nClusterSites);
        }
        
        
        return ret;
    }
    
    void init(){
    
    
    }

    void print() {
        cout<<"---------------------------------------"<<endl;
        cout<<"---------------------------------------"<<endl;
        cout << nClusterSites << endl;
        cout << nElectronsUp << endl;
        cout << nElectronsDown << endl;
        cout << nHoppings << endl;
        //allocate all fields
        for (int i = 0; i < nHoppings; i++) {
            cout << hoppings[i][0] << " " << hoppings[i][1] << " " << hoppingValues[i] << endl;
            ;
        }
        for (int i = 0; i < nClusterSites; i++) {
            cout << correlationValues[i] << " " << chemicalPotentialUp[i] << " " << chemicalPotentialDown[i] << endl;
        }
        cout << nClusters << endl;
        cout << nPerturbations << endl;
        //allocate all fields
        for (int i = 0; i < nPerturbations; i++) {
            cout << perturbations[i][0] << " " << perturbations[i][1] << " " << perturbationValues[i] << endl;
            
        }
        cout << "Total number of sites ="<<nTotalSites<<endl;
        cout << "Total number of electrons up ="<<nTotalElectronsUp<<endl;
        cout << "Total number of electrons down ="<<nTotalElectronsDown<<endl;
        cout << "Running gamma from "<< gammaStart<<" to "<<gammaStop<<" in "<<gammaSteps<<" steps"<<endl;
//        cout << "Downmask = "<<downMask<<endl;
//        cout << "Upmask = "  <<upMask<<endl;
        cout << "---------------------------------------"<<endl;
        cout << "---------------------------------------"<<endl;
    }
    int nClusterSites;
    int nElectronsUp;
    int nElectronsDown;
    int nHoppings;
    int** hoppings;
    double* hoppingValues;
    double* correlationValues;
    double* chemicalPotentialUp;
    double* chemicalPotentialDown;
    int nClusters;
    int nPerturbations;
    int** perturbations;
    double* perturbationValues;
    int nTotalSites;
    int nWarmupSteps;
    int nMeasurements;
    int nSkipSteps;
    int nTotalElectronsUp;
    int nTotalElectronsDown;
    double gammaStart;
    double gammaStop;
    int gammaSteps;
    char* filename;
    double **sitePosition;
    mc_state downMask;
    mc_state upMask;
};
#endif	/* MCCONFIGURATION_H */

