/**************************************************
* File: machine2.h.
* Desc: Definition of the JVMachine2 class.
* Module: AkraLog : JavaKit.
* Rev: 22 novembre 1996 : REV 0 : Hugo DesRosiers : Creation.
**************************************************/

#include "jmachine.h"
#include "stacks.h"
#include "math64bits.h"


class JVMachine2 : public JVMachine {
  protected:
    typedef void (JVMachine2::*instructionFct)(void);
    typedef instructionFct *fctArrayPtr;
    union UFloatAndInt {
	int i;
	float f;
    };


  protected:	// Class variables.
    static fctArrayPtr instructionArray;

  protected:	// Class methods.
    static void loadJVBasicInstructions(void);

  protected:
    ClassFile *aClass;
    FieldInfo *aField;
    MethodInfo *aMethod;
    StringInfo *aCteString;
    JVMethod *nextMethod;
    JVVariable *fieldDesc;
    JVInstance *anInstance;
    JVExecFrame *newExecFrame;
    JVRunValue genValue, genValue2;
    Value64Bits arg64Bits[3];
    union UFloatAndInt _fai, arg32Bits[3];
    int  anInt, tmpInt, tmpLong[2];
    unsigned int anUInt, tmpPCHolder, *tmpDims;
    unsigned int i;
    // short argShort[3];
    short aShort;
    char *initialStr;
    ubyte aByte;
    char signedByte;

  public:
    JVMachine2(JVCup *owner);
    JVMachine2(JVCup *owner, JVAbstractThread *aThread);
    virtual void loadInstructionSet(void);
    virtual void init(void);
    virtual int doInstruction(void);

  protected:
	// Java instruction codes.
    void I_BIPUSH(void);
    void I_SIPUSH(void);
    void I_LDC1(void);
    void I_LDC2(void);
    void I_LDC2W(void);
    void I_ACONST_NULL(void);
    void I_ICONST_M1(void);
    void I_ICONST_0(void);
    void I_ICONST_1(void);
    void I_ICONST_2(void);
    void I_ICONST_3(void);
    void I_ICONST_4(void);
    void I_ICONST_5(void);
    void I_LCONST_0(void);
    void I_LCONST_1(void);
    void I_FCONST_0(void);
    void I_FCONST_1(void);
    void I_FCONST_2(void);
    void I_DCONST_0(void);
    void I_DCONST_1(void);
    void I_ILOAD(void);
    void I_ILOAD_0(void);
    void I_ILOAD_1(void);
    void I_ILOAD_2(void);
    void I_ILOAD_3(void);
    void I_LLOAD(void);
    void I_LLOAD_0(void);
    void I_LLOAD_1(void);
    void I_LLOAD_2(void);
    void I_LLOAD_3(void);
    void I_FLOAD(void);
    void I_FLOAD_0(void);
    void I_FLOAD_1(void);
    void I_FLOAD_2(void);
    void I_FLOAD_3(void);
    void I_DLOAD(void);
    void I_DLOAD_0(void);
    void I_DLOAD_1(void);
    void I_DLOAD_2(void);
    void I_DLOAD_3(void);
    void I_ALOAD(void);
    void I_ALOAD_0(void);
    void I_ALOAD_1(void);
    void I_ALOAD_2(void);
    void I_ALOAD_3(void);
    void I_ISTORE(void);
    void I_ISTORE_0(void);
    void I_ISTORE_1(void);
    void I_ISTORE_2(void);
    void I_ISTORE_3(void);
    void I_LSTORE(void);
    void I_LSTORE_0(void);
    void I_LSTORE_1(void);
    void I_LSTORE_2(void);
    void I_LSTORE_3(void);
    void I_FSTORE(void);
    void I_FSTORE_0(void);
    void I_FSTORE_1(void);
    void I_FSTORE_2(void);
    void I_FSTORE_3(void);
    void I_DSTORE(void);
    void I_DSTORE_0(void);
    void I_DSTORE_1(void);
    void I_DSTORE_2(void);
    void I_DSTORE_3(void);
    void I_ASTORE(void);
    void I_ASTORE_0(void);
    void I_ASTORE_1(void);
    void I_ASTORE_2(void);
    void I_ASTORE_3(void);
    void I_IINC(void);
    void I_WIDE(void);
    void I_NEWARRAY(void);
    void I_ANEWARRAY(void);
    void I_MULTIANEWARRAY(void);
    void I_ARRAYLENGTH(void);
    void I_IALOAD(void);
    void I_LALOAD(void);
    void I_FALOAD(void);
    void I_DALOAD(void);
    void I_AALOAD(void);
    void I_BALOAD(void);
    void I_CALOAD(void);
    void I_SALOAD(void);
    void I_IASTORE(void);
    void I_LASTORE(void);
    void I_FASTORE(void);
    void I_DASTORE(void);
    void I_AASTORE(void);
    void I_BASTORE(void);
    void I_CASTORE(void);
    void I_SASTORE(void);
    void I_NOP(void);
    void I_POP(void);
    void I_POP2(void);
    void I_DUP(void);
    void I_DUP2(void);
    void I_DUP_X1(void);
    void I_DUP2_X1(void);
    void I_DUP_X2(void);
    void I_DUP2_X2(void);
    void I_SWAP(void);
    void I_IADD(void);
    void I_LADD(void);
    void I_FADD(void);
    void I_DADD(void);
    void I_ISUB(void);
    void I_LSUB(void);
    void I_FSUB(void);
    void I_DSUB(void);
    void I_IMUL(void);
    void I_LMUL(void);
    void I_FMUL(void);
    void I_DMUL(void);
    void I_IDIV(void);
    void I_LDIV(void);
    void I_FDIV(void);
    void I_DDIV(void);
    void I_IREM(void);
    void I_LREM(void);
    void I_FREM(void);
    void I_DREM(void);
    void I_INEG(void);
    void I_LNEG(void);
    void I_FNEG(void);
    void I_DNEG(void);
    void I_ISHL(void);
    void I_ISHR(void);
    void I_IUSHR(void);
    void I_LSHL(void);
    void I_LSHR(void);
    void I_LUSHR(void);
    void I_IAND(void);
    void I_LAND(void);
    void I_IOR(void);
    void I_LOR(void);
    void I_IXOR(void);
    void I_LXOR(void);
    void I_I2L(void);
    void I_I2F(void);
    void I_I2D(void);
    void I_L2I(void);
    void I_L2F(void);
    void I_L2D(void);
    void I_F2I(void);
    void I_F2L(void);
    void I_F2D(void);
    void I_D2I(void);
    void I_D2L(void);
    void I_D2F(void);
    void I_INT2BYTE(void);
    void I_INT2CHAR(void);
    void I_INT2SHORT(void);
    void I_IFEQ(void);
    void I_IFNULL(void);
    void I_IFLT(void);
    void I_IFLE(void);
    void I_IFNE(void);
    void I_IFNONNULL(void);
    void I_IFGT(void);
    void I_IFGE(void);
    void I_IF_ICMPEQ(void);
    void I_IF_ICMPNE(void);
    void I_IF_ICMPLT(void);
    void I_IF_ICMPGT(void);
    void I_IF_ICMPLE(void);
    void I_IF_ICMPGE(void);
    void I_LCMP(void);
    void I_FCMPL(void);
    void I_FCMPG(void);
    void I_DCMPL(void);
    void I_DCMPG(void);
    void I_IF_ACMPEQ(void);
    void I_IF_ACMPNE(void);
    void I_GOTO(void);
    void I_GOTO_W(void);
    void I_JSR(void);
    void I_JSR_W(void);
    void I_RET(void);
    void I_RET_W(void);
    void I_IRETURN(void);
    void I_LRETURN(void);
    void I_FRETURN(void);
    void I_DRETURN(void);
    void I_ARETURN(void);
    void I_RETURN(void);
    void I_BREAKPOINT(void);
    void I_TABLESWITCH(void);
    void I_LOOKUPSWITCH(void);
    void I_PUTFIELD(void);
    void I_GETFIELD(void);
    void I_PUTSTATIC(void);
    void I_GETSTATIC(void);
    void I_INVOKEVIRTUAL(void);
    void I_INVOKENONVIRTUAL(void);
    void I_INVOKESTATIC(void);
    void I_INVOKEINTERFACE(void);
    void I_ATHROW(void);
    void I_NEW(void);
    void I_CHECKCAST(void);
    void I_INSTANCEOF(void);
    void I_MONITORENTER(void);
    void I_MONITOREXIT(void);
    void I_UNKNOWN(void);
#if defined(PLZEN_LEVEL_2)
    void I_FIRE(void);
    void I_FIRESTATIC(void);
    void I_FIREFIELD(void);
    void I_WAIT(void);
#endif
};

