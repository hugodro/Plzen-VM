#ifndef _POOLMANAGER_H_
#define _POOLMANAGER_H_
/**************************************************
* File: poolManager.h.
* Desc: Definition of the Pool Manager class.
* Module: AkraLog : JavaKit.
* Rev: 9 mai 1996 : REV 0 : Hugo DesRosiers : Creation.
**************************************************/

#include <akra/AkObject.h>

class JVCup;
class ClassFile;
class ArrayClassFile;
class JVAbstractInstance;
class JVInstance;
class JVBasicArray;
class JVInstanceArray;
class JVArrayArray;


class ClassPool : public AkObject {
    friend class PoolManager;
  protected:
    ClassPool *next;
    unsigned int nbrAllocated;
    unsigned int nbrFree;
    ClassFile *category;
    JVAbstractInstance *allocated;
    JVAbstractInstance *lastAllocated;
    JVAbstractInstance *freed;
    JVAbstractInstance *lastFreed;

  public:
    ClassPool(ClassFile *aClass);
 };


class PoolManager : public AkObject {
  protected:
    JVCup *owner;
    ClassPool *pools;

  public:
    PoolManager(JVCup *creator);

    virtual JVInstance *createInstance(ClassFile *aClass);
    virtual JVInstance *getInstance(ClassFile *category);
    virtual JVBasicArray *createBasicArrayInstance(ubyte type, unsigned int byteSize);
    virtual JVBasicArray *getBasicArray(ubyte type, unsigned int count);
    virtual JVInstanceArray *getInstanceArray(ClassFile *category, unsigned int count);
    virtual JVArrayArray *getArrayArray(ClassFile *category, unsigned int *dimCounts, unsigned int nbrDims);

    virtual void dereference(JVAbstractInstance *anInstance);
 };


#endif	/* _POOLMANAGER_H_ */
