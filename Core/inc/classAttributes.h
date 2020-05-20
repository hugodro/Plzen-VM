#ifndef _CLASSATTRIBUTES_H_
#define _CLASSATTRIBUTES_H_
/**************************************************
* File: classAttributes.h.
* Desc: Definition of the optional attributes modifying basic Info descriptors in class files.
* Module: AkraLog : JavaKit.
* Rev: 3 mai 1996 : REV 0.1 : Hugo DesRosiers : Independance clean-up.
**************************************************/

#include <fstream>

#include <classfile/classFileObject.h>
#include "constantPool.h"


/* Attribute types. */
#define ATTR_GENERIC		1
#define ATTR_SRCFILE		2
#define ATTR_CONSTANT		3
#define ATTR_CODE			4
#define ATTR_EXCEPTION		5
#define ATTR_LINENUMBER	6
#define ATTR_LOCALVAR		7
#if defined(PLZEN_LEVEL_2)
#define ATTR_STATE_CODE	8
#define ATTR_TRANSITIONS	9
#endif


// Forward definitions.
class ExceptionSubAttr;
class LineNumberSubAttr;
class LocalVariableSubAttr;


class AttributeInfo : public ClassFileObject {
  protected:
    unsigned int type;	// Internal type mapping.
    unsigned int name;	// Index to the name stored in 'constant_pool'.
    unsigned int length;	// Length of data following this field.

  public:
    AttributeInfo(unsigned int aName, unsigned int aLength);
    AttributeInfo();

    	// Polymorphic object provider of concrete classes of AttributeInfo.
    virtual bool read(std::istream *aStream, ClassFileConstant **ctePool, AttributeInfo* &realAttribute);
    virtual bool read(std::istream *aStream, ClassFileConstant **ctePool);
    virtual bool read(std::istream *aStream);
    virtual unsigned int category(void);
    virtual void describe(ClassFileConstant **ctePool);

// Temporary, for 'Code' attributes.
    virtual unsigned int codeSize(void);
    virtual ubyte *getCode(void);
    virtual unsigned int nbrLocals(void);
    virtual unsigned int stackDepth(void);
 };


/* Optional attributes for a ClassFile: */

class SourceFileAttribute : public AttributeInfo {	// nameIndex ->  CteUtf8:'SourceFile'.
    unsigned int  sourceFileIndex;	// Index to the name of the source file the class was compiled from.

  public:
    SourceFileAttribute(unsigned int aName, unsigned int aLength) : AttributeInfo(aName, aLength) {};
    virtual bool read(std::istream *aStream, ClassFileConstant **ctePool);
    virtual void describe(ClassFileConstant **ctePool);
 };


/* Optional attributes for a FieldInfo: */
class ConstantValueAttribute : public AttributeInfo {    // nameIndex ->  CteUtf8:'ConstantValue'.
    unsigned int  constantValueIndex;		// Index in constant_pool of the value.

  public:
    ConstantValueAttribute(unsigned int aName, unsigned int aLength) : AttributeInfo(aName, aLength) {};
    virtual bool read(std::istream *aStream, ClassFileConstant **ctePool);
    virtual void describe(ClassFileConstant **ctePool);
 };


/* Optional attributes for a MethodInfo: */
class CodeAttribute : public AttributeInfo {		// nameIndex ->  CteUtf8:'Code'.
  protected:
    unsigned int maxStack;		// Maximum number of entries that will be put on the operational stack during this method execution.
    unsigned int maxLocals;		// Maximum number of local variables used by the method.
    unsigned int codeLength;		// Number of bytes in the method implementation.
    ubyte *code;					// J-machine code (aligned mod 4 in memory).
    unsigned int exceptionTableLength;	// Number of Exception handlers defined.
    ExceptionSubAttr *exceptionTable;    // Exception handlers defined in the method.
    unsigned int attributeCount;
    AttributeInfo **attributes;

  public:
    CodeAttribute(unsigned int aName, unsigned int aLength) : AttributeInfo(aName, aLength) {};
    virtual bool read(std::istream *aStream, ClassFileConstant **ctePool);
    virtual void describe(ClassFileConstant **ctePool);

// Work in progress.
    virtual unsigned int codeSize(void);
    virtual ubyte *getCode(void);
    virtual unsigned int nbrLocals(void);
    virtual unsigned int stackDepth(void);
    virtual unsigned int getNbrExceptions(void);
 };

/* Notes:
  - les seuls attributs definis pour l'instant sont 'LineNumberTable' et
    'LocalVariableTable'.
*/

class ExceptionAttribute : public AttributeInfo {	// nameIndex ->  CteUtf8:'Exceptions'.
    unsigned int exceptionCount;
    unsigned int *exceptionTable;

  public:
    ExceptionAttribute(unsigned int aName, unsigned int aLength) : AttributeInfo(aName, aLength) {};
    virtual bool read(std::istream *aStream, ClassFileConstant **ctePool);
 };


/* Optional Attributes for a CodeAttribute: */
class LineNumberTableAttribute : public AttributeInfo {    // nameIndex ->  CteUtf8:'LineNumberTable'.
    unsigned int tableLength;
    LineNumberSubAttr *lineNumberTable;

  public:
    LineNumberTableAttribute(unsigned int aName, unsigned int aLength) : AttributeInfo(aName, aLength) {};
    virtual bool read(std::istream *aStream, ClassFileConstant **ctePool);
    virtual void describe(ClassFileConstant **ctePool);
 };


class LocalVariableTableAttribute : public AttributeInfo {    // nameIndex ->  CteUtf8:'LocalVariableTable'.
    unsigned int tableLength;
    LocalVariableSubAttr *localVariableTable;

  public:
    LocalVariableTableAttribute(unsigned int aName, unsigned int aLength) : AttributeInfo(aName, aLength) {};
    virtual bool read(std::istream *aStream, ClassFileConstant **ctePool);
    virtual void describe(ClassFileConstant **ctePool);
 };


/* Subcomponents of attributes. */

class ExceptionSubAttr : public ClassFileObject {
  protected:
    unsigned int startPC;		// Start of J-Code where the exception handler is active (offset, inclusive).
    unsigned int endPC;		// End of J-Code where the exception handler is active (offset, exclusive).
    unsigned int handlerPC;	// Start address of the J-Code (offset).
    unsigned int catchType;	// Index in constant_pool of the class of exception to be caught (if zero, catch any).

  public:
    virtual bool read(std::istream *aStream);
 };


class LineNumberSubAttr : public ClassFileObject {
  protected:
    unsigned int startPC;			// Address in J-Code where the line number "begins" (offset).
    unsigned int lineNumber;		// Line number in the source code file.

  public:
    virtual bool read(std::istream *aStream);
    virtual void describe(ClassFileConstant **ctePool);
 };


class LocalVariableSubAttr : public ClassFileObject {
  protected:
    unsigned int startPC;			// Beginning of life of local variable (offset).
    unsigned int length;			// Length of life of local variable.
    unsigned int nameIndex;	// Index of the variable name in constant_pool.
    unsigned int signatureIndex;	// Index of the signature name in constant_pool.
    unsigned int slot;				// Index of the local variable in the method's frame table of local variables.

  public:
    virtual bool read(std::istream *aStream);
    virtual void describe(ClassFileConstant **ctePool);
 };


#if defined(PLZEN_LEVEL_2)

class StateCodeAttribute : public CodeAttribute {	    // nameIndex ->  CteUtf8:'StateCode'.
    friend class PVState;
  protected:

  public:
    StateCodeAttribute(unsigned int aName, unsigned int aLength) : CodeAttribute(aName, aLength) {};
    virtual bool read(std::istream *aStream, ClassFileConstant **ctePool);
    virtual void describe(ClassFileConstant **ctePool);
};

class TransitionsAttribute : public AttributeInfo {	    // nameIndex ->  CteUtf8:'Transitions'.
  friend class PVTransition;
  friend class PVFieldTransition;
  protected:
    unsigned int modifiers;
    unsigned int eventIndex;
    unsigned int destinationIndex;
    unsigned int startPC;			// Address in J-Code where the parameters loader begin.
	// ATTN: Should there be a dereference ?

  public:
    TransitionsAttribute(unsigned int aName, unsigned int aLength) : AttributeInfo(aName, aLength) {};
    virtual bool read(std::istream *aStream, ClassFileConstant **ctePool);
    virtual void describe(ClassFileConstant **ctePool);
    virtual bool isModified(unsigned int flags);
};

#endif

#endif	/* _CLASSATTRIBUTES_H_ */
