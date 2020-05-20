#ifndef _POSIXTHREAD_H_
#define _POSIXTHREAD_H_
/**************************************************
* File: posixThread.cc.
* Desc: Definition of the PosixJVThread class.
* Module: AkraLog : Plzen.
* Rev: 22 sept. 1997 : REV 0 : Hugo DesRosiers.
**************************************************/

#include <pthread.h>
#include "synchro.h"
#include "jvThread.h"


class PosixJVMutex : public JVMutex {
  protected:
    pthread_mutex_t mutex;

  public:
    PosixJVMutex(void);
    PosixJVMutex(char *aName);
    virtual ~PosixJVMutex(void);
    virtual void lock(void);
    virtual bool lockNonInfinite(unsigned int waitTime);
    virtual void unlock(void);
    virtual void releaseWhile(JVCondition *aCondition);
    virtual bool releaseWhileNonInfinite(JVCondition *aCondition, unsigned int waitTime);
};


class PosixJVCondition : public JVCondition {
  protected:
    pthread_mutex_t mutex;
    struct condition nsCondition;

  public:
    PosixJVCondition(void);
    PosixJVCondition(char *aName);
    virtual ~PosixJVCondition(void);
    virtual void declareSatisfied(void);
    virtual void declareSatisfiedToAll(void);
    virtual void wait(void);
    virtual bool waitWithTimeout(unsigned long someTime);
};


class PosixJVSynchro : public JVSynchronizer {
  protected:
    NsJVMutex mutex;
    PosixJVCondition condition;
    JVThread *lockOwner;	// Use to manage multi-relocking.
    unsigned int lockCount;	// How many time a some object has locked the synchro.

  public:
    PosixJVSynchro(void);
    virtual ~PosixJVSynchro(void);

	// Mutex part.
    virtual void lock(JVThread *aThread);
    virtual boolean lockNonInfinite(JVThread *aThread, unsigned int waitTime);
    virtual void unlock(JVThread *aThread);
    virtual void releaseWhile(JVCondition *aCondition);
    virtual bool releaseWhileNonInfinite(JVCondition *aCondition, unsigned int waitTime);

	// Condition part.
    virtual void declareSatisfied(void);
    virtual void declareSatisfiedToAll(void);
    virtual void wait(void);
    virtual bool waitWithTimeout(unsigned long someTime);
};


class PosixJVThread : public JVThread {
  private:
    static any_t nsMachineLauncher(any_t anObject);
    static unsigned int JavaToNSPriority(unsigned int aPriority);

  protected:
    cthread_t nsThread;
    NsJVMutex mainMutex;
    PosixJVCondition mainCondition;

  public:
    PosixJVThread(unsigned int anID, JVThread *parent);
    PosixJVThread(unsigned int anID, JVThreadSet *aSet);
    virtual ~PosixJVThread(void);

    virtual void init(void);
    virtual SubType getSubClass(void);
    virtual unsigned int setPriority(unsigned int aPriority);

    virtual void start(void);
    virtual void sleep(unsigned long someTime);
    virtual void yield(void);			// Offer cpu time to others.
    virtual void resume(void);
    virtual void suspend(void);
    virtual void exit(void);

    virtual void join(JVAbstractThread *aThread);
    virtual void spawn(void (*aFunction)(void *), void *theReceiver);

    virtual JVSynchronizer *createSynchronizer(void);
};


#endif		/* _POSIXTHREAD_H_ */
