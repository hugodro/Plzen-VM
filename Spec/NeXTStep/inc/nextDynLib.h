#ifndef _NEXTDYNLIB_H_
#define _NEXTDYNLIB_H_
/**************************************************
* File: nextDynLib.h.
* Desc: Definition of the NeXTDynLoader class.
* Module: AkraLog : JavaKit.
* Rev: 29 octobre 1996 : REV 0 : Hugo DesRosiers : Creation.
**************************************************/

extern "C" {
#include <mach-o/rld.h>
#include <streams/streams.h>
#include <stdio.h>
}

#include "dynamicLibrary.h"


class NeXTDynamicLibrary : public JVDynamicLibrary {
  protected:
    static NXStream *outputStream;

  protected:
    mach_header *header;

  public:
    NeXTDynamicLibrary(char *aName);
    virtual ~NeXTDynamicLibrary(void);
    virtual bool loadLibrary(char *fileName);
    virtual bool selectSymbol(char *aName, unsigned int (**symbolAddress)(void *, void *));
    virtual bool getSymbol(char *aName, unsigned int (**symbolAddress)(void *, void *));
};


#endif		/* _NEXTDYNLIB_H_ */
