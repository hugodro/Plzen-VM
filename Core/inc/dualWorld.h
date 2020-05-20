#ifndef _DUALWORLD_H_
#define _DUALWORLD_H_
/**************************************************
* File: dualWorld.h.
* Desc: Class definitions for DualWorld, DualObject.
* Module: AkraLog : JavaKit.
* Rev: 25 oct 1996 : REV 0 : Hugo DesRosiers : Creation.
**************************************************/


#include <akra/AkObject.h>
#include "cDefinitions.h"


class ClassFile;
class JVInstance;
class JVExecFrame;
class JVNativeMethod;


/********************************************************
* Class: JVDualMapper.
* Desc: Contains mapping logic for a set of classes that operates in
*	the same way.
* Class variables:
* Instance variables:
* Super-Class: AkObject.
* Module: AkraLog : JavaKit.
********************************************************/

class JVDualMapper : public AkObject {
  protected:
    unsigned int nbrKnownClasses;
    ClassFile **knownClasses;
    bool (*importFunction)(JVInstance *anInstance, void *someData);    // Import data to Plzen world.
    bool (*exportFunction)(JVInstance *anInstance, void *someData);		// Export data to dual world.
    bool (*frameMap)(JVExecFrame *aFrame);

  public:
    JVDualMapper(void);
    virtual ~JVDualMapper(void);
    virtual bool isClassMapable(ClassFile *aClass);
    virtual bool setMaping(bool (*importer)(JVInstance *anInstance, void *someData), bool (*exporter)(JVInstance *anInstance, void *someData), bool (*frameMapper)(JVExecFrame *aFrame));
    virtual void addClass(ClassFile *aClass);
    virtual bool mapToPlzen(JVInstance *anInstance, void *someData);
    virtual bool mapFromPlzen(JVInstance *anInstance, void *someData);
};


/********************************************************
* Class: JVDualWorld.
* Desc: Contains the logic to map classes between Plzen and some other
*	world of data and function representation.
* Class variables:
* Instance variables:
* Super-Class: AkObject.
* Module: AkraLog : JavaKit.
********************************************************/

class JVDualWorld : public AkObject {
  protected:
    char *ourName;
    unsigned int nbrMaps;
    JVDualMapper **maps;
    JVDualWorld *next;		// Next world in a list.

  protected:
    JVDualWorld::JVDualWorld();	// To be used only by subclasses.

  public:
    JVDualWorld(char *aName);
    virtual ~JVDualWorld(void);

    bool mapTo(JVInstance *anInstance, void *someData);    // Sends to dual world.
    bool mapBack(JVInstance *anInstance, void *someData);    // Brings back from dual world.

    virtual bool isClassMapable(ClassFile *aClass);
    virtual bool translateMethodName(JVNativeMethod *aMethod, char *localName);

    JVDualWorld *getNext(void);
    void setNext(JVDualWorld *nextComponent);
    virtual bool isNamed(char *aName);
};



#endif		/* _DUALWORLD_H_ */
