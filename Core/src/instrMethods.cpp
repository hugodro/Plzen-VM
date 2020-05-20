/**************************************************
* File: instructions.cc.
* Desc: Implementation of the JVMachine instruction set.
* Module: AkraLog : JavaKit.
* Rev: 2 mai 1996 : REV 0 : Hugo DesRosiers : Creation.
**************************************************/

extern "C" {
#include <math.h>
#if defined(_WIN32)
#include <float.h>
#endif
 }

#if defined(MSDOS)
#define NAN asinf(2.0f)
#elif defined(IRIX)
#include <nan.h>
#define NAN asinf(2.0f)
#elif defined(_WIN32)
#define NAN ((float)_FPCLASS_QNAN)
#endif


#if defined(MSDOS)
#include "jvInst~2.h"
#include "execFr~1.h"
#include "poolMa~1.h"
#include "jvInst~1.h"
#include "jvcup.h"
#include "classM~1.h"
#include "debugI~1.h"
#include "machine2.h"
// TMPTMP:
#if defined(PLZEN_LEVEL_2)
#include "fsmach~1.h"
#include "statei~1.h"
#include "eventq~1.h"
#include "debugi~1.h"
#endif		/* PLZEN_LEVEL_2. */
#else
#include "jvInstrSet.h"
#include "execFrame.h"
#include "poolManager.h"
#include "jvInstance.h"
#include "jvcup.h"
#include "classManager.h"
#include "machine2.h"
// TMPTMP:
#if defined(PLZEN_LEVEL_2)
#include "fsMachine.h"
#include "stateInstance.h"
#include "eventQueue.h"
#include "debugInfo.h"
#endif		/* PLZEN_LEVEL_2. */
#endif		/* MSDOS. */


void JVMachine2::I_BIPUSH(void)
{

    signedByte= execFrame->fetchByte();
    anInt= signedByte;
    operStack->pushInteger((unsigned int)anInt);
    execFrame->incPCfake(JVI_BIPUSH_SIZE);        // DEBUG.
}


void JVMachine2::I_SIPUSH(void)
{

    aShort= execFrame->fetchShort();
    operStack->pushShort(aShort);
    execFrame->incPCfake(JVI_SIPUSH_SIZE);       // DEBUG.
}


void JVMachine2::I_LDC1(void)
{

    aByte= execFrame->fetchByte();
    switch (execFrame->constantPool[aByte]->type()) {
	case CONSTANT_String:
	    genValue.metaType= TYPE_INSTANCE;
	    aCteString= (StringInfo *)((CteString *)execFrame->constantPool[aByte])->resolve(
				kernel->getClassManager(), execFrame->currentClass);
	    genValue.value= (void *)aCteString->constantValue();
	    operStack->push(genValue);
	    break;
	case CONSTANT_Integer:
	case CONSTANT_Float:	// Float is treated the same way right now.
	    operStack->pushInteger(((CteWord *)execFrame->constantPool[aByte])->value());
	    break;
	default:
	    cout << "Warning: unknown constant type for JVI_LDC1: " << execFrame->constantPool[aByte]->type() << ".\n";
// ATTN: Temporary, to get the machine to keep going.
	    anUInt= 0;
	    operStack->pushInteger(anUInt);
	    break;
	}
    execFrame->incPCfake(JVI_LDC1_SIZE);       // DEBUG.
}


void JVMachine2::I_LDC2(void)
{

    aShort= execFrame->fetchShort();
    switch (execFrame->constantPool[aShort]->type()) {
	case CONSTANT_String:
	    genValue.metaType= TYPE_INSTANCE;
	    genValue.value= ((CteString *)execFrame->constantPool[aShort])->resolve(NULL,
						execFrame->currentClass);
	    operStack->push(genValue);
	    break;
	case CONSTANT_Integer:
	case CONSTANT_Float:	// Float is treated the same way right now.
	    operStack->pushInteger(((CteWord *)execFrame->constantPool[aShort])->value());
	    break;
	default:
	    cout << "Warning: unknown constant type for JVI_LDC2: " << execFrame->constantPool[aByte]->type() << ".\n";
	    break;
	}
    execFrame->incPCfake(JVI_LDC2_SIZE);       // DEBUG.
}


void JVMachine2::I_LDC2W(void)
{

    aShort= execFrame->fetchShort();
    switch (execFrame->constantPool[aShort]->type()) {
	case CONSTANT_Long:
	case CONSTANT_Double:
	    arg64Bits[0].data.lint.lowWord= ((CteDoubleWord *)execFrame->constantPool[aShort])->valueLow();
	    arg64Bits[0].data.lint.highWord= ((CteDoubleWord *)execFrame->constantPool[aShort])->valueHigh();
	    operStack->pushLong(&arg64Bits[0]);
	    break;
	default:
	    cout << "Warning: unknown constant type for JVI_LDC2W: " << execFrame->constantPool[aShort]->type() << ".\n";
	    break;
	}
    execFrame->incPCfake(JVI_LDC2W_SIZE);       // DEBUG.
}


void JVMachine2::I_ACONST_NULL(void)
{

    genValue.metaType= TYPE_INSTANCE;
    genValue.value= JVAbstractInstance::nullObject;
    operStack->push(genValue);
    execFrame->incPCfake(JVI_ACONST_NULL_SIZE);       // DEBUG.
}


void JVMachine2::I_ICONST_M1(void)
{

    operStack->pushInteger((unsigned int)-1);
    execFrame->incPCfake(JVI_ICONST_M1_SIZE);       // DEBUG.
}


void JVMachine2::I_ICONST_0(void)
{

    operStack->pushInteger(0);
    execFrame->incPCfake(JVI_ICONST_0_SIZE);       // DEBUG.
}


void JVMachine2::I_ICONST_1(void)
{

    operStack->pushInteger(1);
    execFrame->incPCfake(JVI_ICONST_1_SIZE);       // DEBUG.
}


void JVMachine2::I_ICONST_2(void)
{

    operStack->pushInteger(2);
    execFrame->incPCfake(JVI_ICONST_2_SIZE);       // DEBUG.
}


void JVMachine2::I_ICONST_3(void)
{

    operStack->pushInteger(3);
    execFrame->incPCfake(JVI_ICONST_3_SIZE);       // DEBUG.
}


void JVMachine2::I_ICONST_4(void)
{

    operStack->pushInteger(4);
    execFrame->incPCfake(JVI_ICONST_4_SIZE);       // DEBUG.
}


void JVMachine2::I_ICONST_5(void)
{

    operStack->pushInteger(5);
    execFrame->incPCfake(JVI_ICONST_5_SIZE);       // DEBUG.
}


void JVMachine2::I_LCONST_0(void)
{

    arg64Bits[0].data.lint.lowWord= 0;
    arg64Bits[0].data.lint.highWord= 0;
    operStack->pushLong(&arg64Bits[0]);
    execFrame->incPCfake(JVI_LCONST_0_SIZE);       // DEBUG.
}


void JVMachine2::I_LCONST_1(void)
{

    arg64Bits[0].data.lint.lowWord= 1;
    arg64Bits[0].data.lint.highWord= 0;
    operStack->pushLong(&arg64Bits[0]);
    execFrame->incPCfake(JVI_LCONST_1_SIZE);       // DEBUG.
}


void JVMachine2::I_FCONST_0(void)
{

	_fai.f= 0.0f;	/* ENNT: ajoute le 'f'. */
    operStack->pushInteger(_fai.i);
    execFrame->incPCfake(JVI_FCONST_0_SIZE);
}


void JVMachine2::I_FCONST_1(void)
{

    _fai.f= 1.0f;		/* ENNT: ajoute le 'f'. */
    operStack->pushInteger(_fai.i);
    execFrame->incPCfake(JVI_FCONST_1_SIZE);       // DEBUG.
}


void JVMachine2::I_FCONST_2(void)
{

    _fai.f= 2.0f;			/* ENNT: ajoute le 'f'. */
    operStack->pushInteger(_fai.i);
    execFrame->incPCfake(JVI_FCONST_2_SIZE);       // DEBUG.
}


void JVMachine2::I_DCONST_0(void)
{

    arg64Bits[0].data.d= 0.0;
    operStack->pushLong(&arg64Bits[0]);
    execFrame->incPCfake(JVI_DCONST_0_SIZE);
}


void JVMachine2::I_DCONST_1(void)
{

    arg64Bits[0].data.d= 1.0;
    operStack->pushLong(&arg64Bits[0]);
    execFrame->incPCfake(JVI_DCONST_1_SIZE);       // DEBUG.
}


void JVMachine2::I_ILOAD(void)
{

    vindex|= execFrame->fetchByte();
    operStack->pushInteger((int)execFrame->localValues[vindex]->value);
    vindex= 0;
    execFrame->incPCfake(JVI_ILOAD_SIZE);       // DEBUG.
}


void JVMachine2::I_ILOAD_0(void)
{

    operStack->pushInteger((int)execFrame->localValues[0]->value);
    execFrame->incPCfake(JVI_ILOAD_0_SIZE);       // DEBUG.
}


void JVMachine2::I_ILOAD_1(void)
{

    operStack->pushInteger((int)execFrame->localValues[1]->value);
    execFrame->incPCfake(JVI_ILOAD_1_SIZE);       // DEBUG.
}


void JVMachine2::I_ILOAD_2(void)
{

    operStack->pushInteger((int)execFrame->localValues[2]->value);
    execFrame->incPCfake(JVI_ILOAD_2_SIZE);       // DEBUG.
}


void JVMachine2::I_ILOAD_3(void)
{

    operStack->pushInteger((int)execFrame->localValues[3]->value);
    execFrame->incPCfake(JVI_ILOAD_3_SIZE);       // DEBUG.
}


void JVMachine2::I_LLOAD(void)
{

    vindex|= execFrame->fetchByte();
// MOD-970909 [HD]: Using now the Value64Bits all over.
    arg64Bits[0].data.lint.highWord= (unsigned int)execFrame->localValues[vindex]->value;
    arg64Bits[0].data.lint.lowWord= (unsigned int)execFrame->localValues[vindex+1]->value;
    operStack->pushLong(&arg64Bits[0]);
    vindex= 0;
    execFrame->incPCfake(JVI_LLOAD_SIZE);       // DEBUG.
}


void JVMachine2::I_LLOAD_0(void)
{

// MOD-970909 [HD]: Using now the Value64Bits all over.
    arg64Bits[0].data.lint.highWord= (unsigned int)execFrame->localValues[0]->value;
    arg64Bits[0].data.lint.lowWord= (unsigned int)execFrame->localValues[1]->value;
    operStack->pushLong(&arg64Bits[0]);
    execFrame->incPCfake(JVI_LLOAD_0_SIZE);       // DEBUG.
}


void JVMachine2::I_LLOAD_1(void)
{

// MOD-970909 [HD]: Using now the Value64Bits all over.
    arg64Bits[0].data.lint.highWord= (unsigned int)execFrame->localValues[1]->value;
    arg64Bits[0].data.lint.lowWord= (unsigned int)execFrame->localValues[2]->value;
    operStack->pushLong(&arg64Bits[0]);
    execFrame->incPCfake(JVI_LLOAD_1_SIZE);       // DEBUG.
}


void JVMachine2::I_LLOAD_2(void)
{

// MOD-970909 [HD]: Using now the Value64Bits all over.
    arg64Bits[0].data.lint.highWord= (unsigned int)execFrame->localValues[2]->value;
    arg64Bits[0].data.lint.lowWord= (unsigned int)execFrame->localValues[3]->value;
    operStack->pushLong(&arg64Bits[0]);
    execFrame->incPCfake(JVI_LLOAD_2_SIZE);       // DEBUG.
}


void JVMachine2::I_LLOAD_3(void)
{

// MOD-970909 [HD]: Using now the Value64Bits all over.
    arg64Bits[0].data.lint.highWord= (unsigned int)execFrame->localValues[3]->value;
    arg64Bits[0].data.lint.lowWord= (unsigned int)execFrame->localValues[4]->value;
    operStack->pushLong(&arg64Bits[0]);
    execFrame->incPCfake(JVI_LLOAD_3_SIZE);       // DEBUG.
}


void JVMachine2::I_FLOAD(void)
{

    vindex|= execFrame->fetchByte();
    operStack->pushInteger((int)execFrame->localValues[vindex]->value);
    vindex= 0;
    execFrame->incPCfake(JVI_FLOAD_SIZE);       // DEBUG.
}


void JVMachine2::I_FLOAD_0(void)
{

    operStack->pushInteger((int)execFrame->localValues[0]->value);
    execFrame->incPCfake(JVI_FLOAD_0_SIZE);       // DEBUG.
}


void JVMachine2::I_FLOAD_1(void)
{

    operStack->pushInteger((int)execFrame->localValues[1]->value);
    execFrame->incPCfake(JVI_FLOAD_1_SIZE);       // DEBUG.
}


void JVMachine2::I_FLOAD_2(void)
{

    operStack->pushInteger((int)execFrame->localValues[2]->value);
    execFrame->incPCfake(JVI_FLOAD_2_SIZE);       // DEBUG.
}


void JVMachine2::I_FLOAD_3(void)
{

    operStack->pushInteger((int)execFrame->localValues[3]->value);
    execFrame->incPCfake(JVI_FLOAD_3_SIZE);       // DEBUG.
}


void JVMachine2::I_DLOAD(void)
{

    vindex|= execFrame->fetchByte();
// MOD-970909 [HD]: Using now the Value64Bits all over.
    arg64Bits[0].data.lint.highWord= (unsigned int)execFrame->localValues[vindex]->value;
    arg64Bits[0].data.lint.lowWord= (unsigned int)execFrame->localValues[vindex+1]->value;
    operStack->pushLong(&arg64Bits[0]);
    vindex= 0;
    execFrame->incPCfake(JVI_DLOAD_SIZE);       // DEBUG.
}


void JVMachine2::I_DLOAD_0(void)
{

// MOD-970909 [HD]: Using now the Value64Bits all over.
    arg64Bits[0].data.lint.highWord= (unsigned int)execFrame->localValues[0]->value;
    arg64Bits[0].data.lint.lowWord= (unsigned int)execFrame->localValues[1]->value;
    operStack->pushLong(&arg64Bits[0]);
    execFrame->incPCfake(JVI_DLOAD_0_SIZE);       // DEBUG.
}


void JVMachine2::I_DLOAD_1(void)
{

// MOD-970909 [HD]: Using now the Value64Bits all over.
    arg64Bits[0].data.lint.highWord= (unsigned int)execFrame->localValues[1]->value;
    arg64Bits[0].data.lint.lowWord= (unsigned int)execFrame->localValues[2]->value;
    operStack->pushLong(&arg64Bits[0]);
    execFrame->incPCfake(JVI_DLOAD_1_SIZE);       // DEBUG.
}


void JVMachine2::I_DLOAD_2(void)
{

// MOD-970909 [HD]: Using now the Value64Bits all over.
    arg64Bits[0].data.lint.highWord= (unsigned int)execFrame->localValues[2]->value;
    arg64Bits[0].data.lint.lowWord= (unsigned int)execFrame->localValues[3]->value;
    operStack->pushLong(&arg64Bits[0]);
    execFrame->incPCfake(JVI_DLOAD_2_SIZE);       // DEBUG.
}


void JVMachine2::I_DLOAD_3(void)
{

// MOD-970909 [HD]: Using now the Value64Bits all over.
    arg64Bits[0].data.lint.highWord= (unsigned int)execFrame->localValues[3]->value;
    arg64Bits[0].data.lint.lowWord= (unsigned int)execFrame->localValues[4]->value;
    operStack->pushLong(&arg64Bits[0]);
    execFrame->incPCfake(JVI_DLOAD_3_SIZE);       // DEBUG.
}


void JVMachine2::I_ALOAD(void)
{

    vindex|= execFrame->fetchByte();
    operStack->push(*execFrame->localValues[vindex]);
    vindex= 0;
    execFrame->incPCfake(JVI_ALOAD_SIZE);       // DEBUG.
}


void JVMachine2::I_ALOAD_0(void)
{

    operStack->push(*execFrame->localValues[0]);
    execFrame->incPCfake(JVI_ALOAD_0_SIZE);       // DEBUG.
}


void JVMachine2::I_ALOAD_1(void)
{

    operStack->push(*execFrame->localValues[1]);
    execFrame->incPCfake(JVI_ALOAD_1_SIZE);       // DEBUG.
}


void JVMachine2::I_ALOAD_2(void)
{

    operStack->push(*execFrame->localValues[2]);
    execFrame->incPCfake(JVI_ALOAD_2_SIZE);       // DEBUG.
}


void JVMachine2::I_ALOAD_3(void)
{

    operStack->push(*execFrame->localValues[3]);
    execFrame->incPCfake(JVI_ALOAD_3_SIZE);       // DEBUG.
}


void JVMachine2::I_ISTORE(void)
{

    vindex|= execFrame->fetchByte();
    operStack->popInteger((unsigned int &)execFrame->localValues[vindex]->value);
    vindex= 0;
    execFrame->incPCfake(JVI_ISTORE_SIZE);       // DEBUG.
}


void JVMachine2::I_ISTORE_0(void)
{

    operStack->popInteger((unsigned int &)(execFrame->localValues[0]->value));
    execFrame->incPCfake(JVI_ISTORE_0_SIZE);       // DEBUG.
}


void JVMachine2::I_ISTORE_1(void)
{

    operStack->popInteger((unsigned int &)execFrame->localValues[1]->value);
    execFrame->incPCfake(JVI_ISTORE_1_SIZE);       // DEBUG.
}


void JVMachine2::I_ISTORE_2(void)
{

    operStack->popInteger((unsigned int &)execFrame->localValues[2]->value);
    execFrame->incPCfake(JVI_ISTORE_2_SIZE);       // DEBUG.
}


void JVMachine2::I_ISTORE_3(void)
{

    operStack->popInteger((unsigned int &)execFrame->localValues[3]->value);
    execFrame->incPCfake(JVI_ISTORE_3_SIZE);       // DEBUG.
}


void JVMachine2::I_LSTORE(void)
{

    vindex|= execFrame->fetchByte();
// MOD-970909 [HD]: Using now the Value64Bits all over.
    operStack->popLong(&arg64Bits[0]);
    execFrame->localValues[vindex]->value= (void *)arg64Bits[0].data.lint.highWord;
    execFrame->localValues[vindex+1]->value= (void *)arg64Bits[0].data.lint.lowWord;
    vindex= 0;
    execFrame->incPCfake(JVI_LSTORE_SIZE);       // DEBUG.
}


void JVMachine2::I_LSTORE_0(void)
{

// MOD-970909 [HD]: Using now the Value64Bits all over.
    operStack->popLong(&arg64Bits[0]);
    execFrame->localValues[0]->value= (void *)arg64Bits[0].data.lint.highWord;
    execFrame->localValues[1]->value= (void *)arg64Bits[0].data.lint.lowWord;
    execFrame->incPCfake(JVI_LSTORE_0_SIZE);       // DEBUG.
}


void JVMachine2::I_LSTORE_1(void)
{

// MOD-970909 [HD]: Using now the Value64Bits all over.
    operStack->popLong(&arg64Bits[0]);
    execFrame->localValues[1]->value= (void *)arg64Bits[0].data.lint.highWord;
    execFrame->localValues[2]->value= (void *)arg64Bits[0].data.lint.lowWord;
    execFrame->incPCfake(JVI_LSTORE_1_SIZE);       // DEBUG.
}


void JVMachine2::I_LSTORE_2(void)
{

// MOD-970909 [HD]: Using now the Value64Bits all over.
    operStack->popLong(&arg64Bits[0]);
    execFrame->localValues[2]->value= (void *)arg64Bits[0].data.lint.highWord;
    execFrame->localValues[3]->value= (void *)arg64Bits[0].data.lint.lowWord;
    execFrame->incPCfake(JVI_LSTORE_2_SIZE);       // DEBUG.
}


void JVMachine2::I_LSTORE_3(void)
{

// MOD-970909 [HD]: Using now the Value64Bits all over.
    operStack->popLong(&arg64Bits[0]);
    execFrame->localValues[3]->value= (void *)arg64Bits[0].data.lint.highWord;
    execFrame->localValues[4]->value= (void *)arg64Bits[0].data.lint.lowWord;
    execFrame->incPCfake(JVI_LSTORE_3_SIZE);       // DEBUG.
}


void JVMachine2::I_FSTORE(void)
{

    vindex|= execFrame->fetchByte();
    operStack->popInteger((unsigned int &)execFrame->localValues[vindex]->value);
    vindex= 0;
    execFrame->incPCfake(JVI_FSTORE_SIZE);       // DEBUG.
}


void JVMachine2::I_FSTORE_0(void)
{

    operStack->popInteger((unsigned int &)execFrame->localValues[0]->value);
    execFrame->incPCfake(JVI_FSTORE_0_SIZE);       // DEBUG.
}


void JVMachine2::I_FSTORE_1(void)
{

    operStack->popInteger((unsigned int &)execFrame->localValues[1]->value);
    execFrame->incPCfake(JVI_FSTORE_1_SIZE);       // DEBUG.
}


void JVMachine2::I_FSTORE_2(void)
{

    operStack->popInteger((unsigned int &)execFrame->localValues[2]->value);
    execFrame->incPCfake(JVI_FSTORE_2_SIZE);       // DEBUG.
}


void JVMachine2::I_FSTORE_3(void)
{

    operStack->popInteger((unsigned int &)execFrame->localValues[3]->value);
    execFrame->incPCfake(JVI_FSTORE_3_SIZE);       // DEBUG.
}


void JVMachine2::I_DSTORE(void)
{

    vindex|= execFrame->fetchByte();
// MOD-970909 [HD]: Using now the Value64Bits all over.
    operStack->popLong(&arg64Bits[0]);
    execFrame->localValues[vindex]->value= (void *)arg64Bits[0].data.lint.highWord;
    execFrame->localValues[vindex+1]->value= (void *)arg64Bits[0].data.lint.lowWord;
    vindex= 0;
    execFrame->incPCfake(JVI_DSTORE_SIZE);       // DEBUG.
}


void JVMachine2::I_DSTORE_0(void)
{

// MOD-970909 [HD]: Using now the Value64Bits all over.
    operStack->popLong(&arg64Bits[0]);
    execFrame->localValues[0]->value= (void *)arg64Bits[0].data.lint.highWord;
    execFrame->localValues[1]->value= (void *)arg64Bits[0].data.lint.lowWord;
    execFrame->incPCfake(JVI_DSTORE_0_SIZE);       // DEBUG.
}


void JVMachine2::I_DSTORE_1(void)
{

// MOD-970909 [HD]: Using now the Value64Bits all over.
    operStack->popLong(&arg64Bits[0]);
    execFrame->localValues[1]->value= (void *)arg64Bits[0].data.lint.highWord;
    execFrame->localValues[2]->value= (void *)arg64Bits[0].data.lint.lowWord;
    execFrame->incPCfake(JVI_DSTORE_1_SIZE);       // DEBUG.
}


void JVMachine2::I_DSTORE_2(void)
{

// MOD-970909 [HD]: Using now the Value64Bits all over.
    operStack->popLong(&arg64Bits[0]);
    execFrame->localValues[2]->value= (void *)arg64Bits[0].data.lint.highWord;
    execFrame->localValues[3]->value= (void *)arg64Bits[0].data.lint.lowWord;
    execFrame->incPCfake(JVI_DSTORE_2_SIZE);       // DEBUG.
}


void JVMachine2::I_DSTORE_3(void)
{

// MOD-970909 [HD]: Using now the Value64Bits all over.
    operStack->popLong(&arg64Bits[0]);
    execFrame->localValues[3]->value= (void *)arg64Bits[0].data.lint.highWord;
    execFrame->localValues[4]->value= (void *)arg64Bits[0].data.lint.lowWord;
    execFrame->incPCfake(JVI_DSTORE_3_SIZE);       // DEBUG.
}


void JVMachine2::I_ASTORE(void)
{

    vindex|= execFrame->fetchByte();
    operStack->pop(*execFrame->localValues[vindex]);
    vindex= 0;
    execFrame->incPCfake(JVI_ASTORE_SIZE);       // DEBUG.
}


void JVMachine2::I_ASTORE_0(void)
{

    operStack->pop(*execFrame->localValues[0]);
    execFrame->incPCfake(JVI_ASTORE_0_SIZE);       // DEBUG.
}


void JVMachine2::I_ASTORE_1(void)
{

    operStack->pop(*execFrame->localValues[1]);
    execFrame->incPCfake(JVI_ASTORE_1_SIZE);       // DEBUG.
}


void JVMachine2::I_ASTORE_2(void)
{

    operStack->pop(*execFrame->localValues[2]);
    execFrame->incPCfake(JVI_ASTORE_2_SIZE);       // DEBUG.
}


void JVMachine2::I_ASTORE_3(void)
{

    operStack->pop(*execFrame->localValues[3]);
    execFrame->incPCfake(JVI_ASTORE_3_SIZE);       // DEBUG.
}


void JVMachine2::I_IINC(void)
{

// ATTN: To be optimized when correct sign handling is known.
    vindex|= execFrame->fetchByte();
    tmpInt= (int)execFrame->localValues[vindex]->value;
    tmpInt+= (int)(char)execFrame->fetchByte();
    execFrame->localValues[vindex]->value= (void *)tmpInt;
    vindex= 0;
    execFrame->incPCfake(JVI_IINC_SIZE);       // DEBUG.
}


void JVMachine2::I_WIDE(void)
{

    vindex= ((unsigned int)execFrame->fetchByte()) << 8;
    execFrame->incPCfake(JVI_WIDE_SIZE);       // DEBUG.
}


void JVMachine2::I_NEWARRAY(void)
{

    operStack->popInteger((unsigned int &)anInt);
    if (anInt > 0) {
	aByte= execFrame->fetchByte();
	genValue.metaType= TYPE_ARRAY | TYPE_BASIC_ARRAY;
	genValue.value= poolManager->getBasicArray(aByte, anInt);
	if (genValue.value != NULL) {
	    operStack->push(genValue);
	    }
	else {
		// THROW run time exception !
	    cout << "RT Exception: OutOfMemoryError.\n";
	    raiseException(NULL);
	    }
	}
    else {
	    // THROW run time exception !
	cout << "RT Exception: NegativeArraySizeException.\n";
	raiseException(NULL);
	}
    execFrame->incPCfake(JVI_NEWARRAY_SIZE);       // DEBUG.
}


void JVMachine2::I_ANEWARRAY(void)
{

    operStack->popInteger((unsigned int &)anInt);		// Get size.
    aShort= execFrame->fetchShort();	// Get index of class.
    if (anInt > 0) {
	aClass= (ClassFile *)execFrame->constantPool[aShort]->resolve(kernel->getClassManager(), execFrame->currentClass);
	if (aClass) {
	    if (aClass->isArrayClass()) {
		genValue.metaType= TYPE_ARRAY | TYPE_ARRAY_ARRAY;
		genValue.value= poolManager->getArrayArray(aClass, (unsigned int *)&anInt, 1);
		}
	    else {
		genValue.metaType= TYPE_ARRAY | TYPE_INST_ARRAY;
		genValue.value= poolManager->getInstanceArray(aClass, anInt);
		}
	    operStack->push(genValue);
	    }
	else {
	    // THROW run time exception !
	    cout << "RT Exception: class not found.\n";
	    raiseException(NULL);
	    }
	}
    else {
	// Throw run time exception !
	cout << "RT Exception: NegativeArraySizeException.\n";
	raiseException(NULL);
	}
    execFrame->incPCfake(JVI_ANEWARRAY_SIZE);       // DEBUG.
}


void JVMachine2::I_MULTIANEWARRAY(void)
{

    aShort= execFrame->fetchShort();
    aByte= execFrame->fetchByte();
    if (aByte > 0) {
	tmpDims= new unsigned int[aByte];
	for (i= aByte; i > 0;) {
	    operStack->popInteger((unsigned int &)tmpDims[i-1]);
	    if (((int)tmpDims[i-1]) < 0) {
		// Throw run time exception !
		cout << "RT Exception: NegativeArraySizeException.\n";
		raiseException(NULL);
		}
		i--;
	    }
	if (!exceptionRaised) {
	    aClass= (ClassFile *)execFrame->constantPool[aShort]->resolve(kernel->getClassManager(), execFrame->currentClass);
	    if (aClass) {
		genValue.metaType= TYPE_ARRAY | TYPE_ARRAY_ARRAY;
		genValue.value= poolManager->getArrayArray(aClass, tmpDims, (unsigned int)aByte);
		operStack->push(genValue);
	    }
	    else {
		// THROW run time exception !
		cout << "RT Exception: class not found.\n";
		raiseException(NULL);
	    }
	    }
	delete tmpDims;
	}
    else {
	// Throw run time exception !
	cout << "RT Exception: NegativeArraySizeException.\n";
	raiseException(NULL);
	}
    execFrame->incPCfake(JVI_MULTIANEWARRAY_SIZE);       // DEBUG.
}


void JVMachine2::I_ARRAYLENGTH(void)
{

    operStack->pop(genValue);
    if (genValue.metaType & TYPE_BASIC_ARRAY) {
	operStack->pushInteger(((JVBasicArray *)genValue.value)->nbrElements);
	}
    else if (genValue.metaType & TYPE_INST_ARRAY) {
	if (genValue.value != JVAbstractInstance::nullObject) {
	    operStack->pushInteger(((JVInstanceArray *)genValue.value)->nbrElements);
	    }
	else {
	    cout << "RT Exception: NullPointerException.\n";
	    raiseException(NULL);
	    }
	}
    else if (genValue.metaType & TYPE_ARRAY_ARRAY) {
	if (genValue.value != JVAbstractInstance::nullObject) {
	    operStack->pushInteger(((JVArrayArray *)genValue.value)->nbrElements);
	    }
	else {
	    cout << "RT Exception: NullPointerException.\n";
	    raiseException(NULL);
	    }
	}
    else {
	operStack->pushInteger(0);
	cout << "RT Exception: Array sizing of a non-array instance.\n";
	raiseException(NULL);
	}
    execFrame->incPCfake(JVI_ARRAYLENGTH_SIZE);       // DEBUG.
}


void JVMachine2::I_IALOAD(void)
{

    operStack->popInteger((unsigned int &)anInt);
    operStack->pop(genValue);
    if (genValue.metaType & TYPE_BASIC_ARRAY) {
	if (((JVBasicArray *)genValue.value)->nbrElements > (unsigned int)anInt) {
	    operStack->pushInteger((int)((JVBasicArray *)genValue.value)->getElement(anInt));
	    }
	else {
	    cout << "RT Exception: ArrayIndexOutOfBoundsException.\n";
	    raiseException(NULL);
	    }
	}
    else {
	cout << "RT Exception: Bad element type in JVI_IALOAD.\n";
	raiseException(NULL);
	}
    execFrame->incPCfake(JVI_IALOAD_SIZE);       // DEBUG.
}


void JVMachine2::I_LALOAD(void)
{

    operStack->popInteger((unsigned int &)anInt);
    operStack->pop(genValue);
    if (genValue.metaType & TYPE_BASIC_ARRAY) {
	if (((JVBasicArray *)genValue.value)->nbrElements > (unsigned int)anInt) {
// MOD-970909 [HD]: Using now the Value64Bits all over.
	    operStack->pushLong((Value64Bits *)((JVBasicArray *)genValue.value)->getElement(anInt));
	    }
	else {
	    cout << "RT Exception: ArrayIndexOutOfBoundsException.\n";
	    raiseException(NULL);
	    }
	}
    else {
	cout << "RT Exception: Bad element type in JVI_LALOAD.\n";
	raiseException(NULL);
	}
    execFrame->incPCfake(JVI_LALOAD_SIZE);       // DEBUG.
}


void JVMachine2::I_FALOAD(void)
{

    operStack->popInteger((unsigned int &)anInt);
    operStack->pop(genValue);
    if (genValue.metaType & TYPE_BASIC_ARRAY) {
	if (((JVBasicArray *)genValue.value)->nbrElements > (unsigned int)anInt) {
	    operStack->pushInteger((int)((JVBasicArray *)genValue.value)->getElement(anInt));
	    }
	else {
	    cout << "RT Exception: ArrayIndexOutOfBoundsException.\n";
	    raiseException(NULL);
	    }
	}
    else {
	cout << "RT Exception: Bad element type in JVI_FALOAD.\n";
	raiseException(NULL);
	}
    execFrame->incPCfake(JVI_FALOAD_SIZE);       // DEBUG.
}


void JVMachine2::I_DALOAD(void)
{

    operStack->popInteger((unsigned int &)anInt);
    operStack->pop(genValue);
    if (genValue.metaType & TYPE_BASIC_ARRAY) {
	if (((JVBasicArray *)genValue.value)->nbrElements > (unsigned int)anInt) {
// MOD-970909 [HD]: Using now the Value64Bits all over.
	    operStack->pushLong((Value64Bits *)((JVBasicArray *)genValue.value)->getElement(anInt));
	    }
	else {
	    cout << "RT Exception: ArrayIndexOutOfBoundsException.\n";
	    raiseException(NULL);
	    }
	}
    else {
	cout << "RT Exception: Bad element type in JVI_DALOAD.\n";
	raiseException(NULL);
	}
    execFrame->incPCfake(JVI_DALOAD_SIZE);       // DEBUG.
}


void JVMachine2::I_AALOAD(void)
{

    operStack->popInteger((unsigned int &)anInt);
    operStack->pop(genValue);
    if (genValue.metaType & TYPE_INST_ARRAY) {
	if (genValue.value != JVAbstractInstance::nullObject) {
	    if (((JVInstanceArray *)genValue.value)->nbrElements > (unsigned int)anInt) {
// ATTENTION: On doit trouver si on a affaire a un autre TYPE_INST_ARRAY.
		genValue.metaType= TYPE_INSTANCE;
		genValue.value= ((JVInstanceArray *)genValue.value)->getElement(anInt);
		operStack->push(genValue);
		}
	    else {
		cout << "RT Exception: ArrayIndexOutOfBoundsException.\n";
		raiseException(NULL);
		}
	    }
	else {
	    cout << "RT Exception: NullPointerException.\n";
	    raiseException(NULL);
	    }
	}
    else if (genValue.metaType & TYPE_ARRAY_ARRAY) {
	if (genValue.value != JVAbstractInstance::nullObject) {
	    if (((JVArrayArray *)genValue.value)->nbrElements > (unsigned int)anInt) {
		genValue.value= ((JVArrayArray *)genValue.value)->getElement(anInt);
		if (((JVAbstractInstance *)genValue.value)->vectorialType() & TYPE_ARRAY) {
		    genValue.metaType= ((JVAbstractArray *)genValue.value)->vectorialType();
		}
		else {
		    genValue.metaType= TYPE_INSTANCE;    // Must be the null object.
		}
		operStack->push(genValue);
		}
	    else {
		cout << "RT Exception: ArrayIndexOutOfBoundsException.\n";
		raiseException(NULL);
		}
	    }
	else {
	    cout << "RT Exception: NullPointerException.\n";
	    raiseException(NULL);
	    }
    }
    else {
	cout << "RT Exception: Bad element type in JVI_AALOAD.\n";
	raiseException(NULL);
	}
    execFrame->incPCfake(JVI_AALOAD_SIZE);       // DEBUG.
}


void JVMachine2::I_BALOAD(void)
{

    operStack->popInteger((unsigned int &)anInt);
    operStack->pop(genValue);
    if (genValue.metaType & TYPE_BASIC_ARRAY) {
	if (((JVBasicArray *)genValue.value)->nbrElements > (unsigned int)anInt) {
#if defined(NeXT) || defined(_WIN32)
	    operStack->pushByte((ubyte)((JVBasicArray *)genValue.value)->getElement(anInt));
#else
	    void *tmpVal= ((JVBasicArray *)genValue.value)->getElement(anInt);
	    void *tmptmpVal= &tmpVal;
	    operStack->pushByte(*(ubyte*)tmptmpVal);
#endif
	    }
	else {
	    cout << "RT Exception: ArrayIndexOutOfBoundsException.\n";
	    raiseException(NULL);
	    }
	}
    else {
	cout << "RT Exception: Bad element type in JVI_BALOAD.\n";
	raiseException(NULL);
	}
    execFrame->incPCfake(JVI_BALOAD_SIZE);       // DEBUG.
}


void JVMachine2::I_CALOAD(void)
{

    operStack->popInteger((unsigned int &)anInt);
    operStack->pop(genValue);
    if (genValue.metaType & TYPE_BASIC_ARRAY) {
	if (((JVBasicArray *)genValue.value)->nbrElements > (unsigned int)anInt) {
#if defined(NeXT) || defined(_WIN32)
	    operStack->pushByte((ubyte)((JVBasicArray *)genValue.value)->getElement(anInt));
#else
	    void *tmpVal= ((JVBasicArray *)genValue.value)->getElement(anInt);
	    void *tmptmpVal= (void *)&tmpVal;
	    operStack->pushByte(*(ubyte*)tmptmpVal);
#endif
	    }
	else {
	    cout << "RT Exception: ArrayIndexOutOfBoundsException.\n";
	    raiseException(NULL);
	    }
	}
    else {
	cout << "RT Exception: Bad element type in JVI_CALOAD.\n";
	raiseException(NULL);
	}
    execFrame->incPCfake(JVI_CALOAD_SIZE);       // DEBUG.
}


void JVMachine2::I_SALOAD(void)
{

    operStack->popInteger((unsigned int &)anInt);
    operStack->pop(genValue);
    if (genValue.metaType & TYPE_BASIC_ARRAY) {
	if (((JVBasicArray *)genValue.value)->nbrElements > (unsigned int)anInt) {
#if defined(NeXT) || defined(_WIN32)
	    operStack->pushShort((short)((JVBasicArray *)genValue.value)->getElement(anInt));
#else
	    void *tmpVal= ((JVBasicArray *)genValue.value)->getElement(anInt);
	    void *tmptmpVal= (void *)&tmpVal;
	    operStack->pushByte(*(short *)tmptmpVal);
#endif
	    }
	else {
	    cout << "RT Exception: ArrayIndexOutOfBoundsException.\n";
	    raiseException(NULL);
	    }
	}
    else {
	cout << "RT Exception: Bad element type in JVI_SALOAD.\n";
	raiseException(NULL);
	}
    execFrame->incPCfake(JVI_SALOAD_SIZE);       // DEBUG.
}


void JVMachine2::I_IASTORE(void)
{

    operStack->popInteger((unsigned int &)tmpInt);		// Value.
    operStack->popInteger((unsigned int &)anInt);		// Index.
    operStack->pop(genValue);		// Array ref.
    if (genValue.metaType & TYPE_BASIC_ARRAY) {
	if (((JVBasicArray *)genValue.value)->nbrElements > (unsigned int)anInt) {
	    ((JVBasicArray *)genValue.value)->setElement(anInt, (void *)tmpInt);
	    }
	else {
	    cout << "RT Exception: ArrayIndexOutOfBoundsException.\n";
	    raiseException(NULL);
	    }
	}
    else {
	cout << "RT Exception: Bad element type in JVI_IASTORE.\n";
	raiseException(NULL);
	}
    execFrame->incPCfake(JVI_IASTORE_SIZE);       // DEBUG.
}


void JVMachine2::I_LASTORE(void)
{

// MOD-970909 [HD]: Using now the Value64Bits all over.
    operStack->popLong(&arg64Bits[0]);	    // Value.
    operStack->popInteger((unsigned int &)anInt);        // Index.
    operStack->pop(genValue);		// Array ref.
    if (genValue.metaType & TYPE_BASIC_ARRAY) {
	if (((JVBasicArray *)genValue.value)->nbrElements > (unsigned int)anInt) {
	    ((JVBasicArray *)genValue.value)->setElement(anInt, (void *)&arg64Bits[0]);
	    }
	else {
	    cout << "RT Exception: ArrayIndexOutOfBoundsException.\n";
	    raiseException(NULL);
	    }
	}
    else {
	cout << "RT Exception: Bad element type in JVI_LASTORE.\n";
	raiseException(NULL);
	}
    execFrame->incPCfake(JVI_LASTORE_SIZE);       // DEBUG.
}


void JVMachine2::I_FASTORE(void)
{

    operStack->popInteger((unsigned int &)tmpInt);		// Value.
    operStack->popInteger((unsigned int &)anInt);		// Index.
    operStack->pop(genValue);		// Array ref.
    if (genValue.metaType & TYPE_BASIC_ARRAY) {
	if (((JVBasicArray *)genValue.value)->nbrElements > (unsigned int)anInt) {
	    ((JVBasicArray *)genValue.value)->setElement(anInt, (void *)tmpInt);
	    }
	else {
	    cout << "RT Exception: ArrayIndexOutOfBoundsException.\n";
	    raiseException(NULL);
	    }
	}
    else {
	cout << "RT Exception: Bad element type in JVI_FASTORE.\n";
	raiseException(NULL);
	}
    execFrame->incPCfake(JVI_FASTORE_SIZE);       // DEBUG.
}


void JVMachine2::I_DASTORE(void)
{

// MOD-970909 [HD]: Using now the Value64Bits all over.
    operStack->popLong(&arg64Bits[0]);	    // Value.
    operStack->popInteger((unsigned int &)anInt);        // Index.
    operStack->pop(genValue);		// Array ref.
    if (genValue.metaType & TYPE_BASIC_ARRAY) {
	if (((JVBasicArray *)genValue.value)->nbrElements > (unsigned int)anInt) {
	    ((JVBasicArray *)genValue.value)->setElement(anInt, (void *)&arg64Bits[0]);
	    }
	else {
	    cout << "RT Exception: ArrayIndexOutOfBoundsException.\n";
	    raiseException(NULL);
	    }
	}
    else {
	cout << "RT Exception: Bad element type in JVI_LASTORE.\n";
	raiseException(NULL);
	}
    execFrame->incPCfake(JVI_DASTORE_SIZE);       // DEBUG.
}


void JVMachine2::I_AASTORE(void)
{

    operStack->pop(genValue2);		// Reference to an object.
    operStack->popInteger((unsigned int &)anInt);		// Index.
    operStack->pop(genValue);		// Array ref. (of objects of type of tmpInt reference).
    if (genValue.metaType & TYPE_INST_ARRAY) {
	if (((JVInstanceArray *)genValue.value)->nbrElements > (unsigned int)anInt) {
		// Check here for correct type between tmpInt ref and genValue.value array.
	    ((JVInstanceArray *)genValue.value)->setElement(anInt, (JVInstance *)genValue2.value);
	    }
	else {
	    cout << "RT Exception: ArrayIndexOutOfBoundsException.\n";
	    raiseException(NULL);
	    }
	}
    else if (genValue.metaType & TYPE_ARRAY_ARRAY) {
	if (((JVArrayArray *)genValue.value)->nbrElements > (unsigned int)anInt) {
		// Check here for correct type between tmpInt ref and genValue.value array.
	    ((JVArrayArray *)genValue.value)->setElement(anInt, (JVInstanceArray *)genValue2.value);
	}
	else {
	    cout << "RT Exception: ArrayIndexOutOfBoundsException.\n";
	    raiseException(NULL);
	    }
	}
    else {
	cout << "RT Exception: Bad element type in JVI_AASTORE.\n";
	raiseException(NULL);
	}
    execFrame->incPCfake(JVI_AASTORE_SIZE);       // DEBUG.
}


void JVMachine2::I_BASTORE(void)
{

    operStack->popInteger((unsigned int &)tmpInt);		// Value.
    operStack->popInteger((unsigned int &)anInt);		// Index.
    operStack->pop(genValue);		// Array ref.
    if (genValue.metaType & TYPE_BASIC_ARRAY) {
	if (((JVBasicArray *)genValue.value)->nbrElements > (unsigned int)anInt) {
	    tmpInt&= 0x0FF;
	    ((JVBasicArray *)genValue.value)->setElement(anInt, (void *)tmpInt);
	    }
	else {
	    cout << "RT Exception: ArrayIndexOutOfBoundsException.\n";
	    raiseException(NULL);
	    }
	}
    else {
	cout << "RT Exception: Bad element type in JVI_BASTORE.\n";
	raiseException(NULL);
	}
    execFrame->incPCfake(JVI_BASTORE_SIZE);       // DEBUG.
}


void JVMachine2::I_CASTORE(void)
{

    operStack->popInteger((unsigned int &)tmpInt);		// Value.
    operStack->popInteger((unsigned int &)anInt);		// Index.
    operStack->pop(genValue);		// Array ref.
    if (genValue.metaType & TYPE_BASIC_ARRAY) {
	if (((JVBasicArray *)genValue.value)->nbrElements > (unsigned int)anInt) {
	    tmpInt&= 0x0FF;
	    ((JVBasicArray *)genValue.value)->setElement(anInt, (void *)tmpInt);
	    }
	else {
	    cout << "RT Exception: ArrayIndexOutOfBoundsException.\n";
	    raiseException(NULL);
	    }
	}
    else {
	cout << "RT Exception: Bad element type in JVI_CASTORE.\n";
	raiseException(NULL);
	}
    execFrame->incPCfake(JVI_CASTORE_SIZE);       // DEBUG.
}


void JVMachine2::I_SASTORE(void)
{

    operStack->popShort((unsigned short &)aShort);		// Value.
    operStack->popInteger((unsigned int &)anInt);		// Index.
    operStack->pop(genValue);		// Array ref.
    if (genValue.metaType & TYPE_BASIC_ARRAY) {
	if (((JVBasicArray *)genValue.value)->nbrElements > (unsigned int)anInt) {
	    ((JVBasicArray *)genValue.value)->setElement(anInt, (void *)aShort);
	    }
	else {
	    cout << "RT Exception: ArrayIndexOutOfBoundsException.\n";
	    raiseException(NULL);
	    }
	}
    else {
	cout << "RT Exception: Bad element type in JVI_SASTORE.\n";
	raiseException(NULL);
	}
    execFrame->incPCfake(JVI_SASTORE_SIZE);       // DEBUG.
}


void JVMachine2::I_NOP(void)
{

	// The famous DO NOTHING instruction !
    execFrame->incPCfake(JVI_NOP_SIZE);       // DEBUG.
}


void JVMachine2::I_POP(void)
{

    operStack->popVoid();
    execFrame->incPCfake(JVI_POP_SIZE);       // DEBUG.
}


void JVMachine2::I_POP2(void)
{

    operStack->popVoidTwice();
    execFrame->incPCfake(JVI_POP2_SIZE);       // DEBUG.
}


void JVMachine2::I_DUP(void)
{

    operStack->dup();
    execFrame->incPCfake(JVI_DUP_SIZE);       // DEBUG.
}


void JVMachine2::I_DUP2(void)
{

    operStack->dup(2);
    execFrame->incPCfake(JVI_DUP2_SIZE);       // DEBUG.
}


void JVMachine2::I_DUP_X1(void)
{

    operStack->dupInsert();
    execFrame->incPCfake(JVI_DUP_X1_SIZE);       // DEBUG.
}


void JVMachine2::I_DUP2_X1(void)
{

    operStack->dupInsert(2);
    execFrame->incPCfake(JVI_DUP2_X1_SIZE);       // DEBUG.
}


void JVMachine2::I_DUP_X2(void)
{

    operStack->dupInsertThree();
    execFrame->incPCfake(JVI_DUP_X2_SIZE);       // DEBUG.
}


void JVMachine2::I_DUP2_X2(void)
{

    operStack->dupInsertThree();
    execFrame->incPCfake(JVI_DUP2_X2_SIZE);       // DEBUG.
}


void JVMachine2::I_SWAP(void)
{

    operStack->swap();
    execFrame->incPCfake(JVI_SWAP_SIZE);       // DEBUG.
}


void JVMachine2::I_IADD(void)
{

    operStack->popInteger((unsigned int &)arg32Bits[1].i);
    operStack->popInteger((unsigned int &)arg32Bits[0].i);
    arg32Bits[2].i= arg32Bits[0].i+arg32Bits[1].i;
    operStack->pushInteger(arg32Bits[2].i);
    execFrame->incPCfake(JVI_IADD_SIZE);       // DEBUG.
}


void JVMachine2::I_LADD(void)
{

    operStack->popLong(&arg64Bits[1]);
    operStack->popLong(&arg64Bits[0]);
    Value64Bits::iadd(&arg64Bits[2], &arg64Bits[0], &arg64Bits[1]);
    operStack->pushLong(&arg64Bits[2]);
    execFrame->incPCfake(JVI_LADD_SIZE);       // DEBUG.
}


void JVMachine2::I_FADD(void)
{

    operStack->popInteger((unsigned int &)arg32Bits[1].i);
    operStack->popInteger((unsigned int &)arg32Bits[0].i);
    arg32Bits[2].f= arg32Bits[0].f+arg32Bits[1].f;
    operStack->pushInteger(arg32Bits[2].i);
    execFrame->incPCfake(JVI_FADD_SIZE);       // DEBUG.
}


void JVMachine2::I_DADD(void)
{

    operStack->popLong(&arg64Bits[1]);
    operStack->popLong(&arg64Bits[0]);
    arg64Bits[2].data.d= arg64Bits[0].data.d + arg64Bits[1].data.d;
    operStack->pushLong(&arg64Bits[2]);
    execFrame->incPCfake(JVI_DADD_SIZE);       // DEBUG.
}


void JVMachine2::I_ISUB(void)
{

    operStack->popInteger((unsigned int &)arg32Bits[1].i);
    operStack->popInteger((unsigned int &)arg32Bits[0].i);
    arg32Bits[2].i= arg32Bits[0].i - arg32Bits[1].i;
    operStack->pushInteger(arg32Bits[2].i);
    execFrame->incPCfake(JVI_ISUB_SIZE);       // DEBUG.
}


void JVMachine2::I_LSUB(void)
{

    operStack->popLong(&arg64Bits[1]);
    operStack->popLong(&arg64Bits[0]);
    Value64Bits::isub(&arg64Bits[2], &arg64Bits[0], &arg64Bits[1]);
    operStack->pushLong(&arg64Bits[2]);
    execFrame->incPCfake(JVI_LSUB_SIZE);       // DEBUG.
}


void JVMachine2::I_FSUB(void)
{

    operStack->popInteger((unsigned int &)arg32Bits[1].i);
    operStack->popInteger((unsigned int &)arg32Bits[0].i);
    arg32Bits[2].f= arg32Bits[0].f - arg32Bits[1].f;
    operStack->pushInteger(arg32Bits[2].i);
    execFrame->incPCfake(JVI_FSUB_SIZE);       // DEBUG.
}


void JVMachine2::I_DSUB(void)
{

    operStack->popLong(&arg64Bits[1]);
    operStack->popLong(&arg64Bits[0]);
    arg64Bits[2].data.d= arg64Bits[0].data.d - arg64Bits[1].data.d;
    operStack->pushLong(&arg64Bits[2]);
    execFrame->incPCfake(JVI_DSUB_SIZE);       // DEBUG.
}


void JVMachine2::I_IMUL(void)
{

    operStack->popInteger((unsigned int &)arg32Bits[1].i);
    operStack->popInteger((unsigned int &)arg32Bits[0].i);
    arg32Bits[2].i= arg32Bits[0].i * arg32Bits[1].i;
    operStack->pushInteger(arg32Bits[2].i);
    execFrame->incPCfake(JVI_IMUL_SIZE);       // DEBUG.
}


void JVMachine2::I_LMUL(void)
{

    operStack->popLong(&arg64Bits[1]);
    operStack->popLong(&arg64Bits[0]);
    Value64Bits::imult(&arg64Bits[2], &arg64Bits[0], &arg64Bits[1]);
    operStack->pushLong(&arg64Bits[2]);
    execFrame->incPCfake(JVI_LMUL_SIZE);       // DEBUG.
}


void JVMachine2::I_FMUL(void)
{

    operStack->popInteger((unsigned int &)arg32Bits[1].i);
    operStack->popInteger((unsigned int &)arg32Bits[0].i);
    arg32Bits[2].f= arg32Bits[0].f * arg32Bits[1].f;
    operStack->pushInteger(arg32Bits[2].i);
    execFrame->incPCfake(JVI_FMUL_SIZE);       // DEBUG.
}


void JVMachine2::I_DMUL(void)
{

    operStack->popLong(&arg64Bits[1]);
    operStack->popLong(&arg64Bits[0]);
    arg64Bits[2].data.d= arg64Bits[0].data.d * arg64Bits[1].data.d;
    operStack->pushLong(&arg64Bits[2]);
    execFrame->incPCfake(JVI_DMUL_SIZE);       // DEBUG.
}


void JVMachine2::I_IDIV(void)
{

    operStack->popInteger((unsigned int &)arg32Bits[1].i);
    operStack->popInteger((unsigned int &)arg32Bits[0].i);
    if (arg32Bits[1].i == 0) {
	cout << "RT Exception: ArithmeticException.\n";
	raiseException(NULL);
	}
    else {
	arg32Bits[2].i= arg32Bits[0].i / arg32Bits[1].i;
	}
    operStack->pushInteger(arg32Bits[2].i);
    execFrame->incPCfake(JVI_IDIV_SIZE);       // DEBUG.
}


void JVMachine2::I_LDIV(void)
{

    operStack->popLong(&arg64Bits[1]);
    operStack->popLong(&arg64Bits[0]);
    Value64Bits::idiv(&arg64Bits[2], &arg64Bits[0], &arg64Bits[1]);
    operStack->pushLong(&arg64Bits[2]);
    execFrame->incPCfake(JVI_LDIV_SIZE);       // DEBUG.
}


void JVMachine2::I_FDIV(void)
{

    operStack->popInteger((unsigned int &)arg32Bits[1].i);
    operStack->popInteger((unsigned int &)arg32Bits[0].i);
    if (arg32Bits[1].f == 0) {
	arg32Bits[2].f= NAN;
	}
    else {
	arg32Bits[2].f= arg32Bits[0].f / arg32Bits[1].f;
	}
    operStack->pushInteger(arg32Bits[2].i);
    execFrame->incPCfake(JVI_FDIV_SIZE);       // DEBUG.
}


void JVMachine2::I_DDIV(void)
{

    operStack->popLong(&arg64Bits[1]);
    operStack->popLong(&arg64Bits[0]);
    if (arg64Bits[1].data.d== 0) {
	arg64Bits[2].data.d= NAN;
	}
    else {
	arg64Bits[2].data.d= arg64Bits[0].data.d / arg64Bits[1].data.d;
	}
    operStack->pushLong(&arg64Bits[2]);
    execFrame->incPCfake(JVI_DDIV_SIZE);       // DEBUG.
}


void JVMachine2::I_IREM(void)
{

    operStack->popInteger((unsigned int &)arg32Bits[1].i);
    operStack->popInteger((unsigned int &)arg32Bits[0].i);
    arg32Bits[2].i= arg32Bits[0].i % arg32Bits[1].i;
    operStack->pushInteger(arg32Bits[2].i);
    execFrame->incPCfake(JVI_IREM_SIZE);       // DEBUG.
}


void JVMachine2::I_LREM(void)
{

    operStack->popLong(&arg64Bits[1]);
    operStack->popLong(&arg64Bits[0]);
    Value64Bits::idiv(&arg64Bits[2], &arg64Bits[0], &arg64Bits[1]);
    operStack->pushLong(&arg64Bits[2]);
    execFrame->incPCfake(JVI_LREM_SIZE);
}


void JVMachine2::I_FREM(void)
{

    operStack->popInteger((unsigned int &)arg32Bits[1].i);
    operStack->popInteger((unsigned int &)arg32Bits[0].i);
    if (arg32Bits[1].f == 0) {
	arg32Bits[2].f= NAN;
	}
    else {
	arg32Bits[2].f= fmod(arg32Bits[0].f, arg32Bits[1].f);
	}
    operStack->pushInteger(arg32Bits[2].i);
    execFrame->incPCfake(JVI_FREM_SIZE);       // DEBUG.
}


void JVMachine2::I_DREM(void)
{

    operStack->popLong(&arg64Bits[1]);
    operStack->popLong(&arg64Bits[0]);
    if (arg64Bits[1].data.d== 0) {
	arg64Bits[2].data.d= NAN;
	}
    else {
#if defined(_WIN32)
	arg64Bits[2].data.d= fmodl(arg64Bits[0].data.d, arg64Bits[1].data.d);
#else
	arg64Bits[2].data.d= drem(arg64Bits[0].data.d, arg64Bits[1].data.d);
#endif
	}
    operStack->pushLong(&arg64Bits[2]);
    execFrame->incPCfake(JVI_DREM_SIZE);       // DEBUG.
}


void JVMachine2::I_INEG(void)
{

    operStack->popInteger((unsigned int &)arg32Bits[0].i);
    operStack->pushInteger(-arg32Bits[0].i);
    execFrame->incPCfake(JVI_INEG_SIZE);       // DEBUG.
}


void JVMachine2::I_LNEG(void)
{

    operStack->popLong(&arg64Bits[0]);
    Value64Bits::ineg(&arg64Bits[0]);
    operStack->pushLong(&arg64Bits[0]);
    execFrame->incPCfake(JVI_LNEG_SIZE);       // DEBUG.
}


void JVMachine2::I_FNEG(void)
{

    operStack->popInteger((unsigned int &)arg32Bits[0].i);
	arg32Bits[0].f*= -1.0f;	/* ENNT: ajoute le 'f'. */
    operStack->pushInteger(arg32Bits[0].i);
    execFrame->incPCfake(JVI_FNEG_SIZE);       // DEBUG.
}


void JVMachine2::I_DNEG(void)
{

    operStack->popLong(&arg64Bits[0]);
    arg64Bits[0].data.d*= -1;
    operStack->pushLong(&arg64Bits[0]);
    execFrame->incPCfake(JVI_DNEG_SIZE);       // DEBUG.
}


void JVMachine2::I_ISHL(void)
{

    operStack->popInteger((unsigned int &)anInt);
    operStack->popInteger((unsigned int &)arg32Bits[0].i);
    anInt&= 0x01F;
    arg32Bits[0].i<<= anInt;
    operStack->pushInteger(arg32Bits[0].i);
    execFrame->incPCfake(JVI_ISHL_SIZE);       // DEBUG.
}


void JVMachine2::I_ISHR(void)
{

    operStack->popInteger((unsigned int &)anInt);
    operStack->popInteger((unsigned int &)arg32Bits[0].i);
    anInt&= 0x01F;
    arg32Bits[0].i>>= anInt;
    operStack->pushInteger(arg32Bits[0].i);
    execFrame->incPCfake(JVI_ISHR_SIZE);       // DEBUG.
}


void JVMachine2::I_IUSHR(void)
{

    operStack->popInteger((unsigned int &)anInt);
    operStack->popInteger((unsigned int &)anUInt);
    anInt&= 0x01F;
    anUInt>>= anInt;
    operStack->pushInteger(anUInt);
    execFrame->incPCfake(JVI_IUSHR_SIZE);       // DEBUG.
}


void JVMachine2::I_LSHL(void)
{

    operStack->popInteger((unsigned int &)anInt);
    anInt&= 0x03F;
    operStack->popLong(&arg64Bits[0]);
    if (anInt > 31) {
	arg64Bits[0].data.lint.highWord= arg64Bits[0].data.lint.lowWord << (anInt - 32);
    }
    else {
	arg64Bits[0].data.lint.highWord<<= anInt;
	arg64Bits[0].data.lint.highWord|= arg64Bits[0].data.lint.lowWord >> (32 - anInt);
	arg64Bits[0].data.lint.lowWord<<= anInt;
    }
    operStack->pushLong(&arg64Bits[0]);
    execFrame->incPCfake(JVI_LSHL_SIZE);       // DEBUG.
}


void JVMachine2::I_LSHR(void)
{

    operStack->popInteger((unsigned int &)anInt);
    anInt&= 0x03F;
    operStack->popLong(&arg64Bits[0]);
    if (anInt > 31) {
	if (arg64Bits[0].data.lint.highWord & (1 << 31)) {
	    arg64Bits[0].data.lint.lowWord= arg64Bits[0].data.lint.highWord >> (anInt - 32);
	    arg64Bits[0].data.lint.lowWord|= ~((1 << (64 - anInt)) - 1);
	}
	else {
	    arg64Bits[0].data.lint.lowWord= arg64Bits[0].data.lint.highWord >> (anInt - 32);
	}
    }
    else {
	if (arg64Bits[0].data.lint.highWord & (1 << 31)) {
	    arg64Bits[0].data.lint.lowWord>>= anInt;
	    arg64Bits[0].data.lint.lowWord|= arg64Bits[0].data.lint.highWord << (32 - anInt);
	    arg64Bits[0].data.lint.highWord>>= anInt;
	    arg64Bits[0].data.lint.highWord|= ~((1 << (32 - anInt)) - 1);
	}
	else {
	    arg64Bits[0].data.lint.lowWord>>= anInt;
	    arg64Bits[0].data.lint.lowWord|= arg64Bits[0].data.lint.highWord << (32 - anInt);
	    arg64Bits[0].data.lint.highWord>>= anInt;
	}
    }
    operStack->pushLong(&arg64Bits[0]);
    execFrame->incPCfake(JVI_LSHR_SIZE);       // DEBUG.
}


void JVMachine2::I_LUSHR(void)
{

    anInt&= 0x03F;
    operStack->popLong(&arg64Bits[0]);
    if (anInt > 31) {
	arg64Bits[0].data.lint.lowWord= arg64Bits[0].data.lint.highWord >> (anInt - 32);
    }
    else {
	arg64Bits[0].data.lint.lowWord>>= anInt;
	arg64Bits[0].data.lint.lowWord|= arg64Bits[0].data.lint.highWord << (32 - anInt);
	arg64Bits[0].data.lint.highWord>>= anInt;
    }
    operStack->pushLong(&arg64Bits[0]);
    execFrame->incPCfake(JVI_LUSHR_SIZE);       // DEBUG.
}


void JVMachine2::I_IAND(void)
{

    operStack->popInteger((unsigned int &)arg32Bits[0].i);
    operStack->popInteger((unsigned int &)arg32Bits[1].i);
    arg32Bits[2].i= arg32Bits[0].i & arg32Bits[1].i;
    operStack->pushInteger(arg32Bits[2].i);
    execFrame->incPCfake(JVI_IAND_SIZE);
}


void JVMachine2::I_LAND(void)
{

    operStack->popLong(&arg64Bits[1]);
    operStack->popLong(&arg64Bits[0]);
    arg64Bits[2].data.lint.highWord= arg64Bits[0].data.lint.highWord & arg64Bits[1].data.lint.highWord;
    arg64Bits[2].data.lint.lowWord= arg64Bits[0].data.lint.lowWord & arg64Bits[1].data.lint.lowWord;
    operStack->pushLong(&arg64Bits[2]);
    execFrame->incPCfake(JVI_LAND_SIZE);       // DEBUG.
}


void JVMachine2::I_IOR(void)
{

    operStack->popInteger((unsigned int &)arg32Bits[0].i);
    operStack->popInteger((unsigned int &)arg32Bits[1].i);
    arg32Bits[2].i= arg32Bits[0].i | arg32Bits[1].i;
    operStack->pushInteger(arg32Bits[2].i);
    execFrame->incPCfake(JVI_IOR_SIZE);       // DEBUG.
}


void JVMachine2::I_LOR(void)
{

    operStack->popLong(&arg64Bits[1]);
    operStack->popLong(&arg64Bits[0]);
    arg64Bits[2].data.lint.highWord= arg64Bits[0].data.lint.highWord | arg64Bits[1].data.lint.highWord;
    arg64Bits[2].data.lint.lowWord= arg64Bits[0].data.lint.lowWord | arg64Bits[1].data.lint.lowWord;
    operStack->pushLong(&arg64Bits[2]);
    execFrame->incPCfake(JVI_LOR_SIZE);       // DEBUG.
}


void JVMachine2::I_IXOR(void)
{

    operStack->popInteger((unsigned int &)arg32Bits[0].i);
    operStack->popInteger((unsigned int &)arg32Bits[1].i);
    arg32Bits[2].i= arg32Bits[0].i ^ arg32Bits[1].i;
    operStack->pushInteger(arg32Bits[2].i);
    execFrame->incPCfake(JVI_IXOR_SIZE);       // DEBUG.
}


void JVMachine2::I_LXOR(void)
{

    operStack->popLong(&arg64Bits[1]);
    operStack->popLong(&arg64Bits[0]);
    arg64Bits[2].data.lint.highWord= arg64Bits[0].data.lint.highWord ^ arg64Bits[1].data.lint.highWord;
    arg64Bits[2].data.lint.lowWord= arg64Bits[0].data.lint.lowWord ^ arg64Bits[1].data.lint.lowWord;
    operStack->pushLong(&arg64Bits[2]);
    execFrame->incPCfake(JVI_LXOR_SIZE);       // DEBUG.
}


void JVMachine2::I_I2L(void)
{

    operStack->popInteger((unsigned int &)arg32Bits[0].i);
    arg64Bits[0].data.lint.lowWord= arg32Bits[0].i;
    arg64Bits[0].data.lint.highWord= ((arg32Bits[0].i & (1 << 31)) ? 0x0FFFFFFFF : 0);
    operStack->pushLong(&arg64Bits[0]);
    execFrame->incPCfake(JVI_I2L_SIZE);       // DEBUG.
}


void JVMachine2::I_I2F(void)
{

    operStack->popInteger((unsigned int &)arg32Bits[0].i);
    arg32Bits[0].f= (float)arg32Bits[0].i;
    operStack->pushInteger(arg32Bits[0].i);
    execFrame->incPCfake(JVI_I2F_SIZE);       // DEBUG.
}


void JVMachine2::I_I2D(void)
{

    operStack->popInteger((unsigned int &)arg32Bits[0].i);
    arg64Bits[0].data.d= (double)arg32Bits[0].i;
    operStack->pushLong(&arg64Bits[0]);
    execFrame->incPCfake(JVI_I2D_SIZE);       // DEBUG.
}


void JVMachine2::I_L2I(void)
{

    operStack->popLong(&arg64Bits[0]);
    operStack->pushInteger(arg64Bits[0].data.lint.lowWord);
    execFrame->incPCfake(JVI_L2I_SIZE);       // DEBUG.
}


void JVMachine2::I_L2F(void)
{

    operStack->popLong(&arg64Bits[0]);
    arg64Bits[1].data.d= (double)arg64Bits[0].data.lint.highWord;
    arg64Bits[1].data.d*= 4294967296.0;	// Rough shift 32 bits.
    arg64Bits[1].data.d+= (double)arg64Bits[0].data.lint.lowWord;
    arg32Bits[0].f= (float)arg64Bits[1].data.d;
    operStack->pushInteger(arg32Bits[0].i);
    execFrame->incPCfake(JVI_L2F_SIZE);       // DEBUG.
}


void JVMachine2::I_L2D(void)
{

    operStack->popLong(&arg64Bits[0]);
    arg64Bits[1].data.d= (double)arg64Bits[0].data.lint.highWord;
    arg64Bits[1].data.d*= 4294967296.0;	// Rough shift 32 bits.
    arg64Bits[1].data.d+= (double)arg64Bits[0].data.lint.lowWord;
    operStack->pushLong(&arg64Bits[1]);
    execFrame->incPCfake(JVI_L2D_SIZE);       // DEBUG.
}


void JVMachine2::I_F2I(void)
{

    operStack->popInteger((unsigned int &)arg32Bits[0].i);
    arg32Bits[0].i= (int)arg32Bits[0].f;
    operStack->pushInteger(arg32Bits[0].i);
    execFrame->incPCfake(JVI_F2I_SIZE);       // DEBUG.
}


void JVMachine2::I_F2L(void)
{

    operStack->popInteger((unsigned int &)arg32Bits[0].i);
    Value64Bits::toLong(&arg64Bits[0], arg32Bits[0].f);
    operStack->pushLong(&arg64Bits[0]);
    execFrame->incPCfake(JVI_F2L_SIZE);       // DEBUG.
}


void JVMachine2::I_F2D(void)
{

    operStack->popInteger((unsigned int &)arg32Bits[0].i);
    arg64Bits[0].data.d= (double)arg32Bits[0].f;
    operStack->pushLong(&arg64Bits[0]);
#if defined(DBGLEVEL_MATH)
    cout << "JVMachine: F2D: " << arg32Bits[0].f << " ==> " << arg64Bits[0].data.d << ".\n";
#endif
    execFrame->incPCfake(JVI_F2D_SIZE);       // DEBUG.
}


void JVMachine2::I_D2I(void)
{

    operStack->popLong(&arg64Bits[0]);
    arg32Bits[0].i= (int)arg64Bits[0].data.d;
    operStack->pushInteger(arg32Bits[0].i);
    execFrame->incPCfake(JVI_D2I_SIZE);       // DEBUG.
}


void JVMachine2::I_D2L(void)
{

    operStack->popLong(&arg64Bits[0]);
    Value64Bits::toLong(&arg64Bits[1], &arg64Bits[0]);
    operStack->pushLong(&arg64Bits[0]);
    execFrame->incPC(JVI_D2L_SIZE - 1);
}


void JVMachine2::I_D2F(void)
{

    operStack->popLong(&arg64Bits[0]);
    arg32Bits[0].f= (float)arg64Bits[0].data.d;
    operStack->pushInteger(arg32Bits[0].i);
    execFrame->incPCfake(JVI_D2F_SIZE);       // DEBUG.
}


void JVMachine2::I_INT2BYTE(void)
{

    operStack->popInteger((unsigned int &)anInt);
    signedByte= (char)anInt;
    operStack->pushInteger((unsigned int)signedByte);
    execFrame->incPCfake(JVI_INT2BYTE_SIZE);
}


void JVMachine2::I_INT2CHAR(void)
{

    operStack->popInteger((unsigned int &)anInt);
    operStack->pushInteger((unsigned short)anInt);
    execFrame->incPCfake(JVI_INT2CHAR_SIZE);
}


void JVMachine2::I_INT2SHORT(void)
{

    operStack->popInteger((unsigned int &)anInt);
    operStack->pushInteger((short)anInt);
    execFrame->incPCfake(JVI_INT2SHORT_SIZE);
}


void JVMachine2::I_IFEQ(void)
{

    aShort= execFrame->fetchShort();
    operStack->popInteger((unsigned int &)anInt);
    if (anInt == 0) execFrame->incPC(aShort - JVI_IFEQ_SIZE);
    else execFrame->incPCfake(JVI_IFEQ_SIZE);       // DEBUG.
}


void JVMachine2::I_IFNULL(void)
{

    aShort= execFrame->fetchShort();
    operStack->pop(genValue);
    if (genValue.value == JVAbstractInstance::nullObject) execFrame->incPC(aShort - JVI_IFNULL_SIZE);
    else execFrame->incPCfake(JVI_IFNULL_SIZE);       // DEBUG.
}


void JVMachine2::I_IFLT(void)
{

    aShort= execFrame->fetchShort();
    operStack->popInteger((unsigned int &)anInt);
    if (anInt < 0) execFrame->incPC(aShort - JVI_IFLT_SIZE);
    else execFrame->incPCfake(JVI_IFLT_SIZE);       // DEBUG.
}


void JVMachine2::I_IFLE(void)
{

    aShort= execFrame->fetchShort();
    operStack->popInteger((unsigned int &)anInt);
    if (anInt <= 0) execFrame->incPC(aShort - JVI_IFLE_SIZE);
    else execFrame->incPCfake(JVI_IFLE_SIZE);       // DEBUG.
}


void JVMachine2::I_IFNE(void)
{

    aShort= execFrame->fetchShort();
    operStack->popInteger((unsigned int &)anInt);
    if (anInt != 0) execFrame->incPC(aShort - JVI_IFNE_SIZE);
    else execFrame->incPCfake(JVI_IFNE_SIZE);       // DEBUG.
}


void JVMachine2::I_IFNONNULL(void)
{

    aShort= execFrame->fetchShort();
    operStack->pop(genValue);
    if (genValue.value != JVAbstractInstance::nullObject) execFrame->incPC(aShort - JVI_IFNONNULL_SIZE);
    else execFrame->incPCfake(JVI_IFNONNULL_SIZE);       // DEBUG.
}


void JVMachine2::I_IFGT(void)
{

    aShort= execFrame->fetchShort();
    operStack->popInteger((unsigned int &)anInt);
    if (anInt > 0) execFrame->incPC(aShort - JVI_IFGT_SIZE);
    else execFrame->incPCfake(JVI_IFGT_SIZE);       // DEBUG.
}


void JVMachine2::I_IFGE(void)
{

    aShort= execFrame->fetchShort();
    operStack->popInteger((unsigned int &)anInt);
    if (anInt >= 0) execFrame->incPC(aShort - JVI_IFGE_SIZE);
    else execFrame->incPCfake(JVI_IFGE_SIZE);       // DEBUG.
}


void JVMachine2::I_IF_ICMPEQ(void)
{

    operStack->popInteger((unsigned int &)arg32Bits[0].i);
    operStack->popInteger((unsigned int &)arg32Bits[1].i);
    aShort= execFrame->fetchShort();
    if (arg32Bits[0].i == arg32Bits[1].i) execFrame->incPC(aShort - JVI_IF_ICMPEQ_SIZE);
    else  execFrame->incPCfake(JVI_IF_ICMPEQ_SIZE);       // DEBUG.
}


void JVMachine2::I_IF_ICMPNE(void)
{

    operStack->popInteger((unsigned int &)arg32Bits[0].i);
    operStack->popInteger((unsigned int &)arg32Bits[1].i);
    aShort= execFrame->fetchShort();
    if (arg32Bits[0].i != arg32Bits[1].i) execFrame->incPC(aShort - JVI_IF_ICMPNE_SIZE);
    else execFrame->incPCfake(JVI_IF_ICMPNE_SIZE);       // DEBUG.
}


void JVMachine2::I_IF_ICMPLT(void)
{

    operStack->popInteger((unsigned int &)arg32Bits[1].i);
    operStack->popInteger((unsigned int &)arg32Bits[0].i);
    aShort= execFrame->fetchShort();
    if (arg32Bits[0].i < arg32Bits[1].i) execFrame->incPC(aShort - JVI_IF_ICMPLT_SIZE);
    else execFrame->incPCfake(JVI_IF_ICMPLT_SIZE);       // DEBUG.
}


void JVMachine2::I_IF_ICMPGT(void)
{

    operStack->popInteger((unsigned int &)arg32Bits[1].i);
    operStack->popInteger((unsigned int &)arg32Bits[0].i);
    aShort= execFrame->fetchShort();
    if (arg32Bits[0].i > arg32Bits[1].i) execFrame->incPC(aShort - JVI_IF_ICMPGT_SIZE);
    else execFrame->incPCfake(JVI_IF_ICMPGT_SIZE);       // DEBUG.
}


void JVMachine2::I_IF_ICMPLE(void)
{

    operStack->popInteger((unsigned int &)arg32Bits[1].i);
    operStack->popInteger((unsigned int &)arg32Bits[0].i);
    aShort= execFrame->fetchShort();
    if (arg32Bits[0].i <= arg32Bits[1].i) execFrame->incPC(aShort - JVI_IF_ICMPLE_SIZE);
    else execFrame->incPCfake(JVI_IF_ICMPLE_SIZE);       // DEBUG.
}


void JVMachine2::I_IF_ICMPGE(void)
{

    operStack->popInteger((unsigned int &)arg32Bits[1].i);
    operStack->popInteger((unsigned int &)arg32Bits[0].i);
    aShort= execFrame->fetchShort();
    if (arg32Bits[0].i >= arg32Bits[1].i) execFrame->incPC(aShort - JVI_IF_ICMPGE_SIZE);
    else execFrame->incPCfake(JVI_IF_ICMPGE_SIZE);       // DEBUG.
}


void JVMachine2::I_LCMP(void)
{

    operStack->popLong(&arg64Bits[1]);
    operStack->popLong(&arg64Bits[0]);
    anUInt= Value64Bits::lcmp(&arg64Bits[0], &arg64Bits[1]);
    operStack->pushInteger(anUInt);
    execFrame->incPCfake(JVI_LCMP_SIZE);       // DEBUG.
}


void JVMachine2::I_FCMPL(void)
{

    operStack->popInteger((unsigned int &)arg32Bits[1].i);
    operStack->popInteger((unsigned int &)arg32Bits[0].i);
#if defined(DBGLEVEL_MATH)
    cout << "JVMachine: FCMPL: " << arg32Bits[0].f << " < " << arg32Bits[1].f << ".\n";
#endif
#if defined(MSDOS)
    if (isnanf(arg32Bits[0].f) || isnanf(arg32Bits[1].f)) {
#else
    if ((arg32Bits[0].f == NAN) || (arg32Bits[1].f == NAN)) {
#endif
	operStack->pushInteger((unsigned int)-1);
	}
    else {
	if (arg32Bits[0].f > arg32Bits[1].f ) operStack->pushInteger(1);
	else if (arg32Bits[0].f < arg32Bits[1].f ) operStack->pushInteger((unsigned int)-1);
	else operStack->pushInteger(0);
	}
    execFrame->incPCfake(JVI_FCMPL_SIZE);       // DEBUG.
}


void JVMachine2::I_FCMPG(void)
{

    operStack->popInteger((unsigned int &)arg32Bits[1].i);
    operStack->popInteger((unsigned int &)arg32Bits[0].i);
#if defined(DBGLEVEL_MATH)
    cout << "JVMachine: FCMPG " << arg32Bits[0].f << " > " << arg32Bits[1].f << ".\n";
#endif
#if defined(MSDOS)
    if (isnanf(arg32Bits[0].f) || isnanf(arg32Bits[1].f)) {
#else
    if ((arg32Bits[0].f == NAN) || (arg32Bits[1].f == NAN)) {
#endif
	operStack->pushInteger(1);
	}
    else {
	if (arg32Bits[0].f > arg32Bits[1].f ) operStack->pushInteger(1);
	else if (arg32Bits[0].f < arg32Bits[1].f ) operStack->pushInteger((unsigned int)-1);
	else operStack->pushInteger(0);
	}
    execFrame->incPCfake(JVI_FCMPG_SIZE);       // DEBUG.
}


void JVMachine2::I_DCMPL(void)
{

    operStack->popLong(&arg64Bits[1]);
    operStack->popLong(&arg64Bits[0]);
#if defined(DBGLEVEL_MATH)
    cout << "JVMachine: DCMPL " << arg64Bits[0].data.d << " < " << arg64Bits[1].data.d << ".\n";
#endif
#if defined(MSDOS)
    if (isnan(arg64Bits[0].data.d) || isnan(arg64Bits[1].data.d)) {
#else
    if ((arg64Bits[0].data.d== NAN) || (arg64Bits[1].data.d == NAN)) {
#endif
	operStack->pushInteger((unsigned int)-1);
	}
    else {
	if (arg64Bits[0].data.d > arg64Bits[1].data.d) operStack->pushInteger(1);
	else if (arg64Bits[0].data.d < arg64Bits[1].data.d ) operStack->pushInteger((unsigned int)-1);
	else operStack->pushInteger(0);
	}
    execFrame->incPCfake(JVI_DCMPL_SIZE);       // DEBUG.
}


void JVMachine2::I_DCMPG(void)
{

    operStack->popLong(&arg64Bits[1]);
    operStack->popLong(&arg64Bits[0]);
#if defined(DBGLEVEL_MATH)
    cout << "JVMachine: DCMPG " << arg64Bits[0].data.d << " > " << arg64Bits[1].data.d << ".\n";
#endif
#if defined(MSDOS)
    if (isnan(arg64Bits[0].data.d) || isnan(arg64Bits[1].data.d)) {
#else
    if ((arg64Bits[0].data.d== NAN) || (arg64Bits[1].data.d == NAN)) {
#endif
	operStack->pushInteger(1);
	}
    else {
	if (arg64Bits[0].data.d > arg64Bits[1].data.d) operStack->pushInteger(1);
	else if (arg64Bits[0].data.d < arg64Bits[1].data.d ) operStack->pushInteger((unsigned int)-1);
	else operStack->pushInteger(0);
	}
    execFrame->incPCfake(JVI_DCMPG_SIZE);       // DEBUG.
}


void JVMachine2::I_IF_ACMPEQ(void)
{

    operStack->pop(genValue);
    operStack->pop(genValue2);
    aShort= execFrame->fetchShort();
    if (genValue.value == genValue2.value) execFrame->incPC(aShort - JVI_IF_ACMPEQ_SIZE);
    else execFrame->incPCfake(JVI_IF_ACMPEQ_SIZE);       // DEBUG.
}


void JVMachine2::I_IF_ACMPNE(void)
{

    operStack->pop(genValue);
    operStack->pop(genValue2);
    aShort= execFrame->fetchShort();
    if (genValue.value != genValue2.value) execFrame->incPC(aShort - JVI_IF_ACMPNE_SIZE);
    else execFrame->incPCfake(JVI_IF_ACMPNE_SIZE);       // DEBUG.
}


void JVMachine2::I_GOTO(void)
{

    aShort= execFrame->fetchShort();
    execFrame->incPCfake(JVI_GOTO_SIZE);       // DEBUG.
    execFrame->incPC(aShort - JVI_GOTO_SIZE);
}


void JVMachine2::I_GOTO_W(void)
{

    anInt= (int)execFrame->fetchInteger();
    execFrame->incPCfake(JVI_GOTO_W_SIZE);       // DEBUG.
    execFrame->incPC(anInt - JVI_GOTO_W_SIZE);
}


void JVMachine2::I_JSR(void)
{

    aShort= execFrame->fetchShort();
    operStack->pushInteger(execFrame->currentPC());
    execFrame->incPC(aShort - JVI_JSR_SIZE);
}


void JVMachine2::I_JSR_W(void)
{

    anInt= (int)execFrame->fetchShort();
    operStack->pushInteger(execFrame->currentPC());
    execFrame->incPC(anInt - JVI_JSR_W_SIZE);
}


void JVMachine2::I_RET(void)
{

    vindex|= execFrame->fetchByte();
    execFrame->incPCfake(JVI_RET_SIZE);       // DEBUG.
    execFrame->setCurrentPC((unsigned int)execFrame->localValues[vindex]->value);
    vindex= 0;
}


void JVMachine2::I_RET_W(void)
{

    vindex= (unsigned short)execFrame->fetchShort();
    execFrame->incPCfake(JVI_RET_W_SIZE);       // DEBUG.
    execFrame->setCurrentPC((unsigned int)execFrame->localValues[vindex]->value);
    vindex= 0;
}


void JVMachine2::I_IRETURN(void)
{

    execFrame->setDead();
    operStack->popInteger(anUInt);
    popExecFrame(anUInt);
//	    execFrame->incPCfake(JVI_IRETURN_SIZE);       // DEBUG.
}


void JVMachine2::I_LRETURN(void)
{

    execFrame->setDead();
    operStack->popLong(&arg64Bits[0]);
    popExecFrame(&arg64Bits[0]);
    execFrame->incPC(JVI_LRETURN_SIZE - 1);
}


void JVMachine2::I_FRETURN(void)
{

    execFrame->setDead();
    operStack->popInteger(anUInt);
    popExecFrame(anUInt);
//	    execFrame->incPCfake(JVI_FRETURN_SIZE);       // DEBUG.
}


void JVMachine2::I_DRETURN(void)
{

    execFrame->setDead();
    operStack->popLong(&arg64Bits[0]);
    popExecFrame(&arg64Bits[0]);
    execFrame->incPC(JVI_DRETURN_SIZE - 1);
}


void JVMachine2::I_ARETURN(void)
{

    execFrame->setDead();
    operStack->pop(genValue);
    popExecFrame(genValue);
//	    execFrame->incPCfake(JVI_ARETURN_SIZE);       // DEBUG.
}


void JVMachine2::I_RETURN(void)
{

// ATTENTION: Weak implementation.
    execFrame->setDead();
    popExecFrame();
//	    execFrame->incPCfake(JVI_RETURN_SIZE);       // DEBUG.
}


void JVMachine2::I_BREAKPOINT(void)
{

    execFrame->incPC(JVI_BREAKPOINT_SIZE - 1);
}


void JVMachine2::I_TABLESWITCH(void)
{

    anUInt= (tmpPCHolder= execFrame->currentPC()) % 4;
    tmpPCHolder--;
    if (anUInt != 0) {
	execFrame->incPC(4-anUInt);
	}
    arg32Bits[0].i= execFrame->fetchInteger();    // Default offset.
    arg32Bits[1].i= execFrame->fetchInteger();    // Low.
    arg32Bits[2].i= execFrame->fetchInteger();    // High
    operStack->popInteger((unsigned int &)anInt);			    // Index of jump offset.
    if ((anInt < arg32Bits[1].i) || (anInt > arg32Bits[2].i)) {
	execFrame->setCurrentPC(tmpPCHolder);
	execFrame->incPC(arg32Bits[0].i);
	}
    else {
	anInt-= arg32Bits[1].i;
	execFrame->incPC(anInt * 4);
	arg32Bits[0].i= execFrame->fetchInteger();
	execFrame->setCurrentPC(tmpPCHolder);
	execFrame->incPC(arg32Bits[0].i);
	}
//	    execFrame->incPCfake(JVI_TABLESWITCH_SIZE);       // DEBUG.
}


void JVMachine2::I_LOOKUPSWITCH(void)
{

    anUInt= (tmpPCHolder= execFrame->currentPC()) % 4;
    tmpPCHolder--;
    if (anUInt != 0) {
	execFrame->incPC(4-anUInt);
	}
    arg32Bits[0].i= execFrame->fetchInteger();    // Default offset.
    anUInt= execFrame->fetchInteger();    	// Nbr of pairs.
    operStack->popInteger((unsigned int &)arg32Bits[1].i);    // Key.
    for (i= 0; i < anUInt; i++) {	// MOD-970909 [HD]: replaced anInt by anUInt.
	arg32Bits[2].i= execFrame->fetchInteger();    // A match-key.
	anInt= execFrame->fetchInteger();    // Fetch offset.
	if (arg32Bits[1].i == arg32Bits[2].i) {
	    execFrame->setCurrentPC(tmpPCHolder);
	    execFrame->incPC(anInt);
	    break;
	}
    }
    if (i == anUInt) {
	execFrame->setCurrentPC(tmpPCHolder);
	execFrame->incPC(arg32Bits[0].i);
	}
//	    execFrame->incPCfake(JVI_LOOKUPSWITCH_SIZE);       // DEBUG.
}


void JVMachine2::I_PUTFIELD(void)
{

    anUInt= (unsigned int)execFrame->fetchShort();
    aField= (FieldInfo *)((CteNameAndType *)
	execFrame->constantPool[anUInt])->resolve(kernel->getClassManager(),
			    execFrame->currentClass);
    if (aField != NULL) {
	fieldDesc= aField->resolve(kernel->getClassManager());
	if (fieldDesc->isStatic()) {
	    cout << "RT Exception: IncompatibleClassChangeError.\n";
	    raiseException(NULL);
	    }
	else if (fieldDesc->isFinal() && false) {  // MOD-970628 [HD]: Mis temporairement hors d'etat de nuire, doit utiliser le check sur 'init'.
	    cout << "RT Exception: Assignment of final field.\n";
	    raiseException(NULL);
	    }
	else {
// ATTENTION: Does not work for many situations (8 bytes value, for example).
	    operStack->pop(genValue);	// Pop value to store.
	    if (genValue.metaType & TYPE_BASIC_64L) {
		operStack->pop(genValue2);	// Pop second component.
		arg64Bits[0].data.lint.lowWord= (unsigned int)genValue.value;
		arg64Bits[0].data.lint.highWord= (unsigned int)genValue2.value;
	    }
	    operStack->pop(genValue2);	// Pop receiver.
	    if (genValue2.metaType != TYPE_INSTANCE) {
		cout << "RT Exception: PutfieldNotOnObject.\n";
		raiseException(NULL);
		}
	    else if (genValue2.value != JVAbstractInstance::nullObject) {
		anInstance= (JVInstance *)genValue2.value;
		if (genValue.metaType & TYPE_BASIC_64L) {
		    if (!anInstance->putField(&arg64Bits[0], fieldDesc)) {
			cout << "RT Exception: Can't put data into field.\n";
			raiseException(NULL);
		    }
		}
		else {
		    if (!anInstance->putField(genValue, fieldDesc)) {
			cout << "RT Exception: Can't put data into field.\n";
			raiseException(NULL);
		    }
		}
		}
	    else {
		cout << "RT Exception: NullPointerException.\n";
		raiseException(NULL);
		}
	    }
	}
    execFrame->incPCfake(JVI_PUTFIELD_SIZE);       // DEBUG.
}


void JVMachine2::I_GETFIELD(void)
{

    anUInt= (unsigned int)execFrame->fetchShort();
    aField= (FieldInfo *)((CteNameAndType *)
	execFrame->constantPool[anUInt])->resolve(kernel->getClassManager(),
			    execFrame->currentClass);
    if (aField != NULL) {
	fieldDesc= aField->resolve(kernel->getClassManager());
	if (fieldDesc->isStatic()) {
	    cout << "RT Exception: IncompatibleClassChangeError.\n";
	    raiseException(NULL);
	    }
	else {
	    operStack->pop(genValue);
	    if (genValue.metaType != TYPE_INSTANCE) {
		cout << "RT Exception: GetfieldNotOnObject.\n";
		raiseException(NULL);
		}
	    else if (genValue.value != JVAbstractInstance::nullObject) {
		JVAbstractInstance *anAbstractInstance;
		anAbstractInstance= ((JVInstance *)genValue.value)->getField(fieldDesc);
		if (anAbstractInstance == NULL) {
		    if (fieldDesc->metaType() == TYPE_BASIC) {
			operStack->pushInteger(0);
			}
		    else if (fieldDesc->metaType() == TYPE_BASELONG) {
			arg64Bits[0].data.lint.lowWord= arg64Bits[0].data.lint.highWord= 0;
			operStack->pushLong(&arg64Bits[0]);
		    }
		    else {
			genValue.metaType= TYPE_INSTANCE;
			genValue.value= (void *)JVAbstractInstance::nullObject;
			operStack->push(genValue);
			}
		    }
		else {
// ATTENTION: Must consolidate the type meanings.
		    if (fieldDesc->metaType() == TYPE_CLASS) {    // Must implement a better way of doing this.
			genValue.metaType= TYPE_INSTANCE;
			genValue.value= (void *)anAbstractInstance;
			operStack->push(genValue);
			}
		    else if (fieldDesc->metaType() == TYPE_BASIC) {
			operStack->pushInteger((unsigned int)anAbstractInstance);
		    }
		    else if (fieldDesc->metaType() == TYPE_BASELONG) {
			operStack->pushLong((Value64Bits *)anAbstractInstance);
		    }
		    else if (fieldDesc->metaType() == TYPE_ARRAY) {
			genValue.metaType= TYPE_ARRAY | fieldDesc->detailedType();
			genValue.value= (void *)anAbstractInstance;
			operStack->push(genValue);
			}
		    else {		// Unknown type, internal error.
			cout << "RT Exception: Unknown data type.\n";
			raiseException(NULL);
			}
		    }
		}
	    else {
		cout << "RT Exception: NullPointerException.\n";
		raiseException(NULL);
		}
	    }
	}
    execFrame->incPCfake(JVI_GETFIELD_SIZE);    // DEBUG.
}


void JVMachine2::I_PUTSTATIC(void)
{

    anUInt= (unsigned int)execFrame->fetchShort();
    aField= (FieldInfo *)((CteNameAndType *)
	execFrame->constantPool[anUInt])->resolve(kernel->getClassManager(),
			    execFrame->currentClass);
    if (aField != NULL) {
	fieldDesc= aField->resolve(kernel->getClassManager());
	if (fieldDesc->isFinal() && !execFrame->isInitializer()) {
	    cout << "RT Exception: Assignment of final field.\n";
	    raiseException(NULL);
	    }
	else if (fieldDesc->isStatic()) {
	    operStack->pop(genValue);	// Pop value to store.
	    if (genValue.metaType & TYPE_BASIC_64L) {
		operStack->pop(genValue2);	// Pop second component.
		arg64Bits[0].data.lint.lowWord= (unsigned int)genValue.value;
		arg64Bits[0].data.lint.highWord= (unsigned int)genValue2.value;
		if (!fieldDesc->setStaticValue(&arg64Bits[0])) {
		    cout << "RT Exception: Can't put data into field.\n";
		    raiseException(NULL);
		}
	    }
	    else {
		if (!fieldDesc->setStaticValue((JVInstance *)genValue.value)) {
		    cout << "RT Exception: Can't put data into field.\n";
		    raiseException(NULL);
		}
	    }
	}
	else {
	    cout << "RT Exception: IncompatibleClassChangeError.\n";
	    raiseException(NULL);
	    }
	}
	else {
	cout << "RT Exception: No Such Field.\n";
	raiseException(NULL);
	}
    execFrame->incPCfake(JVI_PUTSTATIC_SIZE);    // DEBUG.
}


void JVMachine2::I_GETSTATIC(void)
{

    anUInt= (unsigned int)execFrame->fetchShort();
    aField= (FieldInfo *)((CteNameAndType *)
	execFrame->constantPool[anUInt])->resolve(kernel->getClassManager(),
			    execFrame->currentClass);
    if (aField != NULL) {
	fieldDesc= aField->resolve(kernel->getClassManager());
	if (fieldDesc->isStatic()) {
	    anInstance= fieldDesc->getStaticValue();
	    if (anInstance == NULL) {
		genValue.metaType= TYPE_INSTANCE;
		genValue.value= (void *)JVAbstractInstance::nullObject;
		operStack->push(genValue);
		}
	    else {
// ATTENTION: Must consolidate the type meanings.
		if (fieldDesc->metaType() == TYPE_CLASS) {    // Must implement a better way of doing this.
		    genValue.metaType= TYPE_INSTANCE;
		    genValue.value= (void *)anInstance;
		    operStack->push(genValue);
		    }
		else if (fieldDesc->metaType() == TYPE_BASIC) {
		    operStack->pushInteger((unsigned int)anInstance);
		}
		else if (fieldDesc->metaType() == TYPE_BASELONG) {
		    operStack->pushLong((Value64Bits *)anInstance);
		    }
		else if (fieldDesc->metaType() == TYPE_ARRAY) {
		    genValue.metaType= TYPE_ARRAY | fieldDesc->detailedType();
		    genValue.value= (void *)anInstance;
		    operStack->push(genValue);
		    }
		else {		// Unknown type, internal error.
		    cout << "RT Exception: Unknown data type.\n";
		    raiseException(NULL);
		    }
		}
	    }
	else {	// Can't find the field !
	    cout << "RT Exception: IncompatibleClassChangeError.\n";
	    raiseException(NULL);
	    }
	}
	else {
	cout << "RT Exception: No Such Field.\n";
	raiseException(NULL);
	}
    execFrame->incPCfake(JVI_GETSTATIC_SIZE);    // DEBUG.
}


void JVMachine2::I_INVOKEVIRTUAL(void)
{

    aShort= execFrame->fetchShort();
	// First, we work on the assumtion that we have the method at 'supposed class level'.
    aMethod= (MethodInfo *)execFrame->constantPool[aShort]->resolve(kernel->getClassManager(), execFrame->currentClass);
#if defined(DBGLEVEL_1)
    if (traceLevel & DebugInfo::methodLookup)
	    execFrame->constantPool[aShort]->describe(execFrame->constantPool);
#endif
    if (aMethod) {
	nextMethod= aMethod->resolve(kernel->getClassManager());

		// If we got some info about that method, we build a frame.
		// In doing that, we get the receiver object, and will then re-look the method there.
	if (nextMethod) {
	    newExecFrame= new JVExecFrame(nextMethod, aMethod->getOwner());
	    newExecFrame->loadArguments(operStack);
	    if ((newExecFrame->localValues[0]->metaType == TYPE_INSTANCE)
			|| (newExecFrame->localValues[0]->metaType & TYPE_ARRAY)) {
		if (((JVInstance *)newExecFrame->localValues[0]->value) != JVAbstractInstance::nullObject) {
		    aClass= ((JVInstance *)newExecFrame->localValues[0]->value)->category();
			// We look for the same method 'name' implemented in a lower class.
		    nextMethod= aClass->getVirtualMethod(kernel->getClassManager(), nextMethod);
			// Finally, we revise the exec frame to match the method to actually use.
		    if (newExecFrame->updateVirtualMethod(nextMethod)) {
			pushExecFrame(newExecFrame);
			if (nextMethod->isNative()) {
			    if (nextMethod->returnsVoid()) {

// MOD-970521 [HD]: Took care of the result of the native call.
				if (((JVNativeMethod *)nextMethod)->doNativeCall(this, newExecFrame)) {
				    cout << "RT Exception: Unimplemented native call.\n";
				    raiseException(NULL);
				}
				popExecFrame();
			    }
			    else {
				if (((JVNativeMethod *)nextMethod)->doNativeCall(this, newExecFrame)) {
				    cout << "RT Exception: Unimplemented native call.\n";
				    raiseException(NULL);
					// ATTN-970909 [HD]: The exception handler should take care of dead frames.
				    popExecFrame(genValue);
				}
				else {
				    if (newExecFrame->hasLongValue()) {
					newExecFrame->getReturnValue(&arg64Bits[0]);
					popExecFrame(&arg64Bits[0]);
				    }
				    else {
					newExecFrame->getReturnValue(genValue);
					popExecFrame(genValue);
				    }
				}
			    }
			}
			}
		    else {
			cout << "RT Exception: Cant match concrete method with virtual definition.\n";
			raiseException(NULL);
			delete newExecFrame;
			}
		    }
		else {
		    cout << "RT Exception: NullPointerException.\n";
		    raiseException(NULL);
		    delete newExecFrame;
		    }
		}
	    else {
		cout << "RT Exception: MethodNotOnObject.\n";
		raiseException(NULL);
		delete newExecFrame;
		}

	    }
	else {
	    cout << "RT Exception: Cant resolve the method.\n";
	    raiseException(NULL);
	    }
	}
    else {
	cout << "RT Exception: No Such Method.\n";
	raiseException(NULL);
	}
    
//	    execFrame->incPCfake(JVI_INVOKEVIRTUAL_SIZE);       // Debug.
}


void JVMachine2::I_INVOKENONVIRTUAL(void)
{

    aShort= execFrame->fetchShort();
    aMethod= (MethodInfo *)execFrame->constantPool[aShort]->resolve(kernel->getClassManager(), execFrame->currentClass);
#if defined(DBGLEVEL_1)
    if (traceLevel & DebugInfo::methodLookup)
	    execFrame->constantPool[aShort]->describe(execFrame->constantPool);
#endif
    if (aMethod) {

	nextMethod= aMethod->resolve(kernel->getClassManager());
	if (nextMethod) {


	    newExecFrame= new JVExecFrame(nextMethod, aMethod->getOwner());
	    newExecFrame->loadArguments(operStack);
// ATTN: Must we base ourselves on localValues[0] to check receiver type ( ?).
	    if ((newExecFrame->localValues[0]->metaType == TYPE_INSTANCE)
			|| (newExecFrame->localValues[0]->metaType & TYPE_ARRAY)) {
		if (((JVInstance *)newExecFrame->localValues[0]->value) != JVAbstractInstance::nullObject) {
		    if (nextMethod->isConstructor()) {
			if (!((JVInstance *)newExecFrame->localValues[0]->value)->category()->isStateless()) {
			    if (execFrame->isKindOfLaunch()) {
				newExecFrame->setIgnoreLaunch();
			    }
			    else {
				newExecFrame->setLaunch();
			    }
			}
		    }
		    pushExecFrame(newExecFrame);
		    if (nextMethod->isNative()) {
			if (nextMethod->returnsVoid()) {
			    if (((JVNativeMethod *)nextMethod)->doNativeCall(this, newExecFrame)) {
				cout << "RT Exception: Unimplemented native call.\n";
				raiseException(NULL);
			    }
			    popExecFrame();
			}
			else {
			    if (((JVNativeMethod *)nextMethod)->doNativeCall(this, newExecFrame)) {
				cout << "RT Exception: Unimplemented native call.\n";
				raiseException(NULL);
				    // ATTN-970909 [HD]: The exception handler should take care of dead frames.
				popExecFrame(genValue);
			    }
			    else {
				if (newExecFrame->hasLongValue()) {
				    newExecFrame->getReturnValue(&arg64Bits[0]);
				    popExecFrame(&arg64Bits[0]);
				}
				else {
				    newExecFrame->getReturnValue(genValue);
				    popExecFrame(genValue);
				}
			    }
			}
			}
		    }
		else {
		    cout << "RT Exception: NullPointerException.\n";
		    raiseException(NULL);
		    delete newExecFrame;
		    }
		}
	    else {
		cout << "RT Exception: MethodNotOnObject.\n";
		raiseException(NULL);
		delete newExecFrame;
		}
	    }
	else {
	    cout << "RT Exception: Cant resolve the method.\n";
	    raiseException(NULL);
	    }
	}
    else {
	cout << "RT Exception: No Such Method.\n";
	raiseException(NULL);
	}
//	    execFrame->incPCfake(JVI_INVOKENONVIRTUAL_SIZE);       // DEBUG.
}


void JVMachine2::I_INVOKESTATIC(void)
{


    aShort= execFrame->fetchShort();
    aMethod= (MethodInfo *)execFrame->constantPool[aShort]->resolve(kernel->getClassManager(), execFrame->currentClass);
#if defined(DBGLEVEL_1)
    if (traceLevel & DebugInfo::methodLookup)
	    execFrame->constantPool[aShort]->describe(execFrame->constantPool);
#endif
    if (aMethod) {

	nextMethod= aMethod->resolve(kernel->getClassManager());
	if (nextMethod) {
	    newExecFrame= new JVExecFrame(nextMethod, aMethod->getOwner());
	    newExecFrame->loadArgumentsForStatic(operStack);
	    pushExecFrame(newExecFrame);
	    if (nextMethod->isNative()) {
		if (nextMethod->returnsVoid()) {
		    if (((JVNativeMethod *)nextMethod)->doNativeCall(this, newExecFrame)) {
			cout << "RT Exception: Unimplemented native call.\n";
			raiseException(NULL);
		    }
		    popExecFrame();
		}
		else {
		    if (((JVNativeMethod *)nextMethod)->doNativeCall(this, newExecFrame)) {
			cout << "RT Exception: Unimplemented native call.\n";
			raiseException(NULL);
			    // ATTN-970909 [HD]: The exception handler should take care of dead frames.
			popExecFrame(genValue);
		    }
		    else {
			if (newExecFrame->hasLongValue()) {
			    newExecFrame->getReturnValue(&arg64Bits[0]);
			    popExecFrame(&arg64Bits[0]);
			}
			else {
			    newExecFrame->getReturnValue(genValue);
			    popExecFrame(genValue);
			}
		    }
		}
	    }
	}
	else {
	    cout << "RT Exception: Cant resolve the method.\n";
	    raiseException(NULL);
	    }
	}
    else {
	cout << "RT Exception: No Such Method.\n";
	raiseException(NULL);
	}
//	    execFrame->incPCfake(JVI_INVOKESTATIC_SIZE);    // DEBUG.
}


void JVMachine2::I_INVOKEINTERFACE(void)
{

    aShort= execFrame->fetchShort();
    anUInt= (unsigned int)execFrame->fetchByte();
    aByte= execFrame->fetchByte();
    aMethod= (MethodInfo *)execFrame->constantPool[aShort]->resolve(kernel->getClassManager(), execFrame->currentClass);
#if defined(DBGLEVEL_1)
    if (traceLevel & DebugInfo::methodLookup)
	    execFrame->constantPool[aShort]->describe(execFrame->constantPool);
#endif
    if (aMethod != NULL) {

	nextMethod= aMethod->resolve(kernel->getClassManager());
	if (nextMethod) {
	    newExecFrame= new JVExecFrame(nextMethod, aMethod->getOwner());
	    newExecFrame->loadArguments(operStack);
	    if ((newExecFrame->localValues[0]->metaType == TYPE_INSTANCE)
			|| (newExecFrame->localValues[0]->metaType & TYPE_ARRAY)) {
		if (((JVInstance *)newExecFrame->localValues[0]->value) != JVAbstractInstance::nullObject) {
		    aClass= ((JVInstance *)newExecFrame->localValues[0]->value)->category();
			// We look for the same method 'name' implemented in a lower class.
		    nextMethod= aClass->getVirtualMethod(kernel->getClassManager(), nextMethod);
			// Finally, we revise the exec frame to match the method to actually use.
		    if (!newExecFrame->updateInterfaceMethod(nextMethod, aClass)) {
			nextMethod= NULL;
		    }
		    if (nextMethod != NULL) {
			pushExecFrame(newExecFrame);
			if (nextMethod->isNative()) {
			    if (nextMethod->returnsVoid()) {
				if (((JVNativeMethod *)nextMethod)->doNativeCall(this, newExecFrame)) {
				    cout << "RT Exception: Unimplemented native call.\n";
				    raiseException(NULL);
				}
				popExecFrame();
			    }
			    else {
				if (((JVNativeMethod *)nextMethod)->doNativeCall(this, newExecFrame)) {
				    cout << "RT Exception: Unimplemented native call.\n";
				    raiseException(NULL);
					// ATTN-970909 [HD]: The exception handler should take care of dead frames.
				    popExecFrame(genValue);
				}
				else {
				    if (newExecFrame->hasLongValue()) {
					newExecFrame->getReturnValue(&arg64Bits[0]);
					popExecFrame(&arg64Bits[0]);
				    }
				    else {
					newExecFrame->getReturnValue(genValue);
					popExecFrame(genValue);
				    }
				}
			    }
			}
		    }
		    else {
			cout << "RT Exception: Cant find a concrete implementation.\n";
			raiseException(NULL);
			delete newExecFrame;
		    }
		    }
		else {
		    cout << "RT Exception: NullPointerException.\n";
		    raiseException(NULL);
		    delete newExecFrame;
		    }
		}
	    else {
		cout << "RT Exception: MethodNotOnObject.\n";
		raiseException(NULL);
		delete newExecFrame;
		}
	    }
	else {
	    cout << "RT Exception: Cant resolve the method.\n";
	    raiseException(NULL);
	    }
	}
    else {
	cout << "RT Exception: No Such Method.\n";
	raiseException(NULL);
	}

//	    execFrame->incPCfake(JVI_INVOKEINTERFACE_SIZE);    // DEBUG.
}


void JVMachine2::I_ATHROW(void)
{

    // TODO: Find out what management to do here (maybe none).
    raiseException(NULL);
    execFrame->incPC(JVI_ATHROW_SIZE - 1);
}


void JVMachine2::I_NEW(void)
{

    aShort= execFrame->fetchShort();
    // ATTN: If the new object to create is a String constant, then we resolve a CteString instead
    // of a CteClass.  We must then generate a String object and put in it the value of what was
    // resolved.
    switch (execFrame->constantPool[aShort]->type()) {
	case CONSTANT_Class:
	    aClass= (ClassFile *)execFrame->constantPool[aShort]->resolve(
				kernel->getClassManager(), execFrame->currentClass);
#if defined(DBGLEVEL_4)
cout << "JVMachine: NEW of ";
execFrame->constantPool[aShort]->describe(execFrame->constantPool);
cout << ".\n";
#endif
	    initialStr= NULL;
	    break;
	case CONSTANT_String:
#if defined(DBGLEVEL_4)
cout << "JVMachine: NEW cte String for ";
execFrame->constantPool[aShort]->describe(execFrame->constantPool);
cout << ".\n";
#endif
	    aClass= kernel->getClassManager()->loadClass("java/lang/String");
	// ATTN: TMP HACK.
	    initialStr= (char *)execFrame->constantPool[aShort]->resolve(
				kernel->getClassManager(), execFrame->currentClass);
	    break;
	default:	// Should generate some kind of 'better' error'.
	    aClass= NULL;
	    break;
	}
    if (aClass) {
	genValue.metaType= TYPE_INSTANCE;
	genValue.value= poolManager->getInstance(aClass);
	operStack->push(genValue);
	if (initialStr) {			// Have to deal with a 'String' constant !
	    }
	}
    else {
	cout << "RT Exception: Cant find class.\n";
	raiseException(NULL);
	}
    execFrame->incPCfake(JVI_NEW_SIZE);       // DEBUG.
}


void JVMachine2::I_CHECKCAST(void)
{

    execFrame->incPC(JVI_CHECKCAST_SIZE - 1);
}


void JVMachine2::I_INSTANCEOF(void)
{

    aShort= execFrame->fetchShort();
    aClass= (ClassFile *)execFrame->constantPool[aShort]->resolve(
				kernel->getClassManager(), execFrame->currentClass);
    operStack->pop(genValue);
    switch(genValue.metaType) {
	case TYPE_BASIC:
	    operStack->pushInteger(0);
	    break;
	case TYPE_INSTANCE:
	    if (((JVInstance *)genValue.value)->category()->isCastable(aClass))
		operStack->pushInteger(1);
	    else
		operStack->pushInteger(0);
	    break;
	case TYPE_ARRAY | TYPE_BASIC_ARRAY:	// ATTN: How to deal with array matching ?
	case TYPE_ARRAY | TYPE_INST_ARRAY:
	    operStack->pushInteger(1);
	    break;
	default:
	    operStack->pushInteger(0);
	    break;
	}
    execFrame->incPCfake(JVI_INSTANCEOF_SIZE);    // DEBUG.
}


void JVMachine2::I_MONITORENTER(void)
{

	// TMP: Must take out the object ref from the stack.
    operStack->pop(genValue);
    if (genValue.metaType != TYPE_BASIC) {
	if ((JVInstance *)genValue.value == JVAbstractInstance::nullObject) {
	    cout << "RT Exception: Non-object on stack.\n";
	    raiseException(NULL);
	    }
	else {
	    }
	}
    else {
	cout << "RT Exception: Non-object on stack.\n";
	raiseException(NULL);
	}
    execFrame->incPCfake(JVI_MONITORENTER_SIZE);
}


void JVMachine2::I_MONITOREXIT(void)
{

	// TMP: Must take out the object ref from the stack.
    operStack->pop(genValue);
    if (genValue.metaType != TYPE_BASIC) {
	if ((JVInstance *)genValue.value == JVAbstractInstance::nullObject) {
	    cout << "RT Exception: Non-object on stack.\n";
	    raiseException(NULL);
	    }
	else {
	    }
	}
    else {
	cout << "RT Exception: Non-object on stack.\n";
	raiseException(NULL);
	}
    execFrame->incPCfake(JVI_MONITOREXIT_SIZE);
}


void JVMachine2::I_UNKNOWN(void)
{
    cout << "ERRMSG: Unknow instruction code: " << execFrame->fetch() << ".\n";
//	    execFrame->incPC(1);
}

#if defined(PLZEN_LEVEL_2)

void JVMachine2::I_FIRE(void)
{
    cout << "ERRMSG: JVI_FIRE not implemented yet.\n";
    execFrame->incPCfake(JVI_FIRE_SIZE);
}


void JVMachine2::I_FIRESTATIC(void)
{
    anUInt= (unsigned int)execFrame->fetchShort();
    aField= (FieldInfo *)((CteNameAndType *)
	execFrame->constantPool[anUInt])->resolve(kernel->getClassManager(),
			    execFrame->currentClass);
    operStack->pop(genValue);		// Pop the receiver of the event.
    if (aField != NULL) {
	fieldDesc= aField->resolve(kernel->getClassManager());
	if (fieldDesc->isStatic()) {
	    anInstance= fieldDesc->getStaticValue();
	    if (anInstance == NULL) {
		cout << "RT Exception: Null Transition fired.\n";
		raiseException(NULL);
	    }
	    else {
		if (genValue.metaType == TYPE_INSTANCE) {
		    if (!((JVInstance *)genValue.value)->category()->isStateless()) {
			if (fieldDesc->metaType() == TYPE_CLASS) {
			    PVEvent *newEvent;

			    // TODO: create a clone of the instance, 
			    // TMP:
			    newEvent= new PVEvent(PVEvent::isField, (JVInstance *)aField);
			    ((PVStateInstance *)genValue.value)->fire(newEvent);
			}
			else {		// Incorrect type for an event, internal error.
			    cout << "RT Exception: Bad Transition type.\n";
			    raiseException(NULL);
			}
		    }
		    else {
			cout << "RT Exception: FireNotOnStatefulObject.\n";
			raiseException(NULL);
		    }
		}
		else {
		    cout << "RT Exception: FireNotOnStatefulObject.\n";
		    raiseException(NULL);
		}
	    }
	}
	else {	// Can't find the field !
	    cout << "RT Exception: IncompatibleClassChangeError.\n";
	    raiseException(NULL);
	}
    }
    else {
	cout << "RT Exception: No Such Event.\n";
	raiseException(NULL);
    }
    execFrame->incPCfake(JVI_FIRESTATIC_SIZE);
}


void JVMachine2::I_FIREFIELD(void)
{
    anUInt= (unsigned int)execFrame->fetchShort();
    aField= (FieldInfo *)((CteNameAndType *)
	execFrame->constantPool[anUInt])->resolve(kernel->getClassManager(),
			    execFrame->currentClass);
    if (aField != NULL) {
	fieldDesc= aField->resolve(kernel->getClassManager());
	if (fieldDesc->isStatic()) {
	    cout << "RT Exception: IncompatibleClassChangeError.\n";
	    raiseException(NULL);
	}
	else {
	    operStack->pop(genValue);
	    if (genValue.metaType != TYPE_INSTANCE) {
		cout << "RT Exception: FireFieldNotOnObject.\n";
		raiseException(NULL);
	    }
	    else if (genValue.value != JVAbstractInstance::nullObject) {
		JVAbstractInstance *anAbstractInstance;
		anAbstractInstance= ((JVInstance *)genValue.value)->getField(fieldDesc);
		if (anAbstractInstance == NULL) {
		    if (fieldDesc->metaType() == TYPE_BASIC) {
// ATTENTION: Does not work for many situations (8 bytes value, for example).
			operStack->pushInteger(0);
		    }
		    else {
			genValue.metaType= TYPE_INSTANCE;
			genValue.value= (void *)JVAbstractInstance::nullObject;
			operStack->push(genValue);
		    }
		}
		else {
// ATTENTION: Must consolidate the type meanings.
		    if (fieldDesc->metaType() == TYPE_CLASS) {    // Must implement a better way of doing this.
			genValue.metaType= TYPE_INSTANCE;
			genValue.value= (void *)anAbstractInstance;
			operStack->push(genValue);
		    }
		    else if (fieldDesc->metaType() == TYPE_BASIC) {
// ATTENTION: Does not work for many situations (8 bytes value, for example).
			operStack->pushInteger((unsigned int)anAbstractInstance);
		    }
		    else if (fieldDesc->metaType() == TYPE_ARRAY) {
			genValue.metaType= TYPE_ARRAY | fieldDesc->detailedType();
			genValue.value= (void *)anAbstractInstance;
			operStack->push(genValue);
		    }
		    else {		// Unknown type, internal error.
			cout << "RT Exception: Unknown data type.\n";
			raiseException(NULL);
		    }
		}
	    }
	    else {
		cout << "RT Exception: NullPointerException.\n";
		raiseException(NULL);
	    }
	}
    }
    execFrame->incPCfake(JVI_FIREFIELD_SIZE);
}


void JVMachine2::I_WAIT(void)
{
    cout << "ERRMSG: JVI_WAIT not implemented yet.\n";
    execFrame->incPCfake(JVI_WAIT_SIZE);
}

#endif
