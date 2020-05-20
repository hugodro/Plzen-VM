/**************************************************
* File: dynamicLibrary.h.
* Desc: Definition of the JVDynamicLibrary class.
* Module: AkraLog : JavaKit.
* Rev: 29 octobre 1996 : REV 0 : Hugo DesRosiers : Creation.
**************************************************/

#include <string.h>

#include "runtimeDefs.h"
#include "dynamicLibrary.h"


JVDynamicLibrary::JVDynamicLibrary(void)
{
    ourName= NULL;
    loaded= false;
}


JVDynamicLibrary::JVDynamicLibrary(char *fileName)
{
    init(fileName);
}


JVDynamicLibrary::~JVDynamicLibrary(void)
{
    delete[] ourName;
}


void JVDynamicLibrary::init(char *aName)
{
    unsigned int nameLength;
    char *tmpNamePtr, *tmpExtPtr;

    ourName= NULL;
    loaded= false;
    if (loadLibrary(aName)) {
	tmpNamePtr= strrchr(aName, FILE_SEPARATOR);
	if (tmpNamePtr != NULL) {
	    tmpNamePtr++;
	}
	tmpExtPtr= strrchr(aName, '.');
    
	if (tmpNamePtr == NULL) tmpNamePtr= aName;
	if (tmpExtPtr == NULL) {
	    nameLength= strlen(tmpNamePtr);
	}
	else {
	    nameLength= tmpExtPtr - tmpNamePtr;
	}
    
	ourName= new char[nameLength+1];
	memcpy(ourName, tmpNamePtr, nameLength);
	ourName[nameLength]= '\0';
    }

}


bool JVDynamicLibrary::loadLibrary(char *fileName)
{
    return false;
}


bool JVDynamicLibrary::isLoaded(void)
{
    return loaded;
}


bool JVDynamicLibrary::selectSymbol(char *aName, unsigned int (**symbolAddress)(void *, void *))
{
    return false;
}


bool JVDynamicLibrary::getSymbol(char *aName, unsigned int (**symbolAddress)(void *, void *))
{
    return false;
}


JVDynamicLibrary *JVDynamicLibrary::getNext(void)
{
    return next;
}


void JVDynamicLibrary::setNext(JVDynamicLibrary *aDynLib)
{
    next= aDynLib;
}


