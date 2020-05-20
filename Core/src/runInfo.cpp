/**************************************************
* File: runInfo.cc.
* Desc: Implementation of the JVClassRunInfo class.
* Module: AkraLog : PlzenKit.
* Rev: 10 septembre 1997 : REV 0 : Hugo DesRosiers : Creation.
**************************************************/

#include <stdlib.h>
#include <memory.h>
#include "runInfo.h"


/**************************************************
* Implementation: JVClassRunInfo.
**************************************************/

JVClassRunInfo::JVClassRunInfo(void)
{
    nbrLongSlots= 0;
    longSlots= NULL;
}


JVClassRunInfo::JVClassRunInfo(JVClassRunInfo *aSuperRI)
{
    nbrLongSlots= aSuperRI->nbrLongSlots;
    longSlots= new unsigned int[nbrLongSlots];
    memcpy(longSlots, aSuperRI->longSlots, nbrLongSlots * sizeof(unsigned int));
}


JVClassRunInfo::~JVClassRunInfo(void)
{
    if (nbrLongSlots > 0) delete[] longSlots;
}


void JVClassRunInfo::addSlots(unsigned int aCount, unsigned int *slotIDs)
{
    if (nbrLongSlots == 0) {
	nbrLongSlots= aCount;
	longSlots= new unsigned int[aCount];
	memcpy(longSlots, slotIDs, nbrLongSlots * sizeof(unsigned int));
    }
    else {
	unsigned int tmpCount= nbrLongSlots + aCount;
	unsigned int *tmpSlots= new unsigned int[tmpCount];
	memcpy(tmpSlots, longSlots, nbrLongSlots * sizeof(unsigned int));
	memcpy(tmpSlots + nbrLongSlots, slotIDs, aCount * sizeof(unsigned int));
	delete[] longSlots;
	longSlots= tmpSlots;
    }
}


/**************************************************
* Implementation: JVMethodRunInfo.
**************************************************/

JVMethodRunInfo::JVMethodRunInfo(void)
{
    nbrLongSlots= 0;
    longSlots= NULL;
}


JVMethodRunInfo::~JVMethodRunInfo(void)
{
    if (nbrLongSlots > 0) delete[] longSlots;
}


void JVMethodRunInfo::addSlots(unsigned int aCount, unsigned int *slotIDs)
{
    if (nbrLongSlots == 0) {
	nbrLongSlots= aCount;
	longSlots= new unsigned int[aCount];
	memcpy(longSlots, slotIDs, nbrLongSlots * sizeof(unsigned int));
    }
    else {
	unsigned int tmpCount= nbrLongSlots + aCount;
	unsigned int *tmpSlots= new unsigned int[tmpCount];
	memcpy(tmpSlots, longSlots, nbrLongSlots * sizeof(unsigned int));
	memcpy(tmpSlots + nbrLongSlots, slotIDs, aCount * sizeof(unsigned int));
	delete[] longSlots;
	longSlots= tmpSlots;
    }
}

