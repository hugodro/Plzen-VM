#ifndef _RUNTIMEDEFS_H_
#define _RUNTIMEDEFS_H_
/**************************************************
* File: runtimeDefs.h.
* Desc: Definition of the run-time values that are plate-form dependant.
* Module: AkraLog : JavaKit.
* Rev: 11 mai 1997 : REV 0 : Hugo DesRosiers : Creation.
**************************************************/

#if defined(NeXT)
#define DYNLIB_EXTENSION	".lib"
#define PATH_SEPARATOR	':'
#define FILE_SEPARATOR		'/'
#elif defined(_WIN32)
#define DYNLIB_EXTENSION	".dll"
#define PATH_SEPARATOR	';'
#define FILE_SEPARATOR		'\\'
#elif defined(SYSV) || defined(__linux__)
#define DYNLIB_EXTENSION	".so"
#define PATH_SEPARATOR	':'
#define FILE_SEPARATOR		'/'
#elif defined(MSDOS)
#define DYNLIB_EXTENSION	".ddl"
#define PATH_SEPARATOR	';'
#define FILE_SEPARATOR		'\\'
#endif

#define PLZEN_HOME		"PLZENHOME"
#define JAVA_HOME		"JAVAHOME"
#define CLASSPATH		"CLASSPATH"
#define DYNLIBPATH		"DYNLIBPATH"

#endif		/* _RUNTIMEDEFS_H_ */
