#ifndef _FSMTHREAD_H_
#define _FSMTHREAD_H_
/**************************************************
* File: fsmThread.h.
* Desc: Definition of the PVFsmThread class.
* Module: AkraLog : JavaKit.
* Rev: 26 mai 1997 : REV 0 : Hugo DesRosiers : Creation.
**************************************************/

#if defined(NeXT)
#include <mach/cthreads.h>
#elif defined(POSIXTHREAD)
#include <pthread.h>
#elif defined(_WIN32)
#include <windows.h>
#endif

#include <threader/rawThreads.h>


class PVFSMachine;
class PVStateInstance;
class PVEvent;


class PVFsmThread : public JVAbstractThread {
  protected:
    unsigned int relativeTime;
    PVFSMachine *machine;
#if defined(POSIXTHREAD)	// TMPTMP: Test pour Linux pthreads.
    int threadID;
#elif defined(_WIN32)	// TMPTMP: Test pour WIN32.
    HANDLE winThread;
    unsigned int winID;
#endif

  public:
    PVFsmThread(unsigned int anID);

    virtual SubType getSubClass(void);

    virtual void setFSMachine(PVFSMachine *aMachine);
    virtual void start(void);
    virtual void receiveEvent(PVEvent *anEvent);

// TMPTMP: Test.
    virtual JVCondition *getACondition(void);
    virtual JVMutex *getAMutex(void);
};


#if defined(NeXT_Thread)
// TMPTMP: Test avec nextstep.

class JVNXMutex : public JVMutex {
  protected:
    mutex_t llMutex;

  public:
    JVNXMutex();
    ~JVNXMutex();
};


class JVNXCondition : public JVCondition {
  protected:
    condition_t llCondition;
    mutex_t privateMutex;

  public:
    JVNXCondition(void);
    virtual ~JVNXCondition(void);
    virtual void declareSatisfied(void);
    virtual void wait(void);
};

#elif defined(POSIXTHREAD)

class JVPOMutex : public JVMutex {
  protected:
    pthread_mutex_t llMutex;

  public:
    JVPOMutex();
    ~JVPOMutex();
};


class JVPOCondition : public JVCondition {
  protected:
    pthread_cond_t llCondition;
    pthread_mutex_t llMutex;

  public:
    JVPOCondition(void);
    virtual ~JVPOCondition(void);
    virtual void declareSatisfied(void);
    virtual void wait(void);
};

#elif defined(_WIN32)

// Test avec MS Win32.
class JVWNTMutex : public JVMutex {
  protected:
    HANDLE wntMutex;

  public:
    JVWNTMutex();
    ~JVWNTMutex();
};


class JVWNTCondition : public JVCondition {
  protected:
    HANDLE wntCondition;

  public:
    JVWNTCondition(void);
    virtual ~JVWNTCondition(void);
    virtual void declareSatisfied(void);
    virtual void wait(void);
};

#endif

#endif		/* _FSMTHREAD_H_ */
