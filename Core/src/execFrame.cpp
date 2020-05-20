/**************************************************
* File: execFrame.h.
* Desc: Definition of the execution frame for a machine.
* Module: AkraLog : JavaKit.
* Rev: 5 mai 1996 : REV 0 : Hugo DesRosiers : Creation.
**************************************************/

#include <iostream>

#include "classFile.h"
#include "stacks.h"
#include "machineSupport.h"
#include "runInfo.h"
#include "execFrame.h"


/**************************************************
* Implementation: JVExecFrame.
**************************************************/


JVExecFrame::JVExecFrame()
{
    init();
}


JVExecFrame::JVExecFrame(JVMethod *aMethod, ClassFile *source)
{
    init();
    method= aMethod;
    currentClass= source;
    constantPool= currentClass->getConstantPool();
    if ((method->flags & ACC_NATIVE) || (method->flags & ACC_ABSTRACT)) {
	unsigned int i, j= aMethod->nbrArguments + aMethod->nbrLongArgs + 1;

	localValues= new JVRunValue*[j];
	localValues[0]= new JVRunValue[j];
	for (i= 1; i < j; i++) {
	    localValues[i]= localValues[i-1] + 1;
	}
     }
    else if (aMethod->nbrLocals > 0) {
	localValues= new JVRunValue*[aMethod->nbrLocals];
	localValues[0]= new JVRunValue[aMethod->nbrLocals];
	for (unsigned int i= 1; i < aMethod->nbrLocals; i++) {
	    localValues[i]= localValues[i-1] + 1;
	}
    }
    codeSize= aMethod->codeLength;
    instructions= aMethod->code;
    frameDepth= 0;
}


JVExecFrame::~JVExecFrame()
{
    if (method->nbrLocals > 0) {
	delete[] localValues[0];
	delete[] localValues;
     }
}


void JVExecFrame::init()
{
    programCounter= 0;
    codeSize= 0;
    last= NULL;
    flags= 0;
    synchro= NULL;
}


void JVExecFrame::setFlowOn(ubyte *code, unsigned int size)
{
    codeSize= size;
    instructions= code;
}


bool JVExecFrame::loadArguments(JVOperStack *sourceStack)
{
    int i;
    unsigned int j;
    bool result= true;

// ATTN-970909 [HD]: It should be ensured that the arg to local values
//		always goes from last to first.

    j= i= method->nbrArguments;
    j+= method->nbrLongArgs;
    for (; i >= 0;) {
	if (!sourceStack->pop(*localValues[j])) {
	    result= false;
	    break;
	}
	if (localValues[j]->metaType == TYPE_BASIC_64L) {
	    j--;
	    if (!sourceStack->pop(*localValues[j])) {
		result= false;
		break;
	    }
	}
	j--;
	i--;
    }
    return result;
}


bool JVExecFrame::loadArgumentsForStatic(JVOperStack *sourceStack)
{
    int i;
    unsigned int j;
    bool result= true;

// ATTN-960819 [HD]: Faut-il faire le 'upload' a partir de la pos '0' ou '1' ?

	// If we get here, it is that there shouldn't be any long values on the stack to put in the
	//	args.
    j= i= method->nbrArguments;
    j+= method->nbrLongArgs;
    j--;
    for (i= method->nbrArguments; i > 0;) {
	if (!sourceStack->pop(*localValues[j])) {
	    result= false;
	    break;
	}
	if (localValues[j]->metaType == TYPE_BASIC_64L) {
	    j--;
	    if (!sourceStack->pop(*localValues[j])) {
		result= false;
		break;
	    }
	}
	j--;
	i--;
    }
    return result;
}


bool JVExecFrame::updateVirtualMethod(JVMethod *newMethod)
{
    JVRunValue **tmpValues;
    bool result= true;

    if (newMethod == NULL) return false;
    if (newMethod->nbrArguments < method->nbrArguments) return false;

    if (newMethod != method) {
	instructions= newMethod->code;
	codeSize= newMethod->codeLength;
	currentClass= newMethod->implementor;
	constantPool= currentClass->getConstantPool();
	if (newMethod->nbrLocals != method->nbrLocals) {
	    tmpValues= localValues;
	    localValues= new JVRunValue*[newMethod->nbrLocals];
	    localValues[0]= new JVRunValue[newMethod->nbrLocals];
	    for (unsigned int i= 1; i < newMethod->nbrLocals; i++) {
		localValues[i]= localValues[i-1] + 1;
	     }
	    if (method->nbrArguments == 0) {	// Handle void argument case.
		*localValues[0]= *tmpValues[0];
	     }
	    else {
		unsigned int nbrToTransfer= (newMethod->isStatic()) ? method->nbrArguments :
									method->nbrArguments + 1;
		for (unsigned int i= 0; i < nbrToTransfer; i++) {
		    *localValues[i]= *tmpValues[i];
		 }
	     }
	    delete[] tmpValues[0];
	    delete[] tmpValues;
	    method= newMethod;
	 }
/* MOD-960825 [HD]: This is not an error condition.
	else if (newMethod->nbrLocals < method->nbrLocals) {
	    result= false;
	 }
*/
     }

    return result;
}


bool JVExecFrame::updateInterfaceMethod(JVMethod *newMethod, ClassFile *source)
{
    bool result= false;

    if (updateVirtualMethod(newMethod)) {
/* MOD-970919 [HD]: Took this out: it is done by updateVirtualMethod, and 'source' is not the thing
	to use (newMethod->implementor is).
	currentClass= source;
*/
	constantPool= currentClass->getConstantPool();
	result= true;
     }
    return result;
}


bool JVExecFrame::setLocalValue(unsigned int i, void *anInstance)
{
    if (i < method->nbrLocals) {
	localValues[i]->setType(TYPE_INSTANCE);
	localValues[i]->setValue(anInstance);
	return true;
     }
    else return false;
}


bool JVExecFrame::setLocalBasicValue(unsigned int i, int aValue)
{
    if (i < method->nbrLocals) {
	localValues[i]->setType(TYPE_BASIC);
	localValues[i]->setValue((void *)aValue);
	return true;
     }
    else return false;
}


bool JVExecFrame::setLocalLongValue(unsigned int i, int highWord, int lowWord)
{
    if (i < (method->nbrLocals - 1)) {
	localValues[i]->setType(TYPE_BASIC_64H);
	localValues[i++]->setValue((void *)highWord);
	localValues[i]->setType(TYPE_BASIC_64L);
	localValues[i]->setValue((void *)lowWord);
	return true;
     }
    else return false;
}



void JVExecFrame::setCurrentPC(unsigned int i)
{
    programCounter= i;
}


void JVExecFrame::incPC(int i)
{
    programCounter+= i;
}


ubyte JVExecFrame::fetch(void)
{
    lastPC= programCounter;	// DEBUG.

    return instructions[programCounter++];
}


ubyte JVExecFrame::fetchByte(void)
{
    return instructions[programCounter++];
}


short JVExecFrame::fetchShort(void)
{
    unsigned int tmpPC;

    tmpPC= programCounter;
    programCounter+= 2;
    return ((((unsigned short)instructions[tmpPC]) << 8) | (unsigned short)instructions[tmpPC+1]);
}


unsigned int JVExecFrame::fetchInteger(void)
{
    unsigned int tmpPC;

    tmpPC= programCounter;
    programCounter+= 4;
    return ((((unsigned int)instructions[tmpPC]) << 24)| (((unsigned int)instructions[tmpPC+1]) << 16) 
		| (((unsigned int)instructions[tmpPC+2]) << 8) | ((unsigned int)instructions[tmpPC+3]));
}


bool JVExecFrame::isFinished(void)
{
    return ((programCounter >= codeSize) || ((flags & sfDeadFrame) != 0));
}


unsigned int JVExecFrame::currentPC(void)
{
    return programCounter;
}


void JVExecFrame::incPCfake(unsigned int i)
{
    if ((lastPC + i) != programCounter) {
	std::cout << "Warning: incPCfake detects PC increment error (should be " << lastPC + i << ").\n";
     }
}


bool JVExecFrame::isInitializer(void)
{
    return ((flags & sfInitializer) != 0);
}


void JVExecFrame::setInitializer(bool aValue)
{
    flags|= (sfInitializer | sfEntryPoint);
}


// Work in progress.
JVRunValue *JVExecFrame::getLocalValue(unsigned int pos)
{
    return localValues[pos];
}


void JVExecFrame::getReturnValue(JVRunValue &destination)
{
    destination= nativeReturnValue;
}


void JVExecFrame::getReturnValue(Value64Bits *aValue)
{
    *aValue= nativeLRValue;
}


void JVExecFrame::returnInteger(unsigned int aValue)
{
    nativeReturnValue.setType(TYPE_BASIC);
    nativeReturnValue.setValue((void *)aValue);
}


void JVExecFrame::returnFloat(float aValue)
{
    nativeReturnValue.setType(TYPE_BASIC);
    nativeReturnValue.setValue((void *)*(unsigned int *)&aValue);
}


void JVExecFrame::returnLong(Value64Bits *aValue)
{
    flags|= sfLongReturn;
    nativeLRValue= *aValue;
}


void JVExecFrame::returnInstance(JVInstance *anInstance)
{
    nativeReturnValue.setType(TYPE_INSTANCE);
    nativeReturnValue.setValue((void *)anInstance);
}


bool JVExecFrame::isEntryPoint(void)
{
    return ((flags & sfEntryPoint) != 0);
}


void JVExecFrame::setEntryPoint(void)
{
    flags|= sfEntryPoint;
}


void JVExecFrame::setDead(void)
{
    flags|= sfDeadFrame;
}



void JVExecFrame::setLaunch(void)
{
    flags|= sfLaunchFSM;
}


void JVExecFrame::setIgnoreLaunch(void)
{
    flags|= sfIgnoreFSM;
}


bool JVExecFrame::isKindOfLaunch(void)
{
    return (((flags & sfLaunchFSM) | (flags & sfIgnoreFSM)) != 0);
}


bool JVExecFrame::isLauncher(void)
{
    return ((flags & sfLaunchFSM) != 0);
}


bool JVExecFrame::hasLongValue(void)
{
    return ((flags & sfLongReturn) != 0);
}

