/* 
 * File:   mcEigenSystemSolverDirect.h
 * Author: chris
 *
 * Created on August 3, 2011, 4:50 PM
 */
#include<iostream>
#include "mcAbstractEigenSystemSolver.h"
#include "mcVector.h"
#include "mc.h"

#ifndef MCEIGENSYSTEMSOLVERDIRECT_H
#define	MCEIGENSYSTEMSOLVERDIRECT_H
extern "C" void dsyev_(char* jobz, char* uplo, int* n, double* a, int* lda, double* w, double* work, int* lwork, int* info);

using namespace std;

class mcEigenSystemSolverDirect : public mcAbstractEigenSystemSolver {
private:
    double* data;
    long matrixSize;
    long nStatesUp;
    long nStatesDown;
    mcAbstractClusterHamiltonian* theHamiltonian;
    mcConfiguration* theConfiguration;
public:

    mcEigenSystemSolverDirect(mcConfiguration* conf, mcAbstractClusterHamiltonian* ham) {
        theConfiguration = conf;
        theHamiltonian = ham;
        nStatesUp = 1 << theConfiguration->nClusterSites;
        nStatesDown = 1 << theConfiguration->nClusterSites;
        matrixSize = nStatesUp*nStatesDown;
        cout << "nStatesUp = " << nStatesUp << endl;
        cout << "nStatesDown = " << nStatesDown << endl;
        cout << "matrixSize = " << matrixSize << endl;
    }

    void setupMatrix() {
        data = new double[matrixSize * matrixSize];
        int nsites = theConfiguration->nClusterSites;
        long maskdown = (1 << nsites) - 1;
        long maskup = maskdown << nsites;
        for (long i = 0; i < matrixSize; i++) {
            for (long j = 0; j < matrixSize; j++) {
                data[i * matrixSize + j] = theHamiltonian->matrixElement((i & maskup) >> nsites, i&maskdown, (j & maskup) >> nsites, j & maskdown);
            }
        }
    }

    //void crackMatrix(); 


    //void mcEigenSystemSolverDirect::crackMatrix() {

    void crackMatrix() {
        int info, lwork;
        double wkopt;
        double* work;
        int msize = (unsigned int) matrixSize;
        /* Local arrays */
        double* w = new double[matrixSize];
        lwork = -1;
        dsyev_("Vectors", "Upper", &msize, data, &msize, w, &wkopt, &lwork, &info);
        lwork = (int) wkopt;
        work = (double*) malloc(lwork * sizeof (double));
        /* Solve eigenproblem */
        dsyev_("Vectors", "Upper", &msize, data, &msize, w, work, &lwork, &info);
        //        print_matrix("Eigenvalues", 1, n, w, 1);
        //        /* Print eigenvectors */
        //        print_matrix("Eigenvectors (stored columnwise)", n, n, a, lda);
        //        /* Free workspace */
        cout << "Eigenvalue=" << w[0] << endl;
        theGroundStateEnergy = w[0];
        int hsize = 1 << theConfiguration->nClusterSites;
        hsize = hsize*hsize;
        mcVector* retvec = new mcVector(hsize);
        for (unsigned long long int i = 0; i < hsize; i++) {
            retvec->set(i, 0);
        }
        int nsites = theConfiguration->nClusterSites;
        mc_state ZERO = 0;
        double norm = 0;
        for (unsigned long long int i = 0; i < msize; i++) {
            retvec->set(i,  data[i]);
//            cout << i << " " << data[i] << endl;
            norm += data[i] * data[i ];
        }
        cout << "norm of vector = " << norm << endl;
        theGroundStateVector = retvec;


        free((void*) work);
    }

    mcAbstractVector* getGroundState() {
        setupMatrix();
        //        for (int i = 0; i < matrixSize; i++) {
        //            for (int j = 0; j < matrixSize; j++) {
        //                cout << data[i * matrixSize + j] << " ";
        //            }
        //            cout << endl;
        //        }
        crackMatrix();
        cleanUp();
        return theGroundStateVector;

    }

    void cleanUp() {
        delete[] data;
    }
};


#endif	/* MCEIGENSYSTEMSOLVERDIRECT_H */

