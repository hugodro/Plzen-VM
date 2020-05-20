#ifndef _SYSVDYNLIB_H_
#define _SYSVDYNLIB_H_
/**************************************************
* File: sysvDynLib.h.
* Desc: Definition of the SysVDynamicLibrary class.
* Module: AkraLog : JavaKit.
* Rev: 29 octobre 1996 : REV 0 : Hugo DesRosiers : Creation.
**************************************************/


#include <dlfcn.h>


#include <vm/dynamicLibrary.h>


class SysVDynamicLibrary : public JVDynamicLibrary {
  protected:

  protected:
    void *libHandle;

  public:
    SysVDynamicLibrary(char *aName);
    virtual ~SysVDynamicLibrary(void);
    virtual bool loadLibrary(char *fileName);
    virtual bool selectSymbol(char *aName,  unsigned int (**symbolAddress)(void *, void *));
    virtual bool getSymbol(char *aName,  unsigned int (**symbolAddress)(void *, void *));
};


#endif		/* _SYSVDYNLIB_H_ */
