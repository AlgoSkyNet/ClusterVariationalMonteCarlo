/* 
 * File:   mcMeasurementEnergy.h
 * Author: chris
 *
 * Created on August 5, 2011, 9:31 AM
 */
#include "../mc.h"
#include"mcAbstractMeasurement.h"

#ifndef MCMEASUREMENTSPIN2_H
#define	MCMEASUREMENTSPIN2_H

class mcMeasurementSpin2 : public mcAbstractMeasurement {
public:
    mcMeasurementSpin2(){}
    mcMeasurementSpin2(mcConfiguration* conf, mcAbstractVector* twf) {
        theConfiguration = conf;
        theWaveFunction = twf;
        ONE = 1;
        ZERO = 0;
        next = NULL;
        setUp();
        this->init();
    }

    void measurement(mc_state s) {
        double ret = 0;

        mc_state upstate = 0;
        mc_state downstate = 0;
        mc_state single = (ONE << theConfiguration->nClusterSites) - ONE;
        mc_state tmp = 0;
        for (int i = 0; i < theConfiguration->nClusters; i++) {
            int nd = (2 * i) * theConfiguration->nClusterSites;
            int nu = (2 * i + 1) * theConfiguration->nClusterSites;
            int nn = i * theConfiguration->nClusterSites;
            mc_state ssd = (((s & (single << nd)) >> nd) << nn);
            mc_state ssu = (((s & (single << nu)) >> nu) << nn);
            downstate += ssd;
            upstate += ssu;
        }
        mc_state docc = upstate^downstate;        
        mc_state tbit = 0;
        for (int i = 0; i < theConfiguration->nTotalSites; i++) {
            tbit = ((docc & (ONE << i)) >> i);
//            ret += tbit.get_d();
            ret += mc_state_get_d(tbit);
        }

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
#endif	/* MCMEASUREMENTSPIN2_H */

