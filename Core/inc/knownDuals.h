#ifndef _KNOWNDUALS_H_
#define _KNOWNDUALS_H_

/**************************************************
* File: knownDuals.h.
* Desc: Contains the definition of the classes SunDualWorld and
*	KaffeeDualWorld.
* Module: AkraLog : JavaKit.
* Rev: 29 octobre 1996 : REV 0 : Hugo DesRosiers : Creation.
**************************************************/

#include "dualWorld.h"


class JVInstance;
class JVExecFrame;
class ClassFile;

class SunGeneralDualMap : public JVDualMapper {

  private:
    static bool generalImport(JVInstance *anInstance, void *someData);
    static bool generalExport(JVInstance *anInstance, void *someData);
    static bool generalFrameMap(JVExecFrame *aFrame);

  public:
    SunGeneralDualMap(void);
    virtual ~SunGeneralDualMap(void);

    virtual bool isClassMapable(ClassFile *aClass);
    virtual bool setMapping(bool (*importer)(JVInstance *anInstance, void *someData), bool (*exporter)(JVInstance *anInstance, void *someData), bool (*frameMapper)(JVExecFrame *aFrame));
    virtual void addClass(ClassFile *aClass);
};


class SunDualWorld : public JVDualWorld {
  private:
   static JVDualMapper *mapHolder[1];

  public:
    SunDualWorld(void);
    virtual ~SunDualWorld(void);
    virtual bool translateMethodName(JVNativeMethod *aMethod, char *localName);
};


class KaffeeGeneralDualMap : public JVDualMapper {
  private:
    static bool generalImport(JVInstance *anInstance, void *someData);
    static bool generalExport(JVInstance *anInstance, void *someData);
    static bool generalFrameMap(JVExecFrame *aFrame);

  public:
    KaffeeGeneralDualMap(void);
    virtual ~KaffeeGeneralDualMap(void);

    virtual bool isClassMapable(ClassFile *aClass);
    virtual bool setMapping(bool (*importer)(JVInstance *anInstance, void *someData), bool (*exporter)(JVInstance *anInstance, void *someData), bool (*frameMapper)(JVExecFrame *aFrame));
    virtual void addClass(ClassFile *aClass);
};


class KaffeeDualWorld : public JVDualWorld {
  private:
    static JVDualMapper *mapHolder[1];

  public:
    KaffeeDualWorld(void);
    ~KaffeeDualWorld(void);
    virtual bool translateMethodName(JVNativeMethod *aMethod, char *localName);
};


#endif		/* _KNOWNDUALS_H_ */
