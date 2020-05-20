#ifndef _CONSTANTPOOL_H_
#define _CONSTANTPOOL_H_
/**************************************************
* File: constantPool.h.
* Desc: Definition of constant objects for inclusion in the 'Constant Pool'.
* Module: AkraLog : JavaKit.
* Rev: 3 mai 1996 : REV 0.1 : Hugo DesRosiers : Independance clean-up.
**************************************************/

#include <fstream>

#include <classfile/classFileObject.h>


/* Constant types. */
#define CONSTANT_Class			7
#define CONSTANT_FieldRef		9
#define CONSTANT_MethodRef		10
#define CONSTANT_InterfaceMethodRef	11
#define CONSTANT_String			8
#define CONSTANT_Integer			3
#define CONSTANT_Float			4
#define CONSTANT_Long			5
#define CONSTANT_Double		6
#define CONSTANT_NameAndType	12
#define CONSTANT_Utf8			1
#define CONSTANT_Unicode		2

/* Class that are defined in another module, no need to include in this one. */
class ClassManager;
class ClassFile;
class FieldInfo;
class MethodInfo;
class StringInfo;

class ClassFileConstant : public ClassFileObject {
  protected:
    unsigned int tag;			// Holder of type.
    unsigned int index;		// Index in constant_pool of the main object related to the Constant.

  public:
    virtual unsigned int type();
    virtual bool read(std::istream *aStream);
    virtual bool read(std::istream *aStream, ClassFileConstant* &realCte);
    virtual ClassFileObject *resolve(ClassManager *source, ClassFile *owner);
// Test predicates for subclasses.
    virtual bool isStrEqual(char *aString);
    virtual bool isStrEqual(ClassFileConstant **ctePool, char *aString);
// Content to string conversion methods for subclasses..
    virtual char * stringValue(void);				// Generate copy of string.
    virtual void writeStringValue(char *dest);   	 // Use provided buffer.
    virtual char * stringValue(ClassFileConstant **ctePool);    // Generate copy of string.
    virtual void writeStringValue(char *dest, ClassFileConstant **ctePool);    // Use provided buffer.
    virtual void memberName(char *dest, ClassFileConstant **ctePool);
    virtual void typeSignature(char *dest, ClassFileConstant **ctePool);
// Debugging function.
    virtual void describe(ClassFileConstant **ctePool);
 };


class CteUtf8 : public ClassFileConstant {
  protected:
    unsigned int length;			// Number of bytes in the string.
    ubyte *bytes;		// String (without null-termination, but with UTF8 null char appended).

  public:
    virtual bool read(std::istream *aStream);
    virtual bool isStrEqual(char *aString);
    virtual char * stringValue(void);
    virtual void writeStringValue(char *dest);    // Use provided buffer.
    virtual void describe(ClassFileConstant **ctePool);
    virtual unsigned int getLength(void);
 };


class CteUnicode : public ClassFileConstant {
  protected:
    unsigned int length;			// Number of bytes in the string.
    ubyte *bytes;		// String (without null-termination, but with UTF8 null char appended).

  public:
    virtual bool read(std::istream *aStream);
    virtual char * stringValue(void);
    virtual void writeStringValue(char *dest);    // Use provided buffer.
    virtual bool isStrEqual(char *aString);
 };


class CteClass : public ClassFileConstant {
  protected:
    ClassFile *dereference;		// Ptr to the actual ClassFile this cte represents.

  public:
    virtual bool read(std::istream *aStream);
    virtual ClassFileObject *resolve(ClassManager *source, ClassFile *owner);
    virtual bool isStrEqual(ClassFileConstant **ctePool, char *aString);
    virtual char *stringValue(ClassFileConstant **ctePool);
    virtual void writeStringValue(char *dest, ClassFileConstant **ctePool);    // Use provided buffer.
    virtual void describe(ClassFileConstant **ctePool);
 };


class CteFieldRef : public ClassFileConstant {
  protected:
    unsigned int nameTypeIndex;	// Index in constant_pool of the CONSTANT_NameAndType which gives the name and signature of the Field.
    FieldInfo *dereference;		// Ptr to the actual FieldInfo this cte represents.

  public:
    virtual bool read(std::istream *aStream);
    virtual ClassFileObject *resolve(ClassManager *source, ClassFile *owner);
    virtual void describe(ClassFileConstant **ctePool);
 };


class CteMethodRef : public ClassFileConstant {
  protected:
    unsigned int nameTypeIndex;	// Index in constant_pool of the CONSTANT_NameAndType which gives the name and signature of the Method.
    MethodInfo *dereference;		// Ptr to the actual Method this cte represents.

  public:
    virtual bool read(std::istream *aStream);
    virtual ClassFileObject *resolve(ClassManager *source, ClassFile *owner);
    virtual void describe(ClassFileConstant **ctePool);
    virtual char *stringValue(ClassFileConstant **ctePool);
 };


// ATTN: Must find out how to implement this (run-time info).
// MOD-960624 [HD]: Implemented as a CteMethodRef.
class CteInterfaceMethodRef : public ClassFileConstant {
  protected:
    unsigned int nameTypeIndex;	// Index in constant_pool of the CONSTANT_NameAndType which gives the name and signature of the Interface Method.
    MethodInfo *dereference;		// Ptr to the actual ClassFile this cte represents.

  public:
    virtual bool read(std::istream *aStream);
    virtual void describe(ClassFileConstant **ctePool);
    virtual ClassFileObject *resolve(ClassManager *source, ClassFile *owner);
 };


class CteNameAndType : public ClassFileConstant {
  protected:
    unsigned int signatureIndex;	// Index to a string in constant_pool giving the signature of a field or method.
    FieldInfo *dereference;

  public:
    virtual bool read(std::istream *aStream);
    virtual void memberName(char *dest, ClassFileConstant **ctePool);
    virtual void typeSignature(char *dest, ClassFileConstant **ctePool);
    virtual ClassFileObject *resolve(ClassManager *source, ClassFile *ownerl);
    virtual void describe(ClassFileConstant **ctePool);
 };


// ATTN: Must find dereference value to send back to oper stack or variable.
class CteString : public ClassFileConstant {
  protected:
    StringInfo *dereference;

  public:
    virtual bool read(std::istream *aStream);
    virtual ClassFileObject *resolve(ClassManager *source, ClassFile *owner);
    virtual void describe(ClassFileConstant **ctePool);
 };


class CteWord : public ClassFileConstant {
  protected:
    unsigned int bytes;			// Value (4 bytes, Integer format).

  public:
    virtual bool read(std::istream *aStream);
    virtual unsigned int value();
 };


class CteDoubleWord : public CteWord {
  protected:
    unsigned int high_bytes;

  public:
    virtual bool read(std::istream *aStream);
    virtual unsigned int valueLow();
    virtual unsigned int valueHigh();
    virtual void value(char **dest);
 };


// ATTN: Must find dereference value to send back to oper stack or variable.
class CteInteger : public CteWord {

  public:
    virtual void describe(ClassFileConstant **ctePool);
 };


// ATTN: Must find dereference value to send back to oper stack or variable.
class CteFloat : public CteWord {
  // Value (4 bytes, IEEE format).

  public:
    virtual void describe(ClassFileConstant **ctePool);
 };


// ATTN: Must find dereference value to send back to oper stack or variable.
class CteLong : public CteDoubleWord {

  public:
    virtual void describe(ClassFileConstant **ctePool);
 };


// ATTN: Must find dereference value to send back to oper stack or variable.
class CteDouble : public CteDoubleWord {
  protected:
    // bytes: Value (4 bytes, IEEE format).
    // high_bytes: con't (4 bytes, IEEE format).

  public:
    virtual void describe(ClassFileConstant **ctePool);
 };



#endif	/* _CONSTANTPOOL_H_ */
