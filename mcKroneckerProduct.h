/* 
 * File:   mcKroneckerProduct.h
 * Author: chris
 *
 * Created on August 3, 2011, 1:27 PM
 */

//#include <gmpxx.h>
#include"mcAbstractVector.h"
#include "mc.h"

#ifndef MCKRONECKERPRODUCT_H
#define	MCKRONECKERPRODUCT_H
class mcKroneckerProduct : public mcAbstractVector{
private:
    mcAbstractVector* A;
    mcAbstractVector* B;
public: 
    mcKroneckerProduct(){}
    mcKroneckerProduct(mcAbstractVector* AA, mcAbstractVector* BB){
        A=AA;
        B=BB;
        dimension1=A->getDimension1()*B->getDimension1();
    } 
    double get(mc_state m){
        mc_state ma =m/B->getDimension1(); 
        mc_state mb =m%B->getDimension1();
        return A->get(ma)*B->get(mb);
    }            
};

#endif	/* MCKRONECKERPRODUCT_H */

  