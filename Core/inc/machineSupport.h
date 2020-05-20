#ifndef _MACHINESUPPORT_H_
#define _MACHINESUPPORT_H_
/**************************************************
* File: machineSupport.h.
* Desc: Definitions of the objects that are used to support the machine
*	in its operation.
* Module: AkraLog : JavaKit.
* Rev: 9 mai 1996 : REV 0 : Hugo DesRosiers : Creation.
**************************************************/

#include <akra/AkObject.h>
#include "cDefinitions.h"
#include "valueTypes.h"


class ClassFile;
class MethodInfo;
class FieldInfo;
class JVSynchronizer;
class ClassManager;
class JVInstance;
class JVExecFrame;
class JVMachine;
class JVDualWorld;
class JVAbstractInstance;
class TransitionsAttribute;
class ClassFileConstant;
class Value64Bits;
class JVException;
class JVMethodRunInfo;


typedef unsigned char ubyte;

class JVTypeDefinition : public AkObject {
  protected:
    ubyte flags;

  public:
    JVTypeDefinition();

    virtual ubyte metaType(void);
    virtual bool isEqual(JVTypeDefinition *aType);
    virtual unsigned short runType(void);
 };


class JVBasicTypeDef : public JVTypeDefinition {
  protected:
    char basicType;			 // By default, a basic type sets itself to void.

  public:
    JVBasicTypeDef(void);
    JVBasicTypeDef(char aTypeLetter);

    virtual ubyte type(void);
    virtual bool isEqual(JVTypeDefinition *aType);
    virtual unsigned short runType(void);
};


class JVClassTypeDef : public JVTypeDefinition {
  protected:
    char *className;
    ClassFile *inMemClass;

  public:
    JVClassTypeDef(char *aName);
    JVClassTypeDef(char *name, unsigned int nameLength);
    virtual void init(char *aName, unsigned int aLength);
    virtual void setMemClass(ClassFile *aClass);
    virtual void getName(char *dest);
    virtual bool isEqual(JVTypeDefinition *aType);
    virtual unsigned short runType(void);
};


class JVArrayTypeDef : public JVTypeDefinition {
  protected:
    unsigned int size;		// Specification of dim. size (when known).
    bool final;				// Quick assesment that element type is not array.
    JVTypeDefinition *elementType;

  public:
    JVArrayTypeDef(void);
    virtual ~JVArrayTypeDef(void);
    JVArrayTypeDef(JVTypeDefinition *aType, unsigned int optSize);

    virtual void setElementType(JVTypeDefinition *aType, unsigned int optSize);
    virtual bool isEqual(JVTypeDefinition *aType);

    virtual unsigned short runType(void);
    virtual unsigned int dimensions(void);		// Count the nbr of dimension of the array..
    virtual JVTypeDefinition *finalType(void);	// Returns the root type.
};


class JVVariable : public AkObject {
  protected:
    char *name;
    unsigned int flags;	// Flags defining the way the variable lives.
    JVTypeDefinition *isA;	// Definition of the variable class type.
    unsigned int offset;		// Position of the variable in the container object.
    unsigned int size;		// Size of the variable in bytes.
    void *staticValue;
    bool hasStaticValue;

  public:
    JVVariable(ClassFile *owner, FieldInfo *info);

    virtual unsigned int position(void);
    virtual ubyte metaType(void);
    virtual unsigned int varSize(void);
    virtual bool isStatic(void);			// Indicate if the variable is static. 
    virtual bool isFinal(void);			// Indicate if the variable is final. 
    virtual JVInstance *getStaticValue(void);
    virtual unsigned short detailedType(void);

    virtual bool setStaticValue(JVInstance *anInstance);
    virtual bool setStaticValue(Value64Bits *aValue);
    virtual bool resolveTyping(ClassManager *classManager);
    virtual void setPosition(unsigned int i);
 };


class JVMethod : public AkObject {
    friend class ExecFrame;
  public:
    char *name;
    unsigned int flags;
    unsigned int nbrArguments;
    unsigned int nbrLongArgs;
    unsigned int nbrLocals;
    unsigned int stackMaxDepth;
    unsigned int codeLength;
    unsigned int nbrExceptions;
    JVTypeDefinition **argumentTyping;	// Array of (nbrArgs + 1) typing information (0 is return value).
    ubyte *code;
    JVSynchronizer *synchro;
    ClassFile *implementor;
    JVException *exceptHandlers;

  public:
    JVMethod(void);
    JVMethod(ClassFile *owner, MethodInfo *info);

    virtual char *getName(void);
    virtual ClassFile *getImplementor(void);
    virtual bool isArgumentCompatible(unsigned int nbrArgs, JVTypeDefinition **someTypes);
    virtual bool isNative(void);
    virtual unsigned int argumentCount(void);
    virtual JVTypeDefinition **getTyping(void);
    virtual bool returnsVoid(void);
    virtual unsigned int returnsType(void);

    virtual bool resolveTyping(ClassManager *classManager);
    virtual bool isStatic(void);
    virtual unsigned int getNbrExceptions(void);
    virtual bool isConstructor(void);
    virtual bool isSynchronized(void);
 };


class JVNativeMethod : public JVMethod {
  public:
    unsigned int (*internalFunction)(JVMachine *, JVExecFrame *);
    JVDualWorld *dualWorld;

  public:
    JVNativeMethod(ClassFile *owner, MethodInfo *info);

    virtual bool isNative(void);
    virtual void setNativePointer(unsigned int (*anAddress)(void *, void *));
    virtual JVDualWorld *getDualWorld(void);
    virtual void setDualWorld(JVDualWorld *aWorld);
    virtual int doNativeCall(JVMachine *machine, JVExecFrame *currentFrame);
};


// Work in progress:


class JVException : public AkObject {
  protected:
    unsigned int scope[2];
    unsigned int handler;
    ClassFile *filter;

  public:
    JVException(void);
};



#endif	/* _MACHINESUPPORT_H_ */
