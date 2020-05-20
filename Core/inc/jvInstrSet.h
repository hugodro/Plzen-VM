#ifndef _JAVAINSTSET_H_
#define _JAVAINSTSET_H_
/**************************************************
* File: javaInstSet.h.
* Desc: Set of instructions understood by the Java machine.
* Module: AkraLog : JavaKit.
* Rev: 2 mai 1996 : REV 0 : Hugo DesRosiers : Creation.
**************************************************/

// Section 3.2: Pushing constants onto the stack.

// Push one-byte signed integer
#define JVI_BIPUSH		16
#define JVI_BIPUSH_SIZE	2

// Push two-byte signed integer
#define JVI_SIPUSH		17
#define JVI_SIPUSH_SIZE	3

// Push item from constant pool
#define JVI_LDC1			18
#define JVI_LDC1_SIZE	2

// Push item from constant pool
#define JVI_LDC2			19
#define JVI_LDC2_SIZE	3

// Push long or double from constant pool
#define JVI_LDC2W		20		
#define JVI_LDC2W_SIZE	3

// Push null object
#define JVI_ACONST_NULL	1
#define JVI_ACONST_NULL_SIZE	1


// Push integer constant  -1
#define JVI_ICONST_M1		2
#define JVI_ICONST_M1_SIZE	1

// Push integer constant
#define JVI_ICONST_0			3
#define JVI_ICONST_0_SIZE	1

// Push integer constant
#define JVI_ICONST_1			4
#define JVI_ICONST_1_SIZE	1

// Push integer constant
#define JVI_ICONST_2			5
#define JVI_ICONST_2_SIZE	1

// Push integer constant
#define JVI_ICONST_3			6
#define JVI_ICONST_3_SIZE	1

// Push integer constant
#define JVI_ICONST_4			7
#define JVI_ICONST_4_SIZE	1

// Push integer constant
#define JVI_ICONST_5			8
#define JVI_ICONST_5_SIZE	1

// Push long integer constant
#define JVI_LCONST_0		9
#define JVI_LCONST_0_SIZE	1

// Push long integer constant
#define JVI_LCONST_1		10
#define JVI_LCONST_1_SIZE	1

// Push single float
#define JVI_FCONST_0		11
#define JVI_FCONST_0_SIZE	1

// Push single float
#define JVI_FCONST_1		12
#define JVI_FCONST_1_SIZE	1

// Push single float
#define JVI_FCONST_2		13
#define JVI_FCONST_2_SIZE	1

// Push double float
#define JVI_DCONST_0		14
#define JVI_DCONST_0_SIZE	1

// Push double float
#define JVI_DCONST_1		15
#define JVI_DCONST_1_SIZE	1


// Section 3.3: Loading Local Variables Onto the Stack.

// Load integer from local variable
#define JVI_ILOAD			21
#define JVI_ILOAD_SIZE		2

// Load integer from local variable
#define JVI_ILOAD_0			26
#define JVI_ILOAD_0_SIZE		1

// Load integer from local variable
#define JVI_ILOAD_1			27
#define JVI_ILOAD_1_SIZE		1

// Load integer from local variable
#define JVI_ILOAD_2			28
#define JVI_ILOAD_2_SIZE		1

// Load integer from local variable
#define JVI_ILOAD_3			29
#define JVI_ILOAD_3_SIZE		1

// Load long integer from local variable
#define JVI_LLOAD			22
#define JVI_LLOAD_SIZE		2

// Load long integer from local variable
#define JVI_LLOAD_0			30
#define JVI_LLOAD_0_SIZE	1

// Load long integer from local variable
#define JVI_LLOAD_1			31
#define JVI_LLOAD_1_SIZE	1

// Load long integer from local variable
#define JVI_LLOAD_2			32
#define JVI_LLOAD_2_SIZE	1

// Load long integer from local variable
#define JVI_LLOAD_3			33
#define JVI_LLOAD_3_SIZE	1

// Load single float from local variable
#define JVI_FLOAD			23
#define JVI_FLOAD_SIZE		2

// Load single float from local variable
#define JVI_FLOAD_0			34
#define JVI_FLOAD_0_SIZE	1

// Load single float from local variable
#define JVI_FLOAD_1			35
#define JVI_FLOAD_1_SIZE	1

// Load single float from local variable
#define JVI_FLOAD_2			36
#define JVI_FLOAD_2_SIZE	1

// Load single float from local variable
#define JVI_FLOAD_3			37
#define JVI_FLOAD_3_SIZE	1

// Load double float from local variable
#define JVI_DLOAD			24
#define JVI_DLOAD_SIZE		2

// Load double float from local variable
#define JVI_DLOAD_0			38
#define JVI_DLOAD_0_SIZE	1

// Load double float from local variable
#define JVI_DLOAD_1			39
#define JVI_DLOAD_1_SIZE	1

// Load double float from local variable
#define JVI_DLOAD_2			40
#define JVI_DLOAD_2_SIZE	1

// Load double float from local variable
#define JVI_DLOAD_3			41
#define JVI_DLOAD_3_SIZE	1

// Load object reference from local variable
#define JVI_ALOAD			25
#define JVI_ALOAD_SIZE		2

// Load object reference from local variable
#define JVI_ALOAD_0			42
#define JVI_ALOAD_0_SIZE	1

// Load object reference from local variable
#define JVI_ALOAD_1			43
#define JVI_ALOAD_1_SIZE	1

// Load object reference from local variable
#define JVI_ALOAD_2			44
#define JVI_ALOAD_2_SIZE	1

// Load object reference from local variable
#define JVI_ALOAD_3			45
#define JVI_ALOAD_3_SIZE	1


// Section 3.4: Storing Stack Values into Local Variables.

// Store integer into local variable
#define JVI_ISTORE			54
#define JVI_ISTORE_SIZE		2

// Store integer into local variable
#define JVI_ISTORE_0			59
#define JVI_ISTORE_0_SIZE	1

// Store integer into local variable
#define JVI_ISTORE_1			60
#define JVI_ISTORE_1_SIZE	1

// Store integer into local variable
#define JVI_ISTORE_2			61
#define JVI_ISTORE_2_SIZE	1

// Store integer into local variable
#define JVI_ISTORE_3			62
#define JVI_ISTORE_3_SIZE	1

// Store long integer into local variable
#define JVI_LSTORE			55
#define JVI_LSTORE_SIZE		2

// Store long integer into local variable
#define JVI_LSTORE_0		63
#define JVI_LSTORE_0_SIZE	1

// Store long integer into local variable
#define JVI_LSTORE_1		64
#define JVI_LSTORE_1_SIZE	1

// Store long integer into local variable
#define JVI_LSTORE_2		65
#define JVI_LSTORE_2_SIZE	1

// Store long integer into local variable
#define JVI_LSTORE_3		66
#define JVI_LSTORE_3_SIZE	1

// Store single float into local variable
#define JVI_FSTORE			56
#define JVI_FSTORE_SIZE		2

// Store single float into local variable
#define JVI_FSTORE_0		67
#define JVI_FSTORE_0_SIZE	1

// Store single float into local variable
#define JVI_FSTORE_1		68
#define JVI_FSTORE_1_SIZE	1

// Store single float into local variable
#define JVI_FSTORE_2		69
#define JVI_FSTORE_2_SIZE	1

// Store single float into local variable
#define JVI_FSTORE_3		70
#define JVI_FSTORE_3_SIZE	1

// Store double float into local variable
#define JVI_DSTORE			57
#define JVI_DSTORE_SIZE		2

// Store double float into local variable
#define JVI_DSTORE_0		71
#define JVI_DSTORE_0_SIZE	1

// Store double float into local variable
#define JVI_DSTORE_1		72
#define JVI_DSTORE_1_SIZE	1

// Store double float into local variable
#define JVI_DSTORE_2		73
#define JVI_DSTORE_2_SIZE	1

// Store double float into local variable
#define JVI_DSTORE_3		74
#define JVI_DSTORE_3_SIZE	1

// Store double float into local variable
#define JVI_ASTORE			58
#define JVI_ASTORE_SIZE		2

// Store double float into local variable
#define JVI_ASTORE_0		75
#define JVI_ASTORE_0_SIZE	1

// Store double float into local variable
#define JVI_ASTORE_1		76
#define JVI_ASTORE_1_SIZE	1

// Store double float into local variable
#define JVI_ASTORE_2		77
#define JVI_ASTORE_2_SIZE	1

// Store double float into local variable
#define JVI_ASTORE_3		78
#define JVI_ASTORE_3_SIZE	1

// Increment local variable by constant
#define JVI_IINC			132
#define JVI_IINC_SIZE		3


// Section 3.5: Wider index for Loading, Storing and Incrementing.

// Wider index for accessing local variables in load, store and increment.
#define JVI_WIDE			196
#define JVI_WIDE_SIZE	2


// Section 3.6: Managing Arrays.

// Allocate new array
#define JVI_NEWARRAY		188
#define JVI_NEWARRAY_SIZE	2

// Allocate new array
#define JVI_ANEWARRAY			189
#define JVI_ANEWARRAY_SIZE	3

// Allocate new multi-dimensional array
#define JVI_MULTIANEWARRAY	197
#define JVI_MULTIANEWARRAY_SIZE	4

// Get length of array
#define JVI_ARRAYLENGTH	190
#define JVI_ARRAYLENGTH_SIZE	1

// Load integer from array
#define JVI_IALOAD			46
#define JVI_IALOAD_SIZE		1

// Load long integer from array
#define JVI_LALOAD			47
#define JVI_LALOAD_SIZE		1

// Load single float from array
#define JVI_FALOAD			48
#define JVI_FALOAD_SIZE		1

// Load double float from array
#define JVI_DALOAD			49
#define JVI_DALOAD_SIZE		1

// Load object reference from array
#define JVI_AALOAD			50
#define JVI_AALOAD_SIZE		1

// Load signed byte from array.
#define JVI_BALOAD			51
#define JVI_BALOAD_SIZE		1

// Load character from array
#define JVI_CALOAD			52
#define JVI_CALOAD_SIZE		1

// Load short from array
#define JVI_SALOAD			53
#define JVI_SALOAD_SIZE		1

// Store into integer array
#define JVI_IASTORE			79
#define JVI_IASTORE_SIZE	1

// Store into long integer array
#define JVI_LASTORE			80
#define JVI_LASTORE_SIZE	1

// Store into single float array
#define JVI_FASTORE			81
#define JVI_FASTORE_SIZE	1

// Store into double float array
#define JVI_DASTORE			82
#define JVI_DASTORE_SIZE	1

// Store into object reference array
#define JVI_AASTORE			83
#define JVI_AASTORE_SIZE	1

// Store into signed byte array
#define JVI_BASTORE			84
#define JVI_BASTORE_SIZE	1

// Store into character array
#define JVI_CASTORE			85
#define JVI_CASTORE_SIZE	1

// Store into short array
#define JVI_SASTORE			86
#define JVI_SASTORE_SIZE	1


// Section 3.7: Stack Instructions.

// Do nothing.
#define JVI_NOP			0
#define JVI_NOP_SIZE		1

// Pop top stack word
#define JVI_POP			87
#define JVI_POP_SIZE		1

// Pop the top two words from the stack.
#define JVI_POP2			88
#define JVI_POP2_SIZE	1

// Duplicate top stack word.
#define JVI_DUP			89
#define JVI_DUP_SIZE		1

// Duplicate top two stack word.
#define JVI_DUP2			92
#define JVI_DUP2_SIZE	1	

// Duplicate top stack word and put two down.
#define JVI_DUP_X1		90
#define JVI_DUP_X1_SIZE	1

// Duplicate top two stack words and put two down.
#define JVI_DUP2_X1			93
#define JVI_DUP2_X1_SIZE	1

// Duplicate top stack word and put three down.
#define JVI_DUP_X2		91
#define JVI_DUP_X2_SIZE	1

// Duplicate top two stack words and put three down.
#define JVI_DUP2_X2			94
#define JVI_DUP2_X2_SIZE	1

// Swap top two stack words.
#define JVI_SWAP			95
#define JVI_SWAP_SIZE		1


// Section 3.8: Arithmetic Instructions.

// Integer add
#define JVI_IADD			96
#define JVI_IADD_SIZE	1

// Long integer add
#define JVI_LADD			97
#define JVI_LADD_SIZE	1

// Single floats add
#define JVI_FADD			98
#define JVI_FADD_SIZE	1

// Double floats add
#define JVI_DADD		99
#define JVI_DADD_SIZE	1

// Integer subtract.
#define JVI_ISUB			100
#define JVI_ISUB_SIZE	1

// Long integer subtract.
#define JVI_LSUB			101
#define JVI_LSUB_SIZE	1

// Single float subtract.
#define JVI_FSUB			102
#define JVI_FSUB_SIZE	1

// Double float subtract.
#define JVI_DSUB			103
#define JVI_DSUB_SIZE	1

// Integer multiply.
#define JVI_IMUL			104
#define JVI_IMUL_SIZE	1

// Long integer multiply.
#define JVI_LMUL			105
#define JVI_LMUL_SIZE	1

// Single float multiply.
#define JVI_FMUL			106
#define JVI_FMUL_SIZE	1

// Double float multiply.
#define JVI_DMUL			107
#define JVI_DMUL_SIZE	1

// Integer divide.
#define JVI_IDIV			108
#define JVI_IDIV_SIZE		1

// Long integer divide.
#define JVI_LDIV			109
#define JVI_LDIV_SIZE	1

// Single float divide.
#define JVI_FDIV			110
#define JVI_FDIV_SIZE	1

// Double float divide.
#define JVI_DDIV			111
#define JVI_DDIV_SIZE	1

// Integer remainder.
#define JVI_IREM			112
#define JVI_IREM_SIZE	1

// Long integer remainder.
#define JVI_LREM			113
#define JVI_LREM_SIZE	1

// Single float remainder.
#define JVI_FREM			114
#define JVI_FREM_SIZE	1

// Double float remainder.
#define JVI_DREM		115
#define JVI_DREM_SIZE	1

// Integer negate.
#define JVI_INEG			116
#define JVI_INEG_SIZE	1

// Long Integer negate.
#define JVI_LNEG			117
#define JVI_LNEG_SIZE	1

// Single float negate.
#define JVI_FNEG			118
#define JVI_FNEG_SIZE	1

// Double float negate.
#define JVI_DNEG			119
#define JVI_DNEG_SIZE	1

// Integer shift left.
#define JVI_ISHL			120
#define JVI_ISHL_SIZE	1

// Integer arithmetic shift right.
#define JVI_ISHR			122
#define JVI_ISHR_SIZE	1

// Integer logical shift right.
#define JVI_IUSHR		124
#define JVI_IUSHR_SIZE	1

// Long integer shift left.
#define JVI_LSHL			121
#define JVI_LSHL_SIZE	1

// Long integer arithmetic shift right.
#define JVI_LSHR			123
#define JVI_LSHR_SIZE	1

// Long integer logical shift right.
#define JVI_LUSHR		125
#define JVI_LUSHR_SIZE	1

// Integer boolean AND.
#define JVI_IAND			126
#define JVI_IAND_SIZE	1

// Long integer boolean AND.
#define JVI_LAND			127
#define JVI_LAND_SIZE	1

// Integer boolean OR.
#define JVI_IOR			128
#define JVI_IOR_SIZE		1

// Long integer boolean OR.
#define JVI_LOR			129
#define JVI_LOR_SIZE		1

// Integer boolean XOR.
#define JVI_IXOR			130
#define JVI_IXOR_SIZE	1

// Long integer boolean XOR.
#define JVI_LXOR			131
#define JVI_LXOR_SIZE	1


// Section 3.10: Conversion Operations.

// Integer to long integer conversion.
#define JVI_I2L			133
#define JVI_I2L_SIZE		1

// IInteger to single float conversion.
#define JVI_I2F			134
#define JVI_I2F_SIZE		1

// Integer to double float conversion.
#define JVI_I2D			135
#define JVI_I2D_SIZE		1

// Long integer to integer conversion.
#define JVI_L2I			136
#define JVI_L2I_SIZE		1

// Long integer to single float conversion.
#define JVI_L2F			137
#define JVI_L2F_SIZE		1

// Long integer to double float conversion.
#define JVI_L2D			138
#define JVI_L2D_SIZE		1

// Single float to integer conversion.
#define JVI_F2I			139
#define JVI_F2I_SIZE		1

// Single float to long integer conversion.
#define JVI_F2L			140
#define JVI_F2L_SIZE		1

// Single float to double float conversion.
#define JVI_F2D			141
#define JVI_F2D_SIZE		1

// Double float to integer conversion.
#define JVI_D2I			142
#define JVI_D2I_SIZE		1

// Double float to long integer conversion.
#define JVI_D2L			143
#define JVI_D2L_SIZE		1

// Double float to single float conversion.
#define JVI_D2F			144
#define JVI_D2F_SIZE		1

// Integer to signed byte conversion.
#define JVI_INT2BYTE			145
#define JVI_INT2BYTE_SIZE	1

// Integer to char conversion.
#define JVI_INT2CHAR			146
#define JVI_INT2CHAR_SIZE	1

// Integer to char conversion.
#define JVI_INT2SHORT		147
#define JVI_INT2SHORT_SIZE	1


// Section 3.11: Control Transfer Instructions.

// Branch if equal.
#define JVI_IFEQ			153
#define JVI_IFEQ_SIZE	3

// Branch if null.
#define JVI_IFNULL		198	
#define JVI_IFNULL_SIZE	3

// Branch if less than.
#define JVI_IFLT			155
#define JVI_IFLT_SIZE		3

// Branch if less than or equal.
#define JVI_IFLE			158
#define JVI_IFLE_SIZE		3

// Branch if not equal.
#define JVI_IFNE			154
#define JVI_IFNE_SIZE	3

// Branch if not null.
#define JVI_IFNONNULL		199
#define JVI_IFNONNULL_SIZE	3

// Branch if greater than.
#define JVI_IFGT			157
#define JVI_IFGT_SIZE	3

// Branch if greater than or equal.
#define JVI_IFGE			156
#define JVI_IFGE_SIZE	3

// Branch if integers equal.
#define JVI_IF_ICMPEQ		159
#define JVI_IF_ICMPEQ_SIZE	3	

// Branch if integers not equal.
#define JVI_IF_ICMPNE		160
#define JVI_IF_ICMPNE_SIZE	3

// Branch if integer less than.
#define JVI_IF_ICMPLT		161
#define JVI_IF_ICMPLT_SIZE	3	

// Branch if integer greater than.
#define JVI_IF_ICMPGT		163
#define JVI_IF_ICMPGT_SIZE	3

// Branch if integer less than or equal to.
#define JVI_IF_ICMPLE		164
#define JVI_IF_ICMPLE_SIZE	3

// Branch if integer greater than or equal to.
#define JVI_IF_ICMPGE		162
#define JVI_IF_ICMPGE_SIZE	3

// Long integer compare.
#define JVI_LCMP				148
#define JVI_LCMP_SIZE		1

// Single float compare (-1 on NaN).
#define JVI_FCMPL			149
#define JVI_FCMPL_SIZE		1

// Single float compare (1 on NaN).
#define JVI_FCMPG			150
#define JVI_FCMPG_SIZE		1

// Double float compare (-1 on NaN).
#define JVI_DCMPL			151
#define JVI_DCMPL_SIZE		1

// Double float compare (1 on NaN).
#define JVI_DCMPG			152
#define JVI_DCMPG_SIZE		1

// Branch if object references are equal.
#define JVI_IF_ACMPEQ		165
#define JVI_IF_ACMPEQ_SIZE	3

// Branch if object references not equal.
#define JVI_IF_ACMPNE		166	
#define JVI_IF_ACMPNE_SIZE	3

// Branch always.
#define JVI_GOTO			167
#define JVI_GOTO_SIZE		3

// Branch always (wide index).
#define JVI_GOTO_W			200
#define JVI_GOTO_W_SIZE	5

// Jump subroutine.
#define JVI_JSR				168
#define JVI_JSR_SIZE			3

// Jump subroutine (wide index).
#define JVI_JSR_W			201
#define JVI_JSR_W_SIZE		5

// Return from subroutine.
#define JVI_RET				169
#define JVI_RET_SIZE			2

// Return from subroutine (wide index).
#define JVI_RET_W			209
#define JVI_RET_W_SIZE		3


// Section 3.12: Function return.

// Return integer from function.
#define JVI_IRETURN			172
#define JVI_IRETURN_SIZE		1

// Return long integer from function.
#define JVI_LRETURN			173
#define JVI_LRETURN_SIZE	1

// Return single float from function.
#define JVI_FRETURN			174
#define JVI_FRETURN_SIZE	1

// Return double float from function.
#define JVI_DRETURN			175
#define JVI_DRETURN_SIZE	1

// Return object reference from function.
#define JVI_ARETURN			176
#define JVI_ARETURN_SIZE	1

// Return (void) from procedure.
#define JVI_RETURN			177
#define JVI_RETURN_SIZE		1

// Stop and pass control to breakpoint handler.
#define JVI_BREAKPOINT			202
#define JVI_BREAKPOINT_SIZE		1


// Section 3.13: Table Jumping.

// Access jump table by index and jump.
#define JVI_TABLESWITCH			170
#define JVI_TABLESWITCH_SIZE	13 /* + var size. */

// Access jump table by index and jump.
#define JVI_LOOKUPSWITCH			171
#define JVI_LOOKUPSWITCH_SIZE		9 /* + var size. */


// Section 3.14: Manipulating Object Fields.

// Set field in object.
#define JVI_PUTFIELD			181
#define JVI_PUTFIELD_SIZE	3

// Fetch field from object.
#define JVI_GETFIELD			180
#define JVI_GETFIELD_SIZE	3

// Set static field in class.
#define JVI_PUTSTATIC			179
#define JVI_PUTSTATIC_SIZE	3

// Get static field from class.
#define JVI_GETSTATIC			178
#define JVI_GETSTATIC_SIZE	3


// Section 3.15: Method Invocation.

// Invoke instance method.
#define JVI_INVOKEVIRTUAL		182
#define JVI_INVOKEVIRTUAL_SIZE	3

// Invoke instance method, dispatching based on compile-time type.
#define JVI_INVOKENONVIRTUAL		183
#define JVI_INVOKENONVIRTUAL_SIZE	3

// Invoke a class (static) method.
#define JVI_INVOKESTATIC		184
#define JVI_INVOKESTATIC_SIZE	3

// Invoke interface method.
#define JVI_INVOKEINTERFACE		185
#define JVI_INVOKEINTERFACE_SIZE	5


// Section 3.16: Exception handling.

// Throw exception.
#define JVI_ATHROW			191
#define JVI_ATHROW_SIZE	1


// Section 3.17: Miscellaneous Object Operations.

// Create new object
#define JVI_NEW			187
#define JVI_NEW_SIZE	3

// Make sure object is of given type.
#define JVI_CHECKCAST			192
#define JVI_CHECKCAST_SIZE		3

// Determine if an object is of given type.
#define JVI_INSTANCEOF			193
#define JVI_INSTANCEOF_SIZE	3

// Section 3.18: Monitors.

// Enter monitored region of code.
#define JVI_MONITORENTER		194
#define JVI_MONITORENTER_SIZE	1

// Exit monitored region of code.
#define JVI_MONITOREXIT			195
#define JVI_MONITOREXIT_SIZE	1
	
#if defined(PLZEN_LEVEL_2)
// TMPTMP: Should not be here, but while the sub-class of the JVMachine is
//	problematic in nextstep, it is.

// Fire from a local variable.
#define  JVI_FIRE					255
#define  JVI_FIRE_SIZE			3
#define  JVI_FIRESTATIC			254
#define  JVI_FIRESTATIC_SIZE		3
#define  JVI_FIREFIELD			253
#define  JVI_FIREFIELD_SIZE		3
#define  JVI_WAIT					252
#define  JVI_WAIT_SIZE			3

#endif	


#endif	/* _JAVAINSTSET_H_ */
