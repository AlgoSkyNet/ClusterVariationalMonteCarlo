/* 
 * File:   MCAbstractVector.h
 * Author: Chris Dahnken 
 *
 * Created on August 3, 2011, 11:14 AM
 */
#include<iostream>
//#include<gmpxx.h>
#include "../mc.h"

#ifndef MCABSTRACTVECTOR_H
#define	MCABSTRACTVECTOR_H
using namespace std;

/* 
 * mcAbstractVector 
 * Defines a call with just two functions
 * getDimension1: returns the length of the vectors
 * get(i): returns the element i of the vectors 
 * 
 * Additional there is a print function for debugging purposes
*/
class mcAbstractVector {
private:
protected:
    mc_state dimension1;
public:
// Return the length of the vector
    virtual mc_state getDimension1() {
        return dimension1;
    }

//#ifndef USE_64BIT
//    virtual double get(mc_uint64 i) = 0;
//#endif
    // Return the element i of the vector
    virtual double get(mc_state i) = 0;

    void print() {
        for (int i = 0; i < getDimension1(); i++) {
            cout << get(i) << " ";
            cout << endl;
        }
    }
};
#endif	/* MCABSTRACTVECTOR_H */

