/**************************************************
* File: exceptions.cpp.
* Desc: Implementation of the runtime error exception management.
* Module: AkraLog : Animation.
* Rev: 3 decembre 1999 : REV 0 : Hugo DesRosiers : Creation.
**************************************************/

#include "exceptions.h"

// TMPTMPTMP: Very temporary hack.

NativeError::NativeError(int aCode)
{
	errorCode= aCode;
}
