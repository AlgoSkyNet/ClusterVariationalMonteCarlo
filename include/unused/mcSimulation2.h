/* 
 * File:   mcSimulation2.h
 * Author: chris
 *
 * Created on August 4, 2011, 5:36 PM
 */
//#include <gmpxx.h>
//#include <gmp.h>
#include <stdlib.h>
#include <omp.h>
#include "mc.h"
#include "mcAbstractMatrix.h"
#include "mcConfigurationCluster.h"
#include "mcAbstractMeasurement.h"
#include "mcMeasurementEnergy.h"

#ifndef MCSIMULATION2_H
#define	MCSIMULATION2_H

class mcSimulation2 {
private:
    long nWarmupSteps;
    long nSkipSteps;
    long nMeasurements;
    mcAbstractVector* theTrialWaveFunction;
    mcConfiguration* theConfiguration;
    int* currentStateUp;
    int* currentStateDown;
    int* trialStateUp;
    int* trialStateDown;

#ifdef USE_GMP
    gmp_randstate_t theRandState;
#endif

    mc_state upmask;
    mc_state downmask;
    mc_state ONE;
    mc_state ZERO;
    int nElectronsUp;
    int nElectronsDown;
    int nTotalSites;
    double acceptanceRate;
    mcAbstractMeasurement* firstMeasurement;
    int nThreadNum;

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
            //            cout<<"proposing "<<s1<<" "<<s2<<endl;
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
//            mc_state modval = (ONE<<((mc_state) (2 * nTotalSites)))-ONE;
            mc_state modval = mc_state_set_all_bits(2 * nTotalSites);
            retState=mc_state_rand()%modval ;            
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
public:

    mcSimulation2(mcConfiguration* conf, mcAbstractVector* twf) {
        theConfiguration = conf;
        theTrialWaveFunction = twf;
        init();
    }

    mcSimulation2(mcConfiguration* conf, mcAbstractVector* twf, mcAbstractMeasurement* m) {
        theConfiguration = conf;
        theTrialWaveFunction = twf;
        firstMeasurement = m;
        init();
    }

    void initRNG(){
        nThreadNum=omp_get_thread_num();
#ifdef USE_GMP
        gmp_randinit_default(theRandState);
#endif
        srand(time(NULL) + 1000 * (nThreadNum+1));
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
        long t1,t2;
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
//            cout << currentState << " " << weight2 << endl;
            if (weight2 > 0) {
                break;
            }
        }
        t1=time(NULL);
#pragma omp master
        cout<<"... warming up"<<endl;
        for (int j = 0; j < nWarmupSteps; j++) {
            trialState = getNewState(currentState);
            double tweight = theTrialWaveFunction->get(trialState);
            double tweight2 = tweight*tweight;
            double r = drand48();

            if ((tweight2 / weight2 >= r)) {
                currentState = trialState;
                weight2 = tweight2;
            }
        }
        t2=time(NULL);
#pragma omp master
        {
               cout<<"... done warming up"<<endl;
               cout<<"time for warmup = "<<(double) t2-t1<<" ms"<<endl;
               cout<<"time per trial  = "<<(double)(t2-t1)/nWarmupSteps<<" ms"<<endl;
               double etime = (double)(t2-t1)/nWarmupSteps*4*theConfiguration->nHoppings*theConfiguration->nPerturbations*theConfiguration->nMeasurements*theConfiguration->nSkipSteps; 
               cout<<"estimated time for simulation  = "<<etime <<" ms = "<< etime/1000 <<" s"<<endl;
        }

        int mcount = nMeasurements /20;
        for (int i = 0; i < nMeasurements; i++) {
            for (int j = 0; j < nSkipSteps; j++) {
                trialState = getNewState(currentState);
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
                    cout << "|" << flush;
                }
            }
            //            accu += firstMeasurement->getLastEnergy();
        }
        cout << endl;
        cout << "acceptance rate ("<<nThreadNum<<")= " << ((double) acceptedMoves / nMeasurements / nSkipSteps) <<" ("<<acceptedMoves<<"/"<<nMeasurements<< "/"<< nSkipSteps<<")"<<endl;

        //        cout << accu / ((double) nMeasurements) << endl;
    }

    void setNThreadNum(int nThreadNum) {
        this->nThreadNum = nThreadNum;
    }

    int getNThreadNum() const {
        return nThreadNum;
    }
};


#endif	/* MCSIMULATION2_H */

