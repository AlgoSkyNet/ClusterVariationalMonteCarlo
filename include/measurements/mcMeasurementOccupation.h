/* 
 * File:   mcMeasurementEnergy.h
 * Author: chris
 *
 * Created on August 5, 2011, 9:31 AM
 */
#include "../mc.h"
#include "mcAbstractMeasurement.h"

#ifndef MCMEASUREMENTOCCUPATION_H
#define	MCMEASUREMENTOCCUPATION_H

#include "../vectors/mcAbstractVector.h"

class mcMeasurementOccupation : public mcAbstractMeasurement {
public:

    mcMeasurementOccupation() {
    }

    mcMeasurementOccupation(mcConfiguration* conf, mcAbstractVector* twf, int i, int s) {
        theConfiguration = conf;
        theWaveFunction = twf;
        ONE = 1;
        ZERO = 0;
        next = NULL;
        site = i;
        spin = s;
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

        mc_state tbit = 0;
        if (spin == 0) {
            tbit = ((upstate & (ONE << site)) >> site);
        } else {
            tbit = ((downstate & (ONE << site)) >> site);
        }
//        ret=tbit.get_d();
        ret=mc_state_get_d(tbit); 
        this->recordMeasurement(ret);
    }

protected:
private:
    mc_state ONE;
    mc_state ZERO;
    mc_state upmask;
    mc_state downmask;
    int nSites;
    int site;
    int spin;

    void setUp() {

        mc_state tmp = (ONE << theConfiguration->nClusterSites) - ONE;
        for (int i = 0; i < theConfiguration->nClusters; i++) {
            downmask = downmask + (tmp << (2 * i * theConfiguration->nClusterSites));
            upmask = upmask + (tmp << ((2 * i + 1) * theConfiguration->nClusterSites));
        }

    }
};
#endif	/* MCMEASUREMENTSTAGGEREDMAGNETIZATION1D_H */

