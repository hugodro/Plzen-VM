/**************************************************
* File: runOptions.cc.
* Desc: Implementation of the run-time options class.
* Module: AkraLog : JavaKit.
* Rev: 3 mai 1996 : REV 0 : Hugo DesRosiers : Creation.
**************************************************/

extern "C" {
#include <stdlib.h>
#if defined(NeXT)
#include <libc.h>
#endif
#if defined(_WIN32)
#include <stdlib.h>
#include <direct.h>
#endif

// MOD-970507 [JST]: pour getcwd:
#if defined(__linux__) || defined(MSDOS)
#include <unistd.h>
#endif

#include <string.h>
 }

#include <iostream>

#include "jvcup.h"
#include "runOptions.h"
#include "runtimeDefs.h"


/**************************************************
* Implementation: JVCupOptions.
**************************************************/


// Scan the command line and set options accordingly.
int JVCupOptions::scanCommands(char **arguments, unsigned int nbrArgs)
{
    int result= 0;		// result == 1001 if we must end program.
    unsigned int i, j, k;
    char *tmpClassName[256];		// Can load up to 256 classes.
    char *tmpArgVal[256];			// Can specify up to 256 arguments.

    for (j= 1, i= 0; i < nbrArgs;) {
      if (arguments[i][0] == '-') {
	switch(arguments[i][j]) {
	   case 'h':    /* Give program documentation. */
	       i= nbrArgs-1; j++;
	       Document(arguments[0], 1);
	       result= 1001;
	      break;
	   case 'V':    /* Give program version info. */
	       i= nbrArgs-1; j++;
	       Document(arguments[0], 0);
	       result= 1001;
	      break;
	    case 'v':    /* Mode VERBOSE. */
		j++;
		verbose= true;
		break;
	   case 'm':    /* Method to start with */
		i++;
		if (i < nbrArgs) {
		    methodName= new char[strlen(arguments[i]) + 1];
		    strcpy(methodName, arguments[i++]);
		 }
		else {
		    std::cout << "option -m: no method name was specified.\n";
		 }
		j= 1;
		break;
	   case 'a':    /* Argument for method. */
		i++;
		if (i < nbrArgs) {
		    tmpArgVal[nbrMethodArgs]= new char[strlen(arguments[i]) + 1];
		    strcpy(tmpArgVal[nbrMethodArgs], arguments[i++]);
		    nbrMethodArgs++;
		 }
		else {
		    std::cout << "option -a: no argument was specified.\n";
		 }
		j= 1;
		break;
	   case 't':    /* Test mode. */
		i++;
		testMode= true;
		j= 1;
		break;
	   case 'd':    /* Give the debug level to run at. */
	       i++; j= 1;
	       if (i < nbrArgs) {
		    k= atoi(arguments[i++]);
		    if (k < 32) {
			debugSections|= (1 << (k-1));
		     }
		}
	      else {
		    std::cout << "option -d: no debugging section was specified.\n";
	      	}
	      break;
	   case 'D':    /* Give the description level to use when debugging or testing. */
	       i++; j= 1;
	       if (i < nbrArgs) {
		    k= atoi(arguments[i++]);
		    if (k < 32) {
			descSections|= (1 << (k-1));
		     }
		}
	      else {
		    std::cout << "option -D: no description level was specified.\n";
	      	}
	      break;
	   case 'C':    /* Give the description level to use when debugging or testing. */
	       i++; j= 1;
	       if (i < nbrArgs) {
		    k= atoi(arguments[i++]);
		    if (k < 32) {
			classDbgSection|= (1 << (k-1));
		     }
		}
	      else {
		    std::cout << "option -D: no description level was specified.\n";
	      	}
	      break;
	   default:
	      /* Unknown option ! */
		std::cout << "Unknown option [" << arguments[i] << "].\n";
		j++;
		result= -1;
		break;
	 }
        if (i < nbrArgs) {
	    if (!arguments[i][j]) {
		i++;
		j= 1;
	     }
	 }
       }	// End of the scan of a '-' something.
      else {	// We got something else than an option spec.
	    // For now, assume that it is a class to load.
	tmpClassName[nbrClassesToLoad]= new char[strlen(arguments[i]) +1];
	strcpy(tmpClassName[nbrClassesToLoad], arguments[i]);
	nbrClassesToLoad++;
	j= 1; i++;
       }
    }	/* Fin du for nbrArgs. */


    if (nbrClassesToLoad > 0) {
	classToLoad= new char*[nbrClassesToLoad];
	for (i= 0; i < nbrClassesToLoad; i++) {
	    classToLoad[i]= tmpClassName[i];
	 }
     }
    if (nbrMethodArgs > 0) {
	methodArguments= new char*[nbrMethodArgs];
	for (i= 0; i < nbrMethodArgs; i++) {
	    methodArguments[i]= tmpArgVal[i];
	 }
     }
    return 0;
}


// Set the options to their default values.
void JVCupOptions::setDefaultOptions(void)
{
// MOD-970520 [HD]: Modified the Unix data ('/', ':') to fit multi-OS environment, using defines and char[2] blocks.
    char currentPath[1024], *envPath, fileSeparator[2]= { FILE_SEPARATOR, '\0' };

    searchPath= new PathList;

/* MOD-970624 [HD]: Took out the default paths.
    searchPath->addPath("file:/usr/local/packages/");
    searchPath->addPath("file:/usr/local/packages/java/lang/");
*/
    
// MOD-970415 [JST]: Uses the CLASSPATH global var for finding class files.
    envPath = getenv( CLASSPATH );
    if ( envPath != NULL ) { // parse colon separated list
      char destroyEnv[1024];
      char *tok;
      char pathSeparator[2]= { PATH_SEPARATOR, '\0' };

      strncpy( destroyEnv, envPath, 1023 );
      destroyEnv[1023] = '\0';
      tok = strtok( destroyEnv, pathSeparator );
      while ( tok != NULL ) {
	strcpy( currentPath, "file:" ); // reuse currentPath
	strcat( currentPath, tok );
/* MOD-970624 [HD]: No more necessary to add trailing path sep.
	strcat( currentPath, fileSeparator );
*/
	searchPath->addPath( currentPath );
	tok = strtok( NULL, pathSeparator );
      }
    }

    strcpy(currentPath, "file:");

#ifdef NeXT
    getwd(currentPath+5);
#else
    getcwd(currentPath+5, 1024 - 5);
#endif
    strcat(currentPath, fileSeparator);

    searchPath->addPath(currentPath);

    verbose= false;
    testMode= false;
    descSections= 0;
    debugSections= 0;
    classDbgSection= 0;
    methodName= "main";
    nbrMethodArgs= 0;
    nbrClassesToLoad= 0;
}

// Set option 'name' to its default value.
void JVCupOptions::setDefaultOptions(char *name)
{

}


// Set option 'name' to the value 'value'.
int JVCupOptions::setOption(char *name, void *value)
{
    return 0;
}


// Get value of option 'name'.
void * JVCupOptions::getOption(char *name)
{
    switch (*name) {
	case 'm':	return (void *)methodName;
	case 'M': return (void *)nbrMethodArgs;
	case 'v': return (void *)verbose;
	case 't': return (void *)testMode;
	case 'd': return (void *)debugSections;
	case 'D': return (void *)descSections;
	case 'C': return (void *)classDbgSection;
	default: break;
     }
    return 0;
}


PathList *JVCupOptions::classPath()
{
    return searchPath;
}


unsigned int JVCupOptions::classes(char ***dest)
{
    if (nbrClassesToLoad > 0) *dest= classToLoad;
    return nbrClassesToLoad;
}


unsigned int JVCupOptions::methodArgs(char ***dest)
{
    if (nbrMethodArgs > 0) *dest= methodArguments;
    return nbrMethodArgs;
}

char *JVCupOptions::getMainClass(void)
{
    char *result= NULL;

    if (nbrClassesToLoad > 0) {
	result= classToLoad[nbrClassesToLoad-1];
     }

    return result;
}


void JVCupOptions::Document(char *progName, unsigned int level)
{

    std::cout << "Plzen Processor, Akra Logic Inc. '96-'2K1.  CONFIDENTIAL VERSION.\n";
    std::cout << "Version " << JVCup::getMajorVersion() << "." << JVCup::getMinorVersion() << ".\n";
    if (level > 0) {
      std::cout << "Use: " << progName << " [options] <classNames>\n";
      std::cout << "Options are:\n";
      std::cout << "-h: give this documentation only.\n";
      std::cout << "-m <name>: specify method to call on boot.\n";
      std::cout << "-a <value>: specify an argument for the method.\n";
      std::cout << "-v: turn on verbose mode.\n";
      std::cout << "-t: do a test run.\n";
      std::cout << "-d <n>: turn on debugging section <n>.\n";
      std::cout << "-D <n>: give description on component <n> when loading a class.\n";
      std::cout << "-C <n>: turn on class debugging section <n>.\n";
    }
}


void JVCupOptions::testPaths(void)
{
    Path *path;

    if (searchPath != NULL) {
	path= searchPath->getFirst();
	while (path) {
	    path->test();
	    path= searchPath->getNext();
	}
    }
}

