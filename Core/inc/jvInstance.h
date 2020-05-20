#ifndef _JVINSTANCE_H_
#define _JVINSTANCE_H_
/**************************************************
* File: jvInstance.h.
* Desc: Definition of the run-time representation of an object (Class instance).
* Module: AkraLog : JavaKit.
* Rev: 8 mai 1996 : REV 0 : Hugo DesRosiers : Creation.
**************************************************/

#include <akra/AkObject.h>
#include <akra/portableDefs.h>
#include "cDefinitions.h"


class ClassFile;
class ArrayClassFile;
class JVSynchronizer;
class JVRunValue;
class JVInstance;
class JVVariable;
class JVDualObject;
class Value64Bits;


class ClassExport JVAbstractInstance : public AkObject {
    friend class PoolManager;
  protected:
    JVAbstractInstance *prev, *next;
    unsigned int useCount;		// Number of time the instance is referred.
    unsigned int freeMark;		// Number of time the freeing algorithme noticed this object.
    JVSynchronizer *synchro;		// Multi-thread access synchronizer.

  public:
    static JVInstance *nullObject;	// Object that represents the 'emptyness'.

    static void defineNullObject(JVInstance *anObject);    // Define the system-wide null object.
    static JVInstance *globalNullObject(void);	// Provides a quick look-up to the system-wide null object.

    JVAbstractInstance(void);
    virtual void reInitialize(void);		// Reset the instance for re-use (from pool manager).

    virtual unsigned short vectorialType(void);	// Inform if the instance is vectorial or not.

	// Work in progress.
    virtual void retain(void);
    virtual void release(void);
    virtual JVSynchronizer *getSynchronizer(void);
    virtual void setSynchronizer(JVSynchronizer *aSynchro);
 };


class JVInstance : public JVAbstractInstance {
    friend class PoolManager;
  protected:
    ClassFile *isA;				// Class which this object belongs to.
    JVAbstractInstance **fields;	// Block of holders of reference to other instances.
    char *assignedMap;			// Map of the fields that are currently assigned.

  public:
    JVInstance(void);
    JVInstance(ClassFile *owner);
    virtual void reInitialize(void);

    virtual ClassFile *category(void);
    virtual unsigned short vectorialType(void);

    virtual JVAbstractInstance *getField(JVVariable *fieldDesc);
    virtual bool putField(JVRunValue &aValue, JVVariable *fieldDesc);
    virtual bool putField(Value64Bits *aValue, JVVariable *fieldDesc);
    virtual JVAbstractInstance *getAtIndex(unsigned int pos);
    virtual bool putAtIndex(JVRunValue &aValue, unsigned int pos);
    virtual bool hasDualViews(void);

    	// Work in progress.
    virtual void unreference(void);
 };


class JVAbstractArray : public JVAbstractInstance {
    friend class JVMachine;
#if defined(USE_FCT_MACHINE)
    friend class JVMachine2;
#endif

  public:
// MOD-970630 [HD]: Put 'nbrElements' public so that it is available from dpsEngine.
    unsigned int nbrElements;		// Nbr of elements available in array.
  protected:
    unsigned int sizeAllocated;	// Total size (in bytes) allocated in the elements 'block'.
    unsigned int sizeUsed;		// Total size (in bytes) used by elements.

  public:
    JVAbstractArray(void);
 };


class JVBasicArray : public JVAbstractArray {
    friend class PoolManager;
//    friend class JVMachine;
  protected:
    ubyte theElementType;
    ubyte *elements;			// The elements, as actual values.

  public:
    static ubyte byteCodeToBasicType(ubyte aByteCode);
    static unsigned int typeToByteLength(unsigned int type);

    JVBasicArray(ubyte type, unsigned int count);
    virtual void reInitialize(ubyte type, unsigned int count);

    virtual unsigned short vectorialType(void);	// Inform if the instance is vectorial or not.
    virtual ubyte elementType(void);

    virtual void *getElement(unsigned int index);	// Send a ptr to first byte of 8 bytes when type is 64 bits.
    virtual void *getElementPtrAt(unsigned int index);    // Returns a ptr to first byte or the element.
    virtual bool setElement(unsigned int index, void *value);    // Returns true if index is within bounds.
    virtual bool byteCopy(ubyte *data, unsigned int dataSize);
    virtual void byteSet(ubyte *data, unsigned int dataSize);
    virtual void byteSet(ubyte *data, unsigned int dataSize, unsigned int offset);
 };


class JVInstanceArray : public JVAbstractArray {
//    friend class PoolManager;
//    friend class JVMachine;
  protected:
    ClassFile *isA;				// Class which this elements belongs to.
    JVInstance **elements;		// The elements, as reference to other instances.

  public:
    JVInstanceArray(ClassFile *type, unsigned int count);
    virtual void reInitialize(ClassFile *type, unsigned int count);

    virtual unsigned short vectorialType(void);	// Inform if the instance is vectorial or not.
    virtual ClassFile *elementType(void);

    virtual JVInstance *getElement(unsigned int index);
    virtual bool setElement(unsigned int index, JVInstance *value);    // Returns true if index is within bounds.
 };


class JVArrayArray : public JVAbstractArray {
  protected:
    bool rootIsBasicType;			// Indicates if root elements are basic or instances.
    ClassFile *rootIsA;				// Root class, if it is not basic.
    JVAbstractArray **elements;		// The elements, as reference to other arrays.

  public:
    JVArrayArray(unsigned int aSize);
    JVArrayArray(ClassFile *type, unsigned int *dimCounts, unsigned int nbrDims);
    virtual void reInitialize(ArrayClassFile *type, unsigned int *dimCounts);

    virtual unsigned short vectorialType(void);	// Inform if the instance is vectorial or not.
    virtual unsigned int dimensions(void);		// Returns the number of 'sub-dimensions'.

    virtual JVAbstractArray *getElement(unsigned int index);
    virtual bool setElement(unsigned int index, JVAbstractArray *value);    // Returns true if index is within bounds.
 };


// Work in progress.
// Must define and implement JVDualObject.

class JVDualInstance : public JVInstance {
  protected:
    JVDualObject *views;
    unsigned int mustForward;		// Up to 32 invalid dual indicators.
    unsigned int mustBackward;	// Up to 32 newer dual indicators.

  public:

    virtual void makeDualDirty(unsigned int dualIndex);
    virtual bool isDualDirty(unsigned int dualIndex);
    virtual bool hasDualViews(void);
};


#endif	/* _JVINSTANCE_H_ */
