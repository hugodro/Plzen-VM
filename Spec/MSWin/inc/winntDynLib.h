#ifndef _WINNTDYNLIB_H_
#define _WINNTDYNLIB_H_
/**************************************************
* File: winntDynLib.h.
* Desc: Definition of the WinNTDynLoader class.
* Module: AkraLog : JavaKit.
* Rev: 29 octobre 1996 : REV 0 : Hugo DesRosiers : Creation.
**************************************************/

extern "C" {
#include <windows.h>
}

#include "dynamicLibrary.h"

class JVDualWorld;


class ClassExport WinNTDynamicLibrary : public JVDynamicLibrary {
  protected:
    HINSTANCE dllHandle;

  public:
    WinNTDynamicLibrary(char *aName);
    virtual ~WinNTDynamicLibrary(void);
    virtual bool loadLibrary(char *fileName);
    virtual bool selectSymbol(char *aName, unsigned int (**symbolAddress)(void *, void *));
    virtual bool getSymbol(char *aName, unsigned int (**symbolAddress)(void *, void *));
};


#endif		/* _WINNTDYNLIB_H_ */
