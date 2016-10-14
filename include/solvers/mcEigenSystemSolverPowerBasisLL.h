/* 
 * File:   mcEigenSystemSolverDirect.h
 * Author: chris
 *
 * Created on August 3, 2011, 4:50 PM
 */
#include<iostream>
#include<math.h>
#include "../mc.h"
#include "mcAbstractEigenSystemSolver.h"
#include "mcEigenSystemSolverDirectBasis.h"
#include "../vectors/mcVector.h"
#include "../mcMisc.h"
#include <gmpxx.h>

#ifndef MCEIGENSYSTEMSOLVERPOWERBASISLL_H
#define	MCEIGENSYSTEMSOLVERPOWERBASISLL_H

using namespace std;

class mcEigenSystemSolverPowerBasisLL : public mcEigenSystemSolverDirectBasis {
private:

    class LLElement {
    public:

        LLElement() {
            next = NULL;
        }

        LLElement(double val, mc_state ind) {
            value = val;
            index = ind;
            next = NULL;
        }
        LLElement* next;
        double value;
        mc_state index;
    };

    double convergenceCriterion;
    LLElement** data2;
public:
    //    mcEigenSystemSolverPowerBasisLL() {
    //    };
    //

    mcEigenSystemSolverPowerBasisLL(mcConfiguration* conf, mcAbstractClusterHamiltonian* ham) {
        this->theConfiguration = conf;
        this->theHamiltonian = ham;
        this->nStatesUp = mc_state_get_uli((mcMisc::binCoeff(theConfiguration->nClusterSites, theConfiguration->nElectronsUp)));
        this->nStatesDown = mc_state_get_uli(mcMisc::binCoeff(theConfiguration->nClusterSites, theConfiguration->nElectronsUp));
        this->matrixSize = nStatesUp*nStatesDown;
        cout << "nStatesUp = " << nStatesUp << endl;
        cout << "nStatesDown = " << nStatesDown << endl;
        cout << "matrixSize = " << matrixSize << endl;
        convergenceCriterion = 0.0000000001;
    }

    void setupMatrix() {
        setupStates();
        data2 = new LLElement*[matrixSize];
        int nsites = theConfiguration->nClusterSites;
        long maskdown = (1 << nsites) - 1;
        long maskup = maskdown << nsites;
#pragma omp parallel for
        for (long i = 0; i < nStatesUp; i++) {
            for (long j = 0; j < nStatesDown; j++) {
                long m = i * nStatesDown + j;
                LLElement* telem = NULL;
                for (long k = 0; k < nStatesUp; k++) {
                    for (long l = 0; l < nStatesDown; l++) {
                        long n = k * nStatesDown + l;
                        double tmp = theHamiltonian->matrixElement(statesUp[i], statesDown[j], statesUp[k], statesDown[l]);
                        if (tmp != 0.0) {
                            if (telem == NULL) {
                                telem = new LLElement(tmp, n);
                                data2[m] = telem;

                            } else {
                                telem->next = new LLElement(tmp, n);
                                telem = telem->next;
                            }
                        }
                    }
                }
            }
        }
    }

    void crackMatrix() {
        unsigned int msize = (unsigned int) matrixSize;
        double* vec1 = new double[matrixSize];
        double* vec2 = new double[matrixSize];
        double* vectmp;
        double sum = 0;
        double oldsum = 12342154;
        double powerConstant = 1;
        if (matrixSize == 1) {
            cout << "matrix size = 1! exiting!" << endl;
            exit(0);
        }
#pragma omp parallel for
        for (int i = 0; i < matrixSize; i++) {
            vec1[i] = 1.0 / sqrt(matrixSize);
        }
        for (;;) {
#pragma omp parallel for
            for (int i = 0; i < matrixSize; i++) {
                vec2[i] = 0;
                LLElement* telem = data2[i];
                for(;;){
                    if(telem==NULL){
                        break;
                    }
                    vec2[i] -= telem->value * vec1[telem->index];
                    telem=telem->next;
                }
//                for (int j = 0; j < matrixSize; j++) {
//                    vec2[i] -= data2[i]->value * vec1[data2[i]->index];
//                }
                vec2[i] += powerConstant * vec1[i];
            }
            sum = 0;
#pragma omp parallel for reduction(+:sum)
            for (int i = 0; i < matrixSize; i++) {
                sum += vec2[i] * vec2[i];
            }
#pragma omp parallel for
            for (int i = 0; i < matrixSize; i++) {
                vec2[i] = vec2[i] / sqrt(sum);
            }
            vectmp = vec1;
            vec1 = vec2;
            vec2 = vectmp;
            sum = 0;
#pragma omp parallel for reduction(+:sum)
            for (int i = 0; i < matrixSize; i++) {

                vec2[i] = 0;
                LLElement* telem = data2[i];

                for (;;) {
                    if (telem == NULL) {
                        break;
                    }
                    vec2[i] += telem->value * vec1[telem->index];
                    telem = telem->next;
                }
                //                for (int j = 0; j < matrixSize; j++) {
                //                    vec2[i] += data[i * matrixSize + j] * vec1[j];
                //                }

                sum += vec1[i] * vec2[i];
            }
            cout << "\r";
            //            cout << "Power converegence = " << fabs(sum - oldsum) << endl;
            cout << "Power converegence = " << fabs(sum - oldsum) << flush;
            if (fabs(sum - oldsum) < convergenceCriterion) {
                break;
            }
            oldsum = sum;
        }
        cout << endl;
        cout << "Eigenvalue = " << sum << endl;
        theGroundStateEnergy = sum;
        int hsize = 1 << (2 * theConfiguration->nClusterSites);
        //        hsize = hsize*hsize;
        mcVector* retvec = new mcVector(hsize);
        mc_state ZERO = 0;
        for (mc_state i = 0; i < hsize; i++) {
            retvec->set(i, 0.0);
        }
        int nsites = theConfiguration->nClusterSites;
        double norm = 0;
        ofstream outwffile("clusterWaveFunction.dat");
        for (long i = 0; i < nStatesUp; i++) {
            for (long j = 0; j < nStatesDown; j++) {
                long n = i * nStatesDown + j;
                mc_state u = statesUp[i];
                mc_state d = statesDown[j];
                mc_state t = (u << nsites) + d;
                retvec->set(t, vec1[n]);
                //                outwffile << n<<" "<<t <<" "<<mpz_class(t).get_str(2)<<" "<<mpz_class(u).get_str(2)<<" "<<mpz_class(d).get_str(2) << " " << vec1[n] << endl;
#ifndef USE_128BIT
                outwffile << n << " " << t << " " << vec1[n] << endl;
#endif
                norm += vec1[n ] * vec1[n ];
            }
        }
        outwffile.close();
        cout << "norm of vector = " << norm << endl;

        theGroundStateVector = retvec;

        delete[] vec1;
        delete[] vec2;

    }

    void setConvergenceCriterion(double convergenceCriterion) {
        this->convergenceCriterion = convergenceCriterion;
    }

    double getConvergenceCriterion() const {
        return convergenceCriterion;
    }

};


#endif	/* MCEIGENSYSTEMSOLVERPOWERBASISLL_H */

