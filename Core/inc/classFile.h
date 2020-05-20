#ifndef _CLASSFILE_H_
#define _CLASSFILE_H_
/**************************************************
* File: runClass.h.
* Desc: Definitions used to implement the run-time class support.
* Module: AkraLog : JavaKit.
* Rev: 29 avril 1996 : REV 0 : Hugo DesRosiers : Creation.
* Rev: 3 mai 1996 : REV 0.1 : Hugo DesRosiers : Independance clean-up.
**************************************************/

#include <fstream>

#include <akra/portableDefs.h>

#include "constantPool.h"
#include "classAttributes.h"
#include "debugInfo.h"

/* Class that are defined further down that first used. */
class JVVariable;
class JVMethod;
class FieldInfo;
class MethodInfo;
class JVTypeDefinition;
class JVInstance;
class JVBasicArray;
class DebugInfo;
class JVClassRunInfo;
class JVSynchronizer;
class JVMachine;


class ClassFile : public ClassFileObject {
    friend class CteClass;
    friend class CteFieldRef;
    friend class CteMethodRef;
    friend class CteInterfaceMethodRef;
    friend class JVInstance;
    friend class PoolManager;			// Used to access complex map.
  public:
    enum ClassFlags {
	needsStaticSynchro= 0x010000,
	hasSynchroMethods= 0x020000,
	basicTypeCover= 0x040000
	, mask= 0x070000
    };

  protected:
    unsigned int magic;				// 0x0CAFEBABE.
    unsigned int version_minor;		// version of java compiler used to compile.
    unsigned int version_major;		// version of java compiler used to compile.
    unsigned int ctePoolSize;			// Size of constant pool.
    ClassFileConstant **constantPool;		// Constant values (strings, names, etc).
    unsigned int accessFlags;			// Flags of the class definition.
    unsigned int thisClass;			// Index to the class in the constant_pool.
    unsigned int superClass;			// Index to the super-class inthe constant_pool.
    unsigned int interfaceCount;		// Number of interfaces implemented by the class.
    unsigned int *interfaces;  // Indexes of the interfaces implemented (in the constant_pool).
    unsigned int fieldCount;			// Number of fields (variables) defined in the class (current layer only).
    unsigned int staticFieldCount;		// Number of static fields defined in the class.
    FieldInfo *fields;	// Field definitions.
    unsigned int methodCount;			// Number of methods defined in the class (current layer only).
    unsigned int stateCount;			// Number of states defined in the class (current layer only).
    MethodInfo *methods;	// Method definitions.
    unsigned int attributeCount;		// Number of optional attributes given to the class.
    AttributeInfo **attributes;		// Optional attributes attached to the class.
    DebugInfo *debugInfo;		// Debugging information about class members.

// Derived informations.
    unsigned int nbrReferedClasses;
    unsigned int nbrReferedFields;
    unsigned int nbrReferedMethods;
    unsigned int nbrReferedIntefaces;
    ClassFile *superClassFile;
    unsigned int totalNbrSlots;		// Sum of fields an object of this class has.
    unsigned int totalNbrCplxSlots;	// Sum of non-basic data type fields it has.
    unsigned int totalNbrStaticSlots;	// Sum of static fields it has.
    char *complexMap;		// Bit map of complex fields (duplicates super-class info).
    JVClassRunInfo *runInfo;			// Info block, describing runtime details.
    JVSynchronizer *synchro;		// Lock for static methods.
    JVInstance *javaEquiv;		// Java equivalent j.l.Class object.

  protected:
    void init(DebugInfo::SectionTracing sectionSet);    // General initializer.

  public:
    ClassFile();
    ClassFile(std::istream *aStream, DebugInfo::SectionTracing sectionSet);
    virtual bool read(std::istream *aStream);

// For use during run-time.
    virtual bool isMainName(char *aName);
    virtual unsigned int getRefClasses(char ***holders);
    virtual FieldInfo *getFieldNamed(char *aName);
    virtual MethodInfo *getLocalMethodNamed(char *aName);
    virtual MethodInfo *getMethodNamed(char *aName);
    virtual MethodInfo *getMethodNamed(char *aName, char *aType);
    virtual JVMethod *getVirtualMethod(ClassManager *manager, JVMethod *superMethod);
    virtual ClassFileConstant *getConstant(unsigned int i);
    virtual bool resolve(ClassManager *manager);
    virtual ClassFileConstant **getConstantPool();
    virtual bool isInterface(void);
    virtual bool isCastable(ClassFile *aClass);
    virtual void setAsPrimitiveCover(void);    // Only because ClassManager has no access to 'accessFlags'.
    virtual bool isPrimitiveCover(void);
    virtual bool isArrayClass(void);
    virtual bool isAbstract(void);
#if defined(PLZEN_LEVEL_2)
    virtual bool isStateless(void);
#endif

// For debugging information.
    virtual void describe(unsigned int verbosity);

// In progress:
    virtual void getName(char *destination);
    virtual unsigned int nbrFields(void);
    virtual DebugInfo::SectionTracing traceLevel();
    virtual void setSynchronizer(JVSynchronizer *aSynchro);
    virtual JVSynchronizer *getSynchronizer(void);

// Temporary.
    virtual MethodInfo *getMethod(unsigned int i);    // MOD-980318 [HD]: Changed name from 'findMethod'.
    virtual unsigned int nbrMethods(void);
    virtual bool hasGoodMagic(void);

// In progress.
    virtual JVInstance *getJavaEquivalent(JVMachine *aMachine);
    virtual JVInstance *getJavaEquivalent(void);
    virtual ClassFile *getSuperClass(void);
};
 

class ArrayClassFile : public ClassFile {
    friend class PoolManager;			// Used to access complex map.
  protected:
    ubyte basicType;
    unsigned int nbrDimensions;

  public:
    ArrayClassFile(ClassFile *sourceClass, unsigned int dimSize);
    ArrayClassFile(ubyte type, unsigned int dimSize);	// For a basic type array.
    virtual bool isArrayClass(void);
    virtual bool rootIsBasicType(void);
    virtual unsigned int dimensions(void);
    virtual ubyte getBasicType(void);
 };


/*** Major class constituents. ***/

class GenericInfo : public ClassFileObject {
  protected:
    unsigned int accessFlags;		// Access flags of the field.
    unsigned int nameIndex;			// Index of the name in constant_pool.
    unsigned int attributeCount;		// Number of optional attributes for the field.
    AttributeInfo **attributes;
    ClassFile *owner;				// Owner of the info object.

  public:
    virtual bool read(std::istream *aStream, ClassFileConstant **ctePool);
    virtual bool readAttributes(std::istream *aStream, ClassFileConstant **ctePool);
    virtual bool isNamed(char *aName, ClassFileConstant **ctePool);
    virtual void setOwner(ClassFile *classFile);
    virtual ClassFile *getOwner(void);
    virtual bool isStatic(void);
    virtual void describe(ClassFileConstant **ctePool, unsigned int verbosity);
    virtual unsigned int getAccessFlags(void);
 };


class FieldInfo : public GenericInfo {
    friend class JVVariable;
  protected:
    unsigned int signatureIndex;		// Index of the signature name in constant_pool.
    JVVariable *dereference;

  public:
    virtual bool read(std::istream *aStream, ClassFileConstant **ctePool);
    virtual JVVariable *resolve(ClassManager *source);
    static int parseSignature(char *aSignature, JVTypeDefinition **types);
    virtual void describe(ClassFileConstant **ctePool, unsigned int verbosity);
 };


class MethodInfo : public GenericInfo {
    friend class JVMethod;
    friend class JVState;
  protected:
    unsigned int signatureIndex;		// Index of the signature name for the method in the constant_pool.
    bool isStateInfo;			// Indicates if this is in fact a state descriptor.
    JVMethod *dereference;

  public:
    MethodInfo(void);
    virtual bool read(std::istream *aStream, ClassFileConstant **ctePool);
    virtual JVMethod *resolve(ClassManager *source);
    static int parseArguments(char *aSignature, JVTypeDefinition ***types);
    virtual bool isTypeCompatible(unsigned int nbrArgs, JVTypeDefinition **types);
    virtual bool isFromInterface(void);
    virtual void describe(ClassFileConstant **ctePool, unsigned int verbosity);

// Work in progress.
    virtual unsigned int getCodeLength(void);
    virtual ubyte *getCode(void);
    virtual unsigned int maxLocals(void);
    virtual unsigned int stackMaxDepth(void);
    virtual bool hasCodeAttribute(void);
    virtual bool hasStateCodeAttribute(void);
    virtual bool isState(void);
    virtual unsigned int getNbrExceptions(void);
    virtual CodeAttribute *getCodeInfo(void);
    virtual bool isSynchronized(void);
 };


class StringInfo : public ClassFileObject {
  protected:
    JVInstance *value;

  public:
    StringInfo();
    virtual void setConstantValue(JVInstance *aValue);
    virtual void setActualData(JVBasicArray *charArray, char *letters);
    virtual JVInstance *constantValue(void);
 };


/* Signature string syntax:
    <field signature> ::=
	<field type>

    <field type> ::= 
    	<base_type>
	|  <object_type>
	|  <array_type>

    <return signature> ::=
	<field type>
	|  V

    <argument signature> ::=
	<field type>

    <method signature> ::=
	'(' <argument signature>* ')' <return signature>

    <base_type> ::=
	B		Signed byte.
	| C		Character.
	| D		Double precision floating point number.
	| F		Single precision floating point number.
	| I		Integer.
	| J		Long Integer.
	| S		Unsigned short.
	| Z		Boolean.

    <object_type> ::=
	'L'<string> ';'		object of <string> class (full class name).

    <array_type> ::=
	'['<opt_int><field type>		Array with length and field signature.

 */
#endif	/* _CLASSFILE_H_ */
