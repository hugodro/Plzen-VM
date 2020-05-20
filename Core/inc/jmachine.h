#ifndef _JMACHINE_H_
#define _JMACHINE_H_
/**************************************************
* File: jmachine.h.
* Desc: Definition of the Java-Machine.
* Module: AkraLog : JavaKit.
* Rev: 2 mai 1996 : REV 0 : Hugo DesRosiers : Creation.
* Rev: 21 aout 1998 : REV 0.1 : Hugo DesRosiers :
*	Added a new runMethodWithArgs method.
**************************************************/

#include <akra/AkMachine.h>
#include <threader/rawThreads.h>

#include "machineSupport.h"
#include "debugInfo.h"
#include "classFile.h"

class JVOperStack;
class JVExecFrame;
class PoolManager;
class JVCup;
class JVInstance;
class JVRunValue;
class JVVariable;
class JVThread;
class JVAbstractThread;
class DebugInfo;
class Value64Bits;


/********************************************************
* Class: JVMachine.
* Desc: Definition of the run-time components used to describe a Java Virtual Machine.
* Super-Class: AkMachine.
* Module: AkraLog : JavaKit.
********************************************************/

class JVMachine : public AkMachine {
  public:
    static void jvMachine(void *receiver);

  protected:
//    JVVarRegister vars;
//    JVOpStackRegister optop;
//    JVExecRegister frame;

    JVCup *kernel;
    JVThread *workContext;		// Thread of work this machine operates in.
    PoolManager *poolManager;	// Pool manager in charge of getting us new instances.

// MOD-970628 [HD]: Mis public pour acceder de system.cpp:initProperties.
  public:
    JVOperStack *operStack;		// Working stack.

  protected:
    JVExecFrame *execFrame;	// Bottom of exec frame list.
    unsigned int vindex;			// Wide-index used for longer addresses.
    							// ATTN: Should be part of the exec frame (for simulated threading environment).
    bool exceptionRaised;		// Indicates if we are in exception mode.

// Work in progress.
    DebugInfo *debugInfo;		// Debugging context.
    DebugInfo::SectionTracing traceLevel;		// Trace level of this machine.

  public:
    JVMachine(void);
    JVMachine(JVCup *owner);
    JVMachine(JVCup *owner, JVThread *aThread);
    virtual ~JVMachine(void);
    virtual void init(JVCup *owner, JVThread *context);
    virtual void pushExecFrame(JVExecFrame *newFrame);
    virtual void popExecFrame(void);
    virtual void popExecFrame(unsigned int aValue);
    virtual void popExecFrame(Value64Bits *aValue);
    virtual void popExecFrame(JVRunValue &aValue);
    virtual int runMethod(JVMethod *method, JVInstance *receiver);
    virtual void prepareMethod(JVMethod *method, JVInstance *receiver);
    virtual void run(void);
    virtual int doInstruction(void);
    virtual void setStartMethod(JVMethod *method);
    virtual JVInstance *theNullObject(void);
    virtual JVInstance *createObject(ClassFile *objClass);
    virtual int stackArgument(int metaType, void *value);
    virtual int runClInit(ClassFile *aClass);

    virtual JVVariable *getFieldOfClass(ClassFile *aClass, char *name);
    virtual void raiseException(ClassFile *exception);
    virtual void setWorkContext(JVThread *aThread);
    virtual JVAbstractThread *getActiveThread(void);
    virtual JVAbstractThread *getThreadForID(unsigned int anID);
    virtual JVThread *getFreeThread(void);

    virtual ClassManager *getClassManager(void);
    virtual PoolManager *getPoolManager(void);
    virtual JVInstance *createStringFromNarrowArray(char *theBytes);
    virtual JVMethod *getMethodInClass(char *methodName, char *methodType, char *className);
    virtual void runMethodWithArguments(JVInstance *receiver, JVMethod *aMethod, unsigned int nbrArgs, void **arguments);
    virtual void runMethodWithArguments(JVInstance *receiver, JVMethod *aMethod, unsigned int nbrArgs, void **arguments, char *typing);
    virtual void setTraceLevel(DebugInfo::SectionTracing aLevel);

    virtual JVAbstractThread::LauncherType getLauncher(void);
    virtual ClassFile *getClassFile(char *aName);

// New stuff, work in progress.
/*ATTN: To add for native support:
    getMethodInClass(char *methodName, char *methodType, ClassFile *aClass);
    createRawInstanceOf(ClassFile *);
    createInstanceOf(ClassFile *);	// Roule l'initialisateur <init>()V.
    virtual boolean raiseException(char *aClassName);
*/
    virtual JVSynchronizer *addSynchroTo(JVAbstractInstance *anInstance);
};

#endif	/* _JMACHINE_H_ */

