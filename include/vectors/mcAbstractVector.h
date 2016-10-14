/* 
 * File:   MCAbstractVector.h
 * Author: chris
 *
 * Created on August 3, 2011, 11:14 AM
 */
#include<iostream>
//#include<gmpxx.h>
#include "mc.h"

#ifndef MCABSTRACTVECTOR_H
#define	MCABSTRACTVECTOR_H
using namespace std;

class mcAbstractVector {
private:
protected:
    mc_state dimension1;
public:

    virtual mc_state getDimension1() {
        return dimension1;
    }

//#ifndef USE_64BIT
//    virtual double get(mc_uint64 i) = 0;
//#endif
    virtual double get(mc_state i) = 0;

    void print() {
        for (int i = 0; i < getDimension1(); i++) {
            cout << get(i) << " ";
            cout << endl;
        }
    }
};
#endif	/* MCABSTRACTVECTOR_H */

