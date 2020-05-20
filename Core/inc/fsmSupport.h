#ifndef _FSMSUPPORT_H_
#define _FSMSUPPORT_H_
/**************************************************
* File: fsmSupport.h.
* Desc: Definitions of the FSM support classes, PVState, PVTransition, .
* Module: AkraLog : JavaKit.
* Rev: 26 mai 1996 : REV 0 : Hugo DesRosiers : Creation.
**************************************************/

#include "machineSupport.h"


class PVTransition;
class PVEvent;
class PVLocalTransition;
class PVFieldTransition;


/********************************************************
* Class: PVState.
* Desc: Efficient description of a state for run-time utilisation.
* Class variables:
* Instance variables:
* Super-Class: JVMethod.
* Module: JavaKit : RunTime.
********************************************************/

class PVState : public JVMethod {
  public:
    enum Flags {
	blockingTrans= 1, immediateTrans= 2
    };

  protected:
    unsigned int nbrGlobalTransitions;
    unsigned int nbrLocalTransitions;
    PVLocalTransition *localTransitions;
    PVFieldTransition *transitions;
    PVTransition *defaultTransition;
    unsigned int flags;

  public:
    PVState(void);
    PVState(ClassFile *owner, MethodInfo *info);
    virtual bool resolveTransitions(ClassManager *classManager, ClassFile *owner);
    virtual PVTransition *getDefaultTransition(void);
    virtual PVTransition *getMatchFor(PVEvent *anEvent);
    virtual bool hasBlockingTransitions(void);
    virtual bool hasImmediateTransitions(void);
};


/********************************************************
* Class: PVTransition.
* Desc: Efficient description of a transition to another state.  This is the
*	general model, and it doesn't contain the trigger to use for deciding
*	if the transition has to occur.
* Class variables:
* Instance variables:
* Super-Class: AkObject.
* Module: JavaKit : RunTime.
********************************************************/

class PVTransition : public AkObject {
  protected:
    TransitionsAttribute *original;
    unsigned int modifiers;
    MethodInfo *destination;
    unsigned int paramLoader;

  public:
    PVTransition(void);
    PVTransition(TransitionsAttribute *source);
    virtual void linkWith(TransitionsAttribute *source);
    virtual bool resolve(ClassManager *classManager, ClassFile *owner);
    virtual MethodInfo *getDestination(void);
    virtual unsigned int getParamsPC(void);
    virtual bool isMatching(JVInstance *aValue);
    virtual unsigned int getModifiers(void);
};


class PVLocalTransition : public PVTransition {
  protected:
    JVVariable *trigger;

  public:
    PVLocalTransition(void);
    virtual bool isMatching(JVInstance *aValue);
};


class PVFieldTransition : public PVTransition {
  protected:
    FieldInfo *trigger;

  public:
    PVFieldTransition(void);
    virtual bool resolve(ClassManager *classManager, ClassFile *owner);
    virtual bool isMatching(JVInstance *aValue);
};


#endif		/* _FSMSUPPORT_H_ */
