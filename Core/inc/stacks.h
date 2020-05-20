#ifndef _STACKS_H_
#define _STACKS_H_
/**************************************************
* File: stacks.h.
* Desc: Definition of the stack used in Java operations.
* Module: AkraLog : JavaKit.
* Rev: 4 mai 1996 : REV 0 : Hugo DesRosiers : Creation.
**************************************************/


#include <akra/AkObject.h>
#include <akra/portableDefs.h>
#include "cDefinitions.h"


class Value64Bits;


#define STACK_SIZE		256

class ClassExport JVRunValue {		// TMPTMPTMP.
    friend class JVMachine;
#if defined(USE_FCT_MACHINE)
    friend class JVMachine2;
#endif
    friend class JVOperStack;
    friend class JVInstance;
    friend class JVExecFrame;
  protected:
    short metaType;
    void *value;

  public:
    JVRunValue();
    JVRunValue(short aType);
    JVRunValue(short aType, void *aValue);
    void init(short aType, void *aValue);
    virtual void setType(short aType);
    virtual void setValue(void *aValue);

// Work in progress:
    virtual unsigned int extractInteger(void);
    virtual JVInstance *extractInstance(void);
 /* ATTN: A ajouter:
   - extractAbstractInstance
   - extractAbstractArray.
*/
    virtual short getType(void);
 };


class JVStack : public AkObject {
  protected:
    unsigned int nbrElements;		// Number of ele. on stack.
    unsigned int stackSize;		// Stack capacity (this slot).
    JVStack *next;				// Next stack slot.

  public:
    JVStack();
    JVStack(unsigned int size);
    virtual void init(void);
    virtual void init(unsigned int size);

 };


class JVOperStack : public JVStack {
  protected:
    JVRunValue *elements;
    unsigned int startMark;

  public:
    JVOperStack(void);
    JVOperStack(unsigned int aSize);
    virtual ~JVOperStack();
    virtual	bool push(const JVRunValue &value);
    virtual bool pushByte(const ubyte &value);
    virtual bool pushShort(const unsigned short &value);
    virtual bool pushInteger(const unsigned int &value);
    virtual bool pushLong(const Value64Bits *value);
    virtual bool pop(JVRunValue &value);
    virtual bool popLong(Value64Bits *value);
    virtual bool popInteger(unsigned int &value);
    virtual bool popShort(unsigned short &value);
    virtual bool dup(void);			// Dup 1 value.
    virtual bool dup(unsigned int n);	// Dup n values.
    virtual bool empty(void);			// Empty of all elements.
    virtual bool flush(unsigned int mark);	// Flush to a mark.
    virtual bool popVoid();
    virtual bool popVoidTwice();
    virtual bool dupInsert(void);			// Dup 1 value, but insert 2 down.
    virtual bool dupInsert(unsigned int n);	// Dup n values, but insert 2 down.
    virtual bool dupInsertThree(void);			// Dup 1 value, but insert 3 down.
    virtual bool dupInsertThree(unsigned int n);	// Dup n values, but insert 3 down.
    virtual bool swap();
 };


#endif	/* _STACKS_H_ */
