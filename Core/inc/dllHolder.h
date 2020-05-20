#ifndef _DLLHOLDER_H_
#define _DLLHOLDER_H_
/**************************************************
* File: dllHolder.h.
* Desc: Definition of the Dll holder for Windows NT dynamic linking.
* Module: AkraLog : JavaKit.
* Rev: 25 aout 1996 : REV 0 : Hugo DesRosiers : Creation.
**************************************************/

#include <windows.h>
#include <akra/AkObject.h>

class DllHolder : public AkObject {
    friend class JVCup;
  protected:
	DllHolder *nextListItem;
	HINSTANCE dllHandle;
  
  public:
	DllHolder(HINSTANCE anHandle);
    DllHolder *next(void);
	HINSTANCE hInstance(void);
};


#endif		/* _DLLHOLDER_H_ */