/**************************************************
* File: extraCode.cc.
* Desc: File that force the linker to load some extra symbols.
* Module: Plzen : RunTime.
* Rev: 26 mars 1998 : REV 0 :Hugo DesRosiers : Creation.
**************************************************/
#include <sys/time.h>
#include "stringSupport.h"


void fakeCode(void)
{
    gettimeofday(NULL, NULL);
    StringSupport::mapStringToNarrowArray(NULL, NULL);
}
