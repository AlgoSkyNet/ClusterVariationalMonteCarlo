/* 
 * File:   mcMeasurementEnergy.h
 * Author: chris
 *
 * Created on August 5, 2011, 9:31 AM
 */
#include "../mc.h"
#include "../mcMisc.h"
#include"mcAbstractMeasurement.h"


#ifndef MCMEASUREMENTENERGY_H
#define	MCMEASUREMENTENERGY_H

#define _MEASUREMENTSIGN_

#include <iomanip>

class mcMeasurementEnergy : public mcAbstractMeasurement {
public:

    mcMeasurementEnergy() {
    }

    mcMeasurementEnergy(mcConfiguration* conf, mcAbstractVector* twf) {
        theConfiguration = conf;
        theWaveFunction = twf;
        ONE = 1;
        ZERO = 0;
        next = NULL;
        filenameExtension = new string("energy.out");
        setUp();
        this->init();
    }

    void measurement(mc_state s) {
        double ret = 0;
        double owf = theWaveFunction->get(s);
        for (int i = 0; i < nHoppings; i++) {
            //            int n = hopping[i][0];
            //            int m = hopping[i][1];
            int n = min(hopping[i][0], hopping[i][1]);
            int m = max(hopping[i][0], hopping[i][1]);
            mc_state mask = 0;
            mask += ((ONE << m) - ONE);
            mask -= ((ONE << (n + 1)) - ONE);
            mask = mask&s;
            int p = mc_state_popcount(mask);
            p = p & 1;
            p = p << 1;
            double sign = (1 - p);
            double hval = hoppingVal[i];

            mc_state omask = (ONE << n)+(ONE << m);

            mc_state ns = s^omask;

            double wfval = theWaveFunction->get(ns) / owf;

            ret += sign * hval * wfval;

            //            mask = 0;
            //            mask += ((ONE << n) - ONE);
            //            mask -= ((ONE << (m + 1)) - ONE);
            //            cout << "Hopping " << n << " " << m << endl;
            //            mcMisc::printBinaryString(s, 16);
            //            mcMisc::printBinaryString(ns, 16);
            //            mcMisc::printBinaryString(mask, 16);
            //            mcMisc::printBinaryString(omask, 16);
            //            cout << "\t value = " << (hval * wfval) << endl;
            //            cout << "\t sign = " << sign << endl;
            //            cout << "\t cumulative = " << ret << endl;
            //            cout << "\t wave functions = " << theWaveFunction->get(ns) << " " << owf << endl;

        }

        mc_state upstate = 0;
        mc_state downstate = 0;
        mc_state single = (ONE << theConfiguration->nClusterSites) - ONE;
        mc_state tmp = 0;
        for (int i = 0; i < theConfiguration->nClusters; i++) {
            int nd = (2 * i) * theConfiguration->nClusterSites;
            int nu = (2 * i + 1) * theConfiguration->nClusterSites;
            int nn = i * theConfiguration->nClusterSites;
            mc_state ssd = (((s & (single << nd)) >> nd) << nn);
            mc_state ssu = (((s & (single << nu)) >> nu) << nn);
            downstate += ssd;
            upstate += ssu;
        }


        mc_state docc = upstate&downstate;
        mc_state tbit = 0;
        //        cout<< "potential term"<<endl;
        //        cout<<setiosflags(ios::right);
        //        cout<<setiosflags(ios::fixed);
        //        cout <<s.get_str(2)<<endl;
        //        cout <<docc.get_str(2)<<endl;
        //        cout <<upstate.get_str(2)<<endl;
        //        cout <<downstate.get_str(2)<<endl;
        //        cout<<endl;

        for (int i = 0; i < theConfiguration->nTotalSites; i++) {
            tbit = ((docc & (ONE << i)) >> i);
            ret += coulombVals[i] * mc_state_get_d(tbit);
            tbit = ((upstate & (ONE << i)) >> i);
            ret += chemicalPotentialUp[i] * mc_state_get_d(tbit);
            tbit = ((downstate & (ONE << i)) >> i);
            ret += chemicalPotentialDown[i] * mc_state_get_d(tbit);
        }
        //                cout << "Total energy= " << ret << endl;
        //                cout << s.get_str(2) << endl;
        //                cout << upstate.get_str(2) << endl;
        //                cout << downstate.get_str(2) << endl;
        //                stateType tmp2 = mpz_popcount(s.get_mpz_t());
        //                cout << tmp2 << endl;
        //                cout << endl;
        //        cout << "--------------------------"<<endl;
        this->recordMeasurement(ret);
    }

protected:
private:
    mc_state ONE;
    mc_state ZERO;
    mc_state upmask;
    mc_state downmask;
    int nSites;
    int nHoppings;
    int** hopping;
    bool* interClusterHopping;
    int* hoppingSpin;
    double* hoppingVal;
    double* coulombVals;
    double* chemicalPotentialUp;
    double* chemicalPotentialDown;

    void setUp() {

        mc_state tmp = (ONE << theConfiguration->nClusterSites) - ONE;
        for (int i = 0; i < theConfiguration->nClusters; i++) {
            downmask = downmask + (tmp << (2 * i * theConfiguration->nClusterSites));
            upmask = upmask + (tmp << ((2 * i + 1) * theConfiguration->nClusterSites));
        }

        nHoppings = 0;
        nHoppings = 2 * theConfiguration->nHoppings * theConfiguration->nClusters + 2 * theConfiguration->nPerturbations;
        hopping = new int*[nHoppings];
        for (int i = 0; i < nHoppings; i++) {
            hopping[i] = new int[2];
        }
        hoppingVal = new double[nHoppings];
        interClusterHopping = new bool[nHoppings];
        hoppingSpin = new int[nHoppings];
        int hcount = 0;
        for (int i = 0; i < theConfiguration->nClusters; i++) {
            for (int j = 0; j < theConfiguration->nHoppings; j++) {
                for (int s = 0; s < 2; s++) {
                    int s1 = theConfiguration->hoppings[j][0];
                    int s2 = theConfiguration->hoppings[j][1];
                    hoppingVal[hcount] = theConfiguration->hoppingValues[j];
                    interClusterHopping[hcount] = false;
                    hoppingSpin[hcount] = s;
                    int n1 = s1 + (2 * i + s) * theConfiguration->nClusterSites;
                    int n2 = s2 + (2 * i + s) * theConfiguration->nClusterSites;
                    hopping[hcount][0] = n1;
                    hopping[hcount][1] = n2;
                    hcount++;
                }
            }
        }
        // uncomment this
        for (int j = 0; j < theConfiguration->nPerturbations; j++) {
            for (int s = 0; s < 2; s++) {
                int s1 = theConfiguration->perturbations[j][0];
                int s2 = theConfiguration->perturbations[j][1];
                int c1 = theConfiguration->perturbations[j][2];
                int c2 = theConfiguration->perturbations[j][3];

                hoppingVal[hcount] = theConfiguration->perturbationValues[j];
                interClusterHopping[hcount] = true;
                hoppingSpin[hcount] = s;
                int n1 = s1 + (2 * c1 + s) * theConfiguration->nClusterSites;
                int n2 = s2 + (2 * c2 + s) * theConfiguration->nClusterSites;
                hopping[hcount][0] = n1;
                hopping[hcount][1] = n2;
                hcount++;
            }
        }
#pragma omp master
        for (int i = 0; i < nHoppings; i++) {
            cout << "Composite hopping : " << hopping[i][0] << "\t" << hopping[i][1] << "\t" << hoppingVal[i] << "\t" << hoppingSpin[i] << "\t" << interClusterHopping[i] << endl;
        }
        coulombVals = new double[theConfiguration->nTotalSites];
        chemicalPotentialUp = new double[theConfiguration->nTotalSites];
        chemicalPotentialDown = new double[theConfiguration->nTotalSites];
        for (int i = 0; i < theConfiguration->nClusters; i++) {
            for (int j = 0; j < theConfiguration->nClusterSites; j++) {
                coulombVals[i * theConfiguration->nClusterSites + j] = theConfiguration->correlationValues[j];
                chemicalPotentialUp[i * theConfiguration->nClusterSites + j] = theConfiguration->chemicalPotentialUp[j];
                chemicalPotentialDown[i * theConfiguration->nClusterSites + j] = theConfiguration->chemicalPotentialDown[j];
            }
        }
#pragma omp master
        for (int i = 0; i < theConfiguration->nTotalSites; i++) {
            cout << "Composite local : " << coulombVals[i] << " " << chemicalPotentialUp[i] << " " << chemicalPotentialUp[i] << endl;
        }

    }
};
#endif	/* MCMEASUREMENTENERGY_H */

