/**************************************************
* File: cmLoadLib.cc.
* Desc: Definition of a manager of ClassFile.
* Module: AkraLog : JavaKit.
* Rev: 3 mai 1996 : REV 0 : Hugo DesRosiers : Creation.
**************************************************/

#include "dynamicLibrary.h"
#if defined(NeXT)
#include "nextstep/nextDynLib.h"
#elif defined(_WIN32)
#include "mswin/winntDynLib.h"
#elif defined(MSDOS)
//#include "winntDynLib.h"
#include "dos/dosDynLib.h"
#elif defined(SYSV)
#include "sysv/sysvDynLib.h"
#endif
#include "classManager.h"


bool ClassManager::loadDynamicLibrary(char *fileName)
{
#if defined(NeXT)
    NeXTDynamicLibrary *newLib= new NeXTDynamicLibrary(fileName);
#elif defined(_WIN32)
    WinNTDynamicLibrary *newLib= new WinNTDynamicLibrary(fileName);
#elif defined(MSDOS)
    DOSDynamicLibrary *newLib= new DOSDynamicLibrary(fileName);
#elif defined(SYSV)
    SysVDynamicLibrary *newLib= new SysVDynamicLibrary(fileName);
#endif
    bool result;

    if (!(result= newLib->isLoaded())) {
	delete newLib;
    }
    else {
	newLib->setNext(loadedLibraries);
	loadedLibraries= newLib;
    }
    return result;
}
