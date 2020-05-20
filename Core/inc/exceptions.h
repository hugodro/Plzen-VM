#ifndef _NATIVEERROR_H_
#define _NATIVEERROR_H_
/**************************************************
* File: nativeError.h.
* Desc: Definition of the native error exception management.
* Module: AkraLog : Animation.
* Rev: 9 decembre 1997 : REV 0 : Hugo DesRosiers : Creation.
**************************************************/

#include <akra/AkObject.h>
#include <akra/portableDefs.h>

// TMP: Must define this class in the RunTime section.
class JVRtException : public AkObject {
};


// TMP: Must define this class in the general NativeLib support section.
class NativeError : public JVRtException {
  protected:	// Instance variables.
    int errorCode;

  public:		// Instance methods.
    NativeError(int aCode);
};


#endif		/* _NATIVEERROR_H_ */
