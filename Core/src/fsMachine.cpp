/**************************************************
* File: fsMachine.cc.
* Desc: Implementation of the Plzen finite state machine class, PVFSMachine.
* Module: AkraLog : JavaKit.
* Rev: 26 mai 1996 : REV 0 : Hugo DesRosiers : Creation.
**************************************************/

#include "eventQueue.h"
#include "fsmThread.h"
#include "fsmSupport.h"
#include "stateFrame.h"
#include "machineSupport.h"
#include "jvcup.h"
#include "stateInstance.h"
#include "jvThread.h"
#include "fsMachine.h"


/**************************************************
* Implementation: PVFSMachine.
**************************************************/


void PVFSMachine::pvfsMachine(void *receiver)
{
    ((PVFSMachine *)receiver)->run();
}



PVFSMachine::PVFSMachine(JVCup *owner) : JVMachine(owner)
{
    init();
}


PVFSMachine::PVFSMachine(JVCup *owner, PVFsmThread *aThread) : JVMachine(owner, (JVThread *)aThread)
{
    init();
}


void PVFSMachine::init(void)
{
    events= new PVEventQueue();
    stateFrame= NULL;
    gotNewEvent= false;
    stateUnblocker= workContext->getACondition();
}


void PVFSMachine::initForState(PVState *aState, PVStateInstance *anInstance)
{
    stateFrame= new PVStateFrame(aState, aState->getImplementor());
    stateFrame->setReceiver(anInstance);
    stateFrame->setLocalValue(0, anInstance);
    pushExecFrame(stateFrame);
}


void PVFSMachine::popExecFrame(void)
{
    JVExecFrame *oldFrame;

    if (execFrame != NULL) {
	if (execFrame == stateFrame) {
	    // The state code is finished, must do transition handling.
#if defined(DBGLEVEL_1)
	    if (debugInfo->traceLevel() & DebugInfo::frameFollowup) {
		static char className[250];
		execFrame->currentClass->getName(className);
		std::cout << "=== PVFSMachine: [" << execFrame->frameDepth << "] End of state: ";
		std::cout << className << "." << execFrame->method->getName() << ".\n";
	    }
#endif
	}
	else {
	    JVMachine::popExecFrame();
	}
     }
    else {
	std::cout << "ERRMSG: PVFSMachine: Can't pop exec frame.\n";
     }
}


void PVFSMachine::run(void)
{

    if (execFrame != NULL) {
	PVTransition *aTransition;
	PVState *nextState;

	isAlive= true;
	do {
		// Run the current execFrame until it is finished.
	    while (!execFrame->isFinished()) {
		doInstruction();
		if ((stateFrame->stateFlags & PVStateFrame::canImmediate) != 0) {
		    if (gotNewEvent) {
			// Look at new events to see if there is an immediate branch.
		    }
		}
	    }

		// handle state transition.
	    if ((aTransition= findTransition()) != NULL) {
		if ((aTransition->getModifiers() & MOD_EXIT)) {
		    // TODO: Cleanup the fsm, signal gracefull death.
		    std::cout << "WRNMSG: Hit end-of-FSM transition; shutting down machine.\n";
		    isAlive= false;
		}
		else if ((nextState= findNextState(aTransition)) != NULL) {
		    PVStateFrame *newFrame;
		    JVRunValue tmpValue(TYPE_INSTANCE, stateFrame->receiver);

		    operStack->push(tmpValue);

			// Execute the loading sequence from the transition.
		    stateFrame->setParamPC(aTransition->getParamsPC());
		    while (!execFrame->isFinished()) {
			doInstruction();
		    }

		    newFrame= new PVStateFrame(nextState, nextState->getImplementor());
		    newFrame->setReceiver(stateFrame->receiver);
		    newFrame->loadArguments(operStack);

		    newFrame->frameDepth= stateFrame->frameDepth+1;

		    delete stateFrame;
		    stateFrame= newFrame;
		    execFrame= NULL;
		    pushExecFrame(stateFrame);
		}
		else {
		    // TODO: Handle the case of no state specified by transition actually found.
		    std::cout << "ERRMSG: Can't find the state specified by the transition.\n";
		    isAlive= false;
		}
	    }
	    else {
		// TODO: Handle the case of no transition found.
		std::cout << "ERRMSG: Can't find a transition to follow.\n";
		isAlive= false;
	    }
	    events->setAllNew();
	} while (isAlive);
    }
}


PVTransition *PVFSMachine::findTransition(void)
{
    PVTransition *result;
    bool nowhereToGo= true;

    do {
	if (gotNewEvent) {
	    PVEvent *aMatch;

		// First, try to match the new events to some transition.
	    for (unsigned int i= 0; i < events->newCount(); i++) {
		if ((result= ((PVState *)stateFrame->method)->getMatchFor((aMatch= events->getNextNew()))) != NULL) {
		    events->unqueueEvent(aMatch);
		    return result;	// Warning: quick exit.
		}
	    }
	}
    
	    // If no event is a transition, look at implicits.
	// TODO: Implement implicit search.
    
	    // If no event, and not blocking, take the default.
	if ((stateFrame->stateFlags & PVStateFrame::isBlocking) != 0) {
	    // TODO: Tell thread to sleep until next event comes by (and maybe set a timer somewhere).
	    // TEST:
	    stateBlocked= true;
	    stateUnblocker->wait();
	}
	else {
	    result= ((PVState *)stateFrame->method)->getDefaultTransition();
	    nowhereToGo= false;		// Get out always, and no real default transition is handled by caller.
	}
    } while (nowhereToGo);

    return result;
}



PVState *PVFSMachine::findNextState(PVTransition *aTransition)
{
    MethodInfo *nextStateInfo;
    PVState *nextMethod= NULL;

    if ((nextStateInfo= aTransition->getDestination()) != NULL) {
	if ((nextMethod= (PVState *)nextStateInfo->resolve(kernel->getClassManager())) != NULL) {
	    nextMethod= (PVState *)stateFrame->receiver->category()->getVirtualMethod(kernel->getClassManager(), nextMethod);
	}
    }
    return nextMethod;
}


void PVFSMachine::lockAndReceiveEvent(PVEvent *anEvent)
{
    // TODO: Get exclusive access to event queue of the machine.
    gotNewEvent= true;
    events->queueEvent(anEvent);
    if (stateBlocked) {
	stateBlocked= false;
	stateUnblocker->declareSatisfied();
    }
    // TODO: Release exclusive access to event queue.
}


JVAbstractThread::LauncherType PVFSMachine::getLauncher(void)
{
    return pvfsMachine;
}

