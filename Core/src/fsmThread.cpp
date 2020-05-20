/**************************************************
* File: fsmThread.cc.
* Desc: Implementation of the PVFsmThread class.
* Module: AkraLog : JavaKit.
* Rev: 26 mai 1997 : REV 0 : Hugo DesRosiers : Creation.
**************************************************/

#if defined(NeXT)
#include <mach/cthreads.h>
#elif defined(_WIN32)
#include <windows.h>
#include <process.h>	    // For _beginthreadex.
#elif defined(POSIXTHREAD)
#include <pthread.h>
#endif

#include "fsMachine.h"
#include "eventQueue.h"
#include "fsmThread.h"


PVFsmThread::PVFsmThread(unsigned int anID) : JVAbstractThread(anID)
{
    relativeTime= 0;
    machine= NULL;
}


JVAbstractThread::SubType PVFsmThread::getSubClass(void)
{
    return fsm;
}


void PVFsmThread::setFSMachine(PVFSMachine *aMachine)
{
    machine= aMachine;
}


void PVFsmThread::start(void)
{
    if (machine != NULL) {
	JVAbstractThread::LauncherType launcher;

	launcher= machine->getLauncher();
// TMPTMP:
#if defined(NeXT_Thread)
	cthread_detach(cthread_fork((cthread_fn_t)launcher, machine));
#elif defined(_WIN32)
	winThread= (HANDLE)_beginthreadex(NULL, 0, (unsigned int (__stdcall *)(void *))launcher, machine, CREATE_SUSPENDED, (unsigned int *)&winID);
	SetThreadPriority(winThread, THREAD_PRIORITY_NORMAL);
	ResumeThread(winThread);
#elif defined(POSIXTHREAD)
	int result;
	result= pthread_detach(pthread_create((pthread_t *)&threadID, NULL, (void * (*)(void *))launcher, machine));
#else
	launcher(machine);
#endif
    }
}


void PVFsmThread::receiveEvent(PVEvent *anEvent)
{
    machine->lockAndReceiveEvent(anEvent);
}


JVCondition *PVFsmThread::getACondition(void)
{
#if defined(NeXT_Thread)
    return new JVNXCondition();
#elif defined(_WIN32)
    return new JVWNTCondition();
#elif defined(POSIXTHREAD)
    return new JVPOCondition();
#else
    return new JVCondition();
#endif
}


JVMutex *PVFsmThread::getAMutex(void)
{
#if defined(NeXT_Thread)
    return new JVNXMutex();
#elif defined(_WIN32)
    return new JVWNTMutex();
#elif defined(POSIXTHREAD)
    return new JVPOMutex();
#else
    return new JVMutex();
#endif
}


#if defined(NeXT_Thread)
/**************************************************
* Implementation: JVNXMutex.
**************************************************/
JVNXMutex::JVNXMutex(void)
{
    llMutex= mutex_alloc();
}


JVNXMutex::~JVNXMutex(void)
{
   if (!mutex_try_lock(llMutex)) mutex_unlock(llMutex);
   mutex_free(llMutex);
}


/**************************************************
* Implementation: JVNXCondition.
**************************************************/

JVNXCondition::JVNXCondition(void)
{
    llCondition= condition_alloc();
    privateMutex= mutex_alloc();
}


JVNXCondition::~JVNXCondition(void)
{
    condition_signal(llCondition);
    mutex_free(privateMutex);
    condition_free(llCondition);
}


void JVNXCondition::declareSatisfied(void)
{
    condition_signal(llCondition);
}


void JVNXCondition::wait(void)
{
    mutex_lock(privateMutex);
    condition_wait(llCondition, privateMutex);
    mutex_unlock(privateMutex);
}

#elif defined(POSIXTHREAD)

/**************************************************
* Implementation: JVPOMutex.
**************************************************/
JVPOMutex::JVPOMutex(void)
{
    pthread_mutex_init(&llMutex, NULL);
}


JVPOMutex::~JVPOMutex(void)
{
    pthread_mutex_destroy(&llMutex);
}


/**************************************************
* Implementation: JVPOCondition.
**************************************************/

JVPOCondition::JVPOCondition(void)
{
    pthread_cond_init(&llCondition, NULL);
}


JVPOCondition::~JVPOCondition(void)
{
    pthread_cond_destroy(&llCondition);
}


void JVPOCondition::declareSatisfied(void)
{
    pthread_cond_signal(&llCondition);
}


void JVPOCondition::wait(void)
{
    int result;

    result=pthread_cond_wait(&llCondition, &llMutex);
}

#elif defined(_WIN32)

/**************************************************
* Implementation: JVWNTMutex.
**************************************************/
JVWNTMutex::JVWNTMutex(void)
{
    wntMutex= CreateMutex(NULL, FALSE, NULL);
}


JVWNTMutex::~JVWNTMutex(void)
{
    CloseHandle(wntMutex);
}


/**************************************************
* Implementation: JVWNTCondition.
**************************************************/

JVWNTCondition::JVWNTCondition(void)
{
    wntCondition= CreateEvent(NULL, FALSE, FALSE, NULL);
}


JVWNTCondition::~JVWNTCondition(void)
{
    SetEvent(wntCondition);
    CloseHandle(wntCondition);
}


void JVWNTCondition::declareSatisfied(void)
{
    SetEvent(wntCondition);
}


void JVWNTCondition::wait(void)
{
    int result;

    result= WaitForSingleObject(wntCondition, INFINITE);
}

#endif
