/**************************************************
* File: stringSupport.cc.
* Desc: Implementation of the String support class.
* Module: AkraLog : JavaKit.
* Rev: 20 novembre 1996 : REV 0 : Hugo DesRosiers : creation.
* Rev: 19 aout 1998 : REV 0.1 : Hugo DesRosiers.
*    Added new manipulation functions.
**************************************************/

#include "jvInstance.h"
#include "stringSupport.h"

#define min(a, b)  (((a) < (b)) ? (a) : (b))

void StringSupport::mapStringToNarrowArray(JVInstance *aString, char **aCharPtr)
{
    if ((aString != NULL) && (aCharPtr != NULL)) {
	JVBasicArray *charArray;
	unsigned int length, offset;

	charArray= (JVBasicArray *)aString->getAtIndex(0);
	offset= (unsigned int)aString->getAtIndex(1);
	length= (unsigned int)aString->getAtIndex(2);

	if (length > 0) {
	    *aCharPtr= new char[length+1];
	    charArray->byteSet((unsigned char *)*aCharPtr, length, offset);
	    (*aCharPtr)[length]= 0;
	}
	else {
	    *aCharPtr= NULL;
	}
    }
}


int StringSupport::stringCLength(JVInstance *aString)
{
    if (aString != NULL) {
	JVBasicArray *charArray;

// TODO: Check if aString is indeed a j.l.String instance.
	return (unsigned int)aString->getAtIndex(2);
    }

    return -1;
}


char *StringSupport::mapToCString(JVInstance *aString, char *aCharPtr, unsigned int maxLength)
{
    if ((aString != NULL) && (aCharPtr != NULL)) {
	JVBasicArray *charArray;
	unsigned int length, offset;

	charArray= (JVBasicArray *)aString->getAtIndex(0);
	offset= (unsigned int)aString->getAtIndex(1);
	length= (unsigned int)aString->getAtIndex(2);

	if (length > 0) {
	    length= min(length, maxLength - 1);
	    charArray->byteSet((unsigned char *)aCharPtr, length, offset);
	    aCharPtr[length]= 0;
	}
	else {
	    aCharPtr[0]= '\0';
	}
    }
    else {
        aCharPtr[0]= '\0';
    }

    return aCharPtr;
}



