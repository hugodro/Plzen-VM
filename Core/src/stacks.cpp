/**************************************************
* File: stacks.cc.
* Desc: Implementation of stacks for Java operations.
* Module: AkraLog : JavaKit.
* Rev: 4 mai 1996 : REV 0 : Hugo DesRosiers : Creation.
**************************************************/

#include "machineSupport.h"
#include "math64bits.h"
#include "stacks.h"


/**************************************************
* Implementation: JVRunValue.
**************************************************/
JVRunValue::JVRunValue()
{
    init(0, NULL);
}


JVRunValue::JVRunValue(short aType)
{
    init(aType, NULL);
}


JVRunValue::JVRunValue(short aType, void *aValue)
{
    init(aType, aValue);
}


void JVRunValue::init(short aType, void *aValue)
{
    metaType= aType;
    value= aValue;
}


void JVRunValue::setType(short aType)
{
    metaType= aType;
}


void JVRunValue::setValue(void *aValue)
{
    value= aValue;
}


unsigned int JVRunValue::extractInteger(void)
{
    if (metaType == TYPE_BASIC)
	return ((unsigned int)value);
    else return 0;
}


JVInstance *JVRunValue::extractInstance(void)
{
    if (metaType != TYPE_BASIC)		// Will extract INSTANCE and ARRAY.
	return ((JVInstance *)value);
    else return NULL;
}


short JVRunValue::getType(void)
{
    return metaType;
}


/**************************************************
* Implementation: JVStack.
**************************************************/

JVStack::JVStack()
{
    init();
}


JVStack::JVStack(unsigned int size)
{
    init(size);
}


void JVStack::init(void)
{
    init(STACK_SIZE);
}


void JVStack::init(unsigned int size)
{
    nbrElements= 0;
    stackSize= size;
    next= NULL;

}


/**************************************************
* Implementation: JVOperStack.
**************************************************/

JVOperStack::JVOperStack()
{
    init();
    elements= new JVRunValue[stackSize];
    startMark= 0;
}


JVOperStack::JVOperStack(unsigned int aSize)
{
    init(aSize);
    elements= new JVRunValue[stackSize];
    startMark= 0;
}


JVOperStack::~JVOperStack()
{
    // delete elements;

    delete[] elements;
}


// Note: For now, always return 'true'.  Should return 'false' on memory-full problems.
bool JVOperStack::push(const JVRunValue &value)
{
    if (nbrElements < stackSize) {
	elements[nbrElements++]= value;		// ATTN: Premiere implantation avec copie.
     }
    else {
	    // Push own stack set to another stack.
	JVOperStack *newStack= new JVOperStack;
	newStack->next= next;
	newStack->elements= elements;
	newStack->nbrElements= nbrElements;
	newStack->stackSize= stackSize;
	newStack->startMark= startMark;
	next= newStack;
	startMark+= nbrElements;

	    // Reset our element set to a single (new) element.
	nbrElements= 0;
	elements[nbrElements++]= value;		// ATTN: Premiere implantation avec copie.
     }
    return true;
}


bool JVOperStack::pushByte(const ubyte &value)
{
    static JVRunValue aByteValue(TYPE_BASIC, 0);

    aByteValue.value= (void *)(int)(short)value;
    return push(aByteValue);
}


bool JVOperStack::pushShort(const unsigned short &value)
{
    static JVRunValue aShortValue(TYPE_BASIC, 0);

    aShortValue.value= (void *)(int)(short)value;
    return push(aShortValue);
}


bool JVOperStack::pushInteger(const unsigned int &value)
{
    static JVRunValue anIntValue(TYPE_BASIC, 0);

    anIntValue.value= (void *)value;
    return push(anIntValue);
}


bool JVOperStack::pushLong(const Value64Bits *value)
{
    JVRunValue aLowValue(TYPE_BASIC_64L, 0);
    JVRunValue aHighValue(TYPE_BASIC_64H, 0);
    bool result;

    aLowValue.value= (void *)value->data.lint.lowWord;
    aHighValue.value= (void *)value->data.lint.highWord;
// MOD-980614 [HD]: VC4.0++ does not follow lexical order of 'return (push(high) | push(low))'.
    result= push(aHighValue);
    result|= push(aLowValue);
    return result;
}


bool JVOperStack::pop(JVRunValue &value)
{
    bool result= true;			// Hopefully, stack will usually be well used.

    if (nbrElements > 0) {
	nbrElements--;
	value= elements[nbrElements];
     }
    else if (next != NULL) {
	JVOperStack *oldStack= (JVOperStack *)next;

	next= oldStack->next;
	nbrElements= oldStack->nbrElements;
	stackSize= oldStack->stackSize;
	elements= oldStack->elements;
	startMark= oldStack->startMark;
	delete oldStack;

	nbrElements--;
	value= elements[nbrElements];

     }
    else result= false;

    return result;
}


bool JVOperStack::popLong(Value64Bits *value)
{
    static JVRunValue aLowValue;
    static JVRunValue aHighValue;

    if (pop(aLowValue)) {
	if (pop(aHighValue)) {
	    value->data.lint.lowWord= (unsigned int)aLowValue.value;
	    value->data.lint.highWord= (unsigned int)aHighValue.value;
	}
	else return false;
    }
    else return false;
    return true;
}


bool JVOperStack::popInteger(unsigned int &value)
{
    static JVRunValue tmpValue;

    if (pop(tmpValue)) {
	// ATTN: Should check for value compatibility.
	value= (unsigned int)tmpValue.value;
     }
    else return false;
    return true;
}


bool JVOperStack::popShort(unsigned short &value)
{
    JVRunValue tmpValue;

    if (pop(tmpValue)) {
	// ATTN: Should check for value compatibility.
#if defined(NeXT)
      value= (unsigned short)tmpValue.value;
#else
      unsigned int tmpint;
      tmpint = (unsigned int) tmpValue.value;
      value = (unsigned short) tmpint;
#endif
     }
    else return false;
    return true;
}


bool JVOperStack::dup(void)
{
    bool result= true;

    if (nbrElements > 0) {
	return push(elements[nbrElements-1]);
     }
    else if (next != NULL) {
	/* MOD-960730 [HD]: ENNT: Ajoute un casting pour le deuxieme 'next'. */
		elements[nbrElements++]= ((JVOperStack *)next)->elements[((JVOperStack *)next)->nbrElements-1];
     }
    else result= false;

    return result;
}


bool JVOperStack::dup(unsigned int n)
{
    static JVRunValue dum1, dum2;

// ATTENTION: To implement efficiently !
    if (n == 2) {
	pop(dum1);
	pop(dum2);
	push(dum2);
	push(dum1);
	push(dum2);
	push(dum1);
     }
    return false;
}


bool JVOperStack::empty(void)
{
    if (next) {
	((JVOperStack *)next)->empty();
	delete next;
	next= NULL;
	nbrElements= 0;
     }
    else {
	nbrElements= 0;
     }
    return true;
}


bool JVOperStack::flush(unsigned int mark)
{
// ATTENTION: To be implemented.
    return false;
}


bool JVOperStack::popVoid()
{
    static JVRunValue dummy;

// ATTENTION: To implement efficiently !
    return pop(dummy);
}


bool JVOperStack::popVoidTwice()
{
    static JVRunValue dummy;

// ATTENTION: To implement efficiently !
    return (pop(dummy) || pop(dummy));
}


bool JVOperStack::dupInsert(void)			// Dup 1 value, but insert 2 down.
{
    static JVRunValue dum1, dum2;

// ATTENTION: To implement efficiently !
    pop(dum1);
    pop(dum2);
    push(dum1);
    push(dum2);
    push(dum1);
    return true;
}


bool JVOperStack::dupInsert(unsigned int n)
{
    static JVRunValue dum1, dum2, dum3;

// ATTENTION: To implement efficiently !
    if (n == 2) {
	pop(dum1);
	pop(dum2);
	pop(dum3);
	push(dum2);
	push(dum1);
	push(dum3);
	push(dum2);
	push(dum1);
     }
    return true;
}


bool JVOperStack::dupInsertThree(void)
{
    static JVRunValue dum1, dum2, dum3;

// ATTENTION: To implement efficiently !
    pop(dum1);
    pop(dum2);
    pop(dum3);
    push(dum1);
    push(dum3);
    push(dum2);
    push(dum1);
    return true;
}


bool JVOperStack::dupInsertThree(unsigned int n)
{
    static JVRunValue dum1, dum2, dum3, dum4;

// ATTENTION: To implement efficiently !
    if (n == 2) {
	pop(dum1);
	pop(dum2);
	pop(dum3);
	pop(dum4);
	push(dum2);
	push(dum1);
	push(dum4);
	push(dum3);
	push(dum2);
	push(dum1);
     }
    return true;
}


bool JVOperStack::swap()
{
    static JVRunValue dum1, dum2, dum3, dum4;

// ATTENTION: To implement efficiently !
    pop(dum1);
    pop(dum2);
    push(dum1);
    push(dum2);
    return true;
}
