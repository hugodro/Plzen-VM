/**************************************************
* File: jmachine.cc.
* Desc: Implementation of the JVMachine instruction set.
* Module: AkraLog : JavaKit.
* Rev: 2 mai 1996 : REV 0 : Hugo DesRosiers : Creation.
**************************************************/


// MOD-970512 [JST]: pour strlen
#include <string.h>
#include <threader/synchro.h>

#include "jvInstrSet.h"
#include "stacks.h"
#include "execFrame.h"
#include "machineSupport.h"
#include "poolManager.h"
#include "jvInstance.h"
#include "jvThread.h"
#include "jvcup.h"
#include "math64bits.h"
#include "jmachine.h"
#include "debugInfo.h"


/**************************************************
* Implementation: JVMachine.
**************************************************/

void JVMachine::jvMachine(void *receiver)
{
    ((JVMachine *)receiver)->run();
}


JVMachine::JVMachine(void)
{
// ATTN: Should not be used !
}


JVMachine::JVMachine(JVCup *owner)
{
    init(owner, NULL);
}


JVMachine::JVMachine(JVCup *owner, JVThread *aThread)
{
    init(owner, aThread);
}


JVMachine::~JVMachine()
{
    delete execFrame;
    delete operStack;
}

// ATTN: The method must be given a JVThread instead of an JVAbstractThread.
void JVMachine::init(JVCup *owner, JVThread *context)
{
    kernel= owner;
    if ((workContext= context) != NULL) workContext->machine= this;
    poolManager= kernel->getPoolManager();	// ATTN: Should it come from kernel ?
    operStack= new JVOperStack;
    execFrame= NULL;
    vindex= 0;
    exceptionRaised= false;
	// MOD-970520 [HD]: Pour NT.
    debugInfo= new DebugInfo((DebugInfo::SectionTracing)(unsigned int)owner->runtimeOptions()->getOption("d"));
    traceLevel= debugInfo->traceLevel();
}


void JVMachine::pushExecFrame(JVExecFrame *newFrame)
{
    if (execFrame != NULL) {
	newFrame->last= execFrame;
	newFrame->frameDepth= execFrame->frameDepth + 1;
     }
    execFrame= newFrame;
    if (execFrame->method->isSynchronized()) {
	if (execFrame->method->isStatic()) {
	    execFrame->synchro= execFrame->method->getImplementor()->getSynchronizer();
	}
	else {
	    if (execFrame->localValues[0]->metaType != TYPE_BASIC) {
		if ((execFrame->synchro= ((JVInstance *)execFrame->localValues[0]->value)->getSynchronizer()) == NULL) {
		    kernel->putSynchroOn((JVInstance *)execFrame->localValues[0]->value);
		    execFrame->synchro= ((JVInstance *)execFrame->localValues[0]->value)->getSynchronizer();		}
	    }
	    else {
		std::cout << "RT Exception: Method lock on basic parameter.\n";
		raiseException(NULL);
	    }
	}
	if (execFrame->synchro != NULL) execFrame->synchro->lock(workContext);
	else {
	    std::cout << "RT Exception: Missing synchronizer.\n";
	    raiseException(NULL);
	}
    }
#if defined(DBGLEVEL_1)
    if (debugInfo->traceLevel() & DebugInfo::frameFollowup) {
	static char className[250];
	execFrame->currentClass->getName(className);
	std::cout << "=== JVMachine: [" << execFrame->frameDepth << "] Pushed frame for method: ";
	std::cout << className << "." << execFrame->method->getName() << ".\n";
    }
#endif
}


void JVMachine::popExecFrame(void)
{
    JVExecFrame *oldFrame;

    if (execFrame != NULL) {
	    // The spawn of a FSM is done at the end of the top call to a constructor.
	    // The execFrame of that call is marked as such.
	    // ATTN: Can the 'this' be assigned ? If so, the localValues[0] may not be the
	    // object that we want to 'spawn' a FSM in.

	if (execFrame->synchro != NULL) {
	    execFrame->synchro->unlock(workContext);
	}

	if (execFrame->isLauncher()) {
	    kernel->spawnFSM((PVStateInstance *)execFrame->localValues[0]->value);
	}

	if ((execFrame->last) != NULL) {
	    oldFrame= execFrame;
	    execFrame= execFrame->last;
#if defined(DBGLEVEL_1)
	    if (debugInfo->traceLevel() & DebugInfo::frameFollowup) {
		static char className[250];
		execFrame->currentClass->getName(className);
		std::cout << "=== JVMachine: [" << execFrame->frameDepth << "] Popping frame to method: ";
		std::cout << className << "." << execFrame->method->getName() << ".\n";
	    }
#endif
	    if (!oldFrame->isEntryPoint()) delete oldFrame;

/* MOD-970512 [HD]:  For now, only the machine debug level is taken in account.
	    debugInfo->traceLevel()= execFrame->currentClass->traceLevel();
*/

	 }
	else {
#if defined(DBGLEVEL_1)
	    if (debugInfo->traceLevel() & DebugInfo::frameFollowup) {
		std::cout << "JVMachine: Popping out last execFrame.\n";
	    }
#endif
	    execFrame= NULL;
	    traceLevel= DebugInfo::noTrace;
	 }
     }
    else {
	std::cout << "JVMachine: Can't pop exec frame.\n";
     }
}


void JVMachine::popExecFrame(unsigned int aValue)
{
    popExecFrame();
    if (execFrame != NULL) {
	operStack->pushInteger(aValue);
     }
}


void JVMachine::popExecFrame(Value64Bits *aValue)
{
    popExecFrame();
    if (execFrame != NULL) {
	operStack->pushLong(aValue);
     }
}


void JVMachine::popExecFrame(JVRunValue &aValue)
{
    popExecFrame();
    if (execFrame != NULL) {
	operStack->push(aValue);
     }
}


int JVMachine::runMethod(JVMethod *method, JVInstance *receiver)
{
    JVExecFrame *runFrame;

//    execFrame->setFlowOn(method->code, method->codeLength);
/* MOD-970211 [HD]: Must use the constant pool of the method, not of the receiver's class.
    runFrame= new JVExecFrame(method, receiver->category());
*/
	// TMP-970512 [HD]: While there is a better place to do this...
    traceLevel= debugInfo->traceLevel();

    runFrame= new JVExecFrame(method, method->getImplementor());
    if (!method->isStatic()) runFrame->setLocalValue(0, receiver);
    runFrame->setEntryPoint();
    pushExecFrame(runFrame);
    if (method->isNative()) {
	((JVNativeMethod *)method)->doNativeCall(this, runFrame);
    }
    else {
	while (!runFrame->isFinished()) {
	    doInstruction();
	}
     }

    delete runFrame;
/*
    if (currentPC > method->codeLength) {
	std::cout << "runMethode a termine plus loin que la fin du code !\n";
     }
*/
    return 1;
}


void JVMachine::prepareMethod(JVMethod *method, JVInstance *receiver)
{
    JVExecFrame *runFrame;

    runFrame= new JVExecFrame(method, receiver->category());
    runFrame->setLocalValue(0, receiver);
    pushExecFrame(runFrame);
}


void JVMachine::run(void)
{
// ATTN: This is called after a prepareMethod, which must have set the first runFrame.
    if (execFrame != NULL) {
	while (!execFrame->isFinished()) {
	    doInstruction();
	}
	delete execFrame;
    }
}


void JVMachine::setStartMethod(JVMethod *method)
{
// ATTENTION: To be implemented.
}


JVInstance *JVMachine::theNullObject(void)
{
    return kernel->nullConstant();
}


JVInstance *JVMachine::createObject(ClassFile *objClass)
{
    JVInstance *result= JVAbstractInstance::nullObject;

    result= poolManager->getInstance(objClass);
    return result;
}


int JVMachine::stackArgument(int metaType, void *value)
{
// ATTENTION: To be implemented.
    return 0;
}


int JVMachine::runClInit(ClassFile *aClass)
{
    MethodInfo *methodInfo;
    JVMethod *method;
    JVExecFrame *initExecFrame;
    int result= 1;

    methodInfo= aClass->getLocalMethodNamed("<clinit>");
    if (methodInfo) {
#if defined(DBGLEVEL_1)
	if (debugInfo->traceLevel() & DebugInfo::classInit) {
	    std::cout << "+++++++++ Class initialization of ";
	    aClass->describe(0);
	}
#endif
	method= methodInfo->resolve(kernel->getClassManager());
	if (method) {
	    initExecFrame= new JVExecFrame(method, aClass);
	    initExecFrame->setInitializer(true);
	    pushExecFrame(initExecFrame);
	    while (!initExecFrame->isFinished()) {
		doInstruction();
#if defined(_WIN32)
		std::cout.flush();
#endif
	    }
	    delete initExecFrame;
	 }
	else result= -2;
#if defined(DBGLEVEL_1)
	if (debugInfo->traceLevel() & DebugInfo::classInit) {
	    std::cout << "+++++++++ End of Class Initialization of ";
	    aClass->describe(0);
	}
#endif
     }
    else result= -1;

    return result;
}


JVVariable *JVMachine::getFieldOfClass(ClassFile *aClass, char *name)
{
    FieldInfo *aField;
    JVVariable *fieldHandle= NULL;

    aField= aClass->getFieldNamed(name);
    if (aField != NULL) {
	fieldHandle= aField->resolve(kernel->getClassManager());
    }

   return fieldHandle;
}


void JVMachine::raiseException(ClassFile *exception)
{
    // ATTENTION: To implement !
    std::cout.flush();
    exceptionRaised= true;
}


void JVMachine::setWorkContext(JVThread *aThread)
{
    if ((workContext= aThread) != NULL) workContext->machine= this;
}


JVAbstractThread *JVMachine::getActiveThread(void)
{
    if (workContext != NULL) {
	return workContext;
    }
    else {
	return kernel->getCurrentActiveThread();
    }
}


JVAbstractThread *JVMachine::getThreadForID(unsigned int anID)
{
    if (workContext != NULL) {
	if (workContext->getID() == anID) return workContext;    // Warning: quick return.
    }
    return kernel->getThreadForID(anID);
}


JVThread *JVMachine::getFreeThread(void)
{
   return kernel->newThreadFrom(workContext);
}


ClassManager *JVMachine::getClassManager(void)
{
    return kernel->getClassManager();
}


PoolManager *JVMachine::getPoolManager(void)
{
    return poolManager;
}


JVInstance *JVMachine::createStringFromNarrowArray(char *theBytes)
{
    JVInstance *aString;
    unsigned int lettersLength;

    if (theBytes == NULL) {
	// ATTN: Should throw an 'NullCharArray' exception.
	return kernel->nullConstant();		// Warning: quick exit.
    }
    lettersLength= strlen(theBytes);
    if ((aString= poolManager->getInstance(kernel->getStringClass())) != NULL) {
	JVBasicArray *charArray;
	JVRunValue tmpValue;

	charArray= poolManager->createBasicArrayInstance(JVM_TYPE_CHAR, lettersLength+2);
    
	tmpValue.setType(TYPE_ARRAY | TYPE_BASIC_ARRAY);
	tmpValue.setValue((void *)charArray);
	aString->putAtIndex(tmpValue, 0);
    
	tmpValue.setType(TYPE_BASIC);
	tmpValue.setValue(NULL);
	aString->putAtIndex(tmpValue, 1);
    
	tmpValue.setType(TYPE_BASIC);
	tmpValue.setValue((void *)lettersLength);
	aString->putAtIndex(tmpValue, 2);
	    // ATTN: We make sure that there is a trailing '\0', just for C compat.
	charArray->byteCopy((unsigned char *)theBytes, lettersLength+1);
	return aString;
    }

	// ATTN: Should throw an 'OutOfMemory' exception.
    return kernel->nullConstant();
}


JVMethod *JVMachine::getMethodInClass(char *methodName, char *methodType, char *className)
{
    ClassFile *ownerClass;
    MethodInfo *info;
    JVMethod *theMethod= NULL;
    

// ATTN: Should throw exceptions NoSuchClass, NoSuchMethod, CantResolveMethod.

	// Find the class.
    if ((ownerClass= kernel->findClass(className)) != NULL) {
	    // Find the method.
	if ((info= ownerClass->getMethodNamed(methodName, methodType)) != NULL) {
	    theMethod= info->resolve(kernel->getClassManager());
	}
    }
    return theMethod;
}


void JVMachine::runMethodWithArguments(JVInstance *receiver, JVMethod *aMethod, unsigned int nbrArgs, void **arguments)
{
    JVExecFrame *runFrame;

    runFrame= new JVExecFrame(aMethod, aMethod->getImplementor());
    runFrame->setLocalValue(0, receiver);
    runFrame->setEntryPoint();
    for (unsigned int i= 0; i < nbrArgs; i++) {
	runFrame->setLocalValue(i+1, arguments[i]);
    }
    pushExecFrame(runFrame);
    while (!runFrame->isFinished()) {
	doInstruction();
     }

    delete runFrame;
}


void JVMachine::runMethodWithArguments(JVInstance *receiver, JVMethod *aMethod, unsigned int nbrArgs, void **arguments, char *typing)
{
    JVExecFrame *runFrame;

    runFrame= new JVExecFrame(aMethod, aMethod->getImplementor());
    runFrame->setLocalValue(0, receiver);
    runFrame->setEntryPoint();
    for (unsigned int i= 0; i < nbrArgs; i++) {
	switch (typing[i]) {
	    case 'L':	// Instance.
		runFrame->setLocalValue(i+1, arguments[i]);
		break;
	    case 'I':	// Integer.
		runFrame->setLocalBasicValue(i+1, (int)arguments[i]);
		break;
	    case 'J':	// Long.
		runFrame->setLocalLongValue(i+1, (int)arguments[i], (int)arguments[i+1]);
		i++;
		break;
	}
    }
    pushExecFrame(runFrame);
    while (!runFrame->isFinished()) {
	doInstruction();
     }

    delete runFrame;
}


void JVMachine::setTraceLevel(DebugInfo::SectionTracing aLevel)
{
    debugInfo->setTraceSection(aLevel);
}


JVAbstractThread::LauncherType JVMachine::getLauncher(void)
{
    return jvMachine;
}


ClassFile *JVMachine::getClassFile(char *aName)
{
    return kernel->loadClass(aName);
}


JVSynchronizer *JVMachine::addSynchroTo(JVAbstractInstance *anInstance)
{
    if (kernel->putSynchroOn(anInstance)) {
	return anInstance->getSynchronizer();
    }
    else {
	return NULL;
    }
}


