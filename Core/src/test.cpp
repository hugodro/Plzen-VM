/**************************************************
* File: test.cc.
* Desc: Test program for the Java Kit modules.
* Module: AkraLog : JavaKit.
* Rev: 30 avril 1996 : REV 0 : Hugo DesRosiers : Creation.
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

#if defined(MSDOS)
#include <DosSupport/DynLoad/loader.h>
#endif


#if defined(USE_APPKIT)
extern "C" void appKitLink(void);
#endif

//#define USE_MYSQL

#if defined(USE_MYSQL)
#if defined(_WIN32)
typedef unsigned char byte;
#endif
#include <mysql.h>
#endif

#include "jvcup.h"
#include "jmachine.h"

main(int argc, char **argv)
{
    ClassFile *aClass;
    JVMachine *testMachine;
    JVCup cpuOfJava;
    JVMethod *firstMethod;
    unsigned int i;
    int errorCode;


#if defined(NeXT)
#if defined(DEBUG_MEMORY)
    malloc_debug(31);
#endif
#if defined(USE_APPKIT)
     appKitLink();		// Get the appkit to link, so that rld stop complaining about fixed VM stuff.
#endif
#endif

// TMP: Temporary hack to get the exec file in the dyn. linker.  Best would be to read directly from already loaded memory.
#if defined(MSDOS)
    CoffLinker::setExecFilePath(argv[0]);
#endif

#if defined(USE_MYSQL)
    MYSQL *connection;
    char *connArgs[5]= { NULL, NULL, NULL, NULL, NULL };

    connArgs[0]= "localhost";	// TMPTMP.
    if (connArgs[0] != NULL) {
	connection= mysql_connect(NULL, connArgs[0], connArgs[1], connArgs[2]);
	mysql_close(connection);
    }
#endif

    if (cpuOfJava.scanOptions(argc-1, &argv[1])) {
	if ((errorCode= cpuOfJava.boot()) < 0) {
	    switch(errorCode) {
		case -1:
			std::cout << "JVCup: Could not load the java.lang.Object class.\n";
		    break;
		case -2:
		    std::cout << "Plzen Processor, Akra Logic Inc. '96-'2K1; CONFIDENTIAL MATERIAL.\n";
		    std::cout << "JVCup: No main class to run from was given.\n";
		    break;
		case -3:
		    std::cout << "JVCup: Minimum set of java.lang classes are missing.\n";
		    break;
		default:
		    std::cout << "JVCup: Can't do much.\n";
		    break;
	    }
	}
	else {
	    cpuOfJava.start();
	}
     }

#ifdef TEST_1
    debugInfo= 0x080 | 0x001 | 0x020 | 0x040 | 0x0100 | 0x02 | 0x04 | 0x08 | 0x0200;
    aClass= cpuOfJava.loadClass(argv[argc-1]);

    if (aClass != NULL) {
	cpuOfJava.resolveExternalClasses(aClass);
	cpuOfJava.describe(debugInfo);
	if (argc > 3) {
	    firstMethod= cpuOfJava.getMethodNamed(argv[argc-3], argv[argc-2]);
	}
     }
#endif

#ifdef TEST_2
    testClassFile.read(&cin);
    testClassFile.describe();
    for (i= 0; i < testClassFile.nbrMethods(); i++) {
	currentMethod= new JVMethod(testClassFile.findMethod(i));
	std::cout << "Fait methode " << i << ".\n";
	testMachine.runMethod(currentMethod);
	std::cout << "\n";
     }

    std::cout << "Lu " << cin.tellg() << " bytes.\n";
#endif

    exit(0);

#if !defined(_WIN32)
// ATTN: Tmp, this is used to get the linker to load these symbols, since they are used in
//	the java.lang.system native methods.
    {
	struct passwd *passwdInfo;
	if ((passwdInfo= getpwuid(getuid())) != NULL) {
	}
    }
#endif
    return 0;
}

#if !defined(_WIN32)
#if !defined(__linux__)
#include <libc.h>
#endif
#if !defined(MSDOS)
#include <sys/ioctl.h>
#endif
#include <math.h>
#else
#include <winsock.h>
#include <process.h>
#endif

// To make egcs 2 happy with all declarations.
#if defined(__linux__)
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <ctype.h>
#include <netdb.h>
#endif


void fake(void)
{
#if !defined(_WIN32)
    gethostname(NULL, 0);
    gettimeofday(NULL, NULL);
#if !defined(MSDOS)
// Things that MSDOS can't find.
    gethostbyname(NULL);
    ioctl(0, 0, 0);
    send(0, NULL, 0, 0);
    recv(0, NULL, 0, 0);
#endif

// Things required by povsup.lib !
    atoi(NULL);
    fgetc(NULL);
    fgets(NULL, 0, NULL);
    fopen(NULL, NULL);
    fread(NULL, 0, 0, NULL);
    freopen(NULL, NULL, NULL);
    fscanf(NULL, NULL, NULL);
    fseek(NULL, 0, 0);
    ftell(NULL);
    gettimeofday(NULL, NULL);
    memcmp(NULL, NULL, 0);
    rename(NULL, NULL);
    setvbuf(NULL, NULL, 0, 0);
    sscanf(NULL, NULL, NULL);
    strlen(NULL);
    strncat(NULL, NULL, 0);
    system(NULL);
    tolower(64);
    toupper(64);
    ungetc(0, NULL);
    acos(0.0);
    asin(0.0);
    atan(0.0);
    atan2(0.0, 0.0);
    ceil(0.0);
    cos(0.0);
    cosh(0.0);
    exp(0.0);
    floor(0.0);
    log(0.0);
    pow(0.0, 0.0);
    sin(0.0);
    sinh(0.0);
    sqrt(0.0);
    tan(0.0);
    fmod(0.0, 1.0);
    printf(NULL);

// Things required by postgres libpq.
    dup(0);
    fdopen(0, NULL);
    fputs(NULL, NULL);

#endif
    getpid();
    extern int h_error;
}
