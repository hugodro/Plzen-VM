/**************************************************
* File: constantPool.cc.
* Desc: Definition of constant objects for inclusion in the 'Constant Pool'.
* Module: AkraLog : JavaKit.
* Rev: 3 mai 1996 : REV 0.1 : Hugo DesRosiers : Independance clean-up.
**************************************************/

extern "C" {
#include <string.h>
#include <stdlib.h>
 }

#include <akra/convertMacro.h>
#include "constantPool.h"
#include "classFile.h"
#include "jvcup.h"
#include "classManager.h"


/**************************************************
* Implementation: ClassFileConstant.
**************************************************/

unsigned int ClassFileConstant::type()
{
    return tag;
}


bool ClassFileConstant::read(std::istream *aStream, ClassFileConstant* &realCte)
{
     char tmpBuffer[4];

    aStream->read(tmpBuffer, sizeof(u1));
    tag= MakeUByte(tmpBuffer);
 
    switch (tag) {
	case CONSTANT_Class:
	    realCte= new CteClass;
	    break;
	case CONSTANT_FieldRef:
	    realCte= new CteFieldRef;
	    break;
	case CONSTANT_MethodRef:
	    realCte= new CteMethodRef;
	    break;
	case CONSTANT_String:
	    realCte= new CteString;
	    break;
	case CONSTANT_Integer:
	    realCte= new CteInteger;
	    break;
	case CONSTANT_Float:
	    realCte= new CteFloat;
	    break;
	case CONSTANT_Long:
	    realCte= new CteLong;
	    break;
	case CONSTANT_Double:
	    realCte= new CteDouble;
	    break;
	case CONSTANT_InterfaceMethodRef:
	    realCte= new CteInterfaceMethodRef;
	    break;
	case CONSTANT_NameAndType:
	    realCte= new CteNameAndType;
	    break;
	case CONSTANT_Utf8:
	    realCte= new CteUtf8;
	    break;
	case CONSTANT_Unicode:
	    realCte= new CteUnicode;
	    break;
	default:
	    std::cout << "Unknown constant.\n";
	    realCte= this;
	    break;
     }
    if (this != realCte) realCte->tag= tag;
    realCte->read(aStream);
    return true;
}


bool ClassFileConstant::read(std::istream *aStream)
{
    return true;
}


ClassFileObject *ClassFileConstant::resolve(ClassManager *source, ClassFile *owner)
{
    return NULL;
}


bool ClassFileConstant::isStrEqual(char *aString)
{
    return false;			// Default implentation.
}


bool ClassFileConstant::isStrEqual(ClassFileConstant **ctePool, char *aString)
{
    return false;			// Default implentation.
}


char * ClassFileConstant::stringValue(void)
{
    return NULL;
}


void ClassFileConstant::writeStringValue(char *dest)
{
}


char * ClassFileConstant::stringValue(ClassFileConstant **ctePool)
{
    return NULL;
}


void ClassFileConstant::writeStringValue(char *dest, ClassFileConstant **ctePool)
{
}


void ClassFileConstant::memberName(char *dest, ClassFileConstant **ctePool)
{
    ctePool[index]->writeStringValue(dest);
//    return true;		// Will eventually check for errors (index out of bound, no name found).
}


void ClassFileConstant::typeSignature(char *dest, ClassFileConstant **ctePool)
{
}


void ClassFileConstant::describe(ClassFileConstant **ctePool)
{
    std::cout << "ClassFileConstant: Generic info.\n";
}

/**************************************************
* Implementation: CteUtf8.
**************************************************/
bool CteUtf8::read(std::istream *aStream)
{
     char tmpBuffer[2];

    aStream->read(tmpBuffer, sizeof(u2));
    length= MakeUShort(tmpBuffer);
    bytes= new unsigned char[length+1];
    aStream->read((char *)bytes, length * sizeof(u1));
    bytes[length]= '\0';
//  std::cout << "CteUtf8: " << bytes << ".\n";
    return true;
}


// ATTENTION: Only work for ascii data right now.
bool CteUtf8::isStrEqual(char *aString)
{
    unsigned int strLength;
    bool result= false;

    strLength= strlen(aString);
    if (strLength == length) {
	if (strncmp((const char *)bytes, aString, length) == 0) result= true;
     }
    return result;
}


char * CteUtf8::stringValue(void)
{
    char *tmpValue;
    
    tmpValue= (char *)malloc(sizeof(char) * length+1);
    memcpy(tmpValue, bytes, length);
    tmpValue[length]= '\0';
    return tmpValue;
}


void CteUtf8::writeStringValue(char *dest)
{    
    memcpy(dest, bytes, length);
    dest[length]= '\0';
}


void CteUtf8::describe(ClassFileConstant **ctePool)
{
    char *tmpValue;
    
    tmpValue= (char *)malloc(sizeof(char) * length+1);
    memcpy(tmpValue, bytes, length);
    tmpValue[length]= '\0';
    std::cout << tmpValue;
    free(tmpValue);
}


unsigned int CteUtf8::getLength(void)
{
    return length;
}



/**************************************************
* Implementation: CteUnicode.
**************************************************/
bool CteUnicode::read(std::istream *aStream)
{
     char tmpBuffer[2];

    aStream->read(tmpBuffer, sizeof(u2));
    length= MakeUShort(tmpBuffer);
    bytes= new unsigned char[length+2];
    aStream->read((char *)bytes, length * sizeof(u1));
    bytes[length]= (unsigned char)0x0C0;
    bytes[length+1]= (unsigned char)0x080;
    return true;
}


char * CteUnicode::stringValue(void)
{
    char *tmpValue;
    
    tmpValue= (char *)malloc(sizeof(char) * length+1);
    memcpy(tmpValue, bytes, length);
    tmpValue[length]= '\0';
    return tmpValue;
}


void CteUnicode::writeStringValue(char *dest)
{    
    memcpy(dest, bytes, length);
    dest[length]= '\0';
}


// ATTENTION: Only work for ascii data right now.
bool CteUnicode::isStrEqual(char *aString)
{
    unsigned int strLength;
    bool result= false;

    strLength= strlen(aString);
    if (strLength == length) {
	if (strncmp((const char *)bytes, aString, length) == 0) result= true;
     }
    return result;
}

/**************************************************
* Implementation: CteClass.
**************************************************/
bool CteClass::read(std::istream *aStream)
{
     char tmpBuffer[2];

    aStream->read(tmpBuffer, sizeof(u2));
    index= MakeUShort(tmpBuffer);
    return true;
}


ClassFileObject *CteClass::resolve(ClassManager *source, ClassFile *owner)
{
    char *className;

    if (!isResolved) {
    	className= owner->constantPool[index]->stringValue();
	if (className[0] == '[') {		// This is an Array of Class.
	    JVArrayTypeDef *arrayDefinition;
//	    unsigned short tmpType;	// MOD-970520 [HD]: Pour NT.

	    FieldInfo::parseSignature(className, (JVTypeDefinition **)&arrayDefinition);

	    if (arrayDefinition->finalType()->metaType() == TYPE_BASIC) {
		dereference= new ArrayClassFile(((JVBasicTypeDef *)arrayDefinition->finalType())->type(), arrayDefinition->dimensions());
	     }
	    else {
		char tmpName[512];
		((JVClassTypeDef *)arrayDefinition->finalType())->getName(tmpName);
		dereference= source->loadArrayClass(tmpName, arrayDefinition->dimensions());
	     }
	    isResolved= true;
	    delete arrayDefinition;
	 }
	else {
	    if ((dereference= source->loadClass(className)) != NULL) {
		isResolved= true;
	     }
	}
	delete[] className;
     }
    return dereference;
}


bool CteClass::isStrEqual(ClassFileConstant **ctePool, char *aString)
{
    return ctePool[index]->isStrEqual(aString);
}


char * CteClass::stringValue(ClassFileConstant **ctePool)
{
    return ctePool[index]->stringValue();
}


void CteClass::writeStringValue(char *dest, ClassFileConstant **ctePool)
{    
    ctePool[index]->writeStringValue(dest);
}


void CteClass::describe(ClassFileConstant **ctePool)
{
    ctePool[index]->describe(ctePool);
}


/**************************************************
* Implementation: CteFieldRef.
**************************************************/
bool CteFieldRef::read(std::istream *aStream)
{
     char tmpBuffer[2];

    aStream->read(tmpBuffer, sizeof(u2));
    index= MakeUShort(tmpBuffer);
    aStream->read(tmpBuffer, sizeof(u2));
    nameTypeIndex= MakeUShort(tmpBuffer);
    return true;
}


ClassFileObject *CteFieldRef::resolve(ClassManager *source, ClassFile *owner)
{
    ClassFile *container;
    char *className, fieldName[120];

    if (!isResolved) {
    		// First, must find the class in which it is defined.
    	className= owner->constantPool[index]->stringValue(owner->constantPool);
	if (className != NULL) {
	    if ((container= source->loadClass(className)) != NULL) {
		owner->constantPool[nameTypeIndex]->memberName(fieldName, owner->constantPool);
		if ((dereference= container->getFieldNamed(fieldName)) != NULL) {
		    isResolved= true;
		 }
	     }
	    delete[] className;
	 }
     }
    return dereference;
}


void CteFieldRef::describe(ClassFileConstant **ctePool)
{
    std::cout << "Lookup in '";
    ctePool[index]->describe(ctePool);
    std::cout << "',  field '";
    ctePool[nameTypeIndex]->describe(ctePool);
    std::cout << ".\n";
}


/**************************************************
* Implementation: CteMethodRef.
**************************************************/
bool CteMethodRef::read(std::istream *aStream)
{
     char tmpBuffer[2];

    aStream->read(tmpBuffer, sizeof(u2));
    index= MakeUShort(tmpBuffer);
    aStream->read(tmpBuffer, sizeof(u2));
    nameTypeIndex= MakeUShort(tmpBuffer);
    return true;
}


ClassFileObject *CteMethodRef::resolve(ClassManager *source, ClassFile *owner)
{
    ClassFile *container;
    char className[120], methodName[120], methodTyping[250];

    if (!isResolved) {
    		// First, must find the class in which it is defined.
    	owner->constantPool[index]->writeStringValue(className, owner->constantPool);
        dereference= NULL;
	if ((container= source->loadClass(className)) != NULL) {
	    owner->constantPool[nameTypeIndex]->memberName(methodName, owner->constantPool);
	    owner->constantPool[nameTypeIndex]->typeSignature(methodTyping, owner->constantPool);
	    if ((dereference= container->getMethodNamed(methodName, methodTyping)) != NULL) {
		isResolved= true;
	     }
	 }
     }
    return dereference;
}


char * CteMethodRef::stringValue(ClassFileConstant **ctePool)
{
    
    return ctePool[nameTypeIndex]->stringValue(ctePool);
}


void CteMethodRef::describe(ClassFileConstant **ctePool)
{
    std::cout << "Lookup in '";
    ctePool[index]->describe(ctePool);
    std::cout << "', method '";
    ctePool[nameTypeIndex]->describe(ctePool);
    std::cout << ".\n";
}


/**************************************************
* Implementation: CteInterfaceMethodRef.
**************************************************/
bool CteInterfaceMethodRef::read(std::istream *aStream)
{
     char tmpBuffer[2];

    aStream->read(tmpBuffer, sizeof(u2));
    index= MakeUShort(tmpBuffer);
    aStream->read(tmpBuffer, sizeof(u2));
    nameTypeIndex= MakeUShort(tmpBuffer);
    return true;
}


void CteInterfaceMethodRef::describe(ClassFileConstant **ctePool)
{
    std::cout << "Lookup in interface'";
    ctePool[index]->describe(ctePool);
    std::cout << "', method '";
    ctePool[nameTypeIndex]->describe(ctePool);
    std::cout << "\n";
}


ClassFileObject *CteInterfaceMethodRef::resolve(ClassManager *source, ClassFile *owner)
{
    ClassFile *container;
    char className[120], methodName[120], methodTyping[250];

    if (!isResolved) {
    		// First, must find the class in which it is defined.
    	owner->constantPool[index]->writeStringValue(className, owner->constantPool);
	if ((container= source->loadClass(className)) != NULL) {
	    owner->constantPool[nameTypeIndex]->memberName(methodName, owner->constantPool);
	    owner->constantPool[nameTypeIndex]->typeSignature(methodTyping, owner->constantPool);
	    if ((dereference= container->getMethodNamed(methodName, methodTyping)) != NULL) {
		isResolved= true;
	     }
	 }
     }
    return dereference;
}


/**************************************************
* Implementation: CteNameAndType.
**************************************************/
bool CteNameAndType::read(std::istream *aStream)
{
     char tmpBuffer[2];

    aStream->read(tmpBuffer, sizeof(u2));
    index= MakeUShort(tmpBuffer);
    aStream->read(tmpBuffer, sizeof(u2));
    signatureIndex= MakeUShort(tmpBuffer);
    return true;
}


void CteNameAndType::memberName(char *dest, ClassFileConstant **ctePool)
{
    ctePool[index]->writeStringValue(dest);
//    return true;		// Will eventually check for errors (index out of bound, no name found).
}


void CteNameAndType::typeSignature(char *dest, ClassFileConstant **ctePool)
{
    ctePool[signatureIndex]->writeStringValue(dest);
}


ClassFileObject *CteNameAndType::resolve(ClassManager *source, ClassFile *ownerl)
{
    if (!isResolved) {
// ATTENTION: TO DO !
     }

    return dereference;
}


void CteNameAndType::describe(ClassFileConstant **ctePool)
{
    ctePool[index]->describe(ctePool);
    std::cout << "', of type '";
    ctePool[signatureIndex]->describe(ctePool);
    std::cout << "'";
}


/**************************************************
* Implementation: CteString.
**************************************************/
bool CteString::read(std::istream *aStream)
{
     char tmpBuffer[2];

    aStream->read(tmpBuffer, sizeof(u2));
    index= MakeUShort(tmpBuffer);
    return true;
}


ClassFileObject *CteString::resolve(ClassManager *source, ClassFile *owner)
{
// ATTENTION: must implement the 'return' of a constant instance of String class.
    if (!isResolved) {
	dereference= (source->getOwner())->createStringConstant(owner->getConstant(index)->stringValue());
	isResolved= true;
     }

    return dereference;
}


void CteString::describe(ClassFileConstant **ctePool)
{
    std::cout << "String: ";
    ctePool[index]->describe(ctePool);
}


/**************************************************
* Implementation: CteWord.
**************************************************/
bool CteWord::read(std::istream *aStream)
{
     char tmpBuffer[4];

    aStream->read(tmpBuffer, sizeof(u4));
    bytes= MakeUInteger(tmpBuffer);
    return true;
}


unsigned int CteWord::value()
{
    return bytes;
}


/**************************************************
* Implementation: CteInteger.
**************************************************/
void CteInteger::describe(ClassFileConstant **ctePool)
{
    std::cout << "Integer: " << bytes;
}


/**************************************************
* Implementation: CteFloat.
**************************************************/
void CteFloat::describe(ClassFileConstant **ctePool)
{
    float *tmpFloat;
    tmpFloat= (float *)&bytes;

    std::cout << "Float: " << *tmpFloat;
}


/**************************************************
* Implementation: CteDoubleWord.
**************************************************/
bool CteDoubleWord::read(std::istream *aStream)
{
     char tmpBuffer[8];

    aStream->read(tmpBuffer, 2 * sizeof(u4));
    bytes= MakeUInteger((tmpBuffer + 4));
    high_bytes= MakeUInteger(tmpBuffer);
    return true;
}


unsigned int CteDoubleWord::valueLow()
{
    return bytes;
}


unsigned int CteDoubleWord::valueHigh()
{
    return high_bytes;
}


void CteDoubleWord::value(char **dest)
{
    memcpy(dest, &bytes, 4);
    memcpy(dest+4, &high_bytes, 4); 
}


/**************************************************
* Implementation: CteLong.
**************************************************/
void CteLong::describe(ClassFileConstant **ctePool)
{
    std::cout << "Long: h:" << high_bytes << " + l:" << bytes;
}


/**************************************************
* Implementation: CteDouble.
**************************************************/
void CteDouble::describe(ClassFileConstant **ctePool)
{
    double *tmpDouble;
    tmpDouble= (double *)&bytes;

    std::cout << "Double: " << *tmpDouble;
}



