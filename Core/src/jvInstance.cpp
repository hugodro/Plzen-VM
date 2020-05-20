/**************************************************
* File: jvInstance.cc.
* Desc: Implementation of the class instance mechanism.
* Module: AkraLog : JavaKit.
* Rev: 10 mai 1996 : REV 0 : Hugo DesRosiers : Creation.
**************************************************/

extern "C" {
#include <string.h>
}


#include <threader/synchro.h>

#include "classFile.h"
#include "machineSupport.h"
#include "stacks.h"
#include "math64bits.h"
#include "runInfo.h"
#include "jvInstance.h"

/**************************************************
* Implementation: JVAbstractInstance.
**************************************************/

JVInstance*  JVAbstractInstance::nullObject= NULL;

void JVAbstractInstance::defineNullObject(JVInstance *anObject)
{
    nullObject= anObject;
}


JVInstance *JVAbstractInstance::globalNullObject(void)
{
    return nullObject;
}


JVAbstractInstance::JVAbstractInstance(void)
{
    useCount= freeMark= 0;
    synchro= NULL;
}


void JVAbstractInstance::reInitialize(void)
{
    useCount= freeMark= 0;
    synchro= NULL;
}


unsigned short JVAbstractInstance::vectorialType(void)
{
    return 0;		// This class should not be used.
}


void JVAbstractInstance::retain(void)
{
    useCount++;
}


void JVAbstractInstance::release(void)
{
    useCount--;
}


JVSynchronizer *JVAbstractInstance::getSynchronizer(void)
{
    return synchro;
}


void JVAbstractInstance::setSynchronizer(JVSynchronizer *aSynchro)
{
    synchro= aSynchro;
}


/**************************************************
* Implementation: JVInstance.
**************************************************/

JVInstance::JVInstance(void)
{
  // ATTN: Juste pour faire plaisir a GCC.
  // Must throw an exception, since there is no class to relate the instance to.
}


JVInstance::JVInstance(ClassFile *owner) : JVAbstractInstance()
{
    unsigned int mapSize;

    prev= next= NULL;
    isA= owner;
    if (isA != NULL) {
	mapSize= (isA->totalNbrSlots >> 3) + ((isA->totalNbrSlots & 0x07) ? 1 : 0);
	if (mapSize > 0) {
	    assignedMap= new char[mapSize];
	    memset(assignedMap, 0, mapSize);
	 }
	else assignedMap= NULL;
	if (isA->totalNbrSlots > 0) {
	    fields= new JVAbstractInstance*[isA->totalNbrSlots];
	    if (isA->runInfo != NULL) {
		Value64Bits *longValues= new Value64Bits[isA->runInfo->nbrLongSlots];
		for (unsigned int i= 0; i < isA->runInfo->nbrLongSlots; i++) {
		    fields[isA->runInfo->longSlots[i]]= (JVAbstractInstance *)&longValues[i];
		}
	    }
	}
	else fields= NULL;
     }
}


void JVInstance::reInitialize(void)
{
    unsigned int mapSize;

    JVAbstractInstance::reInitialize();
    mapSize= (isA->totalNbrSlots >> 3) + ((isA->totalNbrSlots & 0x07) ? 1 : 0);
    if (mapSize > 0)
	memset(assignedMap, 0, mapSize);
}


ClassFile *JVInstance::category(void)
{
    return isA;
}


unsigned short JVInstance::vectorialType(void)
{
    return TYPE_INSTANCE;		// A non-vectorial element.
}


JVAbstractInstance *JVInstance::getField(JVVariable *fieldDesc)
{
    JVAbstractInstance *result= NULL;		// If a field is not assigned, we return a NULL value, it will interpreted higher.
    unsigned int offset, mapSize;

    offset= fieldDesc->position();
    mapSize= (isA->totalNbrSlots >> 3) + ((isA->totalNbrSlots & 0x07) ? 1 : 0);
    if (mapSize > 0) {
	if ((assignedMap[(offset / 8)] & (1 << (offset % 8))) != 0) {
	    result= fields[offset];
	 }
     }
    return result; 
}


bool JVInstance::putField(JVRunValue &aValue, JVVariable *fieldDesc)
{
    unsigned int offset, mapSize;
    bool result= true;

    offset= fieldDesc->position();
    mapSize= (isA->totalNbrSlots >> 3) + ((isA->totalNbrSlots & 0x07) ? 1 : 0);
    if ((mapSize > 0) && (offset < isA->totalNbrSlots)) {
	// ATTN: We could be doing type-checking here for aValue.metaType and fieldDesc->type.
	fields[offset]= (JVAbstractInstance *)aValue.value;
	assignedMap[(offset / 8)]|= (1 << (offset % 8));
	if ((fieldDesc->metaType() != TYPE_BASIC) && (fieldDesc->metaType() != TYPE_BASELONG)) fields[offset]->retain();
     }
    else result= false;

    return result;
}


bool JVInstance::putField(Value64Bits *aValue, JVVariable *fieldDesc)
{
   unsigned int offset, mapSize;
    bool result= true;

    offset= fieldDesc->position();
    mapSize= (isA->totalNbrSlots >> 3) + ((isA->totalNbrSlots & 0x07) ? 1 : 0);
    if ((mapSize > 0) && (offset < isA->totalNbrSlots)) {
	if ((fieldDesc->metaType() & TYPE_BASELONG) == 0){
	    result= false;
	}
	else {
	    *(Value64Bits *)fields[offset]=*aValue;
	    assignedMap[(offset / 8)]|= (1 << (offset % 8));
	}
     }
    else result= false;

    return result;
}


JVAbstractInstance *JVInstance::getAtIndex(unsigned int pos)
{
    JVAbstractInstance *result= NULL;		// If a field is not assigned, we return a NULL value, it will be interpreted higher.
    unsigned int mapSize;

    mapSize= (isA->totalNbrSlots >> 3) + ((isA->totalNbrSlots & 0x07) ? 1 : 0);
    if ((mapSize > 0) && (pos < isA->totalNbrSlots)) {
	if ((assignedMap[(pos / 8)] & (1 << (pos % 8))) != 0) {
	    result= fields[pos];
	 }
     }
    return result; 
}


bool JVInstance::putAtIndex(JVRunValue &aValue, unsigned int pos)
{
    unsigned int mapSize;
    bool result= true;

    // ATTN: We should be doing type-checking here for aValue.metaType and fieldDesc->type.
    mapSize= (isA->totalNbrSlots >> 3) + ((isA->totalNbrSlots & 0x07) ? 1 : 0);
    if ((mapSize > 0) && (pos < isA->totalNbrSlots)) {
	fields[pos]= (JVAbstractInstance *)aValue.value;
	assignedMap[(pos / 8)]|= (1 << (pos % 8));
	if (isA->complexMap[(pos / 8)] & (1 << (pos % 8))) {
	    fields[pos]->retain();
	}
    }
    else return false;

    return result;
}


bool JVInstance::hasDualViews(void)
{
    return false;
}


void JVInstance::unreference(void)
{
    if (isA->totalNbrCplxSlots > 0) {
	for (unsigned int i= 0; i < isA->totalNbrSlots; i++) {
	    if (isA->complexMap[(i / 8)] & (1 << (i % 8))) fields[i]->release();
	}
    }
}


/**************************************************
* Implementation: JVAbstractArray.
**************************************************/

JVAbstractArray::JVAbstractArray(void) : JVAbstractInstance()
{
    nbrElements= sizeAllocated= sizeUsed= 0;
}


/**************************************************
* Implementation: JVBasicArray.
**************************************************/

ubyte JVBasicArray::byteCodeToBasicType(ubyte aByteCode)
 {
    ubyte type;

    switch(aByteCode) {
	case 9:
	    type= JVM_TYPE_UNSIGNED_SHORT;
	    break;
	case 6:
	    type= JVM_TYPE_FLOAT;
	    break;
	case 10:
	    type= JVM_TYPE_INTEGER;
	    break;
	case 7:
	    type= JVM_TYPE_DOUBLE;
	    break;
	case 11:
	    type= JVM_TYPE_LONG;
	    break;
	case 5:
	    type= JVM_TYPE_CHAR;
	    break;
	case 8:
	    type= JVM_TYPE_SIGNED_BYTE;
	    break;
	case 4:
	    type= JVM_TYPE_BOOLEAN;
	    break;
	default:
	    type= JVM_TYPE_SIGNED_BYTE;
	    break;
     }
    return type;
 }


unsigned int JVBasicArray::typeToByteLength(unsigned int type)
{
    unsigned int size;

    switch (type) {
	case JVM_TYPE_UNSIGNED_SHORT:
	    size= 2;
	    break;
	case JVM_TYPE_FLOAT:
	case JVM_TYPE_INTEGER:
	    size=  4;
	    break;
	case JVM_TYPE_DOUBLE:
	case JVM_TYPE_LONG:
	    size= sizeof(Value64Bits);
	    break;
	default:
	    size= 1;
	    break;
    }	

    return size;
}


JVBasicArray::JVBasicArray(ubyte type, unsigned int count)
{
    nbrElements= count;
    sizeUsed= sizeAllocated= count * JVBasicArray::typeToByteLength(type);
    theElementType= type;
    if (count > 0) {
	elements= new ubyte[sizeAllocated];
    }
}


// ATTN: Should only be used for a lower count than previously assigned.
void JVBasicArray::reInitialize(ubyte type, unsigned int count)
{
    JVAbstractInstance::reInitialize();
    theElementType= type;
    nbrElements= count;
    sizeUsed= count * JVBasicArray::typeToByteLength(type);
    if (count > 0) {
	memset(elements, 0, sizeUsed);
     }
}


unsigned short JVBasicArray::vectorialType(void)
{
    return (TYPE_ARRAY | TYPE_BASIC_ARRAY);
}


ubyte JVBasicArray::elementType(void)
{
    return theElementType;
}


void *JVBasicArray::getElement(unsigned int index)
{
    if (index < nbrElements) {
	switch (theElementType) {
	    case JVM_TYPE_SIGNED_BYTE:
	    case JVM_TYPE_CHAR:
	    case JVM_TYPE_BOOLEAN:
		return (void *)(uint)elements[index];
		// break;
	    case JVM_TYPE_UNSIGNED_SHORT:
		return (void *)(uint)((short *)elements)[index];
		// break;
	    case JVM_TYPE_INTEGER:
	    case JVM_TYPE_FLOAT:
		return (void *)((int *)elements)[index];
		// break;
	    case JVM_TYPE_LONG:
	    case JVM_TYPE_DOUBLE:
		return (void *)(((Value64Bits *)elements)+index);
		// break;
    
	 }
     }
    return NULL;
}


void *JVBasicArray::getElementPtrAt(unsigned int index)
{
    if (index < nbrElements) {
	switch (theElementType) {
	    case JVM_TYPE_SIGNED_BYTE:
	    case JVM_TYPE_CHAR:
	    case JVM_TYPE_BOOLEAN:
		return (void *)&elements[index];
		// break;
	    case JVM_TYPE_UNSIGNED_SHORT:
		return (void *)&((short *)elements)[index];
		// break;
	    case JVM_TYPE_INTEGER:
	    case JVM_TYPE_FLOAT:
		return (void *)&((int *)elements)[index];
		// break;
	    case JVM_TYPE_LONG:
	    case JVM_TYPE_DOUBLE:
		return (void *)(((Value64Bits *)elements)+index);
		// break;
    
	 }
     }
    return NULL;
}


bool JVBasicArray::setElement(unsigned int index, void *value)
{
    bool result;
#if !defined(NeXT)
    void *tmpVal = (void *) &value;
#endif

    if (index < nbrElements) {
	switch (theElementType) {
	    case JVM_TYPE_SIGNED_BYTE:
	    case JVM_TYPE_CHAR:
	    case JVM_TYPE_BOOLEAN:
#if defined(NeXT)
	      elements[index]= (ubyte)value;
#else
	      elements[index] = *(ubyte *)tmpVal;
#endif
		result= true;
		break;
	    case JVM_TYPE_UNSIGNED_SHORT:
#if defined(NeXT)
	      ((short *)elements)[index]= (short)value;
#else
	      ((short *)elements)[index]= *(short *)tmpVal;
#endif
		result= true;
		break;
	    case JVM_TYPE_INTEGER:
	    case JVM_TYPE_FLOAT:
		((int *)elements)[index]= (unsigned int)value;
		result= true;
		break;
	    case JVM_TYPE_LONG:
	    case JVM_TYPE_DOUBLE:
		*(((Value64Bits *)elements)+index)= *(Value64Bits *)value;
		result= true;
		break;
	 }
    }
   else result= false;
   return result;
}


bool JVBasicArray::byteCopy(ubyte *data, unsigned int dataSize)
{
    // ATTN: The array size is not modified by this method (it takes no responsability
    // when the dataSize is not a multiple of element size !).
    if (dataSize <= sizeAllocated) {
	memcpy(elements, data, dataSize);
	return true;
     }
    else return false;
}


void JVBasicArray::byteSet(ubyte *data, unsigned int dataSize)
{
    if (dataSize > sizeUsed) dataSize= sizeUsed;
    memcpy(data, elements, dataSize);
}


void JVBasicArray::byteSet(ubyte *data, unsigned int dataSize, unsigned int offset)
{
    if (offset > sizeUsed) return;  // Warning: quick exit.
    if ((dataSize + offset) > sizeUsed) dataSize= sizeUsed - offset;
    memcpy(data, elements + offset, dataSize);
}


/**************************************************
* Implementation: JVInstanceArray.
**************************************************/

JVInstanceArray::JVInstanceArray(ClassFile *type, unsigned int count) : JVAbstractArray()
{
    nbrElements= count;
    sizeUsed= sizeAllocated= sizeof(JVInstance *) * count;
    isA= type;
    if (count > 0) {
	elements= new JVInstance*[count];
	for (unsigned int i= 0; i < count; i++) {
	    elements[i]= nullObject;
	 }
     }
}


void JVInstanceArray::reInitialize(ClassFile *type, unsigned int count)
{
    JVAbstractInstance::reInitialize();
    nbrElements= count;
    sizeUsed= sizeof(JVInstance *) * count;
    isA= type;
    if (count > 0) {
	for (unsigned int i= 0; i < count; i++) {
	    elements[i]= nullObject;
	 }
     }
}


unsigned short JVInstanceArray::vectorialType(void)
{
    return (TYPE_ARRAY | TYPE_INST_ARRAY);
}


ClassFile *JVInstanceArray::elementType(void)
{
    return isA;
}


JVInstance *JVInstanceArray::getElement(unsigned int index)
{
    if (index < nbrElements) {
	return elements[index];
     }
    else return NULL;
}


bool JVInstanceArray::setElement(unsigned int index, JVInstance *value)
{
    bool result;

    if (index < nbrElements) {
	elements[index]= value;
	result= true;
     }
    else result= false;
    return false;
}


/**************************************************
* Implementation: JVArrayArray.
**************************************************/

JVArrayArray::JVArrayArray(unsigned int aSize)
{
    elements= (JVAbstractArray **)new JVArrayArray*[aSize];
    sizeAllocated= sizeUsed= sizeof(JVArrayArray *) * aSize;
    nbrElements= aSize;
}


JVArrayArray::JVArrayArray(ClassFile *type, unsigned int *dimCounts, unsigned int nbrDims) : JVAbstractArray()
{
    JVAbstractArray ***tmpElementsPtr[256];
    unsigned int arrayCursors[256];
    unsigned int curDim, lastDimAction;

    rootIsBasicType= false;
    rootIsA= type;
    curDim= 0;

    arrayCursors[curDim]= 0;
    tmpElementsPtr[curDim]= &elements;
    *(tmpElementsPtr[curDim])= (JVAbstractArray **)new JVArrayArray*[dimCounts[curDim]];

    if (type->isArrayClass()) {
	if ((nbrDims < ((ArrayClassFile *)type)->dimensions()) || ((nbrDims == 1) && (((ArrayClassFile *)type)->dimensions() == 1))) {
	    // Last dimension is still an array.  Fill the elements with null objects.
	    lastDimAction= 0;
	}
	else {
	    if (((ArrayClassFile *)type)->rootIsBasicType()) {
		// Last dimension is a basic type array.  Create a JVBasicArray.
		lastDimAction= 1;
	    }
	    else {
		// Last dimension is an instance array.  Create a JVInstanceArray.
		lastDimAction= 2;
	    }
	}
    }
    else {
	// Class is not array.  What to do ?
	std::cout << "JVArrayArray: internal error, received a non-array class to generate.\n";
	lastDimAction= 3;
    }

    if (nbrDims == 1) {
	for (unsigned int i= 0; i < dimCounts[0]; i++) {
	    (*tmpElementsPtr[0])[i]= (JVAbstractArray *)nullObject;
	}
    }
    else {
	curDim++;
	if (lastDimAction != 0) nbrDims--;	// We cut short one dim if the last dim will be handled by a different kind of array.
	do {
	    if (curDim < nbrDims) {
//		curDim++;
		(*tmpElementsPtr[curDim-1])[arrayCursors[curDim-1]]= new JVArrayArray(dimCounts[curDim]);
		tmpElementsPtr[curDim]= &((JVArrayArray *)(*tmpElementsPtr[curDim-1])[arrayCursors[curDim-1]])->elements;
		arrayCursors[curDim]= 0;
		curDim++;
	    }
	    else {
		unsigned int i;
		switch(lastDimAction) {
		    case 0:
			curDim--;			// Fast fill, immediately decrement current dimension.
			for (i= 0; i < dimCounts[curDim]; i++) {
			    (*tmpElementsPtr[curDim])[i]= (JVAbstractArray *)nullObject;
			}
			break;
		    case 1:
			(*tmpElementsPtr[curDim-1])[arrayCursors[curDim-1]]= new JVBasicArray(((ArrayClassFile *)type)->getBasicType(), dimCounts[curDim]);
			break;
		    case 2:
			(*tmpElementsPtr[curDim-1])[arrayCursors[curDim-1]]= new JVInstanceArray(type, dimCounts[curDim]);
			break;
		};
    
		arrayCursors[curDim-1]++;
		if (arrayCursors[curDim-1] == dimCounts[curDim - 1]) {
		    curDim--;
		    while (curDim > 0) {
			if (arrayCursors[curDim-1] == (dimCounts[curDim - 1]-1)) {
			    curDim--;
			}
			else {
			    arrayCursors[curDim-1]++;
			    break;
			}
		    }
		}
	    }
	} while (curDim != 0);
    }

    sizeUsed= sizeAllocated= sizeof(JVAbstractArray *) * dimCounts[0];
    nbrElements= dimCounts[0];
}


void JVArrayArray::reInitialize(ArrayClassFile *type, unsigned int *dimCounts)
{
    // TODO.
}


unsigned short JVArrayArray::vectorialType(void)
{
    return (TYPE_ARRAY | TYPE_ARRAY_ARRAY);
}


unsigned int JVArrayArray::dimensions(void)
{
    if (rootIsA->isArrayClass()) {
	return ((ArrayClassFile *)rootIsA)->dimensions();
    }
    else return 1;
}


JVAbstractArray *JVArrayArray::getElement(unsigned int index)
{
    if (index < nbrElements) {
	return elements[index];
     }
    else return NULL;
}


bool JVArrayArray::setElement(unsigned int index, JVAbstractArray *value)
{
    if (index < nbrElements) {
	elements[index]= value;
	return true;
     }
    else return false;
}


// Work in progress.

/**************************************************
* Implementation: JVDualInstance.
**************************************************/

void JVDualInstance::makeDualDirty(unsigned int dualIndex)
{

}


bool JVDualInstance::isDualDirty(unsigned int dualIndex)
{
    return true;
}


bool JVDualInstance::hasDualViews(void)
{
    return false;
}

