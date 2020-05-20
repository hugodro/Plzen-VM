/**************************************************
* File: dynLoaders.cc.
* Desc: Definition of the NeXTDynamicLibrary class (in fact, for the GNU compiler libs).
* Module: AkraLog : JavaKit.
* Rev: 29 octobre 1996 : REV 0 : Hugo DesRosiers : Creation.
**************************************************/

#include <strings.h>
#include "nextDynLib.h"


NXStream *NeXTDynamicLibrary::outputStream= NULL;

#define VERBOSE_RLD	1

NeXTDynamicLibrary::NeXTDynamicLibrary(char *aName) : JVDynamicLibrary()
{
    header= NULL;
#if defined(VERBOSE_RLD)
    outputStream= NXOpenFile(fileno(stdout), NX_WRITEONLY);
#endif
    JVDynamicLibrary::init(aName);

}


NeXTDynamicLibrary::~NeXTDynamicLibrary(void)
{

    if (outputStream != NULL) {
	NXClose(outputStream);
    }

    this->JVDynamicLibrary::~JVDynamicLibrary();
}


bool NeXTDynamicLibrary::loadLibrary(char *fileName)
{

    char *inputName[2];

    inputName[0]= fileName;
    inputName[1]= NULL;
    loaded= rld_load(outputStream, &header, inputName, NULL) != 0;
#if defined(VERBOSE_RLD)
    NXFlush(outputStream);
#endif
    return loaded;
}


bool NeXTDynamicLibrary::selectSymbol(char *aName, unsigned int (**symbolAddress)(void *, void *))
{
    char fullSymbolName[250];

    fullSymbolName[0]= '_';
    fullSymbolName[1]= '\0';

	// Try C calling convention.
    strcat(&fullSymbolName[1], aName);
    if (getSymbol(fullSymbolName, symbolAddress)) return true;

	// Try C++ calling convention.
    strcat(&fullSymbolName[1], "__FP9JVMachineP11JVExecFrame");
    if (getSymbol(fullSymbolName, symbolAddress)) return true;

    return false;
}


bool NeXTDynamicLibrary::getSymbol(char *aName, unsigned int (**symbolAddress)(void *, void *))
{
    return (rld_lookup(NULL, aName, (long unsigned int *)symbolAddress) == 1);
}


