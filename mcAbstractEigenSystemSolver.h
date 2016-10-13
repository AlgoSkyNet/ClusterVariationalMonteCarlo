/* 
 * File:   mcAbstractEigenSystemSolver.h
 * Author: chris
 *
 * Created on August 3, 2011, 3:18 PM
 */
#include "mc.h"
#include "mcConfiguration.h"
#include "mcAbstractVector.h"
#include "mcAbstractClusterHamiltonian.h"

#ifndef MCABSTRACTEIGENSYSTEMSOLVER_H
#define	MCABSTRACTEIGENSYSTEMSOLVER_H

class mcAbstractEigenSystemSolver {
protected:
    mcConfiguration* theConfiguration;
    mcAbstractClusterHamiltonian* theHamiltonian;
    mcAbstractVector* theGroundStateVector;
    double theGroundStateEnergy;

public:
    //    mcAbstractEigenSystemSolver(mcConfigurationCluster* conf, mcAbstractHamiltonian* ham){
    //        theConfiguration=conf;
    //        theHamiltonian=ham;
    //    }

    virtual mcAbstractVector* getGroundState() {
        return theGroundStateVector;
    };

    virtual double getGroundStateEnergy() {
        return theGroundStateEnergy;
    };
    
    
    virtual void cleanUp() {
    };
};


#endif	/* MCABSTRACTEIGENSYSTEMSOLVER_H */

