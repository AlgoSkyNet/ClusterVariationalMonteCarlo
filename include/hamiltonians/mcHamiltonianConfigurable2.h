/* 
 * File:   mcHamiltonianConfigurable.h
 * Author: chris
 *
 * Created on August 3, 2011, 6:45 PM
 */
#include <xmmintrin.h>
#include <bitset>
#include "mcConfigurationCluster.h"
#include "mcAbstractClusterHamiltonian.h"

#ifndef MCHAMILTONIANCONFIGURABLE2_H
#define	MCHAMILTONIANCONFIGURABLE2_H

class mcHamiltonianConfigurable2 : public mcAbstractClusterHamiltonian {
    mcConfiguration* theConfiguration;
public:

    mcHamiltonianConfigurable2(mcConfiguration* c) {
        theConfiguration = c;
    }

    double matrixElementHopping(long left, long right) {
        double ret = 0;
        for (int i = 0; i < theConfiguration->nHoppings; i++) {
            int n = theConfiguration->hoppings[i][0];
            int m = theConfiguration->hoppings[i][1];
            double hval = theConfiguration->hoppingValues[i];
            int n1 = (right & (1 << n)) >> n;
            int n2 = (right & (1 << m)) >> m;
            double val = n1 + n2 - 2*n1*n2;
            long mask = ((1 << m) - 1)-(1 << (n + 1) - 1);
            int p = __builtin_popcountll(mask & right);
            p = p & 1;
            p = p << 1;
            double sign = (2 - p);
            long omask = (1 << n)+(1 << m);
            double olap = ((right^omask) == left);
            ret += hval*olap * sign*val;
        }
        return ret;
    }

    double matrixElementDiagonal(long up, long down) {
        double ret = 0;
        long docc = up&down;

        for (int i = 0; i < theConfiguration->nClusterSites; i++) {
            ret += theConfiguration->chemicalPotentialUp[i]*((up & (1 << i)) >> i);
            ret += theConfiguration->chemicalPotentialDown[i]*((down & (1 << i)) >> i);
            ret += theConfiguration->correlationValues[i]*((docc & (1 << i)) >> i);
        }
        return ret;
    }

    double matrixElement(long leftUp, long leftDown, long rightUp, long rightDown) {
        if ((leftUp == rightUp) && (leftDown != rightDown)) {
            return matrixElementHopping(leftDown, rightDown);
        } else if ((leftUp != rightUp) && (leftDown == rightDown)) {
            return matrixElementHopping(leftUp, rightUp);
        } else if ((leftUp == rightUp) && (leftDown = rightDown)) {
            return matrixElementDiagonal(leftUp, rightDown);
        }
        return 0;
    }
};


#endif	/* MCHAMILTONIANCONFIGURABLE2_H */

