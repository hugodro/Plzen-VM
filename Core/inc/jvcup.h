#ifndef _JVCUP_H_
#define _JVCUP_H_
/**************************************************
* File: jvcup.h.
* Desc: Definition of the run-time unit, the equivalent of the "full computer".
* Module: AkraLog : JavaKit.
* Rev: 3 mai 1996 : REV 0 : Hugo DesRosiers : Creation.
**************************************************/

#include <akra/AkObject.h>

#include "classFile.h"

#include "runOptions.h"
#include "jmachine.h"

class ClassManager;
class PoolManager;
class JVThreadManager;
class JVThread;
class JVInstance;
class PVStateInstance;
class JVDualWorld;
class JVAbstractInstance;


class JVCup : public AkObject {
  private:
    static unsigned int majorVersion;
    static unsigned int minorVersion;

  protected:
    JVThreadManager *threadManager;
    ClassManager *classManager;
    JVCupOptions *options;		// Runtime options.
    JVInstance *nullObject;		// Global null object.
    ClassFile *stringClass;		// String class.
    ClassFile *throwableClass;	// Throw-catch mechanism base class.
    JVMachine *defaultMachine;	// Default machine to run something.

    // Private message queue.
    // Machines message queue.
    // Shared resources (others).
    PoolManager *poolManager;
    unsigned int status;
    bool isTesting;

  public:
    static unsigned int getMajorVersion() { return majorVersion; }
    static unsigned int getMinorVersion() { return minorVersion; }

  public:
    JVCup();
    virtual ~JVCup();
    virtual int start();
    virtual int stop();
    virtual JVInstance *nullConstant(void);
    virtual ClassFile *loadClass(char *name);
    virtual ArrayClassFile *loadArrayClass(char *name, unsigned int dimSize);
    virtual ClassFile *findClass(char *name);
    virtual ClassManager *getClassManager();
    virtual PoolManager *getPoolManager(void);
    virtual JVThreadManager *getThreadManager(void);

    virtual bool resolveExternalClasses(ClassFile *aClass);
    virtual bool checkClassPresenceAt(char *name, Path *path);
    virtual ClassFile *readClassAtLocation(char *name, Path *path);

    virtual int collectGarbages(int cleaningLevel);

    virtual bool scanOptions(int argCount, char **arguments);
    virtual JVMethod *getMethodNamed(char *aName, char *aTypeDesc, char *aClassName);
    virtual int boot(void);
    virtual bool configEnvironment(void);

    virtual StringInfo *createStringConstant(char *aConstantString);

    bool loadLibrary(char *libName);
    virtual JVDualWorld *loadDualWorld(char *aName);

	// New stuff, work in progress.
    virtual JVAbstractThread *getCurrentActiveThread(void);
    virtual JVAbstractThread *getThreadForID(unsigned int anID);
    virtual JVThread *newThreadFrom(JVThread *aThread);
    virtual ClassFile *getStringClass(void);	// Get the well known string class.
    virtual JVCupOptions *runtimeOptions(void);	    // Return the runtime options.

    virtual void spawnFSM(PVStateInstance *anInstance);    // Start a FSM in the instance.

// For debugging purpose.
    virtual void describe(char *mainClass, unsigned int verbosity);
    virtual int doTestPass(char *mainClass);

    virtual int initThreading(void);
    virtual bool putSynchroOn(JVAbstractInstance *anInstance);
    virtual bool putSynchroOn(ClassFile *aClass);
    virtual JVMachine *getDefaultMachine(void);

    JVInstance *createInstance(char *className);
    virtual void runMethod(char *name, char *typing, JVInstance *receiver, unsigned int nbrArgs, void **arguments);

 };
 
#endif	/* _JVCUP_H_ */
 
