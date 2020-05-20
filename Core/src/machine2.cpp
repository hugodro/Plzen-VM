/**************************************************
* File: machine2.cc.
* Desc: Implementation of the JVMachine2 class, not including java instruction methods.
* Module: AkraLog : JavaKit.
* Rev: 22 novembre 1996 : REV 0 : Hugo DesRosiers : Creation.
**************************************************/

#include "jvInstrSet.h"
#include "execFrame.h"
#include "debugInfo.h"
#include "machine2.h"


#define callMemberFunction(anObject, ptrToMember)    \
			((anObject).*(ptrToMember))


JVMachine2::fctArrayPtr JVMachine2::instructionArray= NULL;


void JVMachine2::loadJVBasicInstructions()
{
    instructionArray[JVI_BIPUSH]= &JVMachine2::I_BIPUSH;
    instructionArray[JVI_SIPUSH]= &JVMachine2::I_SIPUSH;
    instructionArray[JVI_LDC1]= &JVMachine2::I_LDC1;
    instructionArray[JVI_LDC2]= &JVMachine2::I_LDC2;
    instructionArray[JVI_LDC2W]= &JVMachine2::I_LDC2W;
    instructionArray[JVI_ACONST_NULL]= &JVMachine2::I_ACONST_NULL;
    instructionArray[JVI_ICONST_M1]= &JVMachine2::I_ICONST_M1;
    instructionArray[JVI_ICONST_0]= &JVMachine2::I_ICONST_0;
    instructionArray[JVI_ICONST_1]= &JVMachine2::I_ICONST_1;
    instructionArray[JVI_ICONST_2]= &JVMachine2::I_ICONST_2;
    instructionArray[JVI_ICONST_3]= &JVMachine2::I_ICONST_3;
    instructionArray[JVI_ICONST_4]= &JVMachine2::I_ICONST_4;
    instructionArray[JVI_ICONST_5]= &JVMachine2::I_ICONST_5;
    instructionArray[JVI_LCONST_0]= &JVMachine2::I_LCONST_0;
    instructionArray[JVI_LCONST_1]= &JVMachine2::I_LCONST_1;
    instructionArray[JVI_FCONST_0]= &JVMachine2::I_FCONST_0;
    instructionArray[JVI_FCONST_1]= &JVMachine2::I_FCONST_1;
    instructionArray[JVI_FCONST_2]= &JVMachine2::I_FCONST_2;
    instructionArray[JVI_DCONST_0]= &JVMachine2::I_DCONST_0;
    instructionArray[JVI_DCONST_1]= &JVMachine2::I_DCONST_1;
    instructionArray[JVI_ILOAD]= &JVMachine2::I_ILOAD;
    instructionArray[JVI_ILOAD_0]= &JVMachine2::I_ILOAD_0;
    instructionArray[JVI_ILOAD_1]= &JVMachine2::I_ILOAD_1;
    instructionArray[JVI_ILOAD_2]= &JVMachine2::I_ILOAD_2;
    instructionArray[JVI_ILOAD_3]= &JVMachine2::I_ILOAD_3;
    instructionArray[JVI_LLOAD]= &JVMachine2::I_LLOAD;
    instructionArray[JVI_LLOAD_0]= &JVMachine2::I_LLOAD_0;
    instructionArray[JVI_LLOAD_1]= &JVMachine2::I_LLOAD_1;
    instructionArray[JVI_LLOAD_2]= &JVMachine2::I_LLOAD_2;
    instructionArray[JVI_LLOAD_3]= &JVMachine2::I_LLOAD_3;
    instructionArray[JVI_FLOAD]= &JVMachine2::I_FLOAD;
    instructionArray[JVI_FLOAD_0]= &JVMachine2::I_FLOAD_0;
    instructionArray[JVI_FLOAD_1]= &JVMachine2::I_FLOAD_1;
    instructionArray[JVI_FLOAD_2]= &JVMachine2::I_FLOAD_2;
    instructionArray[JVI_FLOAD_3]= &JVMachine2::I_FLOAD_3;
    instructionArray[JVI_DLOAD]= &JVMachine2::I_DLOAD;
    instructionArray[JVI_DLOAD_0]= &JVMachine2::I_DLOAD_0;
    instructionArray[JVI_DLOAD_1]= &JVMachine2::I_DLOAD_1;
    instructionArray[JVI_DLOAD_2]= &JVMachine2::I_DLOAD_2;
    instructionArray[JVI_DLOAD_3]= &JVMachine2::I_DLOAD_3;
    instructionArray[JVI_ALOAD]= &JVMachine2::I_ALOAD;
    instructionArray[JVI_ALOAD_0]= &JVMachine2::I_ALOAD_0;
    instructionArray[JVI_ALOAD_1]= &JVMachine2::I_ALOAD_1;
    instructionArray[JVI_ALOAD_2]= &JVMachine2::I_ALOAD_2;
    instructionArray[JVI_ALOAD_3]= &JVMachine2::I_ALOAD_3;
    instructionArray[JVI_ISTORE]= &JVMachine2::I_ISTORE;
    instructionArray[JVI_ISTORE_0]= &JVMachine2::I_ISTORE_0;
    instructionArray[JVI_ISTORE_1]= &JVMachine2::I_ISTORE_1;
    instructionArray[JVI_ISTORE_2]= &JVMachine2::I_ISTORE_2;
    instructionArray[JVI_ISTORE_3]= &JVMachine2::I_ISTORE_3;
    instructionArray[JVI_LSTORE]= &JVMachine2::I_LSTORE;
    instructionArray[JVI_LSTORE_0]= &JVMachine2::I_LSTORE_0;
    instructionArray[JVI_LSTORE_1]= &JVMachine2::I_LSTORE_1;
    instructionArray[JVI_LSTORE_2]= &JVMachine2::I_LSTORE_2;
    instructionArray[JVI_LSTORE_3]= &JVMachine2::I_LSTORE_3;
    instructionArray[JVI_FSTORE]= &JVMachine2::I_FSTORE;
    instructionArray[JVI_FSTORE_0]= &JVMachine2::I_FSTORE_0;
    instructionArray[JVI_FSTORE_1]= &JVMachine2::I_FSTORE_1;
    instructionArray[JVI_FSTORE_2]= &JVMachine2::I_FSTORE_2;
    instructionArray[JVI_FSTORE_3]= &JVMachine2::I_FSTORE_3;
    instructionArray[JVI_DSTORE]= &JVMachine2::I_DSTORE;
    instructionArray[JVI_DSTORE_0]= &JVMachine2::I_DSTORE_0;
    instructionArray[JVI_DSTORE_1]= &JVMachine2::I_DSTORE_1;
    instructionArray[JVI_DSTORE_2]= &JVMachine2::I_DSTORE_2;
    instructionArray[JVI_DSTORE_3]= &JVMachine2::I_DSTORE_3;
    instructionArray[JVI_ASTORE]= &JVMachine2::I_ASTORE;
    instructionArray[JVI_ASTORE_0]= &JVMachine2::I_ASTORE_0;
    instructionArray[JVI_ASTORE_1]= &JVMachine2::I_ASTORE_1;
    instructionArray[JVI_ASTORE_2]= &JVMachine2::I_ASTORE_2;
    instructionArray[JVI_ASTORE_3]= &JVMachine2::I_ASTORE_3;
    instructionArray[JVI_IINC]= &JVMachine2::I_IINC;
    instructionArray[JVI_WIDE]= &JVMachine2::I_WIDE;
    instructionArray[JVI_NEWARRAY]= &JVMachine2::I_NEWARRAY;
    instructionArray[JVI_ANEWARRAY]= &JVMachine2::I_ANEWARRAY;
    instructionArray[JVI_MULTIANEWARRAY]= &JVMachine2::I_MULTIANEWARRAY;
    instructionArray[JVI_ARRAYLENGTH]= &JVMachine2::I_ARRAYLENGTH;
    instructionArray[JVI_IALOAD]= &JVMachine2::I_IALOAD;
    instructionArray[JVI_LALOAD]= &JVMachine2::I_LALOAD;
    instructionArray[JVI_FALOAD]= &JVMachine2::I_FALOAD;
    instructionArray[JVI_DALOAD]= &JVMachine2::I_DALOAD;
    instructionArray[JVI_AALOAD]= &JVMachine2::I_AALOAD;
    instructionArray[JVI_BALOAD]= &JVMachine2::I_BALOAD;
    instructionArray[JVI_CALOAD]= &JVMachine2::I_CALOAD;
    instructionArray[JVI_SALOAD]= &JVMachine2::I_SALOAD;
    instructionArray[JVI_IASTORE]= &JVMachine2::I_IASTORE;
    instructionArray[JVI_LASTORE]= &JVMachine2::I_LASTORE;
    instructionArray[JVI_FASTORE]= &JVMachine2::I_FASTORE;
    instructionArray[JVI_DASTORE]= &JVMachine2::I_DASTORE;
    instructionArray[JVI_AASTORE]= &JVMachine2::I_AASTORE;
    instructionArray[JVI_BASTORE]= &JVMachine2::I_BASTORE;
    instructionArray[JVI_CASTORE]= &JVMachine2::I_CASTORE;
    instructionArray[JVI_SASTORE]= &JVMachine2::I_SASTORE;
    instructionArray[JVI_NOP]= &JVMachine2::I_NOP;
    instructionArray[JVI_POP]= &JVMachine2::I_POP;
    instructionArray[JVI_POP2]= &JVMachine2::I_POP2;
    instructionArray[JVI_DUP]= &JVMachine2::I_DUP;
    instructionArray[JVI_DUP2]= &JVMachine2::I_DUP2;
    instructionArray[JVI_DUP_X1]= &JVMachine2::I_DUP_X1;
    instructionArray[JVI_DUP2_X1]= &JVMachine2::I_DUP2_X1;
    instructionArray[JVI_DUP_X2]= &JVMachine2::I_DUP_X2;
    instructionArray[JVI_DUP2_X2]= &JVMachine2::I_DUP2_X2;
    instructionArray[JVI_SWAP]= &JVMachine2::I_SWAP;
    instructionArray[JVI_IADD]= &JVMachine2::I_IADD;
    instructionArray[JVI_LADD]= &JVMachine2::I_LADD;
    instructionArray[JVI_FADD]= &JVMachine2::I_FADD;
    instructionArray[JVI_DADD]= &JVMachine2::I_DADD;
    instructionArray[JVI_ISUB]= &JVMachine2::I_ISUB;
    instructionArray[JVI_LSUB]= &JVMachine2::I_LSUB;
    instructionArray[JVI_FSUB]= &JVMachine2::I_FSUB;
    instructionArray[JVI_DSUB]= &JVMachine2::I_DSUB;
    instructionArray[JVI_IMUL]= &JVMachine2::I_IMUL;
    instructionArray[JVI_LMUL]= &JVMachine2::I_LMUL;
    instructionArray[JVI_FMUL]= &JVMachine2::I_FMUL;
    instructionArray[JVI_DMUL]= &JVMachine2::I_DMUL;
    instructionArray[JVI_IDIV]= &JVMachine2::I_IDIV;
    instructionArray[JVI_LDIV]= &JVMachine2::I_LDIV;
    instructionArray[JVI_FDIV]= &JVMachine2::I_FDIV;
    instructionArray[JVI_DDIV]= &JVMachine2::I_DDIV;
    instructionArray[JVI_IREM]= &JVMachine2::I_IREM;
    instructionArray[JVI_LREM]= &JVMachine2::I_LREM;
    instructionArray[JVI_FREM]= &JVMachine2::I_FREM;
    instructionArray[JVI_DREM]= &JVMachine2::I_DREM;
    instructionArray[JVI_INEG]= &JVMachine2::I_INEG;
    instructionArray[JVI_LNEG]= &JVMachine2::I_LNEG;
    instructionArray[JVI_FNEG]= &JVMachine2::I_FNEG;
    instructionArray[JVI_DNEG]= &JVMachine2::I_DNEG;
    instructionArray[JVI_ISHL]= &JVMachine2::I_ISHL;
    instructionArray[JVI_ISHR]= &JVMachine2::I_ISHR;
    instructionArray[JVI_IUSHR]= &JVMachine2::I_IUSHR;
    instructionArray[JVI_LSHL]= &JVMachine2::I_LSHL;
    instructionArray[JVI_LSHR]= &JVMachine2::I_LSHR;
    instructionArray[JVI_LUSHR]= &JVMachine2::I_LUSHR;
    instructionArray[JVI_IAND]= &JVMachine2::I_IAND;
    instructionArray[JVI_LAND]= &JVMachine2::I_LAND;
    instructionArray[JVI_IOR]= &JVMachine2::I_IOR;
    instructionArray[JVI_LOR]= &JVMachine2::I_LOR;
    instructionArray[JVI_IXOR]= &JVMachine2::I_IXOR;
    instructionArray[JVI_LXOR]= &JVMachine2::I_LXOR;
    instructionArray[JVI_I2L]= &JVMachine2::I_I2L;
    instructionArray[JVI_I2F]= &JVMachine2::I_I2F;
    instructionArray[JVI_I2D]= &JVMachine2::I_I2D;
    instructionArray[JVI_L2I]= &JVMachine2::I_L2I;
    instructionArray[JVI_L2F]= &JVMachine2::I_L2F;
    instructionArray[JVI_L2D]= &JVMachine2::I_L2D;
    instructionArray[JVI_F2I]= &JVMachine2::I_F2I;
    instructionArray[JVI_F2L]= &JVMachine2::I_F2L;
    instructionArray[JVI_F2D]= &JVMachine2::I_F2D;
    instructionArray[JVI_D2I]= &JVMachine2::I_D2I;
    instructionArray[JVI_D2L]= &JVMachine2::I_D2L;
    instructionArray[JVI_D2F]= &JVMachine2::I_D2F;
    instructionArray[JVI_INT2BYTE]= &JVMachine2::I_INT2BYTE;
    instructionArray[JVI_INT2CHAR]= &JVMachine2::I_INT2CHAR;
    instructionArray[JVI_INT2SHORT]= &JVMachine2::I_INT2SHORT;
    instructionArray[JVI_IFEQ]= &JVMachine2::I_IFEQ;
    instructionArray[JVI_IFNULL]= &JVMachine2::I_IFNULL;
    instructionArray[JVI_IFLT]= &JVMachine2::I_IFLT;
    instructionArray[JVI_IFLE]= &JVMachine2::I_IFLE;
    instructionArray[JVI_IFNE]= &JVMachine2::I_IFNE;
    instructionArray[JVI_IFNONNULL]= &JVMachine2::I_IFNONNULL;
    instructionArray[JVI_IFGT]= &JVMachine2::I_IFGT;
    instructionArray[JVI_IFGE]= &JVMachine2::I_IFGE;
    instructionArray[JVI_IF_ICMPEQ]= &JVMachine2::I_IF_ICMPEQ;
    instructionArray[JVI_IF_ICMPNE]= &JVMachine2::I_IF_ICMPNE;
    instructionArray[JVI_IF_ICMPLT]= &JVMachine2::I_IF_ICMPLT;
    instructionArray[JVI_IF_ICMPGT]= &JVMachine2::I_IF_ICMPGT;
    instructionArray[JVI_IF_ICMPLE]= &JVMachine2::I_IF_ICMPLE;
    instructionArray[JVI_IF_ICMPGE]= &JVMachine2::I_IF_ICMPGE;
    instructionArray[JVI_LCMP]= &JVMachine2::I_LCMP;
    instructionArray[JVI_FCMPL]= &JVMachine2::I_FCMPL;
    instructionArray[JVI_FCMPG]= &JVMachine2::I_FCMPG;
    instructionArray[JVI_DCMPL]= &JVMachine2::I_DCMPL;
    instructionArray[JVI_DCMPG]= &JVMachine2::I_DCMPG;
    instructionArray[JVI_IF_ACMPEQ]= &JVMachine2::I_IF_ACMPEQ;
    instructionArray[JVI_IF_ACMPNE]= &JVMachine2::I_IF_ACMPNE;
    instructionArray[JVI_GOTO]= &JVMachine2::I_GOTO;
    instructionArray[JVI_GOTO_W]= &JVMachine2::I_GOTO_W;
    instructionArray[JVI_JSR]= &JVMachine2::I_JSR;
    instructionArray[JVI_JSR_W]= &JVMachine2::I_JSR_W;
    instructionArray[JVI_RET]= &JVMachine2::I_RET;
    instructionArray[JVI_RET_W]= &JVMachine2::I_RET_W;
    instructionArray[JVI_IRETURN]= &JVMachine2::I_IRETURN;
    instructionArray[JVI_LRETURN]= &JVMachine2::I_LRETURN;
    instructionArray[JVI_FRETURN]= &JVMachine2::I_FRETURN;
    instructionArray[JVI_DRETURN]= &JVMachine2::I_DRETURN;
    instructionArray[JVI_ARETURN]= &JVMachine2::I_ARETURN;
    instructionArray[JVI_RETURN]= &JVMachine2::I_RETURN;
    instructionArray[JVI_BREAKPOINT]= &JVMachine2::I_BREAKPOINT;
    instructionArray[JVI_TABLESWITCH]= &JVMachine2::I_TABLESWITCH;
    instructionArray[JVI_LOOKUPSWITCH]= &JVMachine2::I_LOOKUPSWITCH;
    instructionArray[JVI_PUTFIELD]= &JVMachine2::I_PUTFIELD;
    instructionArray[JVI_GETFIELD]= &JVMachine2::I_GETFIELD;
    instructionArray[JVI_PUTSTATIC]= &JVMachine2::I_PUTSTATIC;
    instructionArray[JVI_GETSTATIC]= &JVMachine2::I_GETSTATIC;
    instructionArray[JVI_INVOKEVIRTUAL]= &JVMachine2::I_INVOKEVIRTUAL;
    instructionArray[JVI_INVOKENONVIRTUAL]= &JVMachine2::I_INVOKENONVIRTUAL;
    instructionArray[JVI_INVOKESTATIC]= &JVMachine2::I_INVOKESTATIC;
    instructionArray[JVI_INVOKEINTERFACE]= &JVMachine2::I_INVOKEINTERFACE;
    instructionArray[JVI_ATHROW]= &JVMachine2::I_ATHROW;
    instructionArray[JVI_NEW]= &JVMachine2::I_NEW;
    instructionArray[JVI_CHECKCAST]= &JVMachine2::I_CHECKCAST;
    instructionArray[JVI_INSTANCEOF]= &JVMachine2::I_INSTANCEOF;
    instructionArray[JVI_MONITORENTER]= &JVMachine2::I_MONITORENTER;
    instructionArray[JVI_MONITOREXIT]= &JVMachine2::I_MONITOREXIT;
    instructionArray[JVI_FIRE]= &JVMachine2::I_FIRE;
    instructionArray[JVI_FIRESTATIC]= &JVMachine2::I_FIRESTATIC;
    instructionArray[JVI_FIREFIELD]= &JVMachine2::I_FIREFIELD;
    instructionArray[JVI_WAIT]= &JVMachine2::I_WAIT;
}


void JVMachine2::loadInstructionSet(void)
{
// ATTN: The following makes an 'Internal compiler error' with nextstep-gnu-cc 2.5.8.
#if !defined(NeXT)
    instructionArray= new instructionFct[256];
#else
    instructionArray= (fctArrayPtr)(new int*[256]);
#endif

    for (unsigned int i= 0; i < 256; i++) {
	instructionArray[i]= &JVMachine2::I_UNKNOWN;
    }
    loadJVBasicInstructions();
}


JVMachine2::JVMachine2(JVCup *owner) : JVMachine(owner)
{
    init();
}


JVMachine2::JVMachine2(JVCup *owner, JVAbstractThread *aThread) : JVMachine(owner, aThread)
{
    init();
}


void JVMachine2::init(void)
{
    if (instructionArray == NULL) {
	loadInstructionSet();
    }
}


int JVMachine2::doInstruction(void)
{

    unsigned int index= (unsigned int)execFrame->fetch();

    if (traceLevel & DebugInfo::instructions) debugInfo->trace(execFrame, index);
    callMemberFunction(*this, instructionArray[index])();

    return 1;
}
