/**************************************************
* File: eventQueue.cc.
* Desc: Implementation of the PVEventQueue class.
* Module: AkraLog : JavaKit.
* Rev: 26 mai 1996 : REV 0 : Hugo DesRosiers : Creation.
**************************************************/

#include "cDefinitions.h"
#include "eventQueue.h"


/**************************************************
* Implementation: PVEvent.
**************************************************/

PVEvent::PVEvent(void)
{
    prev= next= NULL;
    flags= 0;
    value= NULL;
}


PVEvent::PVEvent(unsigned int someFlags, JVInstance *aValue)
{
    prev= next= NULL;
    flags= someFlags;
    value= aValue;
}


bool PVEvent::isLocal(void)
{
    return ((flags & isField) == 0);
}


JVInstance *PVEvent::getValue(void)
{
    return value;
}


/**************************************************
* Implementation: PVEventQueue.
**************************************************/

PVEventQueue::PVEventQueue(void)
{
    posNextNew= nbrEvents= 0;
    cursor= queueHead= queueTail= NULL;
}


void PVEventQueue::setAllNew(void)
{
    posNextNew= 0;
    cursor= queueHead;
}


unsigned int PVEventQueue::newCount(void)
{
    return (nbrEvents - posNextNew);
}


PVEvent *PVEventQueue::getNextNew(void)
{
    if (cursor != NULL) {
	PVEvent *current= cursor;
	cursor= cursor->next;
	posNextNew++;
	return current;
    }

    return NULL;
}


void PVEventQueue::unqueueEvent(PVEvent *anEvent)
{
    if (anEvent->next != NULL) anEvent->next->prev= anEvent->prev;
    if (anEvent->prev != NULL) anEvent->prev->next= anEvent->next;
    if (queueHead == anEvent) queueHead= anEvent->next;
    if (cursor == anEvent) cursor= anEvent->next;
    if (queueTail == anEvent) {
	cursor= queueTail= anEvent->prev;
    }
    nbrEvents--;
    delete anEvent;
}


void PVEventQueue::queueEvent(PVEvent *anEvent)
{
    if (queueHead == NULL) {
	queueHead= anEvent;
	cursor= queueHead;
    }
    else {
	queueTail->next= anEvent;
	anEvent->prev= queueTail;
    }
    queueTail= anEvent;
    nbrEvents++;
}

