#ifndef _CLASSRUNINFO_H_
#define _CLASSRUNINFO_H_
/**************************************************
* File: runInfo.h.
* Desc: Definition of the JVClassRunInfo class.
* Module: AkraLog : PlzenKit.
* Rev: 10 septembre 1997 : REV 0 : Hugo DesRosiers : Creation.
**************************************************/

#include <akra/AkObject.h>
#include <akra/portableDefs.h>

class JVClassRunInfo : public AkObject {
    friend class JVInstance;
  protected:
    unsigned int nbrLongSlots;
    unsigned int *longSlots;

  public:
    JVClassRunInfo(void);
    JVClassRunInfo(JVClassRunInfo *aSuperRI);
    virtual ~JVClassRunInfo(void);
    virtual void addSlots(unsigned int aCount, unsigned int *slotIDs);
};


class JVMethodRunInfo : public AkObject {
    friend class JVExecFrame;
    friend class JVMethod;
  protected:
    unsigned int nbrLongSlots;
    unsigned int *longSlots;

  public:
    JVMethodRunInfo(void);
    virtual ~JVMethodRunInfo(void);
    virtual void addSlots(unsigned int aCount, unsigned int *slotIDs);
};



#endif		/* _CLASSRUNINFO_H_ */
