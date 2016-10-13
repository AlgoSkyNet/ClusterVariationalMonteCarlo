/* 
 * File:   mcSimulationState.h
 * Author: chris
 *
 * Created on December 9, 2011, 3:35 PM
 */

#ifndef MCSIMULATIONSTATE_H
#define	MCSIMULATIONSTATE_H

#include "mc.h"

class mcSimulationState {
public:    
    mcSimulationState();
    mcSimulationState(int ns, int nup, int ndown){
        nSites=ns;
        nUp=nup;
        nDown=ndown;
        upState = new int[nSites];
        downState = new int[nSites];
    }
    mc_state get_mc_state(){
        mc_state ret=0;
        for(int i=0; i<nSites;i++){
            
        }
    }
    mcSimulationState(const mcSimulationState& orig);
    virtual ~mcSimulationState();
private:
    int* upState;
    int* downState;
    int nSites;
    int nUp;
    int nDown;

};

#endif	/* MCSIMULATIONSTATE_H */

