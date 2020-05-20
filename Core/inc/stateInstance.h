#ifndef _STATEINSTANCE_H_
#define _STATEINSTANCE_H_
/**************************************************
* File: stateInstance.h.
* Desc: Definition of the PVStateInstance class.
* Module: AkraLog : JavaKit.
* Rev: 26 mai 1996 : REV 0 : Hugo DesRosiers : Creation.
**************************************************/

#include "jvInstance.h"


class PVState;
class PVFsmThread;
class PVEvent;


class PVStateInstance : public JVInstance {
  protected:
    PVFsmThread *threads;	// FSM which are running 'within' this object.
    
  public:
    PVStateInstance(void);
    PVStateInstance(ClassFile *owner);

    virtual void addThread(PVFsmThread *aThread);
    virtual void fire(PVEvent *anEvent);
};

#endif	/* _STATEINSTANCE_H_ */
