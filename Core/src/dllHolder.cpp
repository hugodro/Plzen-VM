/**************************************************
* File: dllHolder.cc.
* Desc: Definition of a holder of Dll header for Windows NT dynamic linking.
* Module: AkraLog : JavaKit.
* Rev: 25 aout 1996 : REV 0 : Hugo DesRosiers : Creation.
**************************************************/

#include "dllHolder.h"

DllHolder::DllHolder(HINSTANCE anHandle)
{
    dllHandle= anHandle;
}


DllHolder *DllHolder::next(void)
{
    return nextListItem;
}


HINSTANCE DllHolder::hInstance(void)
{
    return dllHandle;
}
