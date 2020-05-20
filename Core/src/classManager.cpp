/**************************************************
* File: classManager.cc.
* Desc: Implementation of a manager of ClassFile.
* Module: AkraLog : JavaKit.
* Rev: 3 mai 1996 : REV 0 : Hugo DesRosiers : Creation.
**************************************************/

extern "C" {
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
}

#include <fstream>
#include <istream>

#include <classfile/zipStream.h>

#include "jvcup.h"

#include "dynamicLibrary.h"
#if defined(NeXT)
#include "nextstep/nextDynLib.h"
#elif defined(_WIN32)
#include "mswin/winntDynLib.h"
#elif defined(SYSV)
#include "sysv/sysvDynLib.h"
#endif

#include "dualWorld.h"
#include "classManager.h"

/**************************************************
* Implementation: ChainedClass.
**************************************************/

ChainedClass::ChainedClass()
{
    next= prev= NULL;
}


ChainedClass::ChainedClass(ClassFile *compo)
{
    next= prev= NULL;
    component= compo;
}


void ChainedClass::linkAfter(ChainedClass *aChain)
{
    if (aChain != NULL) {
	if (aChain->next != NULL) aChain->next->prev= this;
	this->next= aChain->next;
	this->prev= aChain;
	aChain->next= this;
     }
}


void ChainedClass::linkBefore(ChainedClass *aChain)
{
    if (aChain != NULL) {
	if (aChain->prev != NULL) aChain->prev->next= this;
	this->prev= aChain->prev;
	this->next= aChain;
	aChain->prev= this;
     }
}


void ChainedClass::unlink()
{
    if (next != NULL) {
	next->prev= prev;
     }
    if (prev != NULL) {
	prev->next= next;
     }
}


ChainedClass * ChainedClass::getNext(void)
{
    return next;
}


ChainedClass * ChainedClass::getPrevious(void)
{
    return next;
}


ClassFile * ChainedClass::getComponent(void)
{
    return component;
}


/**************************************************
* Implementation: ClassManager.
**************************************************/

ClassFile *ClassManager::primitiveClasses[ClassFileObject::numOfPrimitiveTypes];

unsigned char ClassManager::translateClassToPrimitiveByte(ClassFile *aClass)
{
    if (aClass == primitiveClasses[ClassFileObject::ptBoolean]) {
	return JVM_TYPE_BOOLEAN;
    }
    else if (aClass == primitiveClasses[ClassFileObject::ptChar]) {
	return JVM_TYPE_CHAR;
    }
    else if (aClass == primitiveClasses[ClassFileObject::ptByte]) {
	return JVM_TYPE_SIGNED_BYTE;
    }
    else if (aClass == primitiveClasses[ClassFileObject::ptShort]) {
	return JVM_TYPE_UNSIGNED_SHORT;
    }
    else if (aClass == primitiveClasses[ClassFileObject::ptInteger]) {
	return JVM_TYPE_INTEGER;
    }
    else if (aClass == primitiveClasses[ClassFileObject::ptFloat]) {
	return JVM_TYPE_FLOAT;
    }
    else if (aClass == primitiveClasses[ClassFileObject::ptLong]) {
	return JVM_TYPE_LONG;
    }
    else if (aClass == primitiveClasses[ClassFileObject::ptDouble]) {
	return JVM_TYPE_DOUBLE;
    }
    else if (aClass == primitiveClasses[ClassFileObject::ptVoid]) {
	return JVM_TYPE_VOID;
    }

    return '\0';
}


ClassManager::ClassManager(JVCup *caller)
{
    nbrClasses= 0;
    head= tail= NULL;
    owner= caller;
    loadedLibraries= NULL;
    dualWorlds= NULL;
}


bool ClassManager::addClass(ClassFile *aClass)
{
    ChainedClass *newChain;

    newChain= new ChainedClass(aClass);
    if (tail != NULL) {
	newChain->linkAfter(tail);
     }
    else {
	head= newChain;
     }
    tail= newChain;
    nbrClasses++;
	
	return true;	/* MOD-960730 [HD]: ENNT: Le retour manquait. */
}


bool ClassManager::deleteClass(ClassFile *theClass)
{
    ChainedClass *tmpPtr;
    bool result= false;

    tmpPtr= head;
    while (tmpPtr) {
	if (tmpPtr->getComponent() == theClass) {
	    tmpPtr->unlink();
	    delete tmpPtr;
	    delete theClass;
	    result= true;
	    nbrClasses--;
	    break;
	 }
	tmpPtr= tmpPtr->getNext();
     }
    return result;
}


ClassFile * ClassManager::findClass(char *aName)
{
    ChainedClass *tmpPtr;
    ClassFile *result= NULL, *aClass;

    tmpPtr= head;
    while (tmpPtr) {
	aClass= tmpPtr->getComponent();
	if (aClass->isMainName(aName)) {	// Could try different 'domain' prefixed to aName.
	    result= aClass;
	    break;
	 }
	tmpPtr= tmpPtr->getNext();
     }
    return result;
}


// Cover for owner functionality.
ClassFile * ClassManager::loadClass(char *aName)
{
    return owner->loadClass(aName);
}


// Cover for owner functionality.
ArrayClassFile * ClassManager::loadArrayClass(char *aName, unsigned int dimSize)
{
    return owner->loadArrayClass(aName, dimSize);
}


bool ClassManager::checkClassPresenceAt(char *name, Path *path)
{
    struct stat tmpStatInfo;
    char classPath[512];
    bool result= false;

    if (path->getTypeIndex() == 0) {
	if (path->isArchive()) {
	    ZipArchive *zipArchive= path->getArchive();
	    strcpy(classPath, name);
	    strcat(classPath, ".class");
	    result= (zipArchive->statFile(classPath) != NULL);
	}
	else {
	    path->getFilePath(classPath);
	    if (classPath[0] != '\0') {
		strcat(classPath, "/");
		strcat(classPath, name);
		strcat(classPath, ".class");
		if (stat(classPath, &tmpStatInfo) == 0) result= true;
	    }
	}
     }
    return result;
}


ClassFile *ClassManager::readClassAtLocation(char *name, Path *path)
{
    ClassFile *result;
    std::ifstream *fileStream;
    char classPath[512];

    if (path->getTypeIndex() == 0) {
	if (path->isArchive()) {
	    ZipArchive *zipArchive= path->getArchive();
	    strcpy(classPath, name);
	    strcat(classPath, ".class");
	    fileStream= zipArchive->getFile(classPath);

#if defined(DEBUG_ARCHIVE)
	std::cout << "Reading class " << name;
	path->getFilePath(classPath);
    	std::cout << " from archive " << classPath << ".\n";
#endif

	}
	else {
	    path->getFilePath(classPath);
	    if (classPath[0] != '\0') {
		strcat(classPath, "/");
		strcat(classPath, name);
		strcat(classPath, ".class");
//    std::cout << "Reading class " << name << ".\n";
// 		<< " as " << classPath ;
#if defined(_WIN32)
		fileStream= new std::ifstream(classPath, std::ios::in | std::ios::binary);
#else
		fileStream= new std::ifstream(classPath, std::ios::in);
#endif
	    }
	}
	if (fileStream != NULL) {
	    // MOD-970520 [HD]: Pour NT, ajoute (unsigned int).
	    result= new ClassFile(fileStream, (DebugInfo::SectionTracing)(unsigned int)owner->runtimeOptions()->getOption("C"));
	    fileStream->close();
	    delete fileStream;
	    if (result->hasGoodMagic()) {
		addClass(result);
		    // ATTN: Is there a way to optimize this ?
		if (strcmp(name, "java/lang/") == 0) {	// It is a java.lang member.
		    switch(name[10]) {
			case 'B':
			    if (strcmp(&name[11], "oolean") == 0) {
				primitiveClasses[ClassFileObject::ptBoolean]= result;
				result->setAsPrimitiveCover();
			    }
			    else if (strcmp(&name[11], "yte") == 0) {
				primitiveClasses[ClassFileObject::ptByte]= result;
				result->setAsPrimitiveCover();
			    }
			    break;
			case 'C':
			    if (strcmp(&name[11], "har") == 0) {
				primitiveClasses[ClassFileObject::ptChar]= result;
				result->setAsPrimitiveCover();
			    }
			    break;
			case 'D':
			    if (strcmp(&name[11], "ouble") == 0) {
				primitiveClasses[ClassFileObject::ptDouble]= result;
				result->setAsPrimitiveCover();
			    }
			    break;
			case 'F':
			    if (strcmp(&name[11], "loat") == 0) {
				primitiveClasses[ClassFileObject::ptFloat]= result;
				result->setAsPrimitiveCover();
			    }
			    break;
			case 'I':
			    if (strcmp(&name[11], "nteger") == 0) {
				primitiveClasses[ClassFileObject::ptInteger]= result;
				result->setAsPrimitiveCover();
			    }
			    break;
			case 'L':
			    if (strcmp(&name[11], "ong") == 0) {
				primitiveClasses[ClassFileObject::ptLong]= result;
				result->setAsPrimitiveCover();
			    }
			    break;
			case 'S':
			    if (strcmp(&name[11], "hort") == 0) {
				primitiveClasses[ClassFileObject::ptShort]= result;
				result->setAsPrimitiveCover();
			    }
			    break;
			case 'V':
			    if (strcmp(&name[11], "oid") == 0) {
				primitiveClasses[ClassFileObject::ptVoid]= result;
				result->setAsPrimitiveCover();
			    }
			    break;
		    }
		}
	    }
	    else {
		delete result;
		result= NULL;
		std::cout << "ERRMSG: Could not load class " << name << ".\n";
	    }
	}			
     }

    return result;
}


bool ClassManager::loadAllReferences(ClassFile *aClass)
{
    ClassFile *newClass;
    unsigned int i, nbrRef;
    char **theNames;

    nbrRef= aClass->getRefClasses(&theNames);
    if (nbrRef > 0) {
	for (i= 0; i < nbrRef; i++) {
	    if (aClass->isMainName(theNames[i])) continue;    // Skip self.
	    if (findClass(theNames[i]) == NULL) {
		newClass= owner->loadClass(theNames[i]);
		if (newClass) {
		    loadAllReferences(newClass);
		 }
	     }
	    delete theNames[i];
	 }
	delete[] theNames;
     }
	 return true;		/* MOD-960730 [HD]: ENNT: Manquait le retour d'une valeur. */
}


JVCup *ClassManager::getOwner(void)
{
    return owner;
}


// For debugging purposes.
int ClassManager::nbrLoadedClasses(void)
{
    return nbrClasses;
}


ClassFile * ClassManager::getClass(unsigned int i)
{
    ChainedClass *classCursor;
    ClassFile *result= NULL;

    classCursor= head;
    while (i && (classCursor != NULL)) {
	i--;
	classCursor= classCursor->getNext();
     }
    if (classCursor) result= classCursor->getComponent();
    return result;
}


void ClassManager::describeClass(unsigned int i, unsigned int verbosity)
{
    ChainedClass *classCursor;

    classCursor= head;
    while (i && (classCursor != NULL)) {
	i--;
	classCursor= classCursor->getNext();
     }
    if (classCursor) {
	(classCursor->getComponent())->describe(verbosity);
     }
}


bool ClassManager::linkNativeMethod(JVNativeMethod *aMethod)
{
    JVDynamicLibrary *tmpLib= loadedLibraries;
    JVDualWorld *dualWorld= NULL;
    unsigned int (*symbolAddr)(void *, void *);
    char *name;
    bool result= false;

    if ((dualWorld= aMethod->getDualWorld()) != NULL) {
	name= new char[250];
	dualWorld->translateMethodName(aMethod, name);
    }
    else {
	// ATTN: Must take care of the 'non-dual' objects (what are they ?).
	name= aMethod->getName();
    }

// ATTN: This is slow, since right now the name of the symbol is recomposed
// for each dyn. library, instead of being constructed once and seeked for in the
// different libraries.  A better way of doing this is very much necessary !

    while (tmpLib != NULL) {
	if (tmpLib->selectSymbol(name, &symbolAddr)) {
	    aMethod->setNativePointer(symbolAddr);
	    result= true;
	    break;
	}
	tmpLib= tmpLib->getNext();
    }

    if (dualWorld != NULL) delete[] name;
    return result;
}


#if defined(MOVED_OUT)
bool ClassManager::loadDynamicLibrary(char *fileName)
{
#if defined(NeXT)
    NeXTDynamicLibrary *newLib= new NeXTDynamicLibrary(fileName);
#elif defined(_WIN32)
    WinNTDynamicLibrary *newLib= new WinNTDynamicLibrary(fileName);
#elif defined(MSDOS)
    DOSDynamicLibrary *newLib= new DOSDynamicLibrary(fileName);
#elif defined(SYSV)
    SysVDynamicLibrary *newLib= new SysVDynamicLibrary(fileName);
#endif
    bool result;

    if (!(result= newLib->isLoaded())) {
	delete newLib;
    }
    else {
	newLib->setNext(loadedLibraries);
	loadedLibraries= newLib;
    }
    return result;
}
#endif

void ClassManager::addDualRepresentation(JVDualWorld *aWorld)
{
    if (aWorld != NULL) {
	aWorld->setNext(dualWorlds);
	dualWorlds= aWorld;
    }
}


void ClassManager::removeDual(JVDualWorld *aWorld)
{
    // ATTENTION: TO DO.
}


JVDualWorld *ClassManager::getWorld(char *aName)
{
    JVDualWorld *tmpWorld= dualWorlds;

    while (tmpWorld != NULL) {
	if (tmpWorld->isNamed(aName)) break;
	tmpWorld= tmpWorld->getNext();
    }
   return tmpWorld;
}


void ClassManager::setDefaultWorld(JVDualWorld *aWorld)
{
    defaultDualWorld= aWorld;
}


JVDualWorld *ClassManager:: getDefaultWorld(void)
{
    return defaultDualWorld; 
}


JVDualWorld *ClassManager::loadDualWorld(char *aName)
{
    // ATTENTION: TO DO.
    return false;
}



ClassFile *ClassManager::findClassByJavaEquiv(JVInstance *anEquiv)
{
    ChainedClass *tmpClassPtr= head;

    while (tmpClassPtr != NULL) {
	if (tmpClassPtr->getComponent()->getJavaEquivalent() == anEquiv) {
	    return tmpClassPtr->getComponent();	// Warning: quick exit.
	}
	tmpClassPtr= tmpClassPtr->getNext();
    }
    return NULL;
}


ClassFile *ClassManager::getPrimitiveTypeClass(ClassFileObject::PrimitiveTypes aType)
{
    static char *ptNames[ClassFileObject::numOfPrimitiveTypes]= {
	"Boolean", "Byte","Short","Integer", "Float","Long","Double","Void"
    };

    if ((aType >= ClassFileObject::ptBoolean) && (aType <= ClassFileObject::ptVoid)) {
	if (primitiveClasses[aType] == NULL) {
	    ClassFile *result;
	    char fullName[18]= "java/lang/";

	    strcpy(&fullName[11], ptNames[aType]);
	    result= loadClass(fullName);	// This automatically puts the primiv. type in the array.
	}
	return primitiveClasses[aType];		// Warning: quick exit.
    }

    return NULL;		// This is done upon an errounous situation.
}
