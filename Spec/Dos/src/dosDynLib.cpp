/**************************************************
* File: dosDynLib.h.
* Desc: Definition of the DOSDynLoader, DOSDynamicLibrary class.
* Module: AkraLog : JavaKit.
* Rev: 29 octobre 1996 : REV 0 : Hugo DesRosiers : Creation.
**************************************************/

#if defined(DBGLEVEL_3)
#include <iostream.h>
#endif

#if defined(MSDOS)
#include <string.h>
#endif

#include "dosDynLib.h"
#include <DosSupport/DynLoad/coffFile.h>
#include <DosSupport/DynLoad/loader.h>


/**************************************************
* Implementation: DOSDynamicLibrary.
**************************************************/

CoffLinker *DOSDynamicLibrary::linker= new CoffLinker();


DOSDynamicLibrary::DOSDynamicLibrary(char *aName) : JVDynamicLibrary()
{
    static boolean isFirstPass= true;

    coffHandle= NULL;
    JVDynamicLibrary::init(aName);
}


DOSDynamicLibrary::~DOSDynamicLibrary(void)
{
    if (coffHandle != NULL) 
	delete coffHandle;

   this-> JVDynamicLibrary::~JVDynamicLibrary();
}


bool DOSDynamicLibrary::loadLibrary(char *fileName)
{

    if ((coffHandle= linker->loadFile(fileName)) != NULL) {
	loaded= true;
     }
     else loaded= false;

#if defined(DBGLEVEL_DYNLIB)
    cout << "MSDOS::loadLibrary: tried to load " << fileName << ", it was " << ((loaded) ? "sucessful" : "a failure") << ".\n";
#endif

    return loaded;
}


bool DOSDynamicLibrary::selectSymbol(char *aName, unsigned int (**symbolAddress)(void *, void *))
{
    unsigned int symbolLength;
    char fullSymbolName[250];
    bool gotIt= false;

    fullSymbolName[0]= '_';
    symbolLength= strlen(aName);

	// Try C++ calling convention.
    memcpy(&fullSymbolName[1], aName, symbolLength);
    strcpy(&fullSymbolName[symbolLength + 1], "__FP9JVMachineP11JVExecFrame");
    gotIt= getSymbol(fullSymbolName, symbolAddress);
#if defined(DBGLEVEL_DYNLIB)
    cout << "MSDOS::selectSymbol: tried to find " << fullSymbolName << ", it was " << ((gotIt) ? "sucessful" : "a failure") << ".\n";
#endif
    if (gotIt) return true;

	// Try Microsoft C++ calling convention.
    fullSymbolName[0]= '?';
    strcpy(&fullSymbolName[symbolLength + 1], "@@YAIPAVJVMachine@@PAVJVExecFrame@@@Z");
    gotIt= getSymbol(fullSymbolName, symbolAddress);
#if defined(DBGLEVEL_DYNLIB)
    cout << "MSDOS::selectSymbol: tried to find " << fullSymbolName << ", it was " << ((gotIt) ? "sucessful" : "a failure") << ".\n";
#endif
    if (gotIt) return true;

	// Try C calling convention.
    fullSymbolName[symbolLength + 1]= '\0';
    gotIt= getSymbol(&fullSymbolName[1], symbolAddress);
#if defined(DBGLEVEL_DYNLIB)
    cout << "MSDOS::selectSymbol: tried to find " << fullSymbolName << ", it was " << ((gotIt) ? "sucessful" : "a failure") << ".\n";
#endif

    return gotIt;
}


bool DOSDynamicLibrary::getSymbol(char *aName, unsigned int (**symbolAddress)(void *, void *))
{
    void * tmpAddress= NULL;

    if ((tmpAddress= (void *)coffHandle->findSymbol(aName)) != NULL) {
	*symbolAddress= (unsigned int (*)(void *, void*))tmpAddress;
    }

   return (tmpAddress != NULL);
}


#include <conio.h>
#include <pc.h>
#include <dpmi.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

extern "C" {
void gaga(void)
{
    char *aPointer;

    getcwd();
    srand(1);
    rand();
    kbhit();
    getch();
    fprintf(stdout, "allo");
    sscanf(aPointer, "allo");
    __dpmi_simulate_real_mode_interrupt(0, NULL);
    strncmp("allo", "yeye", 0);
    realloc(aPointer, 100);

    vfprintf(NULL, NULL, NULL);
    getkey();
    __dpmi_physical_address_mapping(NULL);
    __dpmi_set_segment_base_address(0, 0);
    __dpmi_free_physical_address_mapping(NULL);
    __dpmi_free_ldt_descriptor(NULL);
    __dpmi_unlock_linear_region(NULL);
    _go32_dpmi_lock_data(NULL, 0);
}
    
}
