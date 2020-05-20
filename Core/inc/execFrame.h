#ifndef _EXECFRAME_H_
#define _EXECFRAME_H_
/**************************************************
* File: execFrame.h.
* Desc: Definition of the execution frame for a machine.
* Module: AkraLog : JavaKit.
* Rev: 5 mai 1996 : REV 0 : Hugo DesRosiers : Creation.
**************************************************/

#include <akra/AkObject.h>
#include "stacks.h"
#include "cDefinitions.h"
#include "math64bits.h"


class ClassFileConstant;
class ClassFile;
class JVInstance;
class JVSynchronizer;
class JVMethod;
class JVVariable;
class JVRunValue;
class JVOperStack;


/********************************************************
* Class: JVExecFrame.
* Desc: Contains all the necessary information to insure the execution
*	of a code segment by a machine.
* Class variables:
* Instance variables:
* Constants:
*    StatusFlags:
*	sfInitializer		// Indicate if the frame is running a clinit method.
*	sfEntryPoint	    	// Indicate if the frame is an entry point (runMethod call).
*	sfDeadFrame		// Indicate if the frame has finished his job.
* Super-Class: AkObject.
* Module: JavaKit : RunTime.
********************************************************/

class JVExecFrame : public AkObject {
    friend class JVMachine;
#if defined(USE_FCT_MACHINE)
    friend class JVMachine2;
#endif
    friend class PVFSMachine;
  public:		// Constants.
    enum StatusFlags {
	sfInitializer= (1 << 0), sfEntryPoint= (1 << 2), sfDeadFrame= (1 << 3),
	sfLaunchFSM= (1 << 4), sfIgnoreFSM= (1 << 5), sfLongReturn= (1 << 6)
    };

  protected:	// Instance variables.
    JVExecFrame *last;					// Last execution frame (for return values).
    JVMethod *method;					// Method being executed.
    ClassFileConstant **constantPool;		// Constant pool of current class.
    ClassFile *currentClass;				// Current class under which method is run.
    JVRunValue **localValues;			// Local values (of variables).
    JVRunValue nativeReturnValue;		// Value returned by a non-void native call.
    Value64Bits nativeLRValue;	// Long value returned by a non-void native call.
    JVSynchronizer *synchro;				// Monitoring facility for a method.
    int blockFlag;						// Control mechanism for 1st order functions.
    unsigned int mark;					// Mark into the operation stack, for cleaning-up.
    unsigned int programCounter;			// Current position of the byte-code reader.
    ubyte *instructions;					// Byte-codes of the current segment.
    unsigned int codeSize;				// Size of the current segment.
    unsigned int flags;					// Contain some on/off information about the frame.

// ATTN: For debugging purpose.
    unsigned int frameDepth;				// Depth of call stack, for debug.
     unsigned int lastPC;					// Last PC that was used (to check for crazy jumps). 

  public:
    JVExecFrame();
    JVExecFrame(JVMethod *aMethod, ClassFile *source);
    virtual ~JVExecFrame();
    virtual void init();
    virtual void setFlowOn(ubyte *code, unsigned int size);
    virtual bool loadArguments(JVOperStack *sourceStack);
    virtual bool loadArgumentsForStatic(JVOperStack *sourceStack);
    virtual bool setLocalValue(unsigned int i, void *anInstance);
    virtual bool setLocalBasicValue(unsigned int i, int aValue);
    virtual bool setLocalLongValue(unsigned int i, int highWord, int lowWord);
    virtual bool updateVirtualMethod(JVMethod *newMethod);
    virtual bool updateInterfaceMethod(JVMethod *newMethod, ClassFile *source);
    virtual void incPC(int i);
    virtual void setCurrentPC(unsigned int i);
    virtual ubyte fetch(void);
    virtual ubyte fetchByte(void);
    virtual short fetchShort(void);
    virtual unsigned int fetchInteger(void);
    virtual bool isFinished(void);
    virtual unsigned int currentPC(void);
    virtual void incPCfake(unsigned int i);
    virtual bool isInitializer(void);
    virtual void setInitializer(bool aValue);

// Work in progress.
    virtual JVRunValue *getLocalValue(unsigned int pos);
    virtual void getReturnValue(JVRunValue &destination);
    virtual void getReturnValue(Value64Bits *aValue);
    virtual void returnInteger(unsigned int aValue);
    virtual void returnFloat(float aValue);
    virtual void returnLong(Value64Bits *aValue);
    virtual void returnInstance(JVInstance *anInstance);
    virtual bool isEntryPoint(void);
    virtual void setEntryPoint(void);
    virtual void setDead(void);
    virtual void setLaunch(void);
    virtual void setIgnoreLaunch(void);
    virtual bool isKindOfLaunch(void);
    virtual bool isLauncher(void);
    virtual bool hasLongValue(void);
 };


#endif	/* _EXECFRAME_H_ */
