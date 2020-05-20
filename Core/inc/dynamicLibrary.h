#ifndef _DYNAMICLIBRARY_H_
#define _DYNAMICLIBRARY_H_
/**************************************************
* File: dynamicLibrary.h.
* Desc: Definition of the JVDynamicLibrary class.
* Module: AkraLog : JavaKit.
* Rev: 29 octobre 1996 : REV 0 : Hugo DesRosiers : Creation.
**************************************************/

#include <akra/AkObject.h>
#include <akra/portableDefs.h>
#include "cDefinitions.h"


class ClassExport JVDynamicLibrary : public AkObject {
  protected:
    char *ourName;				// The file we got loaded from.
    bool loaded;				// Indicates if the lib is correctly loaded.
    JVDynamicLibrary *next;		// Next item in the list of currently loaded library.

  public:
    JVDynamicLibrary(void);
    JVDynamicLibrary(char *fileName);
    virtual ~JVDynamicLibrary(void);
    virtual void init(char *aName);
    virtual bool loadLibrary(char *fileName);
    virtual bool isLoaded(void);
    virtual bool selectSymbol(char *aName, unsigned int (**symbolAddress)(void *, void *));
    virtual bool getSymbol(char *aName, unsigned int (**symbolAddress)(void *, void *));
    virtual JVDynamicLibrary *getNext(void);
    virtual void setNext(JVDynamicLibrary *aDynLib);
};


#endif		/* _DYNAMICLIBRARY_H_ */
