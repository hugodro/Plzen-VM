#ifndef _CDEFINITIONS_H_
#define _CDEFINITIONS_H_
/**************************************************
* File: cDefintions.h.
* Desc: Missing 'normal' definitions from the C++ compiler environment.
* Module: AkraLog : JavaKit.
* Rev: 3 mai 1996 : REV 0 : Hugo DesRosiers : Creation.
**************************************************/

#if !defined(true)
#define true	1
#endif
#if !defined(false)
#define false	0
#endif

#if !defined(__linux__) && !defined(MSDOS) && !defined(_WIN32)
typedef unsigned char bool;
#endif

typedef unsigned char ubyte;

#if !defined(NULL)
#define NULL		0
#endif

#endif
