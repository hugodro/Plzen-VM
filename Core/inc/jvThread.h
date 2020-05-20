#ifndef _JAVATHREADS_H_
#define _JAVATHREADS_H_
/**************************************************
* File: jvThread.h.
* Desc: Definition of the JVThread and JVThreadSet classes.
* Module: AkraLog : JavaKit.
* Rev: 30 octobre 1996 : REV 0 : Hugo DesRosiers : Creation.
**************************************************/

#include <akra/AkObject.h>
#include <akra/portableDefs.h>

#include <threader/rawThreads.h>

class JVMethod;
class JVInstance;
class JVMachine;
class JVThreadSet;
class JVThreadManager;
class JVCondition;
class JVMessageQueue;

// ATTN: Work in progress.

class JVThread : public JVAbstractThread {
    friend class JVMachine;
    friend class JVThreadManager;
  public:
    enum Category {	// Uses the second 8 bits.
	internal= (1 << 8),
	daemon= (2 << 8),
	regular= (3 << 8),
	categoryMask= (0x0ff00)
    };
    enum SharedMode {	// Uses the third 8 bits.
	timesharing= (1 << 16),
	interactive = (1 << 17),
	fixedPriority= (1 << 18),
	sharingMask= (0x0ff0000)
    };
    enum JavaPriority {
	minimum= 0,
	normal= 5,
	maximum= 10
    };

  protected:
    char *name;
    JVMachine *machine;			// Machine used to run methods on an object.
    JVThreadSet *owner;				// Set in which the thread is located.
    JVThreadSet *children;				// Set of threads spawned from this thread.
    JVThread *nextInSet, *globalNext;
    JVMessageQueue *inQueue;		// Incoming messages.
    JVMessageQueue *outQueue;		// Outgoing messages.
    JVInstance *javaEquiv;			// Java Thread object to which the thread is associated to.

  public:
    JVThread(unsigned int anID, JVThread *parent);
    JVThread(unsigned int anID, JVThreadSet *aSet);
    virtual ~JVThread(void);
    virtual void init(void);

    virtual SubType getSubClass(void);
    virtual unsigned int getCategory(void);
    virtual unsigned int getSchedulingType(void);
    virtual JVThreadSet *getThreadSet(void);
    virtual void setThreadSet(JVThreadSet *aSet);

	// Java support.
    virtual void setJavaDual(JVInstance *anInstance);
    virtual JVInstance *getJavaDual(void);
    virtual bool runMethod(char *methodName, char *typing);    // Sets the method to run and does a resume on the thread.

	// List management.
    virtual JVThread *getFirstDead(void);	// Scan threads in the nextGlobal for a !isAlive.
    virtual JVThread *findID(unsigned int anID);    // Scan threads in the nextGlobal for one with the 'ID'.
    virtual JVThread *getNextGlobal(void);
    virtual void setNextGlobal(JVThread *aThread);
    virtual JVThread *getNextInSet(void);
    virtual void setNextInSet(JVThread *aThread);
};


class JVThreadSet : public AkObject {
  protected:
    JVThreadSet *nextInList;
    char *name;
    unsigned int threadCount;
    JVThread *threads;
    JVThreadSet *parent;
    unsigned int defaultPriority;		// Default priority to threads in this set.
    unsigned int maxPriority;			// Maximum priority for threads in this set.

  public:
    JVThreadSet(JVThreadSet *aSet);
    JVThreadSet(char *aName, JVThreadSet *aSet);
    virtual ~JVThreadSet(void);
    virtual void init(void);
    virtual void addThread(JVThread *aThread);	// UPDATE.
    virtual unsigned int getDefaultPriority(void);	// UPDATE.
    virtual JVThread *runInNewThread(JVMethod *aMethod, JVInstance *anInstance);
    virtual JVThread *runInNewThread(JVMethod *aMethod);
    virtual void setNextInList(JVThreadSet *aSet);
    virtual JVThreadSet *getNextInList(void);

};

#endif		/* _JAVATHREADS_H_  */
