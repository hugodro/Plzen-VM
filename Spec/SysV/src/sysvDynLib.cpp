/**************************************************
* File: sysvDynLib.cc.
* Desc: Definition of the SysVDynamicLibrary class (in fact, for the GNU compiler libs).
* Module: AkraLog : JavaKit.
* Rev: 2 avril 1997 : REV 0 : Hugo DesRosiers : Creation.
**************************************************/

#if defined(__linux__)
#include <string.h>
#endif

#include <strings.h>

#include <iostream>

#include "sysvDynLib.h"
#include <stdio.h>

SysVDynamicLibrary::SysVDynamicLibrary(char *aName) : JVDynamicLibrary()
{
    libHandle= NULL;
    JVDynamicLibrary::init(aName);

}


SysVDynamicLibrary::~SysVDynamicLibrary(void)
{
    if (libHandle != NULL) {
      dlclose(libHandle);
    }
    this->JVDynamicLibrary::~JVDynamicLibrary();
}


bool SysVDynamicLibrary::loadLibrary(char *fileName)
{
    libHandle= dlopen(fileName, RTLD_NOW | RTLD_GLOBAL);
    
    loaded= (libHandle != NULL);
    
    if ( !loaded ) {
	fprintf(stderr,"DLOPEN: %s %s \n", fileName, dlerror() );
    }
    return loaded;
}


bool SysVDynamicLibrary::selectSymbol(char *aName, unsigned int (**symbolAddress)(void *, void *))
{
    char fullSymbolName[250];

	// Try C calling convention.
    strcpy(&fullSymbolName[0], aName);
    if (getSymbol(fullSymbolName, symbolAddress)) return true;

	// Try C++ calling convention.
    // strcat(&fullSymbolName[0], "__FP9JVMachineP11JVExecFrame");
    strcat(&fullSymbolName[0], "P9JVMachineP11JVExecFrame");
    if (getSymbol(fullSymbolName, symbolAddress)) return true;

    return false;
}


bool SysVDynamicLibrary::getSymbol(char *aName, unsigned int (**symbolAddress)(void *, void *))
{
  
  *symbolAddress = (unsigned int (*)(void*,void*)) dlsym(libHandle, aName);
#if defined(DEBUG_2)
  if ( *symbolAddress == NULL ) {
    fprintf(stderr,"DLSYM: %s %s\n", aName, dlerror() );
  }
#endif

  return ( *symbolAddress != NULL);

/*
    symbolAddress= (unsigned long *)dlsym(libHandle, "maFonction__FUi");
    if (symbolAddress != NULL) {
    std::cout << "symbol: " << symbolAddress << ".\n";
      (*(unsigned int (*)(unsigned int))symbolAddress)(10);
    }
    else {
    std::cout << "erreur dl: " << dlerror() << ".\n";
    }
    return (symbolAddress != NULL);
*/
}












