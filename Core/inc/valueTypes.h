#ifndef _VALUETYPES_H_
#define _VALUETYPES_H_
/**************************************************
* File: valueTypes.h.
* Desc: Definitions of the constant representing run-time value types.
* Module: AkraLog : JavaKit.
* Rev: 20 aout 1996 : REV 0 : Hugo DesRosiers : Creation.
**************************************************/

// Meta-types for the type definitions.
#define TYPE_BASIC		(1 << 0)
#define TYPE_CLASS		(1 << 1)
#define TYPE_ARRAY		(1 << 2)
#define TYPE_BASELONG	(1 << 3)
// The following are used to describe JVRunValues on the operations stack.
#define TYPE_BASIC_32	(1 << 3)
#define TYPE_BASIC_64L	(1 << 4)
#define TYPE_BASIC_64H	(1 << 5)
#define TYPE_INSTANCE	(1 << 6)		// Maybe same as TYPE_CLASS.
#define TYPE_BASIC_ARRAY	(1 << 7)		// Maybe same as TYPE_ARRAY.
#define TYPE_INST_ARRAY	(1 << 8)		// Maybe same as TYPE_ARRAY.
#define TYPE_ARRAY_ARRAY	(1 << 9)		// Maybe same as TYPE_ARRAY.


// Definition of the types that are internal to the J-Machine.
#define JVM_TYPE_VOID				'V'
#define JVM_TYPE_SIGNED_BYTE		'B'
#define JVM_TYPE_CHAR				'C'
#define JVM_TYPE_DOUBLE			'D'
#define JVM_TYPE_FLOAT				'F'
#define JVM_TYPE_INTEGER			'I'
#define JVM_TYPE_LONG				'J'
#define JVM_TYPE_UNSIGNED_SHORT	'S'
#define JVM_TYPE_BOOLEAN			'Z'

#endif		/* _VALUETYPES_H_ */
