/* 
 * File:   mcEigenSystemSolverDirect.h
 * Author: chris
 *
 * Created on August 3, 2011, 4:50 PM
 */
#include<iostream>
#include "../mc.h"
#include "mcAbstractEigenSystemSolver.h"
#include "../vectors/mcVector.h"
#include "../mcMisc.h"

#ifndef MCEIGENSYSTEMSOLVERDIRECTBASIS_H
#define	MCEIGENSYSTEMSOLVERDIRECTBASIS_H
extern "C" void dsyev_(char* jobz, char* uplo, int* n, double* a, int* lda, double* w, double* work, int* lwork, int* info);

using namespace std;

class mcEigenSystemSolverDirectBasis : public mcAbstractEigenSystemSolver {
protected:
    double* data;
    long nStatesUp;
    long nStatesDown;
    long* statesUp;
    long* statesDown;
    long matrixSize;

    mcAbstractClusterHamiltonian* theHamiltonian;
    mcConfiguration* theConfiguration;
public:

    mcEigenSystemSolverDirectBasis() {
    }

    mcEigenSystemSolverDirectBasis(mcConfiguration* conf, mcAbstractClusterHamiltonian* ham) {
        theConfiguration = conf;
        theHamiltonian = ham;
        nStatesUp = mc_state_get_uli(mcMisc::binCoeff(theConfiguration->nClusterSites, theConfiguration->nElectronsUp));
        nStatesDown = mc_state_get_uli(mcMisc::binCoeff(theConfiguration->nClusterSites, theConfiguration->nElectronsUp));
        matrixSize = nStatesUp*nStatesDown;
        cout << "nStatesUp = " << nStatesUp << endl;
        cout << "nStatesDown = " << nStatesDown << endl;
        cout << "matrixSize = " << matrixSize << endl;
    }

    void setupStates() {
        statesUp = new long[nStatesUp];
        statesDown = new long[nStatesDown];
        int nup = theConfiguration->nElectronsUp;
        int ndown = theConfiguration->nElectronsDown;
        int nsites = theConfiguration->nClusterSites;
        int cup = 0;
        int cdown = 0;
        for (unsigned int i = 0; i < (1 << nsites); i++) {
            if (__builtin_popcountll(i) == nup) {
                statesUp[cup] = i;
                cup++;
            }
            if (__builtin_popcountll(i) == ndown) {
                statesDown[cdown] = i;
                cdown++;
            }
        }
    }

    virtual void setupMatrix() {
        setupStates();
        data = new double[matrixSize * matrixSize];
        int nsites = theConfiguration->nClusterSites;
        long maskdown = (1 << nsites) - 1;
        long maskup = maskdown << nsites;
#pragma omp parallel for
        for (long i = 0; i < nStatesUp; i++) {
            for (long j = 0; j < nStatesDown; j++) {
                long m = i * nStatesDown + j;
                for (long k = 0; k < nStatesUp; k++) {
                    for (long l = 0; l < nStatesDown; l++) {
                        long n = k * nStatesDown + l;
                        data[m * matrixSize + n] = theHamiltonian->matrixElement(statesUp[i], statesDown[j], statesUp[k], statesDown[l]);
                    }
                }
            }
        }
    }

    //void crackMatrix(); 


    //void mcEigenSystemSolverDirect::crackMatrix() {

    virtual void crackMatrix() {
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
        cout << "Eigenvalue=" << w[0] << endl;
        free((void*) work);
        cout << "... copying ground state vector" << endl;
        theGroundStateEnergy = w[0];
        int hsize = 1 << (2 * theConfiguration->nClusterSites);
        //        hsize = hsize*hsize;
        mcVector* retvec = new mcVector(hsize);
        for (unsigned long long int i = 0; i < hsize; i++) {
            retvec->set(i, 0);
        }
        int nsites = theConfiguration->nClusterSites;
        mc_state ZERO = 0;
        double norm = 0;
        ofstream outwffile("clusterWaveFunction.dat");
        for (long i = 0; i < nStatesUp; i++) {
            for (long j = 0; j < nStatesDown; j++) {
                long n = i * nStatesDown + j;
                mc_state u = statesUp[i];
                mc_state d = statesDown[j];
                mc_state t = (u << nsites) + d;
                //                retvec->set(t, ONE, data[n*matrixSize]);
                //                retvec->set(t, ZERO, data[n * msize]);
                //                cout << n << " " << data[n * msize] << endl;
                //                norm += data[n * msize] * data[n * msize];
                //                outwffile << n<<" "<<t <<" "<<mpz_class(t).get_str(2)<<" "<<mpz_class(u).get_str(2)<<" "<<mpz_class(d).get_str(2) << " " << data[n] << endl;
#ifndef USE_128BIT
                outwffile << n << " " << t << " " << data[n] << endl;
#endif
                retvec->set(t, data[n]);
                //                cout << n << " " << data[n] << endl;
                norm += data[n] * data[n];
            }
        }
        outwffile.close();
        cout << "norm of vector = " << norm << endl;
        theGroundStateVector = retvec;
    }

    mcAbstractVector* getGroundState() {
        cout << "... setting up matrix" << endl;
        setupMatrix();

        //        for (int i = 0; i < matrixSize; i++) {
        //            for (int j = 0; j < matrixSize; j++) {
        //                cout << data[i * matrixSize + j] << " ";
        //            }
        //            cout << endl;
        //        }
        cout << "... diagonalizing matrix" << endl;
        crackMatrix();
        cout << "Ground state energy = " << theGroundStateEnergy << endl;
        cout << "... cleaning up" << endl;
        cleanUp();
        return theGroundStateVector;

    }

    void cleanUp() {
        delete[] data;
        delete[] statesUp;
        delete[] statesDown;
    }
};


#endif	/* MCEIGENSYSTEMSOLVERDIRECTBASIS_H */

