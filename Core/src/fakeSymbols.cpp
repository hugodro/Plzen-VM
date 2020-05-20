/**************************************************
* File: fakeSymbols.cc.
* Desc: Declares some functions so that they are loaded in the library by the
*	linker.
* Module: AkraLog : JavaKit.
* Rev: 2 juin 1998 : REV 0 : Hugo DesRosiers : Creation.
**************************************************/

extern "C" {
#include <stdlib.h>
#if defined(IRIX) || defined(NeXT) || defined(__linux__)
// MOD-970512 [JST]: pour getpwuid, getuid
#include <pwd.h>
#include <sys/types.h>
#include <unistd.h>
#endif
 }

#if !defined(_WIN32)
#if !defined(__GNUC__)
#include <libc.h>
#endif
#include <sys/ioctl.h>
#include <math.h>
#else
#include <winsock.h>
#include <io.h>
#include <process.h>
#endif
#include "stringSupport.h"

#if defined(__linux__)
#include <sys/time.h>
#endif

void fakeCalls(void)
{
// Things required by postgres libpq.
#if !defined(_WIN32)
    gettimeofday(NULL, NULL);
#endif
    StringSupport::mapStringToNarrowArray(NULL, NULL);
    dup(0);
}
