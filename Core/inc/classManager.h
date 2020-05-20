#ifndef _CLASSMANAGER_H_
#define _CLASSMANAGER_H_
/**************************************************
* File: classManager.h.
* Desc: Definition of a manager of ClassFile.
* Module: AkraLog : JavaKit.
* Rev: 3 mai 1996 : REV 0 : Hugo DesRosiers : Creation.
**************************************************/

#include <akra/AkObject.h>
#include <classfile/filePath.h>

#include "classFile.h"


class JVCup;
class JVNativeMethod;
class JVDualWorld;
class JVDynamicLibrary;
//class JVDualInstance;


class ChainedClass : public AkObject {
    ChainedClass *next;
    ChainedClass *prev;
    ClassFile *component;
  public:
    ChainedClass();
    ChainedClass(ClassFile *compo);
    virtual void linkAfter(ChainedClass *aChain);
    virtual void linkBefore(ChainedClass *aChain);
    virtual void unlink();
    virtual ChainedClass *getNext(void);
    virtual ChainedClass *getPrevious(void);
    virtual ClassFile *getComponent(void);
 };


class ClassExport ClassManager : public AkObject {
  protected:
    unsigned int nbrClasses;
    ChainedClass *head;
    ChainedClass *tail;
    JVCup *owner;
    JVDynamicLibrary *loadedLibraries;
    JVDualWorld *dualWorlds;
    JVDualWorld *defaultDualWorld;

  public:
    static unsigned char translateClassToPrimitiveByte(ClassFile *aClass);
    static ClassFile *primitiveClasses[ClassFileObject::numOfPrimitiveTypes];

  public:
    ClassManager(JVCup *caller);
    virtual bool addClass(ClassFile *aClass);
    virtual bool deleteClass(ClassFile *aClass);
    virtual ClassFile *findClass(char *aName);
    virtual ClassFile *loadClass(char *aName);	// Cover for owner.
    virtual ArrayClassFile *loadArrayClass(char *aName, unsigned int dimSize);	// Like the previous, but deal with additional array information.
    virtual bool checkClassPresenceAt(char *name, Path *path);
    virtual ClassFile *readClassAtLocation(char *name, Path *path);
    virtual bool loadAllReferences(ClassFile *aClass);
    virtual JVCup *getOwner(void);


    virtual int nbrLoadedClasses(void);
    virtual ClassFile *getClass(unsigned int i);
    virtual void describeClass(unsigned int i, unsigned int verbosity);

    // Dynamic library services.
    bool loadDynamicLibrary(char *fileName);
    bool getNativeMethodPtr(char *name, unsigned long *resolvedAddress);
    bool linkNativeMethod(JVNativeMethod *aMethod);


    // Dual world services.
    virtual void addDualRepresentation(JVDualWorld *aWorld);
    virtual void removeDual(JVDualWorld *aWorld);
    virtual JVDualWorld *getWorld(char *aName);
    virtual void setDefaultWorld(JVDualWorld *aWorld);
    virtual JVDualWorld *getDefaultWorld(void);
    JVDualWorld *loadDualWorld(char *aName);
//    virtual bool mapTo(JVDualInstance *anInstance, JVDualWorld *aWorld);

    virtual ClassFile *findClassByJavaEquiv(JVInstance *anEquiv);
    virtual ClassFile *getPrimitiveTypeClass(ClassFileObject::PrimitiveTypes aType);

 };


#endif	/* _CLASSMANAGER_H_ */
