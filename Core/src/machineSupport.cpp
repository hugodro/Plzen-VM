/**************************************************
* File: machineSupport.cc.
* Desc: Implementation of the classes that are used during a machine execution.
* Module: AkraLog : JavaKit.
* Rev: 9 mai 1996 : REV 0 : Hugo DesRosiers : Creation.
**************************************************/

#if defined(_WIN32) || defined(IRIX) || defined(__linux__)
extern "C" {
#include <string.h>
}
#endif

#include <threader/synchro.h>

#include "classFile.h"
#include "classManager.h"
#include "math64bits.h"
#include "runInfo.h"
#include "machineSupport.h"
#include "dualWorld.h"


/**************************************************
* Implementation: JVTypeDefinition.
**************************************************/
JVTypeDefinition::JVTypeDefinition()
{
    flags= 0;
}


ubyte JVTypeDefinition::metaType(void)
{
    return flags;
}


bool JVTypeDefinition::isEqual(JVTypeDefinition *aType)
{
    if (flags == aType->flags) return true;
    else return false;
}


unsigned short JVTypeDefinition::runType(void)
{
    return 0;	// Should not be called.
}

/**************************************************
* Implementation: JVBasicTypeDef.
**************************************************/

JVBasicTypeDef::JVBasicTypeDef(void)
{
    flags= TYPE_BASIC;
    basicType= JVM_TYPE_VOID;
}


JVBasicTypeDef::JVBasicTypeDef(char aTypeLetter)
{
    if ((aTypeLetter == JVM_TYPE_DOUBLE) || (aTypeLetter == JVM_TYPE_LONG)) {
	flags= TYPE_BASELONG;
    }
    else {
	flags= TYPE_BASIC;
    }
    basicType= aTypeLetter;
}

ubyte JVBasicTypeDef::type(void)
{
    return basicType;
}


bool JVBasicTypeDef::isEqual(JVTypeDefinition *aType)
{
    if (JVTypeDefinition::isEqual(aType)) {
	if (basicType == ((JVBasicTypeDef *)aType)->basicType) return true;
	else return false;
     }
    else return false;
}


unsigned short JVBasicTypeDef::runType(void)
{
    if ((basicType != JVM_TYPE_DOUBLE) && (basicType != JVM_TYPE_LONG))
	return (unsigned short)TYPE_BASIC_32;
    else
	return (unsigned short)TYPE_BASIC_64H;
}


/**************************************************
* Implementation: JVClassTypeDef.
**************************************************/

JVClassTypeDef::JVClassTypeDef(char *aName)
{
    this->init(aName, strlen(aName));
}


JVClassTypeDef::JVClassTypeDef(char *aName, unsigned int nameLength)
{
    this->init(aName, nameLength);
}


void JVClassTypeDef::init(char *aName, unsigned int aLength)
{
    flags= TYPE_CLASS;
    className= new char[aLength+1];
    memcpy(className, aName, aLength);
    className[aLength]= '\0';
}


void JVClassTypeDef::setMemClass(ClassFile *aClass)
{
    inMemClass= aClass;
}


void JVClassTypeDef::getName(char *dest)
{
    strcpy(dest, className);
}


bool JVClassTypeDef::isEqual(JVTypeDefinition *aType)
{
    if (JVTypeDefinition::isEqual(aType)) {
	if (strcmp(className, ((JVClassTypeDef *)aType)->className) == 0) return true;
	else return false;
     }
    else return false;
}


unsigned short JVClassTypeDef::runType(void)
{
    return TYPE_CLASS;
}


/**************************************************
* Implementation: JVArrayTypeDef.
**************************************************/

JVArrayTypeDef::JVArrayTypeDef(void)
{
    flags= TYPE_ARRAY;
    size= 0;
    final= true;
    elementType= NULL;
}


JVArrayTypeDef::~JVArrayTypeDef(void)
{
    delete elementType;
}


JVArrayTypeDef::JVArrayTypeDef(JVTypeDefinition *aType, unsigned int optSize)
{
    flags= TYPE_ARRAY;
    size= optSize;
    final= (aType->metaType() != TYPE_ARRAY);
    elementType= aType;
}


void JVArrayTypeDef::setElementType(JVTypeDefinition *aType, unsigned int optSize)
{
    size= optSize;
    final= (aType->metaType() != TYPE_ARRAY);
    elementType= aType;
}


bool JVArrayTypeDef::isEqual(JVTypeDefinition *aType)
{
    if (JVTypeDefinition::isEqual(aType)) {
	if (final == ((JVArrayTypeDef *)aType)->final) {
	    return elementType->isEqual(((JVArrayTypeDef *)aType)->elementType);
	 }
	else return false;
     }
    else return false;
}


unsigned short JVArrayTypeDef::runType(void)
{
    if (final) {
	if (elementType->metaType() == TYPE_BASIC)
	    return TYPE_BASIC_ARRAY;
	else
	    return TYPE_INST_ARRAY;
     }
    else {
	return TYPE_ARRAY_ARRAY;
     }
}


unsigned int JVArrayTypeDef::dimensions(void)
{
    JVArrayTypeDef *tmpArrayPtr= this;
    unsigned int nbrDims= 1;

    while (!tmpArrayPtr->final) {
	nbrDims++;
	tmpArrayPtr= (JVArrayTypeDef *)tmpArrayPtr->elementType;
     }

    return nbrDims;
}


JVTypeDefinition *JVArrayTypeDef::finalType(void)
{
    JVArrayTypeDef *tmpArrayPtr= this;

    while (!tmpArrayPtr->final) {
	tmpArrayPtr= (JVArrayTypeDef *)tmpArrayPtr->elementType;
     }

    return tmpArrayPtr->elementType;
}


/**************************************************
* Implementation: JVVariable.
**************************************************/

JVVariable::JVVariable(ClassFile *owner, FieldInfo *info)
{
    ClassFileConstant *aCte;
    char *signature;

    flags= info->accessFlags;
    size= 0;

	// Resolve the field name.
    if ((aCte= owner->getConstant(info->nameIndex)) != NULL) {
	if (aCte->type() == CONSTANT_Utf8) {
	    name= aCte->stringValue();
	 }
	else {
	    name= new char[8];
	    strcpy(name, "Unknown");
	 }
     }

	// Resolve the field type.
    if ((aCte= owner->getConstant(info->signatureIndex)) != NULL) {
	if (aCte->type() == CONSTANT_Utf8) {
	    signature= aCte->stringValue();
	 }
	else {
	    signature= new char[2];
	    strcpy(name, "V");
	 }
	if (FieldInfo::parseSignature(signature, &isA) < 0) {
	    isA= new JVBasicTypeDef(JVM_TYPE_INTEGER);
	 }
	delete[] signature;
     }
    if (flags & ACC_STATIC) {
	if (isA != NULL) {
	    if (isA->metaType() & TYPE_BASELONG) {
		staticValue= (void *)new Value64Bits();
	    }
	}
	hasStaticValue= false;	// In case we are static.
     }
}


bool JVVariable::resolveTyping(ClassManager *classManager)
{
    ClassFile *tmpClass;
    char tmpClassName[255];
    bool result= true;

    if (isA->metaType() == TYPE_CLASS) {
	((JVClassTypeDef *)isA)->getName(tmpClassName);
	if ((tmpClass= classManager->loadClass(tmpClassName)) != NULL) {
	    ((JVClassTypeDef *)isA)->setMemClass(tmpClass);
	 }
	else result= false;
     }

    return result;
}


void JVVariable::setPosition(unsigned int i)
{
    offset= i;
    if (isA) {
	switch (isA->metaType()) {
	    case TYPE_BASIC:
		switch (((JVBasicTypeDef *)isA)->type()) {
		    case JVM_TYPE_BOOLEAN:
		    case JVM_TYPE_SIGNED_BYTE:
		    case JVM_TYPE_CHAR:
			size= 1; break;
		    case JVM_TYPE_UNSIGNED_SHORT:
			size= 2; break;
		    case JVM_TYPE_FLOAT:
		    case JVM_TYPE_INTEGER:
			size= 4; break;
		    case JVM_TYPE_LONG:
		    case JVM_TYPE_DOUBLE:
			size= 8; break;
		    default: 		// Internal error if this happen.
			size= 0; break;
		 }
		break;
	    case TYPE_CLASS:
	    case TYPE_ARRAY:
		size= 4;
		break;
	    default:
		size= 0;	// Internal error if this happen.
		break;
	 }
     }
}


unsigned int JVVariable::position(void)
{
    return offset;
}


ubyte JVVariable::metaType(void)
{
    if (isA) return isA->metaType();
    else return TYPE_CLASS;
}


unsigned int JVVariable::varSize(void)
{
    return size;
}


bool JVVariable::isStatic(void)
{
    return ((flags & ACC_STATIC) != 0);
}


bool JVVariable::isFinal(void)
{
    return ((flags & ACC_FINAL) != 0);
}


bool JVVariable::setStaticValue(JVInstance *anInstance)
{
    staticValue= anInstance;
    hasStaticValue= true;
    return true;			// ATTENTION: Will we check here for conformance of 'static' ?
}


bool JVVariable::setStaticValue(Value64Bits *aValue)
{
    *(Value64Bits *)staticValue= *aValue;
    hasStaticValue= true;
    return true;			// ATTENTION: Will we check here for conformance of 'static' ?
}


JVInstance *JVVariable::getStaticValue(void)
{
    if (hasStaticValue) {
	return (JVInstance *)staticValue;
     }
    else return NULL;
}


unsigned short JVVariable::detailedType(void)
{
    if (isA) return isA->runType();
    else return TYPE_CLASS;
}


/**************************************************
* Implementation: JVMethod.
**************************************************/

JVMethod::JVMethod(void)
{
  // ATTN: Juste pour faire plaisir a GCC.
  // Should throw an error, there is no membership for the method.
}


JVMethod::JVMethod(ClassFile *owner, MethodInfo *info)
{
    ClassFileConstant *aCte;
    char *signature;

    implementor= owner;		// Keep track from where we come from.

    // Resolve the method name.
    if ((aCte= owner->getConstant(info->nameIndex)) != NULL) {
	if (aCte->type() == CONSTANT_Utf8) {
	    name= aCte->stringValue();
	 }
	else {
	    name= new char[8];
	    strcpy(name, "Unknown");
	 }
     }

	// Resolve the method flags.
    flags= info->accessFlags;

	// Note if this is a constructor (for FSM spawning).
    if (strcmp(name, "<init>") == 0) flags|= INTERN_ACC_CONSTR;

	// Resolve the arguments.    
    if (!(flags & ACC_ABSTRACT)) {
	// Resolve the number of local vars.
	if ((aCte= owner->getConstant(info->signatureIndex)) != NULL) {
	    if (aCte->type() == CONSTANT_Utf8) {
		signature= aCte->stringValue();
	     }
	    else {
		signature= new char[4];
		strcpy(name, "()V");
	      }

	    // Count number of arguments:
	    nbrArguments= MethodInfo::parseArguments(signature, &argumentTyping);
	    delete[] signature;
	    if (nbrArguments == 0) {	// Means that no args or return value were found.
		argumentTyping= new JVTypeDefinition*[2];
		argumentTyping[0]= new JVBasicTypeDef;    // By default, a basic type sets itself to void.
		argumentTyping[1]= new JVBasicTypeDef;    // By default, a basic type sets itself to void.
		nbrArguments= 1;
	     }
	    else nbrArguments--;		// Take of the return value from the count.
	 }
	// Resolve the stack size.
	nbrLocals= info->maxLocals();

	// MOD-970628 [HD]: Sun javac puts the count of locals to 0 if it is native.  We want to have
	//	the number of arguments included, like for non-native.
	if (flags & ACC_NATIVE) {
	    nbrLocals= nbrArguments + ((flags & ACC_STATIC) ? 0 : 1);
	}
	stackMaxDepth= info->stackMaxDepth();
	// Resolve the code info.
	codeLength= info->getCodeLength();
	code= info->getCode();
// TODO:
	// Resolve debug information (var names + line number assoc).
	// Resolve the 1st order function that implements this method.
	// ATTN-970628[HD]: The 1st order is seeked already, no need to resolve.
	// Create a table of exception handlers.
	nbrExceptions= info->getNbrExceptions();
	if (nbrExceptions > 0) {
	    exceptHandlers= new JVException[nbrExceptions];
	    for (unsigned int i= 0; i < nbrExceptions; i++) {
		// Transfert les exceptions dans un format utile et compact.
	    }
	}

	if (flags & ACC_SYNCHRONIZED) {
	    synchro= new JVSynchronizer;
	 }
     }
    else {		// Abstract method: has no implementation.
	// It has a signature, though.
	if ((aCte= owner->getConstant(info->signatureIndex)) != NULL) {
	    if (aCte->type() == CONSTANT_Utf8) {
		signature= aCte->stringValue();
	     }
	    else {
		signature= new char[4];
		strcpy(name, "()V");
	      }

	    // Count number of arguments:
	    nbrArguments= MethodInfo::parseArguments(signature, &argumentTyping);
	    delete[] signature;
	    if (nbrArguments == 0) {	// Means that no args or return value were found.
		argumentTyping= new JVTypeDefinition*[2];
		argumentTyping[0]= new JVBasicTypeDef;    // By default, a basic type sets itself to void.
		argumentTyping[1]= new JVBasicTypeDef;    // By default, a basic type sets itself to void.
		nbrArguments= 1;
	    }
	    else nbrArguments--;		// Take of the return value from the count.
	 }
	codeLength= 0;
	code= NULL;
	nbrLocals= nbrArguments+1;	// Get a minimum size of locals to put arguments in.
	synchro= NULL;
     }

    nbrLongArgs= 0;
     if (nbrArguments > 0) {
	for (unsigned int i= 0; i < nbrArguments; i++) {
	    if (argumentTyping[i+1]->metaType() & TYPE_BASELONG) {
		nbrLongArgs++;
	    }
	}
     }
}


bool JVMethod::resolveTyping(ClassManager *classManager)
{
    ClassFile *tmpClass;
    unsigned int i;
    char tmpClassName[255];
    bool result= true;

    for (i= 0; i < (nbrArguments+1); i++) {    // Solve all classes used in typing, including returned type.
	if (argumentTyping[i]->metaType() == TYPE_CLASS) {
	    ((JVClassTypeDef *)argumentTyping[i])->getName(tmpClassName);
	    if ((tmpClass= classManager->loadClass(tmpClassName)) != NULL) {
		((JVClassTypeDef *)argumentTyping[i])->setMemClass(tmpClass);
	     }
	    else result= false;
	 }
      }
    return result;
}


char *JVMethod::getName(void)
{
    return name;
}


ClassFile *JVMethod::getImplementor(void)
{
    return implementor;
}


bool JVMethod::isArgumentCompatible(unsigned int nbrArgs, JVTypeDefinition **someTypes)
{
    unsigned int i;
    bool result= false;

    if (nbrArgs == nbrArguments) {
	for (i= 0; i < nbrArguments; i++) {
	if (!argumentTyping[i+1]->isEqual(someTypes[i+1])) {
/* MOD-960730 [HD]: utilisait un acces non-pointeur. */
		break;
	     }
	 }
	if (i == nbrArguments) result= true;
     }
    return result;
}


bool JVMethod::isNative(void)
{
    return false;
}


unsigned int JVMethod::argumentCount(void)
{
    return nbrArguments;
}

JVTypeDefinition **JVMethod::getTyping(void)
{
    return argumentTyping;
}


bool JVMethod::returnsVoid(void)
{
    if (argumentTyping[0]->metaType() == TYPE_BASIC) {
	if (((JVBasicTypeDef *)argumentTyping[0])->type() == JVM_TYPE_VOID)
	    return true;
	else
	    return false;
     }
    else return false;
}


unsigned int JVMethod::returnsType(void)
{
    return (argumentTyping[0]->metaType());
}


bool JVMethod::isStatic(void)
{
    return ((flags & ACC_STATIC) != 0);
}


unsigned int JVMethod::getNbrExceptions(void)
{
    return nbrExceptions;
}


bool JVMethod::isConstructor(void)
{
    return ((flags & INTERN_ACC_CONSTR) != 0);
}


bool JVMethod::isSynchronized(void)
{
    return ((flags & ACC_SYNCHRONIZED) != 0);
}


/**************************************************
* Implementation: JVNativeMethod.
**************************************************/

JVNativeMethod::JVNativeMethod(ClassFile *owner, MethodInfo *info) : JVMethod(owner, info)
{
    internalFunction= NULL;
    dualWorld= NULL;
}


bool JVNativeMethod::isNative(void)
{
    return true;
}


void JVNativeMethod::setNativePointer(unsigned int (*anAddress)(void *, void *))
{
    internalFunction= (unsigned int (*)(JVMachine *, JVExecFrame *))anAddress;
}


JVDualWorld *JVNativeMethod::getDualWorld(void)
{
    return dualWorld;
}


void JVNativeMethod::setDualWorld(JVDualWorld *aWorld)
{
    dualWorld= aWorld;
}


int JVNativeMethod::doNativeCall(JVMachine *machine, JVExecFrame *currentFrame)
{
    if (internalFunction != NULL) {
	if (dualWorld != NULL) {		// Must do some mapping to a dual world.
	    // RENDU ICI, 961029.  Temporairement, on fait un appel direct.
	    return internalFunction(machine, currentFrame);
	}
	else {
	    return internalFunction(machine, currentFrame);
	}
     }
    else {
	std::cout << "Warning: unimplemented native call.\n";
	std::cout << "Method: " << getName() << ", Class: ";
	getImplementor()->describe(0);
	return -1;
    }
}


/**************************************************
* Implementation: JVException.
**************************************************/

JVException::JVException(void)
{
    scope[0]= scope[1]= 0;
    handler= 0;
    filter= NULL;
}




