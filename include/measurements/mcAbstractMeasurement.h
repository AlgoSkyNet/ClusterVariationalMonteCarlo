/* 
 * File:   mcAbstractMeasurement.h
 * Author: chris
 *
 * Created on August 4, 2011, 5:57 PM
 */

//#include<gmpxx.h>
#include <iostream>
#include <fstream>
#include "mc.h"
#include"mcAbstractVector.h"
#include"mcConfiguration.h"

#ifndef MCABSTRACTMEASUREMENT_H
#define	MCABSTRACTMEASUREMENT_H

//#define MC_MEASUREMENT_SPINUP 1;
//#define MC_MEASUREMENT_SPINDOWN -1;

class mcAbstractMeasurement {
public:
    double* data;
    string* filenameExtension;
    int dataCounter;
    int nSites;
    int nClusterSites;
    mc_state upmask;
    mc_state downmask;

    mcAbstractMeasurement() {
    }

    mcAbstractMeasurement(mcConfiguration* conf, mcAbstractVector* twf) {
        theConfiguration = conf;
        theWaveFunction = twf;
        filenameExtension = new string(".x.out");
        init();
    }

    virtual void init() {
        dataCounter = 0;
        data = new double[theConfiguration->nMeasurements];
    }

    virtual void measurement(mc_state s) = 0;

    void measure(mc_state s) {
        measurement(s);
        if (next != NULL) {
            next->measure(s);
        }
    }

    virtual double getError() {
        double mean = getMean();
        double error = 0;
        for (int i = 0; i < theConfiguration->nMeasurements; i++) {
            error += (data[i] - mean)*(data[i] - mean);
        }
        error = error / (theConfiguration->nMeasurements - 1);
        error = sqrt(error);
        return error;
    };

    double* getDataSeries() {
        return data;
    }

    virtual double getMean() {
        double mean = 0;
        for (int i = 0; i < theConfiguration->nMeasurements; i++) {
            mean += data[i];
        }
        return mean / theConfiguration->nMeasurements;
    };

    virtual void resetMeasurement() {
        dataCounter = 0;
        if (next != NULL) {
            next->resetMeasurement();
        }
    }

    virtual void recordMeasurement(double v) {
        data[dataCounter++] = v;
    }

    void setNext(mcAbstractMeasurement* n) {
        next = n;
    }

    mcAbstractMeasurement* getNext() {
        return next;
    }

    void writeToFile(char* fname_in, int thread){
//        string fnameweightext = ".weight.out";
        string* fname=new string(fname_in);
        std::string nstr;
        std::stringstream out;
        out <<fname->data()<<"."<< thread<<"."<< filenameExtension->data();
        nstr = out.str();
        cout<<"writing results to file "<<nstr<<endl;

        ofstream fout(nstr.data());
        for(int i=0;i<dataCounter;i++){
            fout << data[i]<<endl;            
        }
        fout.close();
    }
    
protected:
    mcAbstractMeasurement* next;
    mcConfiguration* theConfiguration;
    mcAbstractVector* theWaveFunction;
private:
};


#endif	/* MCABSTRACTMEASUREMENT_H */

