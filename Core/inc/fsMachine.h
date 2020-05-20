#ifndef _PVFSMACHINE_H_
#define _PVFSMACHINE_H_
/**************************************************
* File: fsMachine.h.
* Desc: Definition of the Plzen finite state machine class, PVFSMachine.
* Module: AkraLog : JavaKit.
* Rev: 26 mai 1996 : REV 0 : Hugo DesRosiers : Creation.
**************************************************/

#include <threader/rawThreads.h>
#include "jmachine.h"


class PVEventQueue;
class PVStateFrame;
class PVStateInstance;
class PVFsmThread;
class PVEvent;
class PVState;
class PVTransition;


class PVFSMachine : public JVMachine {
  public:
    static void pvfsMachine(void *receiver);

  protected:	// Instance variables.
    PVStateFrame *stateFrame;	// Frame of current state.
    PVEventQueue *events;			// Event queue.
    bool gotNewEvent;				// Signals that new events arrived since last check.
    bool isAlive;
    bool stateBlocked;			// Blocked waiting for an event.
    JVCondition *stateUnblocker;	// Condition used to unblock the state machine.

/* TODO:
    JVTransitions *implicitTransitions;
*/

  public:		// Instance methods.

    PVFSMachine(JVCup *owner);
    PVFSMachine(JVCup *owner, PVFsmThread *aThread);
    void init(void);
    void initForState(PVState *aState, PVStateInstance *anInstance);
    void popExecFrame(void);
    virtual void run(void);
    virtual PVTransition *findTransition(void);
    virtual PVState *findNextState(PVTransition *aTransition);
    virtual void lockAndReceiveEvent(PVEvent *anEvent);

    virtual JVAbstractThread::LauncherType getLauncher(void);
};


#endif		/* _PVFSMACHINE_H_ */
