/**************************************************
* File: winntDynLib.h.
* Desc: Definition of the WinNTDynLoader, WinNTDynamicLibrary class.
* Module: AkraLog : JavaKit.
* Rev: 29 octobre 1996 : REV 0 : Hugo DesRosiers : Creation.
**************************************************/

#if defined(DBGLEVEL_3)
#include <iostream.h>
#endif
#include <windows.h>
#include "winntDynLib.h"


/**************************************************
* Implementation: WinNTDynamicLibrary.
**************************************************/

WinNTDynamicLibrary::WinNTDynamicLibrary(char *aName) : JVDynamicLibrary()
{
    dllHandle= NULL;
    JVDynamicLibrary::init(aName);
}


WinNTDynamicLibrary::~WinNTDynamicLibrary(void)
{
    if (dllHandle) 
	FreeLibrary(dllHandle);
    JVDynamicLibrary::~JVDynamicLibrary();
}


/**************************************************
* Implementation: WinNTDynamicLibrary.
**************************************************/

bool WinNTDynamicLibrary::loadLibrary(char *fileName)
{
    if ((dllHandle= LoadLibraryEx(fileName, NULL, LOAD_WITH_ALTERED_SEARCH_PATH)) != NULL) {
	loaded= true;
     }
     else loaded= false;

#if defined(DBGLEVEL_DYNLIB)
    cout << "WinNT::loadLibrary: tried to load " << fileName << ", it was " << ((loaded) ? "sucessful" : "a failure") << ".\n";
#endif

    return loaded;
}


bool WinNTDynamicLibrary::selectSymbol(char *aName, unsigned int (**symbolAddress)(void *, void *))
{
    char fullSymbolName[250];
    bool gotIt= false;

    fullSymbolName[0]= '?';

	// Try C calling convention.
    strcpy(&fullSymbolName[1], aName);
    gotIt= getSymbol(&fullSymbolName[1], symbolAddress);
#if defined(DBGLEVEL_DYNLIB)
    cout << "WinNT::selectSymbol: tried to find " << fullSymbolName << ", it was " << ((gotIt) ? "sucessful" : "a failure") << ".\n";
#endif
    if (gotIt) return true;

	// Try Microsoft C++ calling convention.
    strcat(&fullSymbolName[1], "@@YAIPAVJVMachine@@PAVJVExecFrame@@@Z");
    gotIt= getSymbol(fullSymbolName, symbolAddress);
#if defined(DBGLEVEL_DYNLIB)
    cout << "WinNT::selectSymbol: tried to find " << fullSymbolName << ", it was " << ((gotIt) ? "sucessful" : "a failure") << ".\n";
#endif

    return gotIt;
}


bool WinNTDynamicLibrary::getSymbol(char *aName, unsigned int (**symbolAddress)(void *, void *))
{
    FARPROC tmpAddress= NULL;

    if ((tmpAddress= GetProcAddress(dllHandle, aName)) != NULL) {
	*symbolAddress= (unsigned int (*)(void *, void*))tmpAddress;
    }

   return (tmpAddress != NULL);
}


