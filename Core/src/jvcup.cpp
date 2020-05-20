/**************************************************
* File: jvcup.cc.
* Desc: Implementation of the run-time global logic.
* Module: AkraLog : JavaKit.
* Rev: 3 mai 1996 : REV 0 : Hugo DesRosiers : Creation.
**************************************************/

extern "C" {
#include <string.h>
#include <stdlib.h>
}
#if defined(_WIN32)
#include <windows.h>
#endif

#include <threader/threadManager.h>

#include "classManager.h"
#include "machineSupport.h"
#include "poolManager.h"
#include "jvInstance.h"
#include "knownDuals.h"
#if defined(USE_FCT_MACHINE)
#include "machine2.h"
#endif
#include "runtimeDefs.h"
#include "fsmThread.h"
#include "fsMachine.h"
#include "fsmSupport.h"
#include "stateInstance.h"
#include "jvThread.h"
#include "stacks.h"

#if defined(NeXT)
#include <threader/nextstep/nsThrMgr.h>
#elif defined(_WIN32)
#include <threader/mswin/winntThrMgr.h>
#elif defined(POSIXTHREAD)
#include <threader/posix/posixThrMgr.h>
#endif

#include "jvcup.h"

/**************************************************
* Implementation: JVCup.
**************************************************/

unsigned int JVCup::majorVersion= 0;
unsigned int JVCup::minorVersion= 1;

JVCup::JVCup()
{
    options= new JVCupOptions;
    options->setDefaultOptions();

    classManager= new ClassManager(this);
    poolManager= new PoolManager(this);
#if defined(NeXT)
    threadManager= new NsThreadManager(this);
#elif defined(_WIN32)
    threadManager= new WntThreadManager(this);
#elif defined(MSDOS)
    threadManager= new JVThreadManager(this);
#elif defined(POSIXTHREAD)
    threadManager= new PosixThreadManager(this);
#elif defined(SYSV)
    threadManager= new JVThreadManager(this);
#endif

    JVAbstractThread::setThreadManager(threadManager);

    status= 0;
    isTesting= false;
}


JVCup::~JVCup()
{
    unsigned int i;

    if (defaultMachine != NULL) {
	delete defaultMachine;
    }

    delete classManager;
    delete options;
}


int JVCup::start()
{
    ClassFile *receiverClass;
    JVMethod *method, *initMethod;
    JVInstance *receiver;	// Initial receiver.
    int result= 0;
    unsigned int nbrArgs;
    char *mainClassName;
    bool isVerbose= (options->getOption("v") != NULL);

    if ((mainClassName= options->getMainClass()) == NULL) return -2;


    if (options->getOption("t")) {

#if defined(DBGLEVEL_3)
    if (isVerbose) {    // Verbose mode.
	std::cout << "Cup: doing a test load pass.\n";
    }
#endif

	isTesting= true;
	result= doTestPass(mainClassName);
    }
    else {
	// Build argument list.
	// Find methods that have the same number of arguments.
	// Find method that have the correct signature.

#if defined(DBGLEVEL_3)
    if (isVerbose) {    // Verbose mode.
	std::cout << "Cup: loading the target class.\n";
    }
#endif

	if ((receiverClass= loadClass(mainClassName)) != NULL) {

#if defined(DBGLEVEL_3)
    if (isVerbose) {    // Verbose mode.
	std::cout << "Cup: target class loaded.\n";
	std::cout << "Cup: looking for the initial method (no arguments).\n";
    }
#endif

	    if ((nbrArgs= (unsigned int)options->getOption("M")) > 0) {
		method= getMethodNamed((char *)options->getOption("m"), "([Ljava/lang/String;)V", mainClassName);
		// TODO: Prepare the call later on to hand over arguments.
	    }
	    else {
		method= getMethodNamed((char *)options->getOption("m"), "()V", mainClassName);
	    }

	/* ATTN: Here, the machines should be replaced by the threads, since
	* they are the basic unit of execution.  A machine runs within a thread, as
	* a function call.  But none the less, it is good to keep a stack of machines,
	* since they represent a 'loaded' object (application) ready to use.
	*/
	    if ((defaultMachine != NULL) && (method != NULL)) {

#if defined(DBGLEVEL_3)
    if (isVerbose) {    // Verbose mode.
	std::cout << "Cup: got the initial method.\n";
    }
#endif

		// Start thread for the method to run in.
		if (!method->isStatic()) {
		    receiver= defaultMachine->createObject(receiverClass);
		    if (strcmp(method->getName(), "<init>") != 0) {
			initMethod= getMethodNamed("<init>", "()V", mainClassName);
			if (initMethod) {

#if defined(DBGLEVEL_3)
    if (isVerbose) {    // Verbose mode.
	std::cout << "Cup: running the initialization method on the new instance.\n";
    }
#endif

				// MOD-970520 [HD]: Pour NT.
			    defaultMachine->setTraceLevel((DebugInfo::SectionTracing)(unsigned int)options->getOption("d"));
			    defaultMachine->runMethod(initMethod, receiver);
			}
			else {
			    std::cout << "No initializer for " << mainClassName << ".\n";
			}
		    }
#if defined(DBGLEVEL_3)
    if (isVerbose) {    // Verbose mode.
	std::cout << "Cup: running main method\n";
    }
#endif
		    defaultMachine->runMethod(method, receiver);
		}
		else {
		    defaultMachine->setTraceLevel((DebugInfo::SectionTracing)(unsigned int)options->getOption("d"));

#if defined(DBGLEVEL_3)
    if (isVerbose) {    // Verbose mode.
	std::cout << "Cup: running main method\n";
    }
#endif

		    defaultMachine->runMethod(method, NULL);
		}
#if defined(DBGLEVEL_3)
    if (isVerbose) {    // Verbose mode.
	std::cout << "Cup: main method exited, terminating.\n";
    }
#endif
	    }
	    else {	// No method or machines.
	    }
	}
	else {		// Receiver class not found.
	}
    }

    threadManager->waitJavaThreads();
    return result;
}


int JVCup::stop()
{
//  TODO: Must end whatever the cup is doing.
    return 0;
}


JVInstance *JVCup::nullConstant(void)
{
    return nullObject;
}


ClassFile * JVCup::loadClass(char *name)
{
    PathList *pathList;
    Path *path;
    ClassFile *result;

    if ((result= findClass(name)) == NULL) {	// Not in-core, try to load it.
	pathList= options->classPath();
	if (pathList != NULL) {
	    path= pathList->getFirst();
	    while (path && (result == NULL)) {
		if (path->getTypeIndex() == 0) {		// If it is a local path.
		    if (checkClassPresenceAt(name, path)) {
			result= readClassAtLocation(name, path);
		     }
		 }
		path= pathList->getNext();
	     }
	 }
     }

    return result;
}


ArrayClassFile *JVCup::loadArrayClass(char *name, unsigned int dimSize)
{
    ArrayClassFile *result= NULL;
    ClassFile *tmpResult;

    if ((tmpResult= loadClass(name)) != NULL) {
	result= new ArrayClassFile(tmpResult, dimSize);
     }

    return result;
}


ClassFile *JVCup::findClass(char *name)
{
    return classManager->findClass(name);
}


ClassManager *JVCup::getClassManager()
{
    return classManager;
}


PoolManager *JVCup::getPoolManager()
{
    return poolManager;
}


JVThreadManager *JVCup::getThreadManager(void)
{
    return threadManager;
}


int JVCup::collectGarbages(int cleaningLevel)
{
    return 0;
}


// Covers for the ClassManager presence.

bool JVCup::resolveExternalClasses(ClassFile *aClass)
{
    return classManager->loadAllReferences(aClass);
}


bool JVCup::checkClassPresenceAt(char *name, Path *path)
{
    return classManager->checkClassPresenceAt(name, path);
}


ClassFile * JVCup::readClassAtLocation(char *name, Path *path)
{
    ClassFile *result;
    if ((result= classManager->readClassAtLocation(name, path)) != NULL) {
	result->resolve(classManager);
	if (result != NULL && !isTesting) defaultMachine->runClInit(result);
     }
    return result;
}



bool JVCup::scanOptions(int argCount, char **arguments)
{
    int tmpRes;
    bool result= true;

    tmpRes= options->scanCommands(arguments, argCount);

    if ((tmpRes < 0) || (tmpRes == 1001)) result= false;
    return result;
}


// ATTENTION: Is not enough to specify one method, must give also desired signature.
JVMethod *JVCup::getMethodNamed(char *aName, char *aTypeDesc, char *aClassName)
{
    ClassFile *theClass;
    MethodInfo *theMethod;
    JVMethod *result= NULL;

    if ((theClass= findClass(aClassName)) != NULL) {
	theMethod= theClass->getMethodNamed(aName, aTypeDesc);
	if (theMethod) {
	    result= theMethod->resolve(classManager);
	 }
     }
    return result;
}


int JVCup::boot()
{
    ClassFile *rootClass;
    int result= 0;
    bool lastSuccess= false;
    bool isVerbose= (options->getOption("v") != NULL);

#if defined(DBGLEVEL_3)
    if (isVerbose) {    // Verbose mode.
	std::cout << "Cup: booting.\n";
    }
#endif

    threadManager->lowLevelInit();

#if defined(USE_FCT_MACHINE)
    defaultMachine= new JVMachine2(this);
#else
//    defaultMachine= new JVMachine(this, threadManager->createNewThreadFrom(NULL));
    defaultMachine= new JVMachine(this);
#endif


    options->testPaths();

#if defined(DBGLEVEL_3)
    if (isVerbose) {    // Verbose mode.
	std::cout << "Cup: trying to get the root class, java.lang.Object...";
    }
#endif

	// Minimalist initialization: get Object class.
    if ((rootClass= loadClass("java/lang/Object")) != NULL) {

#if defined(DBGLEVEL_3)
    if (isVerbose) {    // Verbose mode.
	std::cout << " got it.\n";
    }
#endif

	// Normal runtime initialization: get worlds, then basic classes library.
#if defined(NeXT)
	    classManager->setDefaultWorld(loadDualWorld("kaffee.java.*"));
#elif defined(_WIN32)
	    classManager->setDefaultWorld(loadDualWorld("sun.java.*"));
#elif defined(SYSV)
	    classManager->setDefaultWorld(loadDualWorld("kaffee.java.*"));
#elif defined(MSDOS)
	    classManager->setDefaultWorld(loadDualWorld("kaffee.java.*"));
#endif

#if defined(DBGLEVEL_3)
    if (isVerbose) {    // Verbose mode.
	std::cout << "Cup: trying to load the basic native support library.\n";
    }
#endif

	lastSuccess= loadLibrary("javalib");

#if defined(DBGLEVEL_3)
    if (isVerbose) {    // Verbose mode.
	if (lastSuccess) {
	    std::cout << "Cup: found the basic native support library.\n";
	}
	else {
	    std::cout << "Cup: problem: no basic native support library found !!\n";
	}
    }
#endif

#if defined(DBGLEVEL_3)
    if (isVerbose) {    // Verbose mode.
	std::cout << "Cup: loading basic classes.\n";
    }
#endif

	nullObject= new JVInstance(rootClass);
	JVAbstractInstance::defineNullObject(nullObject);
	stringClass= loadClass("java/lang/String");
	throwableClass= loadClass("java/lang/Throwable");

	if ((stringClass == NULL) || (throwableClass == NULL)) {
	    return -3;		// Can't run without basic support !
	}

	    // Attention: Up to this point, the default machine doesn't have a thread context,
	    // so anything that uses it must not call j.l.Thread.currentThread !
	if ((result= initThreading()) != 0) {
	    return result;
	}

#if defined(DBGLEVEL_3)
    if (isVerbose) {    // Verbose mode.
	std::cout << "Cup: basic classes loaded.\n";
    }
#endif

#if defined(DBGLEVEL_3)
    if (isVerbose) {    // Verbose mode.
	std::cout << "Cup: processing arguments.\n";
    }
#endif

/* MOD-980114 [HD]: In JDK 1.1.5 (probably from 1.1), the System.initializeSystemClass method
* is to be called after thread initialization to setup the various properties.
* NOTE: This should be done only if 'wanted' (that is, if we are running the JDK that needs it).
*/
	JVMethod *initSystemClassMethod;
	if ((initSystemClassMethod= getMethodNamed("initializeSystemClass", "()V", "java/lang/System")) != NULL) {
	    defaultMachine->runMethod(initSystemClassMethod, NULL);
	}
	
	configEnvironment();		// According to the options.

    }
    else result= -1;	// No root class.

    return result;
}


bool JVCup::configEnvironment(void)
{
    unsigned int nbrClass, i;
    char **classNames;

    nbrClass= options->classes(&classNames);

	// MOD-970520 [HD]: Pour NT.
    defaultMachine->setTraceLevel((DebugInfo::SectionTracing)(unsigned int)options->getOption("d"));

    if (nbrClass > 0) {
	for (i= 0; i < nbrClass; i++) {
	    if (loadClass(classNames[i]) == NULL) {
		std::cout << "Error: could not load class " << classNames[i] << ".\n";
	     }
	 }
     }
	 return true;	/* MOD-960730 [HD]: ENNT: Manquait le retour. */
}


StringInfo *JVCup::createStringConstant(char *aConstantString)
{
    StringInfo *result;
    JVBasicArray *tmpArray;

    result= new StringInfo;
    result->setConstantValue(poolManager->getInstance(stringClass));
    tmpArray= poolManager->createBasicArrayInstance(JVM_TYPE_CHAR, strlen(aConstantString)+2);
    result->setActualData(tmpArray, aConstantString);
    delete[] aConstantString;
    return result;
}


bool JVCup::loadLibrary(char *libName)
{
    unsigned int libNameLength, libPathLength;
    char fileName[1024], *originalLibsPath, *libsPath, destroyEnvPath[1024], *nextStop;
    bool result= false, tryMorePath, gotMorePath= true;

    if (fileName != NULL) {    
	strcpy(fileName, libName);
	libNameLength= strlen(libName);
	strcpy(fileName + libNameLength, DYNLIB_EXTENSION);
	tryMorePath= true;
	libsPath= destroyEnvPath;
	if ((originalLibsPath= getenv(DYNLIBPATH)) == NULL) {
	    strcpy(destroyEnvPath, ".");
	    libPathLength= 1;
	}
	else {
	    strncpy(destroyEnvPath, originalLibsPath, 1023);
	    destroyEnvPath[1023]= '\0';
	    libPathLength= strlen(destroyEnvPath);
	}
	do {
		// Build a new path.
	    if ((nextStop= strchr(libsPath, PATH_SEPARATOR)) != NULL) {
		*nextStop= '\0';
		gotMorePath= false;
	    }
	    else {
		nextStop= &libsPath[strlen(libsPath)];
		    // MOD-970520 [HD]: Was missing, caused infinite loops.
		tryMorePath= false;
	    }
	    strcpy(fileName, libsPath);
	    fileName[nextStop - libsPath]= FILE_SEPARATOR;
	    strcpy(&fileName[nextStop - libsPath + 1], libName);
	    strcpy(&fileName[nextStop - libsPath + libNameLength + 1], DYNLIB_EXTENSION);
		// MOD-970520 [HD]: Pour NT.
	    if ((unsigned int)(nextStop - libsPath) < libPathLength) {	    // ATTN: Should make sure that there are never wrap arounds.
		libsPath= nextStop + 1;
	    }
	    else {
		libsPath= NULL;
	    }
	    if (classManager->loadDynamicLibrary(fileName)) {
		result= true;
		tryMorePath= false;
	    }	    
	} while (tryMorePath);
    }
   return result;
}


JVDualWorld *JVCup::loadDualWorld(char *aName)
{
    JVDualWorld *aDual= NULL;

    if (strcmp(aName, "sun.java.*") == 0) {
	aDual= new SunDualWorld();
	classManager->addDualRepresentation(aDual);
    }
    else if (strcmp(aName, "kaffee.java.*") == 0) {
	aDual= new KaffeeDualWorld();
	classManager->addDualRepresentation(aDual);
    }
    else {
	aDual= classManager->loadDualWorld(aName);
    }

    return aDual;
}


JVAbstractThread *JVCup::getCurrentActiveThread(void)
{
    return threadManager->currentThread();
}


JVAbstractThread *JVCup::getThreadForID(unsigned int anID)
{
    return threadManager->getThreadForID(anID);
}


JVThread *JVCup::newThreadFrom(JVThread *aThread)
{
    JVThread *newThread;
	// ATTN-970526 [HD]: Must review what kind of thread we are dealing with.
    newThread= (JVThread *)threadManager->spawnThreadFrom(aThread);
    new JVMachine(this, newThread);
    return newThread;
}


ClassFile *JVCup::getStringClass(void)
{
    return stringClass;
}


JVCupOptions *JVCup::runtimeOptions(void)
{
    return options;
}


void JVCup::spawnFSM(PVStateInstance *anInstance)
{
    MethodInfo *startInfo;

	// ATTN: Can there be more than one start state ?
    if ((startInfo= anInstance->category()->getMethodNamed("<pinit>+")) != NULL) {
	PVState *startState;

	if ((startState= (PVState *)startInfo->resolve(classManager)) != NULL) {
	    PVFsmThread *newThread;
	    PVFSMachine *newMachine;
    
	    newThread= threadManager->createNewFsmThread();
	    newMachine= new PVFSMachine(this, newThread);
	    newMachine->initForState(startState, anInstance);
	    newThread->setFSMachine(newMachine);
	    anInstance->addThread(newThread);
	    newThread->start();
	}
	else {
	    std::cout << "ERRMSG: JVCup: Can't resolve the start state.\n";
	}
    }
    else {
	std::cout << "ERRMSG: JVCup: Can't find the start state to spawn a new FSM.\n";
    }
}



// For debugging purpose.
void JVCup::describe(char *mainClass, unsigned int verbosity)
{
    unsigned int nbrClasses, i;

    nbrClasses= classManager->nbrLoadedClasses();
    std::cout << "Loaded " << nbrClasses << " classes.\n";
    if (verbosity != 0) {
	ClassFile *classFile;

	for (i= 0; i < nbrClasses; i++) {
	    if ((classFile= classManager->getClass(i)) != NULL) {
		if (classFile->isMainName(mainClass)) {
		    classManager->describeClass(i, verbosity);
		    std::cout << "\n";
		    break;
		}
	    }
	 }
     }
}


int JVCup::doTestPass(char *mainClass)
{
    ClassFile *tmpClass;
    unsigned int nbrClasses, i;

    nbrClasses= classManager->nbrLoadedClasses();
    for (i= 0; i < nbrClasses; i++) {
	tmpClass= classManager->getClass(i);
	resolveExternalClasses(tmpClass);
     }

    describe(mainClass, (unsigned int)options->getOption("D"));

    return 0;
}


int JVCup::initThreading(void)
{
#if defined(OLD_CODE)
    MethodInfo *initInfo= NULL;
    JVMethod *initMethod;
    JVInstance *javaThread, *javaGroup;
    JVThread *baseThread;
    JVRunValue *tmpValue;
#endif
    int result;

    if ((result= threadManager->initJavaSupport()) == 0) {
	/* Start threads for: class loader, pool manager thread,
	*	native function resolver, micro-code compiler,
	*	object finalizer (?), message centre.
	*/
    }

    return result;

#if defined(OLD_CODE)
    javaGroup= new JVInstance(threadGroupClass);
    initInfo= threadGroupClass->getMethodNamed("<init>", "()V");
    if (initInfo != NULL) {
	initMethod= initInfo->resolve(classManager);
	if (initMethod != NULL) {
	    defaultMachine->runMethod(initMethod, javaGroup);
	}
    }
    javaThread= new JVInstance(threadClass);
    baseThread= (JVThread *)defaultMachine->getActiveThread();
    baseThread->setJavaDual(javaThread);
    tmpValue= new JVRunValue(TYPE_BASIC, (void *)baseThread->getID());
    javaThread->putAtIndex(*tmpValue, 3);
    tmpValue->setType(TYPE_INSTANCE);
    tmpValue->setValue(javaGroup);
    javaThread->putAtIndex(*tmpValue, 10);
    initInfo= threadClass->getMethodNamed("<init>", "()V");
    if (initInfo != NULL) {
	initMethod= initInfo->resolve(classManager);
	if (initMethod != NULL) {
	    defaultMachine->runMethod(initMethod, javaThread);
	}
    }
    delete tmpValue;

    return 0;	// No error detection yet.
#endif
}


bool JVCup::putSynchroOn(JVAbstractInstance *anInstance)
{
    return threadManager->addSynchronizerFor(anInstance);
}


bool JVCup::putSynchroOn(ClassFile *aClass)
{
    return threadManager->addSynchronizerFor(aClass);
}


JVMachine *JVCup::getDefaultMachine(void)
{
    return defaultMachine;
}


JVInstance *JVCup::createInstance(char *className)
{
    ClassFile *receiverClass;
    JVMethod *initMethod;
    JVInstance *result= NULL;

    if ((receiverClass= loadClass(className)) != NULL) {
	initMethod= getMethodNamed("<init>", "()V", className);
	if ((defaultMachine != NULL) && (initMethod != NULL)) {
	    if ((result= defaultMachine->createObject(receiverClass)) != NULL) {
		// ATTN-980326 [HD]: Should catch j.l.Exceptions !
		defaultMachine->runMethod(initMethod, result);
	    }
	}
    }

    return result;
}


void JVCup::runMethod(char *name, char *typing, JVInstance *receiver, unsigned int nbrArgs, void **arguments)
{
    MethodInfo *info;
    JVMethod *realMethod;

    if ((info= receiver->category()->getMethodNamed(name, typing)) != NULL) {
	realMethod= info->resolve(classManager);
	defaultMachine->runMethodWithArguments(receiver, realMethod, nbrArgs, arguments);
    }
}

