#ifndef _EVENTQUEUE_H_
#define _EVENTQUEUE_H_
/**************************************************
* File: eventQueue.h.
* Desc: Definition of the PVEventQueue class.
* Module: AkraLog : JavaKit.
* Rev: 26 mai 1996 : REV 0 : Hugo DesRosiers : Creation.
**************************************************/

#include <akra/AkObject.h>
#include "cDefinitions.h"


class JVInstance;


class PVEvent : public AkObject {
    friend class PVEventQueue;

  public:
    enum Flags {
	isField= 1
    };

  protected:
    PVEvent *prev, *next;
    unsigned int flags;
    JVInstance *value;

  public:
    PVEvent(void);
    PVEvent(unsigned int someFlags, JVInstance *aValue);
    virtual bool isLocal(void);
    virtual JVInstance *getValue(void);
};


class PVEventQueue : public AkObject {
  protected:
    unsigned int nbrEvents;
    unsigned int posNextNew;
    PVEvent *queueHead;
    PVEvent *queueTail;
    PVEvent *cursor;

  public:
    PVEventQueue(void);
    virtual void setAllNew(void);
    virtual unsigned int newCount(void);
    virtual PVEvent *getNextNew(void);
    virtual void unqueueEvent(PVEvent *anEvent);
    virtual void queueEvent(PVEvent *anEvent);
};


#endif		/* _EVENTQUEUE_H_ */
