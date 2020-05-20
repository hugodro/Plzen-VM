#ifndef _DEBUGINFO_H_
#define _DEBUGINFO_H_
/**************************************************
* File: debugInfo.h.
* Desc: Definition of the user-oriented debugging help and the DebugInfo class..
* Module: AkraLog : JavaKit.
* Rev: 22 aout 1996 : REV 0 : Hugo DesRosiers : Creation.
**************************************************/
#include <akra/AkObject.h>

class JVExecFrame;


class DebugInfo : public AkObject {
  public:
    enum SectionTracing {
    	noTrace= 0,
	instructions= 1, methodLookup= 2,
	classInit= 4, classManager= 8,
	frameFollowup= 16
    };

  protected:
    static char **jCodeName;
    static char *unknownOpcode;

  protected:
    char **methodTraced;
    SectionTracing sectionsTraced;

  protected:
    static void classInitializor(void);

  public:
    DebugInfo(void);
    DebugInfo(SectionTracing sectionSet);
    virtual SectionTracing traceLevel(void);
    virtual void trace(JVExecFrame *aFrame, unsigned int aCode);
    virtual void setTraceSection(SectionTracing aSection);
    virtual void stopTraceSection(SectionTracing aSection);
 };


#endif		/* _DEBUGINFO_H_ */
