/**************************************************
* File: math64bits.h.
* Desc: Implementation of 64 bits mathematics, using 32 bits int.
* Module: AkraLog : JavaKit.
* Rev: 8 septembre 1997 : REV 0 : Hugo DesRosiers : Creation.
**************************************************/

#include "math64bits.h"


Value64Bits::Value64Bits(void)
{
    data.lint.lowWord= data.lint.highWord= 0;
}


Value64Bits::Value64Bits(unsigned int high, unsigned int low)
{
    data.lint.lowWord= low;
    data.lint.highWord= high;
}


Value64Bits::Value64Bits(double aDouble)
{
    data.d= aDouble;
}


void Value64Bits::iadd(Value64Bits *result, Value64Bits *op1, Value64Bits *op2)
{
    result->data.lint.highWord= op1->data.lint.highWord + op2->data.lint.highWord;
    result->data.lint.lowWord= op1->data.lint.lowWord + op2->data.lint.lowWord;

    if ((((op1->data.lint.lowWord >> 1) + (op2->data.lint.lowWord >> 1)
	    + (((op1->data.lint.lowWord & 1) & (op2->data.lint.lowWord & 1)) ? 1 : 0)) & (1 << 31)) != 0) {
	result->data.lint.highWord++;
    }
}


void Value64Bits::isub(Value64Bits *result, Value64Bits *op1, Value64Bits *op2)
{
	// Negate substractor.
    op2->data.lint.highWord= ~op2->data.lint.highWord;
    op2->data.lint.lowWord= (~op2->data.lint.lowWord) + 1;
    if (op2->data.lint.lowWord == 0) op2->data.lint.highWord++;

	// Do addition.
    result->data.lint.highWord= op1->data.lint.highWord + op2->data.lint.highWord;
    result->data.lint.lowWord= op1->data.lint.lowWord + op2->data.lint.lowWord;
    if ((((op1->data.lint.lowWord >> 1) + (op2->data.lint.lowWord >> 1)
	    + (((op1->data.lint.lowWord & 1) & (op2->data.lint.lowWord & 1)) ? 1 : 0)) & (1 << 31)) != 0) {
	result->data.lint.highWord++;
    }
}


void Value64Bits::imult(Value64Bits *result, Value64Bits *op1, Value64Bits *op2)
{
#if defined(NO_64BITMATH)
    unsigned int op1lh, op2lh;
    bool signe= false;

	// Make both operands positive if necessary.
    if ((op1->data.lint.highWord & (1 << 31)) != 0) {
	op1->data.lint.highWord+= (unsigned int)0x0FFFFFFFF;
	if (op1->data.lint.lowWord != 0) op1->data.lint.highWord++;
	op1->data.lint.lowWord+= (unsigned int)0x0FFFFFFFF;
	op1->data.lint.highWord= ~op1->data.lint.highWord;
	op1->data.lint.lowWord= ~op1->data.lint.lowWord;
	signe= !signe;
    }
    if ((op2->data.lint.highWord & (1 << 31)) != 0) {
	op2->data.lint.highWord+= (unsigned int)0x0FFFFFFFF;
	if (op2->data.lint.lowWord != 0) op2->data.lint.highWord++;
	op2->data.lint.lowWord+= (unsigned int)0x0FFFFFFFF;
	op2->data.lint.highWord= ~op2->data.lint.highWord;
	op2->data.lint.lowWord= ~op2->data.lint.lowWord;
	signe= !signe;
    }

    result->data.lint.highWord= op1->data.lint.highWord * op2->data.lint.lowWord + op2->data.lint.highWord * op1->data.lint.lowWord;
	// This will overflow in general, but we are only interested in the low 32 bits.
    result->data.lint.lowWord= op1->data.lint.lowWord * op2->data.lint.lowWord;

	// Count 32 bits overflow from last multiplication, so it is added in high word.
    op1lh= op1->data.lint.lowWord >> 16;
    op2lh= op2->data.lint.lowWord >> 16;
    result->data.lint.highWord+= op1lh * op2lh + (((op1->data.lint.lowWord & (unsigned int)0x0FFFF) * op2lh) >> 16) + (((op2->data.lint.lowWord & (unsigned int)0x0FFFF) * op1lh) >> 16);

    if (signe) {
	result->data.lint.highWord= ~result->data.lint.highWord;
	result->data.lint.lowWord= (~result->data.lint.lowWord) + 1;
	if (result->data.lint.highWord == 0) result->data.lint.highWord++;
    }
#else
    long long a, b, c;

    a= (((unsigned long long)(op1->data.lint.highWord)) << 32) | op1->data.lint.lowWord;
    b= (((unsigned long long)(op2->data.lint.highWord)) << 32) | op2->data.lint.lowWord;
    c= a * b;
    result->data.lint.highWord= ((unsigned long long)c) >> 32;
    result->data.lint.lowWord= ((unsigned long long)c) & 0x0FFFFFFFF;
#endif
}


void Value64Bits::idiv(Value64Bits *result, Value64Bits *op1, Value64Bits *op2)
{
#if defined(NO_64BITMATH)
    bool signe= false;

	// Make both operands positive if necessary.
    if ((op1->data.lint.highWord & (1 << 31)) != 0) {
	op1->data.lint.highWord+= 0x0FFFFFFFF;
	if (op1->data.lint.lowWord != 0) op1->data.lint.highWord++;
	op1->data.lint.lowWord+= 0x0FFFFFFFF;
	signe= !signe;
    }
    if ((op2->data.lint.highWord & (1 << 31)) != 0) {
	op2->data.lint.highWord+= 0x0FFFFFFFF;
	if (op2->data.lint.lowWord != 0) op2->data.lint.highWord++;
	op2->data.lint.lowWord+= 0x0FFFFFFFF;
	signe= !signe;
	signe= !signe;
    }
    // TODO: c= a / b ==> add the cases b > a, b < 2^32, general.
#else
    long long a, b, c;

    a= (((unsigned long long)(op1->data.lint.highWord)) << 32) | op1->data.lint.lowWord;
    b= (((unsigned long long)(op2->data.lint.highWord)) << 32) | op2->data.lint.lowWord;
    c= a / b;
    result->data.lint.highWord= ((unsigned long long)c) >> 32;
    result->data.lint.lowWord= ((unsigned long long)c) & 0x0FFFFFFFF;
#endif
}


void Value64Bits::irem(Value64Bits *result, Value64Bits *op1, Value64Bits *op2)
{
#if defined(NO_64BITMATH)
    bool signe= false;

	// Make both operands positive if necessary.
    if ((op1->data.lint.highWord & (1 << 31)) != 0) {
	op1->data.lint.highWord+= 0x0FFFFFFFF;
	if (op1->data.lint.lowWord != 0) op1->data.lint.highWord++;
	op1->data.lint.lowWord+= 0x0FFFFFFFF;
	signe= !signe;
    }
    if ((op2->data.lint.highWord & (1 << 31)) != 0) {
	op2->data.lint.highWord+= 0x0FFFFFFFF;
	if (op2->data.lint.lowWord != 0) op2->data.lint.highWord++;
	op2->data.lint.lowWord+= 0x0FFFFFFFF;
	signe= !signe;
	signe= !signe;
    }
#else
    long long a, b, c;

    a= (((unsigned long long)(op1->data.lint.highWord)) << 32) | op1->data.lint.lowWord;
    b= (((unsigned long long)(op2->data.lint.highWord)) << 32) | op2->data.lint.lowWord;
    c= a % b;
    result->data.lint.highWord= ((unsigned long long)c) >> 32;
    result->data.lint.lowWord= ((unsigned long long)c) & 0x0FFFFFFFF;
#endif
}


void Value64Bits::ineg(Value64Bits *result)
{
    bool signe= false;

	// Make both operands positive if necessary.
    if ((result->data.lint.highWord & (1 << 31)) != 0) {
	result->data.lint.highWord+= 0x0FFFFFFFF;
	if (result->data.lint.lowWord != 0) result->data.lint.highWord++;
	result->data.lint.lowWord+= 0x0FFFFFFFF;
	signe= !signe;
    }
    else {
	result->data.lint.highWord= ~result->data.lint.highWord;
	result->data.lint.lowWord= (~result->data.lint.lowWord) + 1;
	if (result->data.lint.lowWord == 0) result->data.lint.highWord++;
    }
}


unsigned int Value64Bits::lcmp(Value64Bits *op1, Value64Bits *op2)
{
    Value64Bits tmpResult;

    isub(&tmpResult, op1, op2);

    if (tmpResult.data.lint.highWord & (1 << 31)) {
	return 0x0FFFFFFFF;
    }
    else if ((tmpResult.data.lint.highWord == 0) && (tmpResult.data.lint.highWord == 0)) {
	return 0;
    }

    return 1;
}


void Value64Bits::toLong(Value64Bits *result, float aFloat)
{
#if defined(NO_64BITMATH)
#else
    long long tmpLL= (long long)aFloat;

    result->data.lint.lowWord= tmpLL & 0x0FFFFFFFF;
    result->data.lint.highWord= ((unsigned long long)tmpLL) >> 32;
#endif
}


void Value64Bits::toLong(Value64Bits *result, Value64Bits *op1)
{
#if defined(NO_64BITMATH)
#else
    long long tmpLL= (long long)op1->data.d;

    result->data.lint.lowWord= tmpLL & 0x0FFFFFFFF;
    result->data.lint.highWord= ((unsigned long long)tmpLL) >> 32;
#endif
};
