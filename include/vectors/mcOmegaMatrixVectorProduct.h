/* 
 * File:   mcOmegaMatrixVectorProduct.h
 * Author: chris
 *
 * Created on August 3, 2011, 2:17 PM
 */

#include<math.h>
#include"mcAbstractVector.h"
#include "../mc.h"

#ifndef MCOMEGAMATRIXVECTORPRODUCT_H
#define	MCOMEGAMATRIXVECTORPRODUCT_H

#define _OMEGASIGN_

class mcOmegaMatrixVectorProduct : public mcAbstractVector {
private:
    mcAbstractVector* A;
    double gamma;
    int site1;
    int site2;
    double perturbation;
    double csinh;
    double ccosh;
    mc_state mask1;
    mc_state mask2;
    mc_state mask;
public:

    mcOmegaMatrixVectorProduct() {
    }

    mcOmegaMatrixVectorProduct(mcAbstractVector* iA, int isite1, int isite2, double val) {
        A = iA;
        gamma = 0;
        site1 = isite1;
        site2 = isite2;
        perturbation = val;
        init();
    }

    mcOmegaMatrixVectorProduct(mcAbstractVector* iA, double igamma, int isite1, int isite2) {
        A = iA;
        gamma = igamma;
        site1 = isite1;
        site2 = isite2;
        init();
    }

    void init() {
        mc_state ONE = 1;
        mask1 = ONE << site1;
        mask2 = ONE << site2;

        int mins = min(site1, site2);
        int maxs = max(site1, site2);
        mc_state minm = ONE << mins;
        mc_state maxm = ONE << maxs;
        if ((maxs - mins) < 2) {
            mask = 0;
        } else {
            if (mins > 0) {
                mask = (maxm - 1) - (minm - 1) - minm;

            } else {
                mask = (maxm - 1) - minm;
            }
        }
#ifdef USE_64BIT
        cout << "Projector connecting " << site1 << " and " << site2 << " mask = " << mask << endl;
        mcMisc::printBinaryString(mask, 16);
#else
        cout << "Projector connecting " << site1 << " and " << site2 << endl;
        mcMisc::printBinaryString(mask, 16);
#endif
    }

    void setGamma(double g) {
        gamma = g;
        csinh = perturbation * sinh(gamma);
        ccosh = perturbation * perturbation * (cosh(gamma));
        //        csinh=perturbation*sin(gamma);
        //        ccosh=cos(gamma);
    }

    double get(mc_state i) {
        mc_state ONE = 1;
        double ret = 0;
        bool n1 = false;
        bool n2 = false;
        double r1 = 0;
        double r2 = 0;
        int sign = 0;
        unsigned int bcount=0;

        if ((i & (ONE << site1)) >> site1 == 1) {
            n1 = true;
        }
        if ((i & (ONE << site2)) >> site2 == 1) {
            n2 = true;
        }

        if (n1 & n2) {
            ret = A->get(i);
        } else if ((!n1) & (!n2)) {
            ret = A->get(i);
        } else if (n1 & (!n2)) {
            mc_state ti = i - mask1 + mask2;
            mc_state tmp = mask&i;
            bcount = mc_state_popcount(tmp);
            bcount = bcount & 1;
            bcount = bcount << 1;
            sign = 1 - bcount;
            r1 = ccosh * A->get(i);
            r2 = csinh * A->get(ti);
            ret = r1 - sign*r2;
        } else if ((!n1) & n2) {
            mc_state ti = i + mask1 - mask2;
            mc_state tmp = mask&i;
            bcount = mc_state_popcount(tmp);
            bcount = bcount & 1;
            bcount = bcount << 1;
            sign = 1 - bcount;
            r1 = ccosh * A->get(i);
            r2 = csinh * A->get(ti);
            ret = r1 - sign*r2;
        }
        return ret;
    }
};

#endif	/* MCOMEGAMATRIXVECTORPRODUCT_H */

