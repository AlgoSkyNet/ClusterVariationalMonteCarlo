/* 
 * File:   mcMisc.h
 * Author: chris
 *
 * Created on August 3, 2011, 8:34 PM
 */
#include <stdio.h>
#include <stdlib.h>
#include "mc.h"

#ifndef MCMISC_H
#define	MCMISC_H

class mcMisc {
public:

    static mc_state faculty(int n) {

        if ((n == 1) || (n == 0)) {
            mc_state ONE = 1;
            return ONE;
        } else {
            return faculty(n - 1) * n;
        }
    }

    static mc_state binCoeff(int a, int b) {
        mc_state ret;
        ret = mcMisc::faculty(a);
        ret = ret / faculty(b);
        ret = ret / faculty(a - b);
        return ret;
    }

    static void printBinaryString(int l) {
        printBinaryString(l, 32);
    }

    static void printBinaryString(mc_state l, int ubd) {
        for (int i = (ubd - 1); i >= 0; i--) {
            printf("%d", ((l&(ONE<<i))>>i));
        }
        printf("\n");
    }

    static mc_state getSystemState(mc_state state, int nClusterSites, int nClusters, int spin) {
        mc_state retState = 0;
        mc_state tmask = ((ONE << nClusterSites) - ONE);
        for (int i = 0; i < nClusters; i++) {
            int shft1 = i * nClusterSites;
            int shft2 = (2 * i + spin) * nClusterSites;
            retState += (((state & (tmask << shft1)) >> shft1) << shft2);
        }
        return retState;
    }

    static mc_state getSpinState(mc_state state, int nClusterSites, int nClusters, int spin) {
        mc_state retState = 0;
        mc_state tmask = ((ONE << nClusterSites) - ONE);
        for (int i = 0; i < nClusters; i++) {
            int shft2 = i * nClusterSites;
            int shft1 = (2 * i + spin) * nClusterSites;
            retState += (((state & (tmask << shft1)) >> shft1) << shft2);
        }
        return retState;
    }

    static mc_state getSystemState_Up(mc_state state, int nClusterSites, int nClusters) {
        return getSystemState(state, nClusterSites, nClusters, 1);
    }

    static mc_state getSystemState_Down(mc_state state, int nClusterSites, int nClusters) {
        return getSystemState(state, nClusterSites, nClusters, 0);
    }

    static mc_state getSpinState_Up(mc_state state, int nClusterSites, int nClusters) {
        return getSpinState(state, nClusterSites, nClusters, 1);
    }

    static mc_state getSpinState_Down(mc_state state, int nClusterSites, int nClusters) {
        return getSpinState(state, nClusterSites, nClusters, 0);
    }


};


#endif	/* MCMISC_H */

