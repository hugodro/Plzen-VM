/**************************************************
* File: debugInfo.cc.
* Desc: Implementation of the DebugInfo class.
* Module: AkraLog : JavaKit.
* Rev: 23 aout 1996 : REV 0 : Hugo DesRosiers : Creation.
**************************************************/

#include <iostream>

#include "execFrame.h"
#include "jvInstrSet.h"
#include "debugInfo.h"


/**************************************************
* Implementation: ClassFile.
**************************************************/

char *DebugInfo::unknownOpcode= NULL;
char **DebugInfo::jCodeName= new char*[256];

void DebugInfo::classInitializor(void)
{
    unknownOpcode= "UNKNOWN";

    for (unsigned int i= 0; i < 256; i++) {
	jCodeName[i]= unknownOpcode;
    }
    jCodeName[JVI_BIPUSH]= "BIPUSH";
    jCodeName[JVI_SIPUSH]= "SIPUSH";
    jCodeName[JVI_LDC1]= "LDC1";
    jCodeName[JVI_LDC2]= "LDC2";
    jCodeName[JVI_LDC2W]= "LDC2W";
    jCodeName[JVI_ACONST_NULL]= "ACONST_NULL";
    jCodeName[JVI_ICONST_M1]= "ICONST_M1";
    jCodeName[JVI_ICONST_0]= "ICONST_0";
    jCodeName[JVI_ICONST_1]= "ICONST_1";
    jCodeName[JVI_ICONST_2]= "ICONST_2";
    jCodeName[JVI_ICONST_3]= "ICONST_3";
    jCodeName[JVI_ICONST_4]= "ICONST_4";
    jCodeName[JVI_ICONST_5]= "ICONST_5";
    jCodeName[JVI_LCONST_0]= "LCONST_0";
    jCodeName[JVI_LCONST_1]= "LCONST_1";
    jCodeName[JVI_FCONST_0]= "FCONST_0";
    jCodeName[JVI_FCONST_1]= "FCONST_1";
    jCodeName[JVI_FCONST_2]= "FCONST_2";
    jCodeName[JVI_DCONST_0]= "DCONST_0";
    jCodeName[JVI_DCONST_1]= "DCONST_1";
    jCodeName[JVI_ILOAD]= "ILOAD";
    jCodeName[JVI_ILOAD_0]= "ILOAD_0";
    jCodeName[JVI_ILOAD_1]= "ILOAD_1";
    jCodeName[JVI_ILOAD_2]= "ILOAD_2";
    jCodeName[JVI_ILOAD_3]= "ILOAD_3";
    jCodeName[JVI_LLOAD]= "LLOAD";
    jCodeName[JVI_LLOAD_0]= "LLOAD_0";
    jCodeName[JVI_LLOAD_1]= "LLOAD_1";
    jCodeName[JVI_LLOAD_2]= "LLOAD_2";
    jCodeName[JVI_LLOAD_3]= "LLOAD_3";
    jCodeName[JVI_FLOAD]= "FLOAD";
    jCodeName[JVI_FLOAD_0]= "FLOAD_0";
    jCodeName[JVI_FLOAD_1]= "FLOAD_1";
    jCodeName[JVI_FLOAD_2]= "FLOAD_2";
    jCodeName[JVI_FLOAD_3]= "FLOAD_3";
    jCodeName[JVI_DLOAD]= "DLOAD";
    jCodeName[JVI_DLOAD_0]= "DLOAD_0";
    jCodeName[JVI_DLOAD_1]= "DLOAD_1";
    jCodeName[JVI_DLOAD_2]= "DLOAD_2";
    jCodeName[JVI_DLOAD_3]= "DLOAD_3";
    jCodeName[JVI_ALOAD]= "ALOAD";
    jCodeName[JVI_ALOAD_0]= "ALOAD_0";
    jCodeName[JVI_ALOAD_1]= "ALOAD_1";
    jCodeName[JVI_ALOAD_2]= "ALOAD_2";
    jCodeName[JVI_ALOAD_3]= "ALOAD_3";
    jCodeName[JVI_ISTORE]= "ISTORE";
    jCodeName[JVI_ISTORE_0]= "ISTORE_0";
    jCodeName[JVI_ISTORE_1]= "ISTORE_1";
    jCodeName[JVI_ISTORE_2]= "ISTORE_2";
    jCodeName[JVI_ISTORE_3]= "ISTORE_3";
    jCodeName[JVI_LSTORE]= "LSTORE";
    jCodeName[JVI_LSTORE_0]= "LSTORE_0";
    jCodeName[JVI_LSTORE_1]= "LSTORE_1";
    jCodeName[JVI_LSTORE_2]= "LSTORE_2";
    jCodeName[JVI_LSTORE_3]= "LSTORE_3";
    jCodeName[JVI_FSTORE]= "FSTORE";
    jCodeName[JVI_FSTORE_0]= "FSTORE_0";
    jCodeName[JVI_FSTORE_1]= "FSTORE_1";
    jCodeName[JVI_FSTORE_2]= "FSTORE_2";
    jCodeName[JVI_FSTORE_3]= "FSTORE_3";
    jCodeName[JVI_DSTORE]= "DSTORE";
    jCodeName[JVI_DSTORE_0]= "DSTORE_0";
    jCodeName[JVI_DSTORE_1]= "DSTORE_1";
    jCodeName[JVI_DSTORE_2]= "DSTORE_2";
    jCodeName[JVI_DSTORE_3]= "DSTORE_3";
    jCodeName[JVI_ASTORE]= "ASTORE";
    jCodeName[JVI_ASTORE_0]= "ASTORE_0";
    jCodeName[JVI_ASTORE_1]= "ASTORE_1";
    jCodeName[JVI_ASTORE_2]= "ASTORE_2";
    jCodeName[JVI_ASTORE_3]= "ASTORE_3";
    jCodeName[JVI_IINC]= "IINC";
    jCodeName[JVI_WIDE]= "WIDE";
    jCodeName[JVI_NEWARRAY]= "NEWARRAY";
    jCodeName[JVI_ANEWARRAY]= "ANEWARRAY";
    jCodeName[JVI_MULTIANEWARRAY]= "MULTIANEWARRAY";
    jCodeName[JVI_ARRAYLENGTH]= "ARRAYLENGTH";
    jCodeName[JVI_IALOAD]= "IALOAD";
    jCodeName[JVI_LALOAD]= "LALOAD";
    jCodeName[JVI_FALOAD]= "FALOAD";
    jCodeName[JVI_DALOAD]= "DALOAD";
    jCodeName[JVI_AALOAD]= "AALOAD";
    jCodeName[JVI_BALOAD]= "BALOAD";
    jCodeName[JVI_CALOAD]= "CALOAD";
    jCodeName[JVI_SALOAD]= "SALOAD";
    jCodeName[JVI_IASTORE]= "IASTORE";
    jCodeName[JVI_LASTORE]= "LASTORE";
    jCodeName[JVI_FASTORE]= "FASTORE";
    jCodeName[JVI_DASTORE]= "DASTORE";
    jCodeName[JVI_AASTORE]= "AASTORE";
    jCodeName[JVI_BASTORE]= "BASTORE";
    jCodeName[JVI_CASTORE]= "CASTORE";
    jCodeName[JVI_SASTORE]= "SASTORE";
    jCodeName[JVI_NOP]= "NOP";
    jCodeName[JVI_POP]= "POP";
    jCodeName[JVI_POP2]= "POP2";
    jCodeName[JVI_DUP]= "DUP";
    jCodeName[JVI_DUP2]= "DUP2";
    jCodeName[JVI_DUP_X1]= "DUP_X1";
    jCodeName[JVI_DUP2_X1]= "DUP2_X1";
    jCodeName[JVI_DUP_X2]= "DUP_X2";
    jCodeName[JVI_DUP2_X2]= "DUP2_X2";
    jCodeName[JVI_SWAP]= "SWAP";
    jCodeName[JVI_IADD]= "IADD";
    jCodeName[JVI_LADD]= "LADD";
    jCodeName[JVI_FADD]= "FADD";
    jCodeName[JVI_DADD]= "DADD";
    jCodeName[JVI_ISUB]= "ISUB";
    jCodeName[JVI_LSUB]= "LSUB";
    jCodeName[JVI_FSUB]= "FSUB";
    jCodeName[JVI_DSUB]= "DSUB";
    jCodeName[JVI_IMUL]= "IMUL";
    jCodeName[JVI_LMUL]= "LMUL";
    jCodeName[JVI_FMUL]= "FMUL";
    jCodeName[JVI_DMUL]= "DMUL";
    jCodeName[JVI_IDIV]= "IDIV";
    jCodeName[JVI_LDIV]= "LDIV";
    jCodeName[JVI_FDIV]= "FDIV";
    jCodeName[JVI_DDIV]= "DDIV";
    jCodeName[JVI_IREM]= "IREM";
    jCodeName[JVI_LREM]= "LREM";
    jCodeName[JVI_FREM]= "FREM";
    jCodeName[JVI_DREM]= "DREM";
    jCodeName[JVI_INEG]= "INEG";
    jCodeName[JVI_LNEG]= "LNEG";
    jCodeName[JVI_FNEG]= "FNEG";
    jCodeName[JVI_DNEG]= "DNEG";
    jCodeName[JVI_ISHL]= "ISHL";
    jCodeName[JVI_ISHR]= "ISHR";
    jCodeName[JVI_IUSHR]= "IUSHR";
    jCodeName[JVI_LSHL]= "LSHL";
    jCodeName[JVI_LSHR]= "LSHR";
    jCodeName[JVI_LUSHR]= "LUSHR";
    jCodeName[JVI_IAND]= "IAND";
    jCodeName[JVI_LAND]= "LAND";
    jCodeName[JVI_IOR]= "IOR";
    jCodeName[JVI_LOR]= "LOR";
    jCodeName[JVI_IXOR]= "IXOR";
    jCodeName[JVI_LXOR]= "LXOR";
    jCodeName[JVI_I2L]= "I2L";
    jCodeName[JVI_I2F]= "I2F";
    jCodeName[JVI_I2D]= "I2D";
    jCodeName[JVI_L2I]= "L2I";
    jCodeName[JVI_L2F]= "L2F";
    jCodeName[JVI_L2D]= "L2D";
    jCodeName[JVI_F2I]= "F2I";
    jCodeName[JVI_F2L]= "F2L";
    jCodeName[JVI_F2D]= "F2D";
    jCodeName[JVI_D2I]= "D2I";
    jCodeName[JVI_D2L]= "D2L";
    jCodeName[JVI_D2F]= "D2F";
    jCodeName[JVI_INT2BYTE]= "INT2BYTE";
    jCodeName[JVI_INT2CHAR]= "INT2CHAR";
    jCodeName[JVI_INT2SHORT]= "INT2SHORT";
    jCodeName[JVI_IFEQ]= "IFEQ";
    jCodeName[JVI_IFNULL]= "IFNULL";
    jCodeName[JVI_IFLT]= "IFLT";
    jCodeName[JVI_IFLE]= "IFLE";
    jCodeName[JVI_IFNE]= "IFNE";
    jCodeName[JVI_IFNONNULL]= "IFNONNULL";
    jCodeName[JVI_IFGT]= "IFGT";
    jCodeName[JVI_IFGE]= "IFGE";
    jCodeName[JVI_IF_ICMPEQ]= "IF_ICMPEQ";
    jCodeName[JVI_IF_ICMPNE]= "IF_ICMPNE";
    jCodeName[JVI_IF_ICMPLT]= "IF_ICMPLT";
    jCodeName[JVI_IF_ICMPGT]= "IF_ICMPGT";
    jCodeName[JVI_IF_ICMPLE]= "IF_ICMPLE";
    jCodeName[JVI_IF_ICMPGE]= "IF_ICMPGE";
    jCodeName[JVI_LCMP]= "LCMP";
    jCodeName[JVI_FCMPL]= "FCMPL";
    jCodeName[JVI_FCMPG]= "FCMPG";
    jCodeName[JVI_DCMPL]= "DCMPL";
    jCodeName[JVI_DCMPG]= "DCMPG";
    jCodeName[JVI_IF_ACMPEQ]= "IF_ACMPEQ";
    jCodeName[JVI_IF_ACMPNE]= "IF_ACMPNE";
    jCodeName[JVI_GOTO]= "GOTO";
    jCodeName[JVI_GOTO_W]= "GOTO_W";
    jCodeName[JVI_JSR]= "JSR";
    jCodeName[JVI_JSR_W]= "JSR_W";
    jCodeName[JVI_RET]= "RET";
    jCodeName[JVI_RET_W]= "RET_W";
    jCodeName[JVI_IRETURN]= "IRETURN";
    jCodeName[JVI_LRETURN]= "LRETURN";
    jCodeName[JVI_FRETURN]= "FRETURN";
    jCodeName[JVI_DRETURN]= "DRETURN";
    jCodeName[JVI_ARETURN]= "ARETURN";
    jCodeName[JVI_RETURN]= "RETURN";
    jCodeName[JVI_BREAKPOINT]= "BREAKPOINT";
    jCodeName[JVI_TABLESWITCH]= "TABLESWITCH";
    jCodeName[JVI_LOOKUPSWITCH]= "LOOKUPSWITCH";
    jCodeName[JVI_PUTFIELD]= "PUTFIELD";
    jCodeName[JVI_GETFIELD]= "GETFIELD";
    jCodeName[JVI_PUTSTATIC]= "PUTSTATIC";
    jCodeName[JVI_GETSTATIC]= "GETSTATIC";
    jCodeName[JVI_INVOKEVIRTUAL]= "INVOKEVIRTUAL";
    jCodeName[JVI_INVOKENONVIRTUAL]= "INVOKENONVIRTUAL";
    jCodeName[JVI_INVOKESTATIC]= "INVOKESTATIC";
    jCodeName[JVI_INVOKEINTERFACE]= "INVOKEINTERFACE";
    jCodeName[JVI_ATHROW]= "ATHROW";
    jCodeName[JVI_NEW]= "NEW";
    jCodeName[JVI_CHECKCAST]= "CHECKCAST";
    jCodeName[JVI_INSTANCEOF]= "INSTANCEOF";
    jCodeName[JVI_MONITORENTER]= "MONITORENTER";
    jCodeName[JVI_MONITOREXIT]= "MONITOREXIT";
#if defined(PLZEN_LEVEL_2)
    jCodeName[JVI_FIRE]= "FIRE";
    jCodeName[JVI_FIRESTATIC]= "FIRESTATIC";
    jCodeName[JVI_FIREFIELD]= "FIREFIELD";
    jCodeName[JVI_WAIT]= "WAIT";
#endif
}


DebugInfo::DebugInfo(void)
{
    if (unknownOpcode == NULL) {
	classInitializor();
    }
    methodTraced= NULL;
    sectionsTraced= noTrace;
}


DebugInfo::DebugInfo(SectionTracing sectionSet)
{
    if (unknownOpcode == NULL) {
	classInitializor();
    }
    methodTraced= NULL;
    sectionsTraced= sectionSet;
}


DebugInfo::SectionTracing DebugInfo::traceLevel(void)
{
    return sectionsTraced;
}


void DebugInfo::trace(JVExecFrame *aFrame, unsigned int aCode)
{
    std::cout << "@" << aFrame->currentPC() << ": " << jCodeName[aCode] << ".\n";
    std::cout.flush();
}


void DebugInfo::setTraceSection(SectionTracing aSection)
{
    sectionsTraced= (SectionTracing)(sectionsTraced | aSection);
}


void DebugInfo::stopTraceSection(SectionTracing aSection)
{
    sectionsTraced= (SectionTracing)(sectionsTraced ^ aSection);
}


