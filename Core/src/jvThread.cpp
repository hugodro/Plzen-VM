/**************************************************
* File: threadSupport.cc.
* Desc: Implementation of the JVThread and JVThreadSet classes.
* Module: AkraLog : JavaKit.
* Rev: 16 novembre 1996 : REV 0 : Hugo DesRosiers : Creation.
**************************************************/

#include <stdio.h>
#include <string.h>

#include <threader/threadManager.h>

#include "classFile.h"
#include "jmachine.h"
#include "jvInstance.h"
#include "jvcup.h"
#include "jvThread.h"

/**************************************************
* Implementation: JVThread.
**************************************************/

JVThread::JVThread(unsigned int anID, JVThread *parent) : JVAbstractThread(anID)
{
    init();
    if (parent->children == NULL) {
	parent->children= new JVThreadSet(parent->owner);
    }
    parent->children->addThread(this);
    priority= parent->owner->getDefaultPriority();
}


JVThread::JVThread(unsigned int anID, JVThreadSet *aSet) : JVAbstractThread(anID)
{
    init();
    aSet->addThread(this);
    priority= aSet->getDefaultPriority();
}


JVThread::~JVThread(void)
{
	// Is it necessary to delete the children set ?
}


void JVThread::init(void)
{
    name= NULL;
    priority= normal;
    status|= regular | interactive;	// Status, Category, SharedMode.
    machine= NULL;
    owner= NULL;
    children= NULL;
    nextInSet= NULL;
    globalNext= NULL;
    inQueue= NULL;
    outQueue= NULL;
    javaEquiv= NULL;
}


JVAbstractThread::SubType JVThread::getSubClass(void)
{
    return java;
}


unsigned int JVThread::getCategory(void)
{
    return (status & categoryMask);
}


unsigned int JVThread::getSchedulingType(void)
{
    return (status & sharingMask);
}


JVThreadSet *JVThread::getThreadSet(void)
{
    return owner;
}


void JVThread::setThreadSet(JVThreadSet *aSet)
{
    owner= aSet;
}


JVThread *JVThread::getFirstDead(void)
{
    if ((status & statusMask) == dead) return this;    // Warning: quick return.
    else {
	JVThread *tmpThread= globalNext;

	while (tmpThread != NULL) {
	    if ((tmpThread->status & statusMask) != dead) {
		tmpThread= tmpThread->globalNext;
	    }
	    else break;	// Found one.
	}

	return tmpThread;
    }
}


void JVThread::setJavaDual(JVInstance *anInstance)
{
    javaEquiv= anInstance;
}


JVInstance *JVThread::getJavaDual(void)
{
    return javaEquiv;
}


bool JVThread::runMethod(char *methodName, char *typing)
{
    MethodInfo *methodInfo;
    JVMethod *theMethod;
    bool result= false;

    if (javaEquiv != NULL) {
	methodInfo= javaEquiv->category()->getMethodNamed(methodName, typing);
	if (methodInfo) {
	    theMethod= methodInfo->resolve(threadManager->getKernel()->getClassManager());
	    if (theMethod) {
		status= (status & ~statusMask) | running;
		machine->runMethod(theMethod, javaEquiv);
	    }
	 }
    }
    status= (status & ~statusMask) | dying;

    return result;
}


JVThread *JVThread::findID(unsigned int anID)
{
    if (privateID == anID) return this;		// Warning: quick return.
    else {
	JVThread *tmpThread= globalNext;

	while (tmpThread != NULL) {
	    if (tmpThread->privateID != anID) {
		tmpThread= tmpThread->globalNext;
	    }
	    else break;	// Found one.
	}

	return tmpThread;
    }
}


JVThread *JVThread::getNextGlobal(void)
{
    return globalNext;
}


void JVThread::setNextGlobal(JVThread *aThread)
{
   globalNext= aThread;
}


JVThread *JVThread::getNextInSet(void)
{
    return nextInSet;
}


void JVThread::setNextInSet(JVThread *aThread)
{
   nextInSet= aThread;
}


/**************************************************
* Implementation: JVThreadSet.
**************************************************/

JVThreadSet::JVThreadSet(JVThreadSet *aSet)
{
    init();
    parent= aSet;
    if (aSet) {
	name= new char[strlen(aSet->name) + 4];
	strcpy(name, "sub");		// Must find a way to define a unique name.
	strcpy(&name[3], aSet->name);
	defaultPriority= aSet->defaultPriority;
	maxPriority= aSet->maxPriority;
    }
    else {
	defaultPriority= JVThread::normal;
	maxPriority= JVThread::maximum;
    }
}


JVThreadSet::JVThreadSet(char *aName, JVThreadSet *aSet)
{
    init();

    name= new char[strlen(aName) + 1];
    strcpy(name, aName);

    parent= aSet;

    if (aSet) {
	defaultPriority= aSet->defaultPriority;
	maxPriority= aSet->maxPriority;
    }
    else {
	defaultPriority= JVThread::normal;
	maxPriority= JVThread::maximum;
    }
}


JVThreadSet::~JVThreadSet(void)
{
    delete[] name;
}


void JVThreadSet::init(void)
{
    nextInList= NULL;
    name= NULL;
    threadCount= 0;
    threads= NULL;
    parent= NULL;
}


void JVThreadSet::addThread(JVThread *aThread)
{
    // lock region.
    threadCount++;
    aThread->setNextInSet(threads);
    threads= aThread;
    // unlock region.
    aThread->setThreadSet(this);
}


unsigned int JVThreadSet::getDefaultPriority(void)
{
    return defaultPriority;
}


JVThread *JVThreadSet::runInNewThread(JVMethod *aMethod, JVInstance *anInstance)
{
    if ((aMethod != NULL) && (anInstance != NULL)) {
	// TODO: Create new thread object, attach instance/method in it, spawn concurrent
	//	code execution.
    }
    return NULL;
}


JVThread *JVThreadSet::runInNewThread(JVMethod *aMethod)
{
    if (aMethod != NULL) {
	// TODO: Create new thread object, attach instance/method in it, spawn concurrent
	//	code execution.
    }
    return NULL;
}


void JVThreadSet::setNextInList(JVThreadSet *aSet)
{
    nextInList= aSet;
}


JVThreadSet *JVThreadSet::getNextInList(void)
{
    return nextInList;
}



