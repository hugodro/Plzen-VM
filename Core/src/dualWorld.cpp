/**************************************************
* File: dualWorld.cc.
* Desc: Class implementation for DualWorld, DualObject.
* Module: AkraLog : JavaKit.
* Rev: 25 oct 1996 : REV 0 : Hugo DesRosiers : Creation.
**************************************************/

#include <string.h>
#include "jvInstance.h"
#include "dualWorld.h"


/**************************************************
* Implementation: JVDualMapper.
**************************************************/

JVDualMapper::JVDualMapper(void)
{
    nbrKnownClasses= 0;
    knownClasses= NULL;
    importFunction= NULL;
    exportFunction= NULL;
    frameMap= NULL;
}


JVDualMapper::~JVDualMapper(void)
{
    if (nbrKnownClasses > 0) {
	delete[] knownClasses;
    }
}


bool JVDualMapper::isClassMapable(ClassFile *aClass)
{
    bool result= false;

    if (nbrKnownClasses > 0) {
	for (unsigned int i = 0; i < nbrKnownClasses; i++) {
	    if (knownClasses[i] == aClass) {
		result= true;
		break;
	    }
	}
    }

    return result;
}


bool JVDualMapper::setMaping(bool (*importer)(JVInstance *anInstance, void *someData), bool (*exporter)(JVInstance *anInstance, void *someData), bool (*frameMapper)(JVExecFrame *aFrame))
{
    importFunction= importer;
    exportFunction= exporter;
    frameMap= frameMapper;

    return true;		// ATTN: Should return something more meaningful !
}


void JVDualMapper::addClass(ClassFile *aClass)
{
    if ((nbrKnownClasses % 5) == 0) {
	ClassFile **tmpArray;

	tmpArray= knownClasses;
	knownClasses= new ClassFile*[nbrKnownClasses + 5];
	memcpy(knownClasses, tmpArray, nbrKnownClasses * sizeof(ClassFile *));
	delete[] tmpArray;	// ATTN-971124 [HD]: This is a bug ??
    }
    knownClasses[nbrKnownClasses++]= aClass;
}


bool JVDualMapper::mapToPlzen(JVInstance *anInstance, void *someData)
{
    return importFunction(anInstance, someData);
}


bool JVDualMapper::mapFromPlzen(JVInstance *anInstance, void *someData)
{
// ATTENTION: Ou s'en va le data dual ??
    return exportFunction(anInstance, someData);
}


/**************************************************
* Implementation: JVDualWorld.
**************************************************/

JVDualWorld::JVDualWorld()
{
    nbrMaps= 0;
    maps= NULL;
    
}


JVDualWorld::JVDualWorld(char *aName)
{
    ourName= new char[strlen(aName) + 1];
    strcpy(ourName, aName);
    nbrMaps= 0;
    maps= NULL;
    
}


JVDualWorld::~JVDualWorld(void)
{
    if (nbrMaps > 0) {
	delete[] maps;
    }
    delete[] ourName;
}


bool JVDualWorld::mapTo(JVInstance *anInstance, void *someData)
{
    for (unsigned int i= 0; i < nbrMaps; i++) {
	if (maps[i]->isClassMapable(anInstance->category())) {
	    return maps[i]->mapFromPlzen(anInstance, someData);
	}
    }

   return false;
}


bool JVDualWorld::mapBack(JVInstance *anInstance, void *someData)
{
    for (unsigned int i= 0; i < nbrMaps; i++) {
	if (maps[i]->isClassMapable(anInstance->category())) {
	    return maps[i]->mapToPlzen(anInstance, someData);
	}
    }

   return false;
}


bool JVDualWorld::isClassMapable(ClassFile *aClass)
{
    bool result= false;

    for (unsigned int i= 0; i < nbrMaps; i++) {
	if (maps[i]->isClassMapable(aClass)) {
	    result= true;
	    break;
	}
    }

   return result;
}


bool JVDualWorld::translateMethodName(JVNativeMethod *aMethod, char *localName)
{
    return false;
}


JVDualWorld *JVDualWorld::getNext(void)
{
    return next;
}


void JVDualWorld::setNext(JVDualWorld *nextComponent)
{
    next= nextComponent;
}


bool JVDualWorld::isNamed(char *aName)
{
    return (strcmp(aName, ourName) == 0);		// Is this OK for all name schemes ?
}

