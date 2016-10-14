/* 
 * File:   mcTests.h
 * Author: chris
 *
 * Created on August 21, 2011, 8:45 AM
 */

#ifndef MCTESTS_H
#define	MCTESTS_H

using namespace std;

class mcTests {
public:

    static void overlapVariationalAndKronecker(mcConfiguration* config) {
        int nThreads;
        mc_state ZERO = 0;
#pragma omp parallel
        {
            nThreads = omp_get_num_threads();
        }
        config->print();
        mcAbstractClusterHamiltonian* ham = new mcHamiltonianConfigurable(config);
        mcAbstractEigenSystemSolver* ess1 = new mcEigenSystemSolverPowerBasis(config, ham);
        mcAbstractEigenSystemSolver* ess2 = new mcEigenSystemSolverDirect(config, ham);
        mcAbstractVector* gsv1 = ess1->getGroundState();
        mcAbstractVector* gsv2 = ess2->getGroundState();

        mcTrialWaveFunctionConfigurable* twf = new mcTrialWaveFunctionConfigurable(config, gsv1);
        mcKroneckerProduct* kp = new mcKroneckerProduct(gsv2, gsv2);
        twf->setGamma(0);
        double sum = 0;
        for (mc_state i = 0; i < kp->getDimension1(); i++) {
            sum += twf->get(i) * kp->get(i);
        }
        cout << "Trial times cluster wave functions = " << sum << endl;
        exit(0);
    }
    //    mcAbstractEigenSystemSolver* ess = new mcEigenSystemSolverDirectBasis(config, ham);
    //    mcAbstractEigenSystemSolver* ess = new mcEigenSystemSolverDirect(config, ham);

    static void overlapActualAndKronecker(mcConfiguration* config, mcConfiguration* config2) {
        int nThreads;
        mc_state ZERO = 0;
#pragma omp parallel
        {
            nThreads = omp_get_num_threads();
        }
        config->print();
        mcAbstractClusterHamiltonian* ham2 = new mcHamiltonianConfigurable(config2);
        mcAbstractEigenSystemSolver* ess2 = new mcEigenSystemSolverPowerBasis(config2, ham2);
        mcAbstractClusterHamiltonian* ham = new mcHamiltonianConfigurable(config);
        mcAbstractEigenSystemSolver* ess1 = new mcEigenSystemSolverDirect(config, ham);
        mcAbstractVector* gsv1 = ess1->getGroundState();
        mcAbstractVector* gsv2 = ess2->getGroundState();

//        mcTrialWaveFunctionConfigurable* twf = new mcTrialWaveFunctionConfigurable(config, gsv1);
        mcKroneckerProduct* kp = new mcKroneckerProduct(gsv1, gsv1);
//        twf->setGamma(0);
        double sum = 0;
        for (mc_state i = 0; i < kp->getDimension1(); i++) {
            sum += gsv2->get(i) * kp->get(i);
        }
        cout << "Actual times Kronecker wave functions = " << sum << endl;
        exit(0);
    }
    //    mcAbstractEigenSystemSolver* ess = new mcEigenSystemSolverDirectBasis(config, ham);
    //    mcAbstractEigenSystemSolver* ess = new mcEigenSystemSolverDirect(config, ham);

};





#endif	/* MCTESTS_H */

