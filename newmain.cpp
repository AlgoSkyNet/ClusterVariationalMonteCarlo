/* 
 * File:   newmain.cpp
 * Author: chris
 *
 * Created on August 4, 2011, 8:49 AM
 */

#include <cstdlib>
#include <omp.h>
#include <sstream>
#include "mc.h"
#include "mcConfiguration.h"
#include "mcHamiltonianConfigurable.h"
#include "mcEigenSystemSolverDirect.h"
#include "mcEigenSystemSolverDirectBasis.h"
#include "mcEigenSystemSolverPowerBasis.h"
#include "mcEigenSystemSolverPowerBasisLL.h"
#include "mcTrialWaveFunctionConfigurable.h"
#include "mcAbstractVector.h"
#include "mcSimulation.h"
#include "mcMeasurementEnergy.h"
#include "mcMeasurementEnergy2.h"
#include "mcMeasurementDummy.h"
#include "mcMeasurementDoubleOccupation.h"
#include "mcMeasurementSpin2.h"
#include "mcTests.h"
#include "mcMeasurementTotalOccupation.h"
#include "mcMeasurementStaggeredMagnetization1D.h"
#include "mcDataSeriesAnalyzer.h"
#include "mcMeasurementOccupation.h"
#include "mcMeasurementWeight.h"
#include "mcMeasurementPerturbation.h"


using namespace std;

//#define _OMEGASIGN_
//#define _MEASUREMENTSIGN_

/*
 * 
 */
//int serialmain(int argc, char** argv) {
//    //    char* filename = "/home/chris/NetBeansProjects/ClusterVariationalMonteCarlo/4x1OBC.conf";
//    char* filename = argv[1];
//    mcConfigurationCluster* config = mcConfigurationCluster::readFromFile(filename);
//    mcAbstractClusterHamiltonian* ham = new mcHamiltonianConfigurable(config);
//    //   mcAbstractEigenSystemSolver* ess = new mcEigenSystemSolverDirect(config, ham);
//    //mcAbstractEigenSystemSolver* ess = new mcEigenSystemSolverDirectBasis(config, ham);
//    mcAbstractEigenSystemSolver* ess = new mcEigenSystemSolverPowerBasis(config, ham);
//    mcAbstractMatrix* gsv = ess->getGroundState();
//    mcTrialWaveFunctionConfigurable* twf = new mcTrialWaveFunctionConfigurable(config, gsv);
//
//
//    mcAbstractMeasurement* mce = new mcMeasurementEnergy(config, twf);
//    mcAbstractMeasurement* mdo = new mcMeasurementDoubleOccupation(config, twf);
//    mcAbstractMeasurement* ms2 = new mcMeasurementSpin2(config, twf);
//    mce->setNext(mdo);
//    mdo->setNext(ms2);
//    mcSimulation* sim = new mcSimulation(config, twf, mce);
//
//
//    ofstream outfile("MonteCarlo.out");
//    for (double g = 0; g < 1; g += 0.05) {
//        cout << "gamma=" << g << endl;
//        twf->setGamma(g);
//        sim->run();
//        //        outfile << g<<" "
//        //                <<mce->getMean()
//        //                <<" "<<mce->getMean() <<endl;
//        outfile << g << " " << mce->getMean() << " " << mdo->getMean() << " " << ms2->getMean() << endl;
//
//        mce->resetMeasurement();
//    }
//    outfile.close();
//    //    for(stateType i = 0; i<twf->getDimension1();i++){
//    //        cout<<i<<" "<<twf->get(i,0)<<endl;
//    //    }
//
//    return 0;
//}

int testmain(int argc, char** argv) {
    if (argc == 2) {
        mcConfiguration* config = mcConfiguration::readFromFile(argv[1]);
        mcTests::overlapVariationalAndKronecker(config);
    } else if (argc == 3) {
        mcConfiguration* config1 = mcConfiguration::readFromFile(argv[1]);
        mcConfiguration* config2 = mcConfiguration::readFromFile(argv[2]);
        mcTests::overlapActualAndKronecker(config1, config2);
    } else {
        cout << "nothing to do" << endl;
    }
}

int threadedmain(int argc, char** argv) {
    string* filename = new string(argv[1]);
    filename->append(".out");
    int nThreads;


//    mc_state ts = 0;
//    mcMisc::printBinaryString(ts, 128);
//    ts = mc_state_set_bit(ts, 54);
//    mcMisc::printBinaryString(ts, 128);
//    ts = mc_state_unset_bit(ts, 2);
//    mcMisc::printBinaryString(ts, 128);
//    ts = 0;
//    for (int i = 0; i < 120; i++) {
//        ts = mc_state_set_bit(ts, i);
//        mcMisc::printBinaryString(ts, 120);
//        ts = mc_state_unset_bit(ts, i);
//    }
//    exit(0);

    //    int tns=12;
    //    int tnc=3;
    //    int tn=tns*tnc;
    //    mc_state ts=(ONE<<(tn))-ONE;
    //    mc_state tcsu=mcMisc::getSystemState_Up(ts,tns,tnc);
    //    mc_state tcsd=mcMisc::getSystemState_Down(ts,tns,tnc);
    //    mc_state tssu=mcMisc::getSpinState_Up(tcsu,tns,tnc);
    //    mc_state tssd=mcMisc::getSpinState_Down(tcsd,tns,tnc);
    //    mcMisc::printBinaryString(ts,2*tn);
    //    mcMisc::printBinaryString(tcsd,2*tn);
    //    mcMisc::printBinaryString(tcsu,2*tn);
    //    mcMisc::printBinaryString(tssd,2*tn);
    //    mcMisc::printBinaryString(tssu,2*tn);
    //    exit(0);
#pragma omp parallel
    {
        nThreads = omp_get_num_threads();
    }
    mcConfiguration* config = mcConfiguration::readFromFile(argv[1]);


    //    mcTests::overlapVariationalAndKronecker(config);
    //    exit(0);


    config->print();
    mcAbstractClusterHamiltonian* ham = new mcHamiltonianConfigurable(config);
    mcEigenSystemSolverPowerBasisLL* esspb = new mcEigenSystemSolverPowerBasisLL(config, ham);
    esspb->setConvergenceCriterion(1e-7);
    mcAbstractEigenSystemSolver* ess = esspb;
    //    mcAbstractEigenSystemSolver* ess = new mcEigenSystemSolverDirectBasis(config, ham);
    //    mcAbstractEigenSystemSolver* ess = new mcEigenSystemSolverDirect(config, ham);
    mcAbstractVector* gsv = ess->getGroundState();
    mcTrialWaveFunctionConfigurable* twf = new mcTrialWaveFunctionConfigurable(config, gsv);
    mc_state ZERO = 0;
    twf->setGamma(0);
    //    ofstream outwffile("waveFunction-full.dat");
    //    for (stateType i = 0; i < twf->getDimension1(); i++) {
    //        outwffile << i << " " << i.get_str(2) << " " << twf->get(i, ZERO) << endl;
    //    }
    //    outwffile.close();

    //    mcAbstractMeasurement** mce = new mcMeasurementEnergy*[nThreads];
    //    mcAbstractMeasurement** mdo = new mcMeasurementDoubleOccupation*[nThreads];
    //    mcAbstractMeasurement** ms2 = new mcMeasurementSpin2*[nThreads];
    mcAbstractMeasurement** mce = new mcAbstractMeasurement*[nThreads];
    mcAbstractMeasurement** mcN = new mcAbstractMeasurement*[nThreads];
    mcAbstractMeasurement** mdo = new mcAbstractMeasurement*[nThreads];
    mcAbstractMeasurement** ms2 = new mcAbstractMeasurement*[nThreads];
    mcAbstractMeasurement** msm = new mcAbstractMeasurement*[nThreads];
    mcAbstractMeasurement** mw = new mcAbstractMeasurement*[nThreads];
    mcAbstractMeasurement** mp = new mcAbstractMeasurement*[nThreads];
    mcAbstractMeasurement*** mocc = new mcAbstractMeasurement**[nThreads];
    for (int i = 0; i < nThreads; i++) {
    }
    mcDataSeriesAnalyzer* amce = new mcDataSeriesAnalyzer(nThreads, config->nMeasurements);

    mcSimulation** sim = new mcSimulation*[nThreads];
#pragma omp parallel for
    for (int i = 0; i < nThreads; i++) {
        mce[i] = new mcMeasurementEnergy(config, twf);
        mcN[i] = new mcMeasurementTotalOccupation(config, twf);
        mdo[i] = new mcMeasurementDoubleOccupation(config, twf);
        ms2[i] = new mcMeasurementSpin2(config, twf);
        mw[i] = new mcMeasurementWeight(config, twf);
        mp[i] = new mcMeasurementPerturbation(config, twf);
        msm[i] = new mcMeasurementStaggeredMagnetization1D(config, twf);
        mce[i]->setNext(mcN[i]);
        mcN[i]->setNext(mdo[i]);
        mdo[i]->setNext(ms2[i]);
        ms2[i]->setNext(mp[i]);
        mp[i]->setNext(mw[i]);
        mw[i]->setNext(msm[i]);
        mcAbstractMeasurement* prev = msm[i];
        mocc[i] = new mcAbstractMeasurement*[config->nTotalSites * 2];
        for (int s = 0; s < 2; s++)
            for (int n = 0; n < config->nTotalSites; n++) {
                mocc[i][config->nTotalSites * s + n] = new mcMeasurementOccupation(config, twf, n, s);
                prev->setNext(mocc[i][config->nTotalSites * s + n]);
                prev = mocc[i][config->nTotalSites * s + n];
            }
        sim[i] = new mcSimulation(config, twf, mce[i]);
        amce->setMeasurement(i, mce[i]);
    }


    ofstream outfile(filename->data());
    cout << "writing to file " << filename->data() << endl;
    double emean = 0;
    double nmean = 0;
    double domean = 0;
    double s2mean = 0;
    double staggmag = 0;
    double epert = 0;
    outfile << "#ClusterEigenvalue " << ess->getGroundStateEnergy() << endl;
    outfile << "# g" << " "
            << "emean" << " "
            << "emean / nmean" << " "
            << "nmean" << " "
            << "domean / config->nTotalSites" << " "
            << "s2mean / config->nTotalSites" << " "
            << "staggmag / config->nTotalSites" << " "
            << "epert / config->nTotalSites" << " "
            << "ess->getGroundStateEnergy() * config->nClusters" << " "
            << "ess->getGroundStateEnergy() * config->nClusters / nmean" << " ";
    outfile << endl;
    for (double g = config->gammaStart; g < config->gammaStop; g += (config->gammaStop - config->gammaStart) / config->gammaSteps) {
        cout << "gamma=" << g << endl;
        twf->setGamma(g);
        emean = 0;
        nmean = 0;
        domean = 0;
        s2mean = 0;
        staggmag = 0;
        epert = 0;
#pragma omp parallel for reduction(+:emean,nmean,domean,s2mean,staggmag,epert)
        for (int i = 0; i < nThreads; i++) {
            mce[i]->resetMeasurement();
            sim[i]->run();
            emean = mce[i]->getMean();
            nmean = mcN[i]->getMean();
            domean = mdo[i]->getMean();
            s2mean = ms2[i]->getMean();
            staggmag = msm[i]->getMean();
            epert = mp[i]->getMean();
            cout << "Energy(" << omp_get_thread_num() << ") = " << emean << endl;
        }

        emean = emean / nThreads;
        nmean = nmean / nThreads;
        domean = domean / nThreads;
        s2mean = s2mean / nThreads;
        staggmag = staggmag / nThreads;
        epert = epert / nThreads;
        cout << emean / nmean;
        for (int i = 10; i < nThreads * config->nMeasurements; i = i * 2)
            cout << " +- " << amce->getBinError(i) / nmean << "(" << i << ")";
        cout << endl;

        outfile << g << " "
                << emean << " "
                << emean / nmean << " "
                << nmean << " "
                << domean / config->nTotalSites << " "
                << s2mean / config->nTotalSites << " "
                << staggmag / config->nTotalSites << " "
                << epert << " "
                << ess->getGroundStateEnergy() * config->nClusters << " "
                << ess->getGroundStateEnergy() * config->nClusters / nmean << " ";
        outfile << endl;
    }
    outfile.close();

    // occupation
    //    string* fnamenocc = new string(argv[1]);
    //    fnamenocc->append(".nocc");
    //    ofstream noccout(fnamenocc->data());
    //    for (int i = 0; i < config->nTotalSites; i++) {
    //        noccout << i << " ";
    //        for (int s = 0; s < 2; s++) {
    //            double sum = 0;
    //            for (int n = 0; n < nThreads; n++) {
    //                sum += mocc[n][i + config->nTotalSites * s]->getMean();
    //            }
    //            sum = sum / nThreads;
    //            noccout << sum << " ";
    //        }
    //        noccout << endl;
    //    }

    //    for (int i = 0; i < nThreads; i++) {
    //        mw[i]->writeToFile(argv[1],i);
    //    }
    //
    //    for (int i = 0; i < nThreads; i++) {
    //        mce[i]->writeToFile(argv[1],i);
    //    }

    return 0;
}

int main(int argc, char** argv) {
    //testmain(argc, argv);
    threadedmain(argc, argv);
}


