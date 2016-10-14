/* 
 * File:   mcTrialWaveFunctionConfigurable.h
 * Author: chris
 *
 * Created on August 4, 2011, 2:01 PM
 */

#include "mc.h"
#include "mcAbstractVector.h"
#include "mcOmegaMatrixVectorProduct.h"
#include "mcKroneckerProduct.h"

#ifndef MCTRIALWAVEFUNCTIONCONFIGURABLE_H
#define	MCTRIALWAVEFUNCTIONCONFIGURABLE_H

class mcTrialWaveFunctionConfigurable : public mcAbstractVector {
private:
    mcConfiguration* theConfiguration;
    mcAbstractVector* theClusterWaveFunction;
    mcAbstractVector* theSingleClusterWaveFunction;
    mcOmegaMatrixVectorProduct** theProjector;
    mcAbstractVector* theFirstProjector;

    void setup() {
        theClusterWaveFunction = theSingleClusterWaveFunction;

        for (int i = 0; i < theConfiguration->nClusters - 1; i++) {
            theClusterWaveFunction = new mcKroneckerProduct(theClusterWaveFunction, theSingleClusterWaveFunction);
        }

        theFirstProjector = theClusterWaveFunction;
        int npert = theConfiguration->nPerturbations;
        int nsites = theConfiguration->nClusterSites;
        theProjector = new mcOmegaMatrixVectorProduct*[2 * npert];
        for (int i = 0; i < npert; i++) {
            for (int s = 0; s < 2; s++) {
                int n = 2 * i + s;
                int s1 = theConfiguration->perturbations[i][0];
                int s2 = theConfiguration->perturbations[i][1];
                int c1 = theConfiguration->perturbations[i][2];
                int c2 = theConfiguration->perturbations[i][3];
                int n1 = s1 + (2 * c1 + s) * nsites;
                int n2 = s2 + (2 * c2 + s) * nsites;
                theProjector[n] = new mcOmegaMatrixVectorProduct(theFirstProjector, n1, n2, theConfiguration->perturbationValues[i]);
                theFirstProjector = theProjector[n];
            }
        }

//        cout << "Dimension of trial wave function = " << getDimension1() << endl;
    }
public:

    mcTrialWaveFunctionConfigurable(mcConfiguration* conf, mcAbstractVector* vec) {
        theConfiguration = conf;
        theSingleClusterWaveFunction = vec;
        setup();
    }

    mc_state getDimension1() {
        return theClusterWaveFunction->getDimension1();
    }


    double get(mc_state i) {
        return theFirstProjector->get(i);
    }

    void setGamma(double g) {
        for (int i = 0; i < 2 * theConfiguration->nPerturbations; i++) {
            theProjector[i]->setGamma(g);
        }
    }

};


#endif	/* MCTRIALWAVEFUNCTIONCONFIGURABLE_H */

