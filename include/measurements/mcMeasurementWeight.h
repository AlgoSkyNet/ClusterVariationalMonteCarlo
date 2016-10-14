/* 
 * File:   mcMeasurementEnergy.h
 * Author: chris
 *
 * Created on August 5, 2011, 9:31 AM
 */
#include "../mc.h"
#include"mcAbstractMeasurement.h"

#ifndef MCMEASUREMENTWEIGHT_H
#define	MCMEASUREMENTWEIGHT_H

#include "../vectors/mcAbstractVector.h"

class mcMeasurementWeight : public mcAbstractMeasurement {
public:

    mcMeasurementWeight() {
    }

    mcMeasurementWeight(mcConfiguration* conf, mcAbstractVector* twf) {
        theConfiguration = conf;
        theWaveFunction = twf;
        filenameExtension=new string("weight.out");
        setUp();
        this->init();
    }

    void measurement(mc_state s) {
        this->recordMeasurement(theWaveFunction->get(s));
    }

protected:
private:

    void setUp() {

    }
};
#endif	

