/* 
 * File:   mcMeasurementEnergy.h
 * Author: chris
 *
 * Created on August 5, 2011, 9:31 AM
 */
#include "../mc.h"
#include"mcAbstractMeasurement.h"

#ifndef MCMEASUREMENTTOTALOCCUPATION_H
#define	MCMEASUREMENTTOTALOCCUPATION_H

class mcMeasurementTotalOccupation : public mcAbstractMeasurement {
public:
    
    mcMeasurementTotalOccupation(){
    }
    mcMeasurementTotalOccupation(mcConfiguration* conf, mcAbstractVector* twf) {
        theConfiguration = conf;
        theWaveFunction = twf;
        ONE = 1;
        ZERO = 0;
        next=NULL;
        setUp();
        this->init();
    }

    void measurement(mc_state s) {
        double ret = 0;

//        stateType upstate = 0;
//        stateType downstate = 0;
//        stateType single = (ONE << theConfiguration->nClusterSites) - ONE;
//        stateType tmp = 0;
//        for (int i = 0; i < theConfiguration->nClusters; i++) {
//            int nd = (2 * i) * theConfiguration->nClusterSites;
//            int nu = (2 * i + 1) * theConfiguration->nClusterSites;
//            int nn = i * theConfiguration->nClusterSites;
//            stateType ssd = (((s & (single << nd)) >> nd) << nn);
//            stateType ssu = (((s & (single << nu)) >> nu) << nn);
//            downstate += ssd;
//            upstate += ssu;
//        }
//        stateType docc = upstate&downstate;        
//        stateType tbit = 0;
//        for (int i = 0; i < theConfiguration->nTotalSites; i++) {
//            tbit = ((docc & (ONE << i)) >> i);
//            ret += tbit.get_d();
//        }
        ret = mc_state_popcount(s);
        this->recordMeasurement(ret);
    }

protected:
private:
    mc_state ONE;
    mc_state ZERO;
    mc_state upmask;
    mc_state downmask;
    int nSites;

    void setUp() {

        mc_state tmp = (ONE << theConfiguration->nClusterSites) - ONE;
        for (int i = 0; i < theConfiguration->nClusters; i++) {
            downmask = downmask + (tmp << (2 * i * theConfiguration->nClusterSites));
            upmask = upmask + (tmp << ((2 * i + 1) * theConfiguration->nClusterSites));
        }

    }
};
#endif	/* MCMEASUREMENTOCCUPATION_H */

