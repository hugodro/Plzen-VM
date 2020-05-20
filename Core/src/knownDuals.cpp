/**************************************************
* File: knownDuals.cc.
* Desc: Contains the implementation of classes SunDualWorld and
*	KaffeeDualWorld.
* Module: AkraLog : JavaKit.
* Rev: 29 octobre 1996 : REV 0 : Hugo DesRosiers : Creation.
**************************************************/

#include <string.h>
// MOD-970520 [HD]: Pour NT.
#if !defined(_WIN32) && !defined(MSDOS)
#include <strings.h>
#endif

#include "classFile.h"
#include "jvInstance.h"
#include "machineSupport.h"
#include "knownDuals.h"


/**************************************************
* Implementation: SunGeneralDualMap.
**************************************************/

bool SunGeneralDualMap::generalImport(JVInstance *anInstance, void *someData)
{
    bool result= false;

    return result;
}


bool  SunGeneralDualMap::generalExport(JVInstance *anInstance, void *someData)
{
    bool result= false;

    return result;
}


bool SunGeneralDualMap::generalFrameMap(JVExecFrame *aFrame)
{
    bool result= false;

    return result;
}


SunGeneralDualMap::SunGeneralDualMap(void)
{
    nbrKnownClasses= 0;
    knownClasses= NULL;
    importFunction= SunGeneralDualMap::generalImport;
    exportFunction= SunGeneralDualMap::generalExport;
    frameMap= SunGeneralDualMap::generalFrameMap;
}


SunGeneralDualMap::~SunGeneralDualMap(void)
{
    // rien a detruire pour le moment.
}


bool SunGeneralDualMap::isClassMapable(ClassFile *aClass)
{
    return true;		// We know how to translate any class.
}


bool SunGeneralDualMap::setMapping(bool (*importer)(JVInstance *anInstance, void *someData), bool (*exporter)(JVInstance *anInstance, void *someData), bool (*frameMapper)(JVExecFrame *aFrame))
{
    return false;
}


void SunGeneralDualMap::addClass(ClassFile *aClass)
{

}


/**************************************************
* Implementation: SunDualWorld.
**************************************************/

JVDualMapper *SunDualWorld::mapHolder[1]= { new SunGeneralDualMap() };


SunDualWorld::SunDualWorld(void)
{
    ourName= "sun.java.*";
    nbrMaps= 1;
    maps= mapHolder;
}


SunDualWorld::~SunDualWorld(void)
{
// The last object should delete the SunGeneralDualMap !
    delete mapHolder[0];
}


bool SunDualWorld::translateMethodName(JVNativeMethod *aMethod, char *localName)
{
    unsigned int nameLength;
    char *tmpPos;

    aMethod->implementor->getName(localName);

    do {
	tmpPos= strchr(localName, '/');
	if (tmpPos != NULL) {
	    *tmpPos= '_';
	}
     } while (tmpPos);

    nameLength= strlen(localName);
    localName[nameLength]= '_';
    strcpy(&localName[nameLength+1], aMethod->getName());

    return true;		// Succeds for all classes.
}


/**************************************************
* Implementation: KaffeeGeneralDualMap.
**************************************************/

bool KaffeeGeneralDualMap::generalImport(JVInstance *anInstance, void *someData)
{
    bool result= false;

    return result;
}


bool KaffeeGeneralDualMap::generalExport(JVInstance *anInstance, void *someData)
{
    bool result= false;

    return result;
}


bool KaffeeGeneralDualMap::generalFrameMap(JVExecFrame *aFrame)
{
    bool result= false;

    return result;
}


KaffeeGeneralDualMap::KaffeeGeneralDualMap(void)
{
    nbrKnownClasses= 0;
    knownClasses= NULL;
    importFunction= KaffeeGeneralDualMap::generalImport;
    exportFunction= KaffeeGeneralDualMap::generalExport;
    frameMap= KaffeeGeneralDualMap::generalFrameMap;
}


KaffeeGeneralDualMap::~KaffeeGeneralDualMap(void)
{
    // rien a detruire pour le moment.
}


bool KaffeeGeneralDualMap::isClassMapable(ClassFile *aClass)
{
    return true;		// We know how to translate any class.
}


bool KaffeeGeneralDualMap::setMapping(bool (*importer)(JVInstance *anInstance, void *someData), bool (*exporter)(JVInstance *anInstance, void *someData), bool (*frameMapper)(JVExecFrame *aFrame))
{
    return false;
}


void KaffeeGeneralDualMap::addClass(ClassFile *aClass)
{

}



/**************************************************
* Implementation: KaffeeDualWorld.
**************************************************/

JVDualMapper *KaffeeDualWorld::mapHolder[1]= { new KaffeeGeneralDualMap() };

KaffeeDualWorld::KaffeeDualWorld(void)
{
    ourName= "kaffee.java.*";
    nbrMaps= 1;
    maps= mapHolder;
}


KaffeeDualWorld::~KaffeeDualWorld(void)
{
//    delete mapHolder[0];
// The last object should delete the KaffeGeneralDualMap !
}


bool KaffeeDualWorld::translateMethodName(JVNativeMethod *aMethod, char *localName)
{
    unsigned int nameLength;
    char *tmpPos;

    aMethod->implementor->getName(localName);

    do {
	tmpPos= strchr(localName, '/');
	if (tmpPos != NULL) {
	    *tmpPos= '_';
	}
     } while (tmpPos);

    nameLength= strlen(localName);
    localName[nameLength]= '_';
    strcpy(&localName[nameLength+1], aMethod->getName());

    return true;		// Succeds for all classes.
}


