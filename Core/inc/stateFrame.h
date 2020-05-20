#ifndef _STATEEXECFRAME_H_
#define _STATEEXECFRAME_H_
/**************************************************
* File: stateFrame.h.
* Desc: Definition of the PVStateFrame.
* Module: AkraLog : JavaKit.
* Rev: 26 mai 1997 : REV 0 : Hugo DesRosiers : Creation.
**************************************************/

#include "execFrame.h"


class ClassFile;
class PVState;
class PVStateInstance;

class PVStateFrame : public JVExecFrame {
    friend class PVFSMachine;

  public:
    enum StateFlags {
	canImmediate= 1, isBlocking= 2
    };

  protected:	// Instance variables.
    unsigned int stateFlags;
    PVStateInstance *receiver;

  public:	// Instance methods.
    PVStateFrame(PVState *aState, ClassFile *source);
    virtual void setReceiver(PVStateInstance *anInstance);
    virtual void setParamPC(unsigned int aPC);
};

#endif		/* _STATEEXECFRAME_H_ */
