/**************************************************
* File: poolManager.cc.
* Desc: Implementation of the Pool Manager class.
* Module: AkraLog : JavaKit.
* Rev: 9 mai 1996 : REV 0 : Hugo DesRosiers : Creation.
**************************************************/


#include "jvcup.h"
#include "jvInstance.h"
#include "fsmThread.h"
#include "stateInstance.h"
#include "poolManager.h"


/**************************************************
* Implementation: ClassPool.
**************************************************/

ClassPool::ClassPool(ClassFile *aClass)
{
    next= NULL;
    nbrAllocated= nbrFree= 0;
    category= aClass;
    allocated= lastAllocated= freed= lastFreed= NULL;
}


/**************************************************
* Implementation: PoolManager.
**************************************************/

PoolManager::PoolManager(JVCup *creator)
{
// ATTN: This has to go away, and separate 'block' pools are to be used for array
// creation/destruction.
    pools= new ClassPool((ClassFile *)1);		// Fake category for basic array instances.
    pools->next= new ClassPool((ClassFile *)2);	// Fake category for instance array instances.
    owner= creator;
}


JVInstance *PoolManager::createInstance(ClassFile *aClass)
{
    JVInstance *result= NULL;
    if (aClass->isStateless()) {
	result= new JVInstance(aClass);
    }
    else {
	result= new PVStateInstance(aClass);
    }
    return result;
}


JVInstance *PoolManager::getInstance(ClassFile *category)
{
    ClassPool *tmpPoolPtr;
    JVInstance *result= NULL;

    tmpPoolPtr= pools;
    while (tmpPoolPtr != NULL) {
	if (tmpPoolPtr->category == category) break;
	tmpPoolPtr= tmpPoolPtr->next;
     }
    if (tmpPoolPtr != NULL) {
	if (tmpPoolPtr->nbrFree > 0) {
	    result= (JVInstance *)tmpPoolPtr->freed;
	    tmpPoolPtr->nbrFree--;
	    if ((tmpPoolPtr->freed= result->next) == NULL) {
		tmpPoolPtr->lastFreed= NULL;		// Update the queue of the pool.
	     }
	    else {
		result->next->prev= NULL;
	     }
	 }
     }
    else {
	tmpPoolPtr= new ClassPool(category);
	tmpPoolPtr->next= pools;
	pools= tmpPoolPtr;
     }

    if (result == NULL) {
	if ((result= createInstance(category)) != NULL) {
	    if (tmpPoolPtr->lastAllocated == NULL) {
		tmpPoolPtr->allocated= result;
	     }
	    else {
		tmpPoolPtr->lastAllocated->next= result;
		result->prev= tmpPoolPtr->lastAllocated;
	     }
	    tmpPoolPtr->lastAllocated= result;
	    tmpPoolPtr->nbrAllocated++;
	 }
     }
    else {
	result->reInitialize();
     }

    return result;
}


JVBasicArray *PoolManager::createBasicArrayInstance(ubyte type, unsigned int byteSize)
{
    JVBasicArray *result= NULL;

    result= new JVBasicArray(type, byteSize);
    return result;
}


JVBasicArray *PoolManager::getBasicArray(ubyte type, unsigned int count)
{
    ClassPool *tmpPoolPtr;
    JVBasicArray *result= NULL;
    unsigned int byteCount;

    tmpPoolPtr= pools;
    while (tmpPoolPtr != NULL) {
	if (tmpPoolPtr->category == (ClassFile *)1) break;
	tmpPoolPtr= tmpPoolPtr->next;
     }

    if (tmpPoolPtr != NULL) {		// Should always find the ClassPool !
	type= JVBasicArray::byteCodeToBasicType(type);
	byteCount= count * JVBasicArray::typeToByteLength(type);
	if (tmpPoolPtr->nbrFree > 0) {
	    result= (JVBasicArray *)tmpPoolPtr->freed;
	    while (result != NULL) {
		if (result->sizeAllocated >= byteCount) break;
	     }
	    if (result != NULL) {
		tmpPoolPtr->nbrFree--;
		if ((tmpPoolPtr->freed= result->next) == NULL) {
		    tmpPoolPtr->lastFreed= NULL;		// Update the queue of the pool.
		 }
		else {
		    result->next->prev= NULL;
		 }
	     }
	 }

	if (result == NULL) {
	    if ((result= createBasicArrayInstance(type, count)) != NULL) {
		if (tmpPoolPtr->lastAllocated == NULL) {
		    tmpPoolPtr->allocated= result;
		}
		else {
		    tmpPoolPtr->lastAllocated->next= result;
		    result->prev= tmpPoolPtr->lastAllocated;
		}
		tmpPoolPtr->lastAllocated= result;
		tmpPoolPtr->nbrAllocated++;
	    }
	}
	else {
	    result->reInitialize(type, count);
	}
     }

    return result;
}


JVInstanceArray *PoolManager::getInstanceArray(ClassFile *category, unsigned int count)
{
    JVInstanceArray *result= NULL;

// Temporary implementation, without elements block recuperation.
    result= new JVInstanceArray(category, count);
    return result;
}


JVArrayArray *PoolManager::getArrayArray(ClassFile *category, unsigned int *dimCounts, unsigned int nbrDims)
{
    JVArrayArray *result= NULL;

// Temporary implementation, without elements block recuperation.
    result= new JVArrayArray(category, dimCounts, nbrDims);
    return result;
}


void PoolManager::dereference(JVAbstractInstance *anInstance)
{
    unsigned int i, j;

    anInstance->useCount--;		// ATTN: Will have to check for under-zero condition.
    if (anInstance->useCount == 0) {
	    // First, dereference objects accessed by instance fields.
	if (anInstance->vectorialType() == TYPE_INSTANCE) {
	    JVInstance *aRealnstance= (JVInstance *)anInstance;
	    for (i= j= 0; i < aRealnstance->isA->totalNbrCplxSlots; j++) {
		if (aRealnstance->isA->complexMap[j >> 3] & (1 << (j % 8))) {
		    dereference(aRealnstance->fields[j]);
		    i++;
		}
	    }
	 }
	// ATTENTION: Move anInstance from allocated to freed list.
     }
}


