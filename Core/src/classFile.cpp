/**************************************************
* File: runClass.cc.
* Desc: Implements the ClassFile classes definition used by the J-Machine.
* Module: AkraLog : JavaKit.
* Rev: 29 avril 1996 : REV 0 : Hugo DesRosiers : Creation.
**************************************************/

extern "C" {
#include <string.h>
}

#include <akra/convertMacro.h>
#include "classFile.h"
#include "classManager.h"
#include "machineSupport.h"
#include "jmachine.h"
#include "jvInstance.h"
#include "fsmSupport.h"
#include "stacks.h"
#include "runInfo.h"
#include "jvcup.h"
#include "debugInfo.h"


/**************************************************
* Implementation: ClassFile.
**************************************************/

ClassFile::ClassFile()
{
    init(DebugInfo::noTrace);
}


ClassFile::ClassFile(std::istream *aStream, DebugInfo::SectionTracing sectionSet)
{
    init(sectionSet);
    read(aStream);
}


void ClassFile::init(DebugInfo::SectionTracing sectionSet)
{
    magic= 0;
    nbrReferedClasses= nbrReferedFields= nbrReferedMethods= nbrReferedIntefaces= 0;
#if defined(PLZEN_LEVEL_2)
    stateCount= 0;
#endif
    superClassFile= NULL;
    debugInfo= new DebugInfo(sectionSet);
    runInfo= NULL;
    synchro= NULL;
    javaEquiv= NULL;
}


bool ClassFile::read(std::istream *aStream)
{
    unsigned int i, tmpCteType;
     char tmpBuffer[4];

    if (ClassFileObject::read(aStream) == true) {
	aStream->read(tmpBuffer, sizeof(u4));
	magic= MakeUInteger(tmpBuffer);
	if (magic == 0x0cafebabe) {
	    aStream->read(tmpBuffer, sizeof(u2));
	    version_minor= MakeUShort(tmpBuffer);
	    aStream->read(tmpBuffer, sizeof(u2));
	    version_major= MakeUShort(tmpBuffer);
	    aStream->read(tmpBuffer, sizeof(u2));
	    ctePoolSize= MakeUShort(tmpBuffer);
    
	    if (ctePoolSize > 0) {		/* Read constant if there are some. */
		ClassFileConstant *tmpPoolCte, *realCte;
		constantPool= new ClassFileConstant*[ctePoolSize +1];
		tmpPoolCte= new ClassFileConstant;
		for (i= 1; i < ctePoolSize; i++) {	// CtePool 0 is not provided by class stream.
		    tmpPoolCte->read(aStream, realCte);
		    constantPool[i]= realCte;
//		if (realCte != tmpPoolCte) delete tmpPoolCte;
		    tmpCteType= constantPool[i]->type();
		    if ((tmpCteType == CONSTANT_Long) ||
			(tmpCteType == CONSTANT_Double)) {
			    i++;		// 8-bytes constant count for two pool slots.
			    constantPool[i]= realCte;
			}
		    else if (tmpCteType == CONSTANT_Class) {
			nbrReferedClasses++;		// Should substract self.
			}
		    else if (tmpCteType == CONSTANT_FieldRef) {
			nbrReferedFields++;			// Should substract internals.
			}
		    else if (tmpCteType == CONSTANT_MethodRef) {
			nbrReferedMethods++;		// Should substract internals.
			}
		    else if (tmpCteType == CONSTANT_InterfaceMethodRef) {
			nbrReferedIntefaces++;		// Should substract internals.
			}
		    }
		}
    
	    aStream->read(tmpBuffer, sizeof(u2));			// Access Flags.
	    accessFlags= MakeUShort(tmpBuffer);
	    aStream->read(tmpBuffer, sizeof(u2));			// This Class index.
	    thisClass= MakeUShort(tmpBuffer);
	    aStream->read(tmpBuffer, sizeof(u2));			// Super Class index.
	    superClass= MakeUShort(tmpBuffer);
	    aStream->read(tmpBuffer, sizeof(u2));			// Num. of interfaces.
	    interfaceCount= MakeUShort(tmpBuffer);
    
	    if (interfaceCount > 0) {
		    /* Read the interface indexes. */		
		interfaces= new unsigned int[interfaceCount];
		for (i= 0; i < interfaceCount; i++) {
		    aStream->read(tmpBuffer, sizeof(u2));
		    interfaces[i]= MakeUShort(tmpBuffer);
		    }
		}
    
	    aStream->read(tmpBuffer, sizeof(u2));			// Num of fields.
	    fieldCount= MakeUShort(tmpBuffer);
	    staticFieldCount= 0;
	    if (fieldCount > 0) {
		fields= new FieldInfo[fieldCount];
		for (i= 0; i < fieldCount; i++) {
		    fields[i].read(aStream, constantPool);
		    fields[i].setOwner(this);
		    if (fields[i].isStatic()) staticFieldCount++;
		    }
		}
    
	    aStream->read(tmpBuffer, sizeof(u2));			// Num of methods.
	    methodCount= MakeUShort(tmpBuffer);
	    if (methodCount > 0) {
		methods= new MethodInfo[methodCount];
		for (i= 0; i < methodCount; i++) {
		    methods[i].read(aStream, constantPool);
		    methods[i].setOwner(this);
		    if (methods[i].isSynchronized()) {
			if (methods[i].isStatic()) {
			    accessFlags|= needsStaticSynchro;
			}
			else {
			    accessFlags|= hasSynchroMethods;
			}
		    }
		}
	    }
    
	    aStream->read(tmpBuffer, sizeof(u2));			// Num of attributes.
	    attributeCount= MakeUShort(tmpBuffer);
	    if (attributeCount > 0) {
		AttributeInfo *tmpAttrInfo, *realAttribute;
		attributes= new AttributeInfo*[attributeCount];
		for (i= 0; i < attributeCount; i++) {
		    tmpAttrInfo= new AttributeInfo;
		    tmpAttrInfo->read(aStream, constantPool, realAttribute);
		    attributes[i]= realAttribute;
		    if (tmpAttrInfo != realAttribute) delete tmpAttrInfo;
		    }
		}
	}
	else {		// The magic number is not valid.
	    std::cout << "RT Exception: bad magic number in class file.\n";
	    return false;		// Warning: quick exit.
	}
	return true;		// Warning: quick exit.
     }
    return false;			// Warning: quick exit.
}


bool ClassFile::isMainName(char *aName)
{
    return constantPool[thisClass]->isStrEqual(constantPool, aName);
}


unsigned int ClassFile::getRefClasses(char ***holders)
{
    unsigned int i, j;
    char **tmpPtr;

    if (nbrReferedClasses > 0) {
	tmpPtr= *holders= new char*[nbrReferedClasses];
	for (j= 0, i= 1; i < ctePoolSize; i++) {
	    if (constantPool[i]->type() == CONSTANT_Class) {
		tmpPtr[j++]= constantPool[i]->stringValue(constantPool);
	     }
	 }
	
     }

    return nbrReferedClasses;
}


FieldInfo *ClassFile::getFieldNamed(char *aName)
{
    FieldInfo *result= NULL;
    unsigned int i;

    for (i= 0; i < fieldCount; i++) {
	if (fields[i].isNamed(aName, constantPool)) {
	    result= &fields[i];
	    break;
	 }
     }
// MOD-970630 [HD]: Added super-class seek.
    if (result == NULL) {
	if (superClassFile != NULL) {
	    return superClassFile->getFieldNamed(aName);
	}	
    }
    return result;
}


MethodInfo *ClassFile::getLocalMethodNamed(char *aName)
{
    MethodInfo *result= NULL;
    unsigned int i;

    for (i= 0; i < methodCount; i++) {
	if (methods[i].isNamed(aName, constantPool)) {
	    result= &methods[i];
	    break;
	 }
     }
    return result;
}


MethodInfo *ClassFile::getMethodNamed(char *aName)
{
    MethodInfo *result= NULL;
    unsigned int i;

    for (i= 0; i < methodCount; i++) {
	if (methods[i].isNamed(aName, constantPool)) {
	    result= &methods[i];
	    break;
	 }
     }
    if (result == NULL) {
	if (superClassFile) result= superClassFile->getMethodNamed(aName);
     }
    return result;
}


MethodInfo *ClassFile::getMethodNamed(char *aName, char *aType)
{
    MethodInfo *result= NULL;
    JVTypeDefinition **methodTyping= NULL;
    unsigned int i, nbrArgs;

    nbrArgs= MethodInfo::parseArguments(aType, &methodTyping);
    for (i= 0; i < methodCount; i++) {
	if (methods[i].isNamed(aName, constantPool)) {
	    if (methods[i].isTypeCompatible(nbrArgs, methodTyping)) {
		result= &methods[i];
		break;
	     }
	 }
     }
    if (result == NULL) {
	if (superClassFile) result= superClassFile->getMethodNamed(aName, aType);
     }

    if (nbrArgs > 0) delete[] methodTyping;

    return result;
}


JVMethod *ClassFile::getVirtualMethod(ClassManager *manager, JVMethod *superMethod)
{
    JVMethod *result= NULL;
    MethodInfo *mInfo= NULL;
    unsigned int i;
    char *aName;

	// First, check if a lookup is necessary.
    if (superMethod->getImplementor() == this) return superMethod;

    aName= superMethod->getName();
    for (i= 0; i < methodCount; i++) {
	if (methods[i].isNamed(aName, constantPool)) {
	    if (methods[i].isTypeCompatible(superMethod->argumentCount()+1, superMethod->getTyping())) {
		mInfo= &methods[i];
		break;
	     }
	 }
     }
    if (mInfo != NULL) {
	result= mInfo->resolve(manager);
     }
    else {
	if (superClassFile) result= superClassFile->getVirtualMethod(manager, superMethod);
     }
    return result;
}


ClassFileConstant *ClassFile::getConstant(unsigned int i)
{
    if ((i < ctePoolSize) && (i > 0)) {
	return constantPool[i];
     }
    else {
	return NULL;
     }
}


bool ClassFile::resolve(ClassManager *manager)
{
    JVVariable *tmpVarPtr;
    unsigned int i, mapSize, superSlotOffset;
    char superName[80];
    bool result= false;

    if (isResolved) return true;		// Warning: quick exit.

#if defined(DBGLEVEL_4)
    std::cout << "ClassFile: Resolving class: ";
    constantPool[thisClass]->describe(constantPool);
    std::cout << ".\n";
#endif

    if (superClass != 0) {		// If we are not the 'Object' class.
	constantPool[superClass]->writeStringValue(superName, constantPool);
	if ((superClassFile= manager->loadClass(superName)) != NULL) {
	    result= superClassFile->resolve(manager);
	    totalNbrSlots= fieldCount + superClassFile->totalNbrSlots;
	    totalNbrCplxSlots= superClassFile->totalNbrCplxSlots;    // Correct value is set later in this function.
	    totalNbrStaticSlots= superClassFile->totalNbrStaticSlots + staticFieldCount;
	    mapSize= (totalNbrSlots >> 3) + (((totalNbrSlots & 0x07) != 0) ? 1 : 0);
	    if (mapSize > 0) {
		complexMap= new char[mapSize];
		if (superClassFile->totalNbrSlots > 0) {
		    memcpy(complexMap, superClassFile->complexMap, ((superClassFile->totalNbrSlots >> 3) + ((superClassFile->totalNbrSlots & 0x07) != 0) ? 1 : 0));
		}
		else {
		    memset(complexMap, 0, mapSize);
		}
	     }
	    superSlotOffset= superClassFile->totalNbrSlots;
	    accessFlags|= superClassFile->accessFlags & hasSynchroMethods;
/* MOD-980113 [HD]: Inserted the runInfo creation within the non-null superClassFile section.
*/
	    if (superClassFile->runInfo != NULL) {
		runInfo= new JVClassRunInfo(superClassFile->runInfo);
	    }
	 }
	 else {
	    std::cout << "ERRMSG: Can't locate super-class " << superName << ".\n";
	 }
     }
    else {
	totalNbrSlots= fieldCount;
	totalNbrCplxSlots= 0;
	totalNbrStaticSlots= 0;
	mapSize= (fieldCount >> 3) + (((fieldCount & 0x07) != 0) ? 1 : 0);
	if (mapSize > 0) {
	    complexMap= new char[mapSize];
	    memset(complexMap, 0, mapSize);
	 }
	superSlotOffset= 0;
	result= true;
     }
    if (result) {		// Create complex types map.
	unsigned int tmpLngCount= 0;
	unsigned int *tmpLongIDs;

	if (fieldCount > 0) {
	    tmpLongIDs= new unsigned int[fieldCount];
	}
	for (i= 0; i < fieldCount; i++) {
	    if ((tmpVarPtr= fields[i].resolve(manager)) != NULL) {
		if (tmpVarPtr->metaType() != TYPE_BASIC) {
		    if (tmpVarPtr->metaType() & TYPE_BASELONG) {
			tmpLongIDs[tmpLngCount++]= i + superSlotOffset;
		    }
		    else {
			complexMap[((i + superSlotOffset) >> 3)]|= (1 << ((i + superSlotOffset) % 8));
			totalNbrCplxSlots++;
		    }
		}
		tmpVarPtr->setPosition(superSlotOffset + i);
	     }
	 }
	 if (tmpLngCount) {
	    if (runInfo == NULL) {
		runInfo= new JVClassRunInfo();
	    }
	    runInfo->addSlots(tmpLngCount, tmpLongIDs);
	 }
	if (fieldCount > 0) {
	    delete[] tmpLongIDs;
	}
     }

#if defined(PLZEN_LEVEL_2)
    if (result) {		// Build state information if necessary. 
	for (i= 0; i < methodCount; i++) {
	    if (methods[i].isState()) stateCount++;
	}
    }
#endif

    if (result) {
	if (accessFlags & needsStaticSynchro) {
	    manager->getOwner()->putSynchroOn(this);
	}
	isResolved= true;
    }
    return result;
}


ClassFileConstant **ClassFile::getConstantPool()
{
    return constantPool;
}


bool ClassFile::isInterface(void)
{
    return ((accessFlags & ACC_INTERFACE) != 0);
}


// ATTENTION: Must implement !
bool ClassFile::isCastable(ClassFile *aClass)
{
    return true;
}


void ClassFile::setAsPrimitiveCover(void)
{
    accessFlags|= basicTypeCover;
}


bool ClassFile::isPrimitiveCover(void)
{
    return ((accessFlags & basicTypeCover) != 0);
}


bool ClassFile::isArrayClass(void)
{
    return false;
}


bool ClassFile::isAbstract(void)
{
    return ((accessFlags & ACC_ABSTRACT) != 0);
}


#if defined(PLZEN_LEVEL_2)
bool ClassFile::isStateless(void)
{
    if (stateCount != 0) {
	return false;
    }
    else if (superClassFile != NULL) {
	return superClassFile->isStateless();
    }
    else return true;
}
#endif


void ClassFile::describe(unsigned int verbosity)
{
    unsigned int i;

//    std::cout << "Magic number: " << magic << ".\n";
    std::cout << "thisClass: ";
    constantPool[thisClass]->describe(constantPool);
    std::cout << ".\n";
    std::cout << "superClass: ";
    if (superClass > 0) {
	constantPool[superClass]->describe(constantPool);
     }
    else {
	std::cout << "None";
     }
    std::cout << ".\n";

if (verbosity & 0x01) {
    std::cout << "Version number: " << version_major << "." << version_minor << ".\n";
    std::cout << "Constant Pool size: " << ctePoolSize << ".\n";
    std::cout << "accessFlags: " << accessFlags << ".\n";

    std::cout << "interface count: " << interfaceCount << ".\n";
    std::cout << "field count: " << fieldCount << ".\n";
    std::cout << "method count: " << methodCount << ".\n";
    std::cout << "attribute count: " << attributeCount << ".\n";
}

    if (verbosity & 0x02) {
	    // Show the CONSTANT_Class.
	std::cout << "\nClasses to resolve:\n";
	for (i= 1; i < ctePoolSize; i++) {
	    if (constantPool[i]->type() == CONSTANT_Class) {
		constantPool[i]->describe(constantPool);
		std::cout << ".\n";
	    }
	}
	std::cout << "\n";
     }

    if (verbosity & 0x04) {
	    // Show the CONSTANT_FieldRef.
	std::cout << "Fields:\n";
	for (i= 1; i < ctePoolSize; i++) {
	    if (constantPool[i]->type() == CONSTANT_FieldRef) {
		constantPool[i]->describe(constantPool);
	    }
	}
	std::cout << "\n";    
	    // Show the CONSTANT_MethodRef.
	std::cout << "Methods:\n";
	for (i= 1; i < ctePoolSize; i++) {
	    if (constantPool[i]->type() == CONSTANT_MethodRef) {
		constantPool[i]->describe(constantPool);
	    }
	}
	std::cout << "\n";
     }


    if (verbosity & 0x08) {
	    // Show the CONSTANT_InterfaceMethodRef.
	std::cout << "Interfaces:\n";
	for (i= 1; i < ctePoolSize; i++) {
	    if (constantPool[i]->type() == CONSTANT_InterfaceMethodRef) {
		constantPool[i]->describe(constantPool);
	    }
	}
	std::cout << "\n";
     }

    if (verbosity & 0x010) {
	    // Show the CONSTANT_String.
	std::cout << "String objects:\n";
	for (i= 1; i < ctePoolSize; i++) {
	    if (constantPool[i]->type() == CONSTANT_String) {
		constantPool[i]->describe(constantPool);
		std::cout << ".\n";
	    }
	}
	std::cout << "\n";
     }

    if (verbosity & 0x020) {
	std::cout << "Fields defined in class:\n";
	for (i= 0; i < fieldCount; i++) {
	    fields[i].describe(constantPool, verbosity);
	}
	std::cout << "\n";
     }

    if (verbosity & 0x040) {
	std::cout << "Methods defined in class:\n";
	for (i= 0; i < methodCount; i++) {
	    methods[i].describe(constantPool, verbosity);
	    std::cout << "\n";
	}
	std::cout << "\n";
     }

    if (verbosity & 0x080) {
	if (attributeCount > 0) {
	    std::cout << "Attributes of the class file:\n";
	    for (i= 0; i < attributeCount; i++) {
		attributes[i]->describe(constantPool);
		std::cout << "\n";
		}
	    }
    }
}


MethodInfo *ClassFile::getMethod(unsigned int i)
{
    return &methods[i];
}


unsigned int ClassFile::nbrMethods(void)
{
    return methodCount;
}


void ClassFile::getName(char *destination)
{
    constantPool[thisClass]->writeStringValue(destination, constantPool);
}


unsigned int ClassFile::nbrFields(void)
{
    return totalNbrSlots;
}


DebugInfo::SectionTracing ClassFile::traceLevel()
{
    return debugInfo->traceLevel();
}


bool ClassFile::hasGoodMagic(void)
{
    return (magic == 0x0cafebabe);
}


void ClassFile::setSynchronizer(JVSynchronizer *aSynchro)
{
    synchro= aSynchro;
}


JVSynchronizer *ClassFile::getSynchronizer(void)
{
    return synchro;
}


JVInstance *ClassFile::getJavaEquivalent(JVMachine *aMachine)
{
// ATTN: Tmp implementation, the classManager must provide a better support for this operation.
    static ClassFile *classClass= NULL;

    if (javaEquiv == NULL) {
	if (classClass == NULL) {
	    classClass= aMachine->getClassFile("java/lang/Class");
	}
	javaEquiv= aMachine->createObject(classClass);
    }
    return javaEquiv;
}


JVInstance *ClassFile::getJavaEquivalent(void)
{
    return javaEquiv;
}


ClassFile *ClassFile::getSuperClass(void)
{
    return superClassFile;
}


/**************************************************
* Implementation: ArrayClassFile.
**************************************************/

ArrayClassFile::ArrayClassFile(ClassFile *sourceClass, unsigned int dimSize)
{
    *(ClassFile *)this= *sourceClass;
    basicType= 0;
    nbrDimensions= dimSize;
}


ArrayClassFile::ArrayClassFile(ubyte type, unsigned int dimSize)
{
    basicType= type;
    nbrDimensions= dimSize;
}


bool ArrayClassFile::isArrayClass(void)
{
    return true;
}


bool ArrayClassFile::rootIsBasicType(void)
{
    return (basicType != 0);
}


unsigned int ArrayClassFile::dimensions(void)
{
    return nbrDimensions;
}


ubyte ArrayClassFile::getBasicType(void)
{
    return basicType;
}


/**************************************************
* Implementation: GenericInfo.
**************************************************/

bool GenericInfo::read(std::istream *aStream, ClassFileConstant **ctePool)
{
     char tmpBuffer[4];

    if (ClassFileObject::read(aStream) == true) {
	aStream->read(tmpBuffer, sizeof(u2));
	accessFlags= MakeUShort(tmpBuffer);
	aStream->read(tmpBuffer, sizeof(u2));
	nameIndex= MakeUShort(tmpBuffer);
     }

    return true;
}


bool GenericInfo::readAttributes(std::istream *aStream, ClassFileConstant **ctePool)
{
     char tmpBuffer[4];

    aStream->read(tmpBuffer, sizeof(u2));
    attributeCount= MakeUShort(tmpBuffer);
    if (attributeCount > 0) {
	AttributeInfo *tmpAttrInfo, *realAttribute;
	attributes= new AttributeInfo*[attributeCount];
	for (unsigned int i= 0; i < attributeCount; i++) {
	    tmpAttrInfo= new AttributeInfo;
	    tmpAttrInfo->read(aStream, ctePool, realAttribute);
	    attributes[i]= realAttribute;
	    if (tmpAttrInfo != realAttribute) delete tmpAttrInfo;
	 }
     }
    return true;
}


bool GenericInfo::isNamed(char *aName, ClassFileConstant **ctePool)
{
    return ctePool[nameIndex]->isStrEqual(aName);
}

void GenericInfo::setOwner(ClassFile *classFile)
{
    owner= classFile;
}


ClassFile *GenericInfo::getOwner(void)
{
    return owner;
}


bool GenericInfo::isStatic(void)
{
    return ((accessFlags & ACC_STATIC) != 0);
}


void GenericInfo::describe(ClassFileConstant **ctePool, unsigned int verbosity)
{
    // Should do most of the job, but for reading clarity, all is done by subclasses.
}


unsigned int GenericInfo::getAccessFlags(void)
{
    return accessFlags;
}


/**************************************************
* Implementation: FieldInfo.
**************************************************/

bool FieldInfo::read(std::istream *aStream, ClassFileConstant **ctePool)
{
    int result= -1;
     char tmpBuffer[4];

	dereference= NULL;	/* MOD-960730 [HD]: ENNT: Ajoute l'initialisation a NULL. */
    if (GenericInfo::read(aStream, ctePool) == true) {
	aStream->read(tmpBuffer, sizeof(u2));
	signatureIndex= MakeUShort(tmpBuffer);
	readAttributes(aStream, ctePool);
     }
    return true;
}


JVVariable *FieldInfo::resolve(ClassManager *source)
{
    // resolve type and constant value (if any).
    if (!isResolved) {
	dereference= new JVVariable(owner, this);
	isResolved= true;
     }

    return dereference;
}


int FieldInfo::parseSignature(char *aSignature, JVTypeDefinition **type)
{
    JVArrayTypeDef *lastArrayDef;
    JVTypeDefinition *tmpTypeDef, *theType= NULL;
    int result= 0;
    unsigned int nbrArgs= 0, optionalSize= 0, parentLevel;
    char *tmpPtr, *currentClassName;
    bool inClassName= false, inArraySpec= false;

    tmpPtr= &aSignature[0];
    if (*tmpPtr != '\0') {
	while (*tmpPtr != '\0') {	// Scan the args.
	    switch (*tmpPtr) {
		case 'C':		// Basic type, or char of class name.
		case 'D':
		case 'F':
		case 'I':
		case 'J':
		case 'S':
		case 'Z':
		case 'B':
		case 'V':		// Should only occur when parentLevel == 0.
		    if (inClassName) {
			// Do nothing, part of class name.
		     }
		    else {
			if (*tmpPtr == 'V') {
			    if (parentLevel != 0) {	// Weird situation, not looking at the return type.
			     }
			 }
			tmpTypeDef= new JVBasicTypeDef(*tmpPtr);
			if (inArraySpec) {
			    // End of array spec.
			    lastArrayDef->setElementType(tmpTypeDef, optionalSize);
			    inArraySpec= false;
			 }
			else {
			    if (theType != NULL) delete theType;    // Erase previous definition.
			    theType= tmpTypeDef;
			    nbrArgs++;
			 }
		     }
		    break;
		case 'L':		// Beginning of class name.
		    if (!inClassName) {
			inClassName= true;
			currentClassName= tmpPtr + 1;
		     }
		    else {
			// Part of class name, do nothing.
		     }
		    break;
		case ';':		// End of object name.
		    if (inClassName) {
			if (tmpPtr > currentClassName) {
			    tmpTypeDef= new JVClassTypeDef(currentClassName, (tmpPtr - currentClassName));
			    if (inArraySpec) {
				lastArrayDef->setElementType(tmpTypeDef, optionalSize);
				inArraySpec= false;
			     }
			    else {	// Individual argument definition.
				theType= tmpTypeDef;
				nbrArgs++;
			     }
			 }
			else {		// Weird situation, name length is 0.
			 }
			inClassName= false;
		     }
		    else {		// Weird situation, unknown type ';'.
		     }
		    break;
		case '[':		// Array type.
		    if (inClassName) {
			// Part of class name (!!), do nothing.
		     }
		    else {
			tmpTypeDef= new JVArrayTypeDef;
			if (inArraySpec) {
			    lastArrayDef->setElementType(tmpTypeDef, optionalSize);
			 }
			else {
			    if (theType != NULL) delete theType;    // Erase previous definition.
			    theType= tmpTypeDef;
			    nbrArgs++;
			 }
			lastArrayDef= (JVArrayTypeDef *)tmpTypeDef;
			inArraySpec= true;
			optionalSize= 0;
		     }
		    break;
		case '0': case '1': case '2': case '3': case '4':
		case '5': case '6': case '7': case '8': case '9':
		    if (inArraySpec) {
			optionalSize= optionalSize * 10 + (unsigned int)(*tmpPtr - '0');
		     }
		    else if (inClassName) {
			// Ok, do nothing, part of class name.
		     }
		    else {		// Weird situation.
		     }
		    break;
		case '(':
		    // Should it ever occur ??.
		    parentLevel++;
		    break;
		case ')':
		    // Should denote end of arguments.
		    parentLevel--;
		    break;
		default:		// Unknown type.
		    if (inClassName) {
			// Ok, do nothing part of class name.
		     }
		    else {		// Weird situation, unknown type.
			optionalSize= 0;
		     }
		    break;
	     }
	    tmpPtr++;
	 }

     }

    if (nbrArgs != 1) result= -1;
    *type= theType;

    return result;
}


void FieldInfo::describe(ClassFileConstant **ctePool, unsigned int verbosity)
{

	std::cout << "Field: Access Flags: " << accessFlags;

	std::cout << ", Name: ";
	if (nameIndex > 0) {
	    ctePool[nameIndex]->describe(ctePool);
	 }
	else std::cout << "Unknown ???";

	std::cout << ", Signature: ";
	if (signatureIndex > 0) {
	    ctePool[signatureIndex]->describe(ctePool);
	 }
	else std::cout << "Unknown ???";
	std::cout  << ".\n";

	if (attributeCount > 0) {
	for (unsigned int i= 0; i < attributeCount; i++) {
		attributes[i]->describe(ctePool);
	    }
	}
}



/**************************************************
* Implementation: MethodInfo.
**************************************************/

MethodInfo::MethodInfo(void)
{
    dereference= NULL;
    isStateInfo= false;
}


bool MethodInfo::read(std::istream *aStream, ClassFileConstant **ctePool)
{
     char tmpBuffer[4];

    if (GenericInfo::read(aStream,  ctePool) == true) {
	aStream->read(tmpBuffer, sizeof(u2));
	signatureIndex= MakeUShort(tmpBuffer);
	readAttributes(aStream, ctePool);

#if defined(PLZEN_LEVEL_2)
	    // Decide if it is a method or state descriptor.
	if (hasCodeAttribute()) {
	    if (hasStateCodeAttribute()) {
		isStateInfo= true;
	    }
	}
	else {
	    char methodName[80];

	    ((CteUtf8 *)ctePool[nameIndex])->writeStringValue(methodName);
	    if (methodName[((CteUtf8 *)ctePool[nameIndex])->getLength()] == '+') {
		isStateInfo= true;
	    }
	}
#endif

     }
    return true;
}


JVMethod *MethodInfo::resolve(ClassManager *source)
{
    if (!isResolved) {
#if defined(PLZEN_LEVEL_2)
	if (isStateInfo) {
	    if ((dereference= new PVState(owner, this)) != NULL) {
		dereference->resolveTyping(source);
		((PVState *)dereference)->resolveTransitions(source, owner);
		isResolved= true;
	    }
	}
	else {
#endif
	    if (accessFlags & ACC_NATIVE) {
		dereference= new JVNativeMethod(owner, this);
    // ATTENTION: Temp. way of setting a dual world to a JVNativeMethod.
		if (((JVNativeMethod *)dereference)->getDualWorld() == NULL) {
		    ((JVNativeMethod *)dereference)->setDualWorld(source->getDefaultWorld());
		}
	    }
	    else {
		dereference= new JVMethod(owner, this);
	    }
	    if (dereference) {
		dereference->resolveTyping(source);
		isResolved= true;
		if (accessFlags & ACC_NATIVE) {
		    if (!source->linkNativeMethod((JVNativeMethod *)dereference)) {
			std::cout << "RT Exception: Can't link native method.\n";
		    }
		}
	    }
#if defined(PLZEN_LEVEL_2)
	}
#endif
     }
    return dereference;
}


void MethodInfo::describe(ClassFileConstant **ctePool, unsigned int verbosity)
{
	std::cout << "Method: '";
	if (nameIndex > 0) {
	    ctePool[nameIndex]->describe(ctePool);
	 }
	else std::cout << "Unknown ???";

    if (verbosity & 0x0100) {
	std::cout << "', Signature: '";
	if (signatureIndex > 0) {
	    ctePool[signatureIndex]->describe(ctePool);
	 }
	else std::cout << "Unknown ???";
	std::cout << "', Access Flags: " << accessFlags;
	std::cout  << ".\n";
     }

    if (verbosity & 0x0200) {
	if (attributeCount > 0) {
	for (unsigned int i= 0; i < attributeCount; i++) {
		attributes[i]->describe(ctePool);
	    }
	}
     }
}


unsigned int MethodInfo::getCodeLength(void)
{
    unsigned int i;

    if (attributeCount > 0) {
	for (i= 0; i < attributeCount; i++) {
	    if ((attributes[i]->category() == ATTR_CODE)
#if defined(PLZEN_LEVEL_2)
		|| (attributes[i]->category() == ATTR_STATE_CODE))
#endif
	    {
		return attributes[i]->codeSize();
	     }
	 }
     }
    return 0;
}


ubyte *MethodInfo::getCode(void)
{
    unsigned int i;

    if (attributeCount > 0) {
	for (i= 0; i < attributeCount; i++) {
	    if ((attributes[i]->category() == ATTR_CODE) || (attributes[i]->category() == ATTR_STATE_CODE)) {
		return attributes[i]->getCode();
	     }
	 }
     }
    return NULL;
}


unsigned int MethodInfo::maxLocals(void)
{
    unsigned int i;

    if (attributeCount > 0) {
	for (i= 0; i < attributeCount; i++) {
	    if ((attributes[i]->category() == ATTR_CODE)
#if defined(PLZEN_LEVEL_2)
		    || (attributes[i]->category() == ATTR_STATE_CODE))
#endif
	    {
 		return attributes[i]->nbrLocals();
	     }
	 }
     }
    return 0;
}


unsigned int MethodInfo::stackMaxDepth(void)
{
    unsigned int i;

    if (attributeCount > 0) {
	for (i= 0; i < attributeCount; i++) {
	    if ((attributes[i]->category() == ATTR_CODE)
#if defined(PLZEN_LEVEL_2)
		    || (attributes[i]->category() == ATTR_STATE_CODE))
#endif
	    {
		return attributes[i]->stackDepth();
	     }
	 }
     }
    return 0;
}


int MethodInfo::parseArguments(char *aSignature, JVTypeDefinition ***types)
{
    JVArrayTypeDef *lastArrayDef;
    JVTypeDefinition *tmpTypingArray[257], *tmpTypeDef;	// Can scan up to 256 arguments.
    unsigned int nbrArgs= 0, optionalSize= 0, parentLevel;
    char *tmpPtr, *currentClassName;
    bool inClassName= false, inArraySpec= false;

    if (aSignature[0] == '(') {		// Well started signature.
	tmpPtr= &aSignature[1];
	parentLevel= 1;
	while (*tmpPtr != '\0') {	// Scan the args.

	    switch (*tmpPtr) {
		case 'C':		// Basic type, or char of class name.
		case 'D':
		case 'F':
		case 'I':
		case 'J':
		case 'S':
		case 'Z':
		case 'B':
		case 'V':		// Should only occur when parentLevel == 0.
		    if (inClassName) {
			// Do nothing, part of class name.
		     }
		    else {
			if (*tmpPtr == 'V') {
			    if (parentLevel != 0) {	// Weird situation, not looking at the return type.
			     }
			 }
			tmpTypeDef= new JVBasicTypeDef(*tmpPtr);
			if (inArraySpec) {
			    // End of array spec.
			    lastArrayDef->setElementType(tmpTypeDef, optionalSize);
			    inArraySpec= false;
			 }
			else {
			    tmpTypingArray[nbrArgs++]= tmpTypeDef;
			 }
		     }
		    break;
		case 'L':		// Beginning of class name.
		    if (!inClassName) {
			inClassName= true;
			currentClassName= tmpPtr + 1;
		     }
		    else {
			// Part of class name, do nothing.
		     }
		    break;
		case ';':		// End of object name.
		    if (inClassName) {
			if (tmpPtr > currentClassName) {
			    tmpTypeDef= new JVClassTypeDef(currentClassName, (tmpPtr - currentClassName));
			    if (inArraySpec) {
				lastArrayDef->setElementType(tmpTypeDef, optionalSize);
				inArraySpec= false;
			     }
			    else {	// Individual argument definition.
				tmpTypingArray[nbrArgs++]= tmpTypeDef;
			     }
			 }
			else {		// Weird situation, name length is 0.
			 }
			inClassName= false;
		     }
		    else {		// Weird situation, unknown type ';'.
		     }
		    break;
		case '[':		// Array type.
		    if (inClassName) {
			// Part of class name (!!), do nothing.
		     }
		    else {
			tmpTypeDef= new JVArrayTypeDef;
			if (inArraySpec) {
			    lastArrayDef->setElementType(tmpTypeDef, optionalSize);
			 }
			else {
			    tmpTypingArray[nbrArgs++]= tmpTypeDef;
			 }
			lastArrayDef= (JVArrayTypeDef *)tmpTypeDef;
			inArraySpec= true;
			optionalSize= 0;
		     }
		    break;
		case '0': case '1': case '2': case '3': case '4':
		case '5': case '6': case '7': case '8': case '9':
		    if (inArraySpec) {
			optionalSize= optionalSize * 10 + (unsigned int)(*tmpPtr - '0');
		     }
		    else if (inClassName) {
			// Ok, do nothing, part of class name.
		     }
		    else {		// Weird situation.
		     }
		    break;
		case '(':
		    // Should it ever occur ??.
		    parentLevel++;
		    break;
		case ')':
		    // Should denote end of arguments.
		    parentLevel--;
		    break;
		default:		// Unknown type.
		    if (inClassName) {
			// Ok, do nothing part of class name.
		     }
		    else {		// Weird situation, unknown type.
			optionalSize= 0;
		     }
		    break;
	     }
	    tmpPtr++;
	 }
	// Normally, read all arguments and return type.
	if (nbrArgs > 0) {
	    *types= new JVTypeDefinition*[nbrArgs];
	    for (unsigned int i= 1; i < nbrArgs; i++) {
		(*types)[i]= tmpTypingArray[i-1];
	     }
		(*types)[0]= tmpTypingArray[nbrArgs-1];
	 }
     }
    else {
	// Maybe the '('')' pair was not there.  Verify if there is a valid description.
     }
    return nbrArgs;
}


bool MethodInfo::isTypeCompatible(unsigned int nbrArgs, JVTypeDefinition **types)
{
    JVTypeDefinition **ourTyping;
    unsigned int i, ourNbrArgs;
    char typeString[1025];
    bool result= false;

    if (dereference) {
/* MOD-960825 [HD]: Was passing return type first. */
	result= dereference->isArgumentCompatible(nbrArgs - 1, types);
     }
    else {
	owner->getConstant(signatureIndex)->writeStringValue(typeString);
	ourNbrArgs= parseArguments(typeString, &ourTyping);
	if (ourNbrArgs == nbrArgs) {
/* MOD-960825 [HD]: Was checking return type first. */
	    for (i= 1; i < nbrArgs; i++) {
		if (!ourTyping[i]->isEqual(types[i])) {
		    break;
		 }
	     }
	    if (i == nbrArgs) result= true;	// All types were matching.
	 }
	for (i= 0; i < ourNbrArgs; i++) delete ourTyping[i];
	delete[] ourTyping;
     }
    return result;
}


bool MethodInfo::isFromInterface(void)
{
    return (owner->isInterface());
}


unsigned int MethodInfo::getNbrExceptions(void)
{
    unsigned int i;

    if (attributeCount > 0) {
	for (i= 0; i < attributeCount; i++) {
	    if ((attributes[i]->category() == ATTR_CODE)
#if defined(PLZEN_LEVEL_2)
		    || (attributes[i]->category() == ATTR_STATE_CODE))
#endif
	    {
		return ((CodeAttribute *)attributes[i])->getNbrExceptions();
	     }
	 }
     }
    return 0;
}


CodeAttribute *MethodInfo::getCodeInfo(void)
{
    if (attributeCount > 0) {
	for (unsigned int i= 0; i < attributeCount; i++) {
	    if ((attributes[i]->category() == ATTR_CODE)
#if defined(PLZEN_LEVEL_2)
		    || (attributes[i]->category() == ATTR_STATE_CODE))
#endif
	    {
		return (CodeAttribute *)attributes[i];
	     }
	 }
     }
     return NULL;
}


#if defined(PLZEN_LEVEL_2)

bool MethodInfo::hasCodeAttribute(void)
{
    unsigned int i;

    if (attributeCount > 0) {
	for (i= 0; i < attributeCount; i++) {
	    if ((attributes[i]->category() == ATTR_CODE) || (attributes[i]->category() == ATTR_STATE_CODE)) {
		return true;
	     }
	 }
     }
    return false;
}


bool MethodInfo::hasStateCodeAttribute(void)
{
    unsigned int i;

    if (attributeCount > 0) {
	for (i= 0; i < attributeCount; i++) {
	    if (attributes[i]->category() == ATTR_STATE_CODE) {
		return true;
	     }
	 }
     }
    return false;
}


bool MethodInfo::isState(void)
{
    return isStateInfo;
}

#endif


bool MethodInfo::isSynchronized(void)
{
    return ((accessFlags & ACC_SYNCHRONIZED) != 0);
}


/**************************************************
* Implementation: StringInfo.
**************************************************/

StringInfo::StringInfo()
{
    value= NULL;
}


void StringInfo::setConstantValue(JVInstance *aValue)
{
    value= aValue;
}


void StringInfo::setActualData(JVBasicArray *charArray, char *letters)
{
    JVRunValue tmpValue;

    unsigned int lettersLength;

// ATTN: This takes for granted that the 'java.lang.String' class
// has the char array and its length as the first two instance variables.
    lettersLength= strlen(letters);
    tmpValue.setType(TYPE_ARRAY | TYPE_BASIC_ARRAY);
    tmpValue.setValue((void *)charArray);
    value->putAtIndex(tmpValue, 0);

    tmpValue.setType(TYPE_BASIC);
    tmpValue.setValue(NULL);
    value->putAtIndex(tmpValue, 1);

    tmpValue .setType(TYPE_BASIC);
    tmpValue .setValue((void *)lettersLength);
    value->putAtIndex(tmpValue, 2);
	// ATTN: We make sure that there is a trailing '\0', just for C compat.
    charArray->byteCopy((unsigned char *)letters, lettersLength+1);
}


JVInstance *StringInfo::constantValue(void)
{
    return value;
}

