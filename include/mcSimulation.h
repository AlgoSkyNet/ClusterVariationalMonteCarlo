/* 
 * File:   mcSimulation.h
 * Author: chris
 *
 * Created on August 4, 2011, 5:36 PM
 */
//#include <gmpxx.h>
//#include <gmp.h>
#include <stdlib.h>
#include <omp.h>
#include "mc.h"
#include "mcAbstractVector.h"
#include "mcConfiguration.h"
#include "mcAbstractMeasurement.h"
#include "mcMeasurementEnergy.h"

#ifndef MCSIMULATION_H
#define	MCSIMULATION_H

class mcSimulation {
private:
    long nWarmupSteps;
    long nSkipSteps;
    long nMeasurements;
    mcAbstractVector* theTrialWaveFunction;
    mcConfiguration* theConfiguration;

#ifdef USE_GMP
    gmp_randstate_t theRandState;
#endif

    mc_state upmask;
    mc_state downmask;
    mc_state ONE;
    mc_state ZERO;
    int nElectronsUp;
    int nElectronsDown;
    int nClusters;
    int nClustersSites;
    int nTotalSites;
    double acceptanceRate;
    mcAbstractMeasurement* firstMeasurement;
    int nThreadNum;
    //#define PROPOSALMETHOD1

#ifdef PROPOSALMETHOD1

    mc_state getNewState(mc_state s) {
        mc_state upstate = 0;
        mc_state downstate = 0;
        mc_state retState;
        int s1, s2;
        for (;;) {
            for (;;) {
                s1 = rand() % (2 * theConfiguration->nTotalSites);
                s2 = rand() % (2 * theConfiguration->nTotalSites);
                mc_state mask1 = (ONE << s1);
                mc_state mask2 = (ONE << s2);
                mc_state tmp1 = ((s & mask1) >> s1);
                mc_state tmp2 = ((s & mask2) >> s2);
                //                if ((tmp1.get_ui() == 1) && (tmp2.get_ui() == 0)) {
                if ((tmp1 == 1) && (tmp2 == 0)) {
                    retState = s - mask1 + mask2;
                    break;
                }
            }
            upstate = retState&upmask;
            downstate = retState&downmask;
            int upcount = mc_state_popcount(upstate);
            int downcount = mc_state_popcount(downstate);
            if ((upcount == nElectronsUp) && (downcount == nElectronsDown)) {
                break;
            }
        }
        return retState;
    }

    mc_state getNewState() {
        mc_state upstate = 0;
        mc_state downstate = 0;
        mc_state retState;
        for (;;) {
#ifdef USE_GMP
            mpz_urandomb(retState.get_mpz_t(), theRandState, 2 * nTotalSites);
#else
            mc_state modval = (ONE << ((mc_state) (2 * nTotalSites))) - ONE;
            //            mc_state modval = mc_state_set_all_bits(2 * nTotalSites);
            retState = mc_state_rand() % modval;
#endif
            upstate = retState&upmask;
            downstate = retState&downmask;
            int upcount = mc_state_popcount(upstate);
            int downcount = mc_state_popcount(downstate);
            if ((upcount == nElectronsUp) && (downcount == nElectronsDown)) {
                break;
            }
        }
        return retState;
    }
#else

    mc_state put_ran_bit(mc_state s, int maxpos, int maxbits) {
        //        printf("%llu\n", (long long unsigned int) s);
        //        mc_state_print_binary_string(s, maxbits);
        if (maxbits == 0) return s;
        for (;;) {
            //pick random position
            int p = rand() % maxpos;
            // check if the option is occupied
            int c = ((s & (ONE << p)) >> p);
            if (c == 0) {
                s += (ONE << p);
                return put_ran_bit(s, maxpos, maxbits - 1);
            }
        }
    }

    mc_state getNewState() {
        mc_state upstate = ZERO;
        mc_state downstate = ZERO;
        mc_state retState = ZERO;
        upstate = put_ran_bit(0, nTotalSites, nElectronsUp);
        downstate = put_ran_bit(0, nTotalSites, nElectronsDown);
        return mcMisc::getSystemState_Down(downstate, nClustersSites, nClusters)
                + mcMisc::getSystemState_Up(upstate, nClustersSites, nClusters);
    }

    mc_state getNewState(mc_state s) {
        mc_state upstate = mcMisc::getSpinState_Up(s, nClustersSites, nClusters);
        mc_state downstate = mcMisc::getSpinState_Down(s, nClustersSites, nClusters);
        mc_state xstate = ZERO;
        mc_state retState = ZERO;

//        cout << "--------------------------------"<<endl;
//        mcMisc::printBinaryString(upstate, nTotalSites);
//        mcMisc::printBinaryString(downstate, nTotalSites);
//        cout << endl;

        // pick spin
        int ts = rand() % 2;
        if (ts == 0) {
            xstate = downstate;
        } else if (ts == 1) {
            xstate = upstate;
        } else {
            cout << "getNewState2(s): something went wrong" << endl;
        }
        // pick a spin and put it somewhere else
        for (;;) {
            int ts1 = rand() % nTotalSites;
            int ts2 = rand() % nTotalSites;
            if ((mc_state_get_bit(xstate, ts1) == 1) && (mc_state_get_bit(xstate, ts2) == 0)) {
                xstate = mc_state_unset_bit(xstate, ts1);
                xstate = mc_state_set_bit(xstate, ts2);
//                cout <<ts1<<" "<<ts2<<" "<<ts<<endl ;
                break;
            }
        }
        if (ts == 0) {
            downstate = xstate;
        } else if (ts == 1) {
            upstate = xstate;
        } else {
            cout << "getNewState2(s): something went wrong" << endl;
        }
//        mcMisc::printBinaryString(upstate, nTotalSites);
//        mcMisc::printBinaryString(downstate, nTotalSites);
//        cout << "--------------------------------"<<endl;
//        cout << endl;
        
        mc_state mctmp = mcMisc::getSystemState_Down(downstate, nClustersSites, nClusters)
                + mcMisc::getSystemState_Up(upstate, nClustersSites, nClusters);
        return mctmp;
    }
#endif
public:

    mcSimulation(mcConfiguration* conf, mcAbstractVector* twf) {
        theConfiguration = conf;
        theTrialWaveFunction = twf;
        init();
    }

    mcSimulation(mcConfiguration* conf, mcAbstractVector* twf, mcAbstractMeasurement* m) {
        theConfiguration = conf;
        theTrialWaveFunction = twf;
        firstMeasurement = m;
        init();
    }

    void initRNG() {
        nThreadNum = omp_get_thread_num();
#ifdef USE_GMP
        gmp_randinit_default(theRandState);
#endif
        srand(time(NULL) + 1000 * (nThreadNum + 1));
#ifdef USE_GMP
        mc_state rseed;
        rseed = rand();
        rseed = rand();
        gmp_randseed(theRandState, rseed.get_mpz_t());
#endif
    }

    void init() {
        initRNG();
        upmask = 0;
        downmask = 0;
        mc_state tmp;
        ONE = 1;
        ZERO = 0;
        nClusters = theConfiguration->nClusters;
        nClustersSites = theConfiguration->nClusterSites;
        tmp = (ONE << theConfiguration->nClusterSites) - ONE;
        for (int i = 0; i < theConfiguration->nClusters; i++) {
            downmask = downmask + (tmp << (2 * i * theConfiguration->nClusterSites));
            upmask = upmask + (tmp << ((2 * i + 1) * theConfiguration->nClusterSites));
        }
        nElectronsUp = theConfiguration->nElectronsUp * theConfiguration->nClusters;
        nElectronsDown = theConfiguration->nElectronsDown * theConfiguration->nClusters;
        nTotalSites = theConfiguration->nTotalSites;
        //        firstMeasurement = new mcMeasurementEnergy(theConfiguration, theTrialWaveFunction);
        nWarmupSteps = theConfiguration->nWarmupSteps;
        nMeasurements = theConfiguration->nMeasurements;
        nSkipSteps = theConfiguration->nSkipSteps;

    }

    void run() {
        long t1, t2;
        mc_state currentState = 0;
        mc_state trialState = 0;
        //        stateType ZERO = 0;
        //        stateType ONE = 1;
        long acceptedMoves = 0;


        double weight;
        double weight2;

        srand((unsigned) time(NULL));


        // find first state with weight
        for (;;) {
            currentState = getNewState();
            double weight = theTrialWaveFunction->get(currentState);
            double weight2 = weight*weight;
            //            cout << weight2 << endl;
            if (weight2 > 0) {
                break;
            }
        }
        t1 = time(NULL);
//        cout << "... warming up" << endl;
        for (int j = 0; j < nWarmupSteps; j++) {
            trialState = getNewState(currentState);
            //            trialState = getNewState2();

            double tweight = theTrialWaveFunction->get(trialState);
            double tweight2 = tweight*tweight;
            double r = drand48();

            if ((tweight2 / weight2 >= r)) {
                currentState = trialState;
                weight2 = tweight2;
            }
        }
        t2 = time(NULL);

//#pragma omp critical
//        {
//            cout << "... done warming up" << endl;
//            cout << "time for warmup = " << t1 << " " << t2 << " " << (double) (t2 - t1) << " ms" << endl;
//            cout << "time per trial  = " << (double) (t2 - t1) / nWarmupSteps << " ms" << endl;
//            double etime = (double) (t2 - t1) / nWarmupSteps * 4 * theConfiguration->nHoppings * theConfiguration->nPerturbations * theConfiguration->nMeasurements * theConfiguration->nSkipSteps;
//            cout << "estimated time for simulation  = " << etime << " ms = " << etime / 1000 << " s" << endl;
//        }
#pragma omp barrier

        int mcount = nMeasurements / 20;
        for (int i = 0; i < nMeasurements; i++) {
            for (int j = 0; j < nSkipSteps; j++) {
                trialState = getNewState(currentState);
                //                trialState = getNewState2();
                double tweight = theTrialWaveFunction->get(trialState);
                double tweight2 = tweight*tweight;
                double r = drand48();

                if ((tweight2 / weight2 >= r)) {
                    currentState = trialState;
                    weight2 = tweight2;
                    acceptedMoves++;
                }
            }
            firstMeasurement->measure(currentState);
#pragma omp master
            {
                if ((i % mcount) == 0) {
                    cout << ((double) (100*((double ) i)))/((double)nMeasurements)<<"%" << flush;
                }
            }
            //            accu += firstMeasurement->getLastEnergy();
        }
        cout << endl;
        cout << "acceptance rate (" << nThreadNum << ")= " << ((double) acceptedMoves / nMeasurements / nSkipSteps) << " (" << acceptedMoves << "/" << nMeasurements << "/" << nSkipSteps << ")" << endl;
        cout << "local energy value ("<< omp_get_thread_num()<<") ="<< firstMeasurement->getMean() << endl;
    }

    void setNThreadNum(int nThreadNum) {
        this->nThreadNum = nThreadNum;
    }

    int getNThreadNum() const {
        return nThreadNum;
    }
};


#endif	/* MCSIMULATION_H */

