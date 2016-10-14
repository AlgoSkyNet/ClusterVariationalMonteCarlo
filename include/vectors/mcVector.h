/* 
 * File:   mcMatrix.h
 * Author: chris
 *
 * Created on August 3, 2011, 11:42 AM
 */
#include"mcAbstractVector.h"
//#include<gmpxx.h>
#include "../mc.h"

#ifndef MCVECTOR_H
#define	MCVECTOR_H

class mcVector : public mcAbstractVector {
public:

    mcVector() {
    };

    mcVector(const mcVector& orig) {
    };

    mcVector(mc_state l1) {
#ifdef USE_64BIT
        mc_uint64 l2=(mc_uint64) l1;
        dimension1 = l2;
        data = new double[l2];
#else
        dimension1 = l1;
        data = new double[l1];
#endif
    }

    virtual ~mcVector() {
    };


    //    double get(stateType* m) {
    //        return get(m->get_ui());
    //    }
#ifdef USE_GMP

    double get(mc_uint64 m) {
        return data[m];
    }

    double get(mc_state m) {
        return get(mc_state_get_uli(m));
    }
#endif
#ifdef USE_128BIT

    double get(mc_state m) {
        return data[mc_state_get_uli(m)];
    }
#endif
#ifdef USE_64BIT

    double get(mc_uint64 m) {
        return data[m];
    }
#endif

    void set(mc_uint64 m, double v) {
        data[m] = v;
    }
#ifdef XXUSE_128BIT

    void set(mc_state m, double v) {
        set(mc_state_get_uli(m), v);
    }
#endif

    void cleanUp() {
        delete[] data;
    }

private:
    double* data;

};

#endif	/* MCVECTOR_H */

