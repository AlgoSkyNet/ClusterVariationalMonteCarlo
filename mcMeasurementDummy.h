/* 
 * File:   mcMeasurementDummy.h
 * Author: chris
 *
 * Created on August 5, 2011, 2:08 PM
 */

#include "mc.h"

#ifndef MCMEASUREMENTDUMMY_H
#define	MCMEASUREMENTDUMMY_H

class mcMeasurementDummy : public mcAbstractMeasurement{
public:
    mcMeasurementDummy(){};
    mcMeasurementDummy(mcConfiguration* c, mcAbstractVector* twf){
        theConfiguration=c;
        theWaveFunction=twf;
        init();
    } 
    void measurement(mc_state i){
//        recordMeasurement(i.get_d());
        recordMeasurement(mc_state_get_d(i));
    }
};


#endif	/* MCMEASUREMENTDUMMY_H */

