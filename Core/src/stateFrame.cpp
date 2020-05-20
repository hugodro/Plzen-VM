/**************************************************
* File: stateFrame.cc.
* Desc: Implementation of the PVStateFrame.
* Module: AkraLog : JavaKit.
* Rev: 26 mai 1997 : REV 0 : Hugo DesRosiers : Creation.
**************************************************/

#include "fsmSupport.h"
#include "stateFrame.h"


PVStateFrame::PVStateFrame(PVState *aState, ClassFile *source) : JVExecFrame(aState, source)
{
    stateFlags= 0;

	// Must look at the flags of the state transitions, and setup stateFlags correctly:
    if (aState->hasBlockingTransitions()) stateFlags|= isBlocking;
    if (aState->hasImmediateTransitions()) stateFlags|= isBlocking;

    // stateFlags should be in accordance with previous 'implicit' definition !!
}


void PVStateFrame::setReceiver(PVStateInstance *anInstance)
{
    receiver= anInstance;
}


void PVStateFrame::setParamPC(unsigned int aPC)
{
    flags^= sfDeadFrame;
    JVExecFrame::setCurrentPC(aPC);
}

