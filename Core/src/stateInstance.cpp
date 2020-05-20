/**************************************************
* File: stateInstance.h.
* Desc: Implementation of the PVStateInstance class.
* Module: AkraLog : JavaKit.
* Rev: 26 mai 1996 : REV 0 : Hugo DesRosiers : Creation.
**************************************************/

#include "fsmThread.h"
#include "stateInstance.h"


PVStateInstance::PVStateInstance(void)
{
  // ATTN: Juste pour faire plaisir a GCC.
  // Must throw an exception, since there is no class to relate the instance to.
}


PVStateInstance::PVStateInstance(ClassFile *owner) : JVInstance(owner)
{
    threads= NULL;
}


void PVStateInstance::addThread(PVFsmThread *aThread)
{
    threads= aThread;	// ATTN-970526 [HD]: Single thread for now.
}


void PVStateInstance::fire(PVEvent *anEvent)
{
    threads->receiveEvent(anEvent);
}

