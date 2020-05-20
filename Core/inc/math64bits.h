#ifndef _MATH64BITS_H_
#define _MATH64BITS_H_
/**************************************************
* File: math64bits.h.
* Desc: Implementation of 64 bits mathematics, using 32 bits int.
* Module: AkraLog : JavaKit.
* Rev: 8 septembre 1997 : REV 0 : Hugo DesRosiers : Creation.
**************************************************/

#include <akra/AkObject.h>
#include <akra/portableDefs.h>



class ClassExport Value64Bits {
  public:
    struct LongInt {
	unsigned int lowWord;
	unsigned int highWord;
    };
    
    union _LongAndDouble {
	struct LongInt lint;
	double d;
    } data;

  public:
    Value64Bits(void);
    Value64Bits(unsigned int high, unsigned int low);
    Value64Bits(double aDouble);
    static void iadd(Value64Bits *result, Value64Bits *op1, Value64Bits *op2);
    static void isub(Value64Bits *result, Value64Bits *op1, Value64Bits *op2);
    static void imult(Value64Bits *result, Value64Bits *op1, Value64Bits *op2);
    static void idiv(Value64Bits *result, Value64Bits *op1, Value64Bits *op2);
    static void irem(Value64Bits *result, Value64Bits *op1, Value64Bits *op2);
    static void ineg(Value64Bits *result);
    static unsigned int lcmp(Value64Bits *op1, Value64Bits *op2);
    static void toLong(Value64Bits *result, float aFloat);
    static void toLong(Value64Bits *result, Value64Bits *op1);
};


#endif		/* _MATH64BITS_H_ */
