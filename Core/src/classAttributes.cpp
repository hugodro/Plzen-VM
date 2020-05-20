/**************************************************
* File: classAttributes.cc.
* Desc: Implementation of the optional attributes modifying basic Info descriptors in class files.
* Module: AkraLog : JavaKit.
* Rev: 3 mai 1996 : REV 0.1 : Hugo DesRosiers : Independance clean-up.
**************************************************/

#include <akra/convertMacro.h>
#include "classAttributes.h"


/**************************************************
* Implementation: AttributeInfo.
**************************************************/

AttributeInfo::AttributeInfo(unsigned int aName, unsigned int aLength)
{
    name= aName;
    length= aLength;
}


AttributeInfo::AttributeInfo()
{
    AttributeInfo(0, 0);
}


bool AttributeInfo::read(std::istream *aStream, ClassFileConstant **ctePool, AttributeInfo* &realAttribute)
{
     char tmpBuffer[4];

    aStream->read(tmpBuffer, sizeof(u2));
    name= MakeUShort(tmpBuffer);
    aStream->read(tmpBuffer, sizeof(u4));
    length= MakeUInteger(tmpBuffer);
 
    if (name > 0) {
	if (ctePool[name]->isStrEqual("SourceFile")) {
	    realAttribute= new SourceFileAttribute(name, length);
	}
	else if (ctePool[name]->isStrEqual("ConstantValue")) {
	    realAttribute= new ConstantValueAttribute(name, length);
	}
	else if (ctePool[name]->isStrEqual("Code")) {
	    realAttribute= new CodeAttribute(name, length);
	}
/* ATTN-970516 [HD]: The FatCode attribute will contain micro-cpu specific code.
	else if (ctePool[name]->isStrEqual("FatCode")) {
	    realAttribute= new CodeAttribute(name, length);
	}
*/

#if defined(PLZEN_LEVEL_2)
	else if (ctePool[name]->isStrEqual("Transitions")) {
	    realAttribute= new TransitionsAttribute(name, length);
	}	
	else if (ctePool[name]->isStrEqual("StateCode")) {
	    realAttribute= new StateCodeAttribute(name, length);
	}	
#endif
	else if (ctePool[name]->isStrEqual("Exceptions")) {
	    realAttribute= new ExceptionAttribute(name, length);
	}
	else if (ctePool[name]->isStrEqual("LineNumberTable")) {
	    realAttribute= new LineNumberTableAttribute(name, length);
	}
	else if (ctePool[name]->isStrEqual("LocalVariableTable")) {
	    realAttribute= new LocalVariableTableAttribute(name, length);
	}	
	else {
	    char tmpAttrName[80];
	    ctePool[name]->writeStringValue(tmpAttrName);
	    std::cout << "WRNMSG: Unknown attribute called " << tmpAttrName;
	    std::cout << " in current classfile, skipping.\n";
	    realAttribute= this;
	}
    }
    else {
	    std::cout << "ERRMSG: attribute with bad name index in current classfile, skipping.\n";
	    realAttribute= this;
    }
    realAttribute->read(aStream, ctePool);
    return true;
}


bool AttributeInfo::read(std::istream *aStream, ClassFileConstant **ctePool)
{
    this->type= ATTR_GENERIC;
    aStream->seekg(length, std::ios::cur);
    return true;
}


bool AttributeInfo::read(std::istream *aStream)
{
     char tmpBuffer[4];

    if (ClassFileObject::read(aStream) == true) {
	aStream->read(tmpBuffer, sizeof(u2));
	name= MakeUShort(tmpBuffer);
	aStream->read(tmpBuffer, sizeof(u4));
	length= MakeUInteger(tmpBuffer);
     }
    return true;
}

void AttributeInfo::describe(ClassFileConstant **ctePool)
{

}


unsigned int AttributeInfo::category(void)
{
    return type;
}


unsigned int AttributeInfo::codeSize(void)
{
    return 0;
}


ubyte *AttributeInfo::getCode(void)
{
    return NULL;
}


unsigned int AttributeInfo::nbrLocals(void)
{
    return 0;
}


unsigned int AttributeInfo::stackDepth(void)
{
    return 0;
}


/**************************************************
* Implementation: SourceFileAttribute.
**************************************************/

bool SourceFileAttribute::read(std::istream *aStream, ClassFileConstant **ctePool)
{
     char tmpBuffer[4];

    type= ATTR_SRCFILE;
    aStream->read(tmpBuffer, sizeof(u2));
    sourceFileIndex= MakeUShort(tmpBuffer);
    return true;
}


void SourceFileAttribute::describe(ClassFileConstant **ctePool)
{
    if (sourceFileIndex > 0) {
	std::cout << "SourceFile: ";
	ctePool[sourceFileIndex]->describe(ctePool);
	std::cout << ".\n";
     }
}


/**************************************************
* Implementation: ConstantValueAttribute.
**************************************************/

bool ConstantValueAttribute::read(std::istream *aStream, ClassFileConstant **ctePool)
{
     char tmpBuffer[4];

    type= ATTR_CONSTANT;
    aStream->read(tmpBuffer, sizeof(u2));
    constantValueIndex= MakeUShort(tmpBuffer);
    return true;
}


void ConstantValueAttribute::describe(ClassFileConstant **ctePool)
{
    if (constantValueIndex > 0) {
	std::cout << "ConstantValue: ";
	ctePool[constantValueIndex]->describe(ctePool);
	std::cout << ".\n";
     }
}


/**************************************************
* Implementation: CodeAttribute.
**************************************************/

bool CodeAttribute::read(std::istream *aStream, ClassFileConstant **ctePool)
{
    unsigned int i;
     char tmpBuffer[4];

    type= ATTR_CODE;

    aStream->read(tmpBuffer, sizeof(u2));
    maxStack= MakeUShort(tmpBuffer);
    aStream->read(tmpBuffer, sizeof(u2));
    maxLocals= MakeUShort(tmpBuffer);
    aStream->read(tmpBuffer, sizeof(u4));
    codeLength= MakeUInteger(tmpBuffer);
    if (codeLength > 0) {
	code= new ubyte[codeLength];
	aStream->read((char *)code, codeLength * sizeof(ubyte));
     }

    aStream->read(tmpBuffer, sizeof(u2));
    exceptionTableLength= MakeUShort(tmpBuffer);
    if (exceptionTableLength > 0) {
	exceptionTable= new ExceptionSubAttr[exceptionTableLength];
	for (i= 0; i < exceptionTableLength; i++) {
	    exceptionTable[i].read(aStream);
	 }
     }

    aStream->read(tmpBuffer, sizeof(u2));
    attributeCount= MakeUShort(tmpBuffer);
    if (attributeCount > 0) {
	AttributeInfo *tmpAttrInfo, *realAttribute;
	attributes= new AttributeInfo*[attributeCount];
	for (i= 0; i < attributeCount; i++) {
	    tmpAttrInfo= new AttributeInfo;
	    tmpAttrInfo->read(aStream, ctePool, realAttribute);
	    attributes[i]= realAttribute;
	    if (tmpAttrInfo != realAttribute) delete tmpAttrInfo;
	 }
     }

    return true;
}


void CodeAttribute::describe(ClassFileConstant **ctePool)
{
    unsigned int i;

    std::cout << "Code: stack " << maxStack << ", locals " << maxLocals;
    std::cout << ", length " << codeLength << " bytes,\n";
    for (i= 0; i < exceptionTableLength; i++) {

     }

    for (i= 0; i < attributeCount; i++) {
	attributes[i]->describe(ctePool);
     }
}


unsigned int CodeAttribute::codeSize(void)
{
    return codeLength;
}


ubyte *CodeAttribute::getCode(void)
{
    return code;
}


unsigned int CodeAttribute::nbrLocals(void)
{
    return maxLocals;
}


unsigned int CodeAttribute::stackDepth(void)
{
    return maxStack;
}


unsigned int CodeAttribute::getNbrExceptions(void)
{
    return exceptionTableLength;
}



/**************************************************
* Implementation: ExceptionAttribute.
**************************************************/

bool ExceptionAttribute::read(std::istream *aStream, ClassFileConstant **ctePool)
{
     char tmpBuffer[4];

    type= ATTR_EXCEPTION;
    aStream->read(tmpBuffer, sizeof(u2));
    exceptionCount= MakeUShort(tmpBuffer);

    if (exceptionCount > 0) {
	exceptionTable= new unsigned int[exceptionCount];
	for (unsigned int i= 0; i < exceptionCount; i++) {
	    aStream->read(tmpBuffer, sizeof(u2));
	    exceptionTable[i]= MakeUShort(tmpBuffer);
	 }
     }

    return true;
}


/**************************************************
* Implementation: LineNumberTableAttribute.
**************************************************/

bool LineNumberTableAttribute::read(std::istream *aStream, ClassFileConstant **ctePool)
{
     char tmpBuffer[4];

    type= ATTR_LINENUMBER;
    aStream->read(tmpBuffer, sizeof(u2));
    tableLength= MakeUShort(tmpBuffer);

    if (tableLength > 0) {
	lineNumberTable= new LineNumberSubAttr[tableLength];
	for (unsigned int i= 0; i < tableLength; i++) {
	    lineNumberTable[i].read(aStream);
	 }
     }

    return true;
}


void LineNumberTableAttribute::describe(ClassFileConstant **ctePool)
{
    unsigned int i;

    for (i= 0; i < tableLength; i++) {
	lineNumberTable[i].describe(ctePool);
     }
}


/**************************************************
* Implementation: LocalVariableTableAttribute.
**************************************************/

bool LocalVariableTableAttribute::read(std::istream *aStream, ClassFileConstant **ctePool)
{
     char tmpBuffer[4];

    type= ATTR_LOCALVAR;
    aStream->read(tmpBuffer, sizeof(u2));
    tableLength= MakeUShort(tmpBuffer);

    if (tableLength > 0) {
	localVariableTable= new LocalVariableSubAttr[tableLength];
	for (unsigned int i= 0; i < tableLength; i++) {
	    localVariableTable[i].read(aStream);
	 }
     }
    return true;
}


void LocalVariableTableAttribute::describe(ClassFileConstant **ctePool)
{
    unsigned int i;

    for (i= 0; i < tableLength; i++) {
	localVariableTable[i].describe(ctePool);
     }
}


/**************************************************
* Implementation: ExceptionSubAttr.
**************************************************/

bool ExceptionSubAttr::read(std::istream *aStream)
{
     char tmpBuffer[4];

    aStream->read(tmpBuffer, sizeof(u2));
    startPC= MakeUShort(tmpBuffer);
    aStream->read(tmpBuffer, sizeof(u2));
    endPC= MakeUShort(tmpBuffer);
    aStream->read(tmpBuffer, sizeof(u2));
    handlerPC= MakeUShort(tmpBuffer);
    aStream->read(tmpBuffer, sizeof(u2));
    catchType= MakeUShort(tmpBuffer);

    return true;
}


/**************************************************
* Implementation: LineNumberSubAttr.
**************************************************/

bool LineNumberSubAttr::read(std::istream *aStream)
{
     char tmpBuffer[4];

    aStream->read(tmpBuffer, sizeof(u2));
    startPC= MakeUShort(tmpBuffer);
    aStream->read(tmpBuffer, sizeof(u2));
    lineNumber= MakeUShort(tmpBuffer);

    return true;
}


void LineNumberSubAttr::describe(ClassFileConstant **ctePool)
{
    std::cout << "LineNumber: " << lineNumber << " start at PC: " << startPC << ".\n";
}


/**************************************************
* Implementation: LocalVariableSubAttr.
**************************************************/

bool LocalVariableSubAttr::read(std::istream *aStream)
{
     char tmpBuffer[4];

    aStream->read(tmpBuffer, sizeof(u2));
    startPC= MakeUShort(tmpBuffer);
    aStream->read(tmpBuffer, sizeof(u2));
    length= MakeUShort(tmpBuffer);
    aStream->read(tmpBuffer, sizeof(u2));
    nameIndex= MakeUShort(tmpBuffer);
    aStream->read(tmpBuffer, sizeof(u2));
    signatureIndex= MakeUShort(tmpBuffer);
    aStream->read(tmpBuffer, sizeof(u2));
    slot= MakeUShort(tmpBuffer);

    return true;
}


void LocalVariableSubAttr::describe(ClassFileConstant **ctePool)
{
    std::cout << "Var: ";
    ctePool[nameIndex]->describe(ctePool);
    std::cout << " of type ";
    ctePool[signatureIndex]->describe(ctePool);
    std::cout << " start at PC: " << startPC << " for length " << length << " as slot " << slot << ".\n";
}


/**************************************************
* Implementation: StateCodeAttribute.
**************************************************/

#if defined(PLZEN_LEVEL_2)
bool StateCodeAttribute::read(std::istream *aStream, ClassFileConstant **ctePool)
{
    bool result;

    result= CodeAttribute::read(aStream, ctePool);
    type= ATTR_STATE_CODE;
    return result;
}


void StateCodeAttribute::describe(ClassFileConstant **ctePool)
{
    std::cout << "State ";
    CodeAttribute::describe(ctePool);
}


/**************************************************
* Implementation: TransitionsAttribute.
**************************************************/

bool TransitionsAttribute::read(std::istream *aStream, ClassFileConstant **ctePool)
{
     char tmpBuffer[4];

    type= ATTR_TRANSITIONS;
    aStream->read(tmpBuffer, sizeof(u2));
    modifiers= MakeUShort(tmpBuffer);
    aStream->read(tmpBuffer, sizeof(u2));
    eventIndex= MakeUShort(tmpBuffer);
    aStream->read(tmpBuffer, sizeof(u2));
    destinationIndex= MakeUShort(tmpBuffer);
    aStream->read(tmpBuffer, sizeof(u4));
    startPC= MakeUInteger(tmpBuffer);

    return true;
}


void TransitionsAttribute::describe(ClassFileConstant **ctePool)
{
    std::cout << "Transition: ";
    std::cout << "modifiers: " << modifiers << ".\n";
    std::cout << "Event: ";
    if (eventIndex > 0) {
	ctePool[eventIndex]->describe(ctePool);
    }
    else {
	std::cout << "None.\n";
    }
    std::cout << "Destination: ";
    if (destinationIndex > 0) {
	ctePool[destinationIndex]->describe(ctePool);
    }
    else {
	std::cout << "None.\n";
    }
    std::cout << "startPC: " << startPC << ".\n";
}


bool TransitionsAttribute::isModified(unsigned int flags)
{
    return (modifiers & flags) != 0;
}


#endif

