#ifndef _RUNOPTIONS_H_
#define _RUNOPTIONS_H_
/**************************************************
* File: runOptions.h.
* Desc: Definition of the run-time options.
* Module: AkraLog : JavaKit.
* Rev: 3 mai 1996 : REV 0 : Hugo DesRosiers : Creation.
**************************************************/

#include <akra/AkObject.h>

#include <classfile/filePath.h>

class JVCupOptions : public AkObject {
    PathList *searchPath;
    bool verbose;
    bool testMode;
    unsigned int debugSections;
    unsigned int descSections;
    unsigned int nbrClassesToLoad;
    char **classToLoad;
    char *methodName;
    unsigned int nbrMethodArgs;
    char **methodArguments;
    unsigned int classDbgSection;

  public:
    virtual int scanCommands(char **arguments, unsigned int nbrArgs);
    virtual void setDefaultOptions(void);
    virtual void setDefaultOptions(char *name);
    virtual int setOption(char *name, void *value);
    virtual void *getOption(char *name);

    virtual PathList *classPath();
    virtual unsigned int classes(char ***dest);
    virtual unsigned int methodArgs(char ***dest);
    virtual char *getMainClass(void);

    virtual void Document(char *progName, unsigned int level);
    virtual void testPaths(void);
 };


#endif	/* _RUNOPTIONS_H_ */
