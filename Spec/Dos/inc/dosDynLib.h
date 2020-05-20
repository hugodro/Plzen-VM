#ifndef _DOSDYNLIB_H_
#define _DOSDYNLIB_H_
/**************************************************
* File: dosDynLib.h.
* Desc: Definition of the DOSDynLoader class.
* Module: AkraLog : JavaKit.
* Rev: 29 octobre 1996 : REV 0 : Hugo DesRosiers : Creation.
**************************************************/

#include "dynamicLibrary.h"


class JVDualWorld;
class CoffFile;
class CoffLinker;


class DOSDynamicLibrary : public JVDynamicLibrary {
  private:
    static CoffLinker *linker;
  protected:		// Instance variables.
    CoffFile *coffHandle;

  public:
    DOSDynamicLibrary(char *aName);
    virtual ~DOSDynamicLibrary(void);
    virtual bool loadLibrary(char *fileName);
    virtual bool selectSymbol(char *aName, unsigned int (**symbolAddress)(void *, void *));
    virtual bool getSymbol(char *aName, unsigned int (**symbolAddress)(void *, void *));
};


#endif		/* _DOSDYNLIB_H_ */
