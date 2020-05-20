/**************************************************
* File: stringSupport.h.
* Desc: Definition of the String support class.
* Module: AkraLog : JavaKit.
* Rev: 20 novembre 1996 : REV 0 : Hugo DesRosiers : creation.
**************************************************/

#include <akra/AkObject.h>
#include <akra/portableDefs.h>


class JVInstance;


class ClassExport StringSupport : public AkObject {

  public:
    static void mapStringToNarrowArray(JVInstance *aString, char **aCharPtr);
    static int stringCLength(JVInstance *aString);    // Return -1 if aString is not a j.l.String object.
    static char *mapToCString(JVInstance *aString, char *aCharPtr, unsigned int maxLength);

};
