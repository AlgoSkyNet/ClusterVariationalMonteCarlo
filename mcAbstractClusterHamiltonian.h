/* 
 * File:   mcAbstractHamiltonian.h
 * Author: chris
 *
 * Created on August 3, 2011, 6:36 PM
 */

#ifndef MCABSTRACTCLUSTERHAMILTONIAN_H
#define	MCABSTRACTCLUSTERHAMILTONIAN_H
class mcAbstractClusterHamiltonian{
private:

public:
    virtual double matrixElementHopping(long left, long right){return 0;};
    virtual double matrixElementDiagonal(long up, long down){return 0;};
    virtual double matrixElement(long leftUp, long leftDown, long rightUp, long rightDown){return 0;};
};


#endif	/* MCABSTRACTCLUSTERHAMILTONIAN_H */

