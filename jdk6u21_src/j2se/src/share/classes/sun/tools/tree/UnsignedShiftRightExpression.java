/*
 * @(#)UnsignedShiftRightExpression.java	1.24 10/03/23
 *
 * Copyright (c) 2006, Oracle and/or its affiliates. All rights reserved.
 * ORACLE PROPRIETARY/CONFIDENTIAL. Use is subject to license terms.
 */

package sun.tools.tree;
import sun.tools.java.*;
import sun.tools.asm.Assembler;

/**
 * WARNING: The contents of this source file are not part of any
 * supported API.  Code that depends on them does so at its own risk:
 * they are subject to change or removal without notice.
 */
public
class UnsignedShiftRightExpression extends BinaryShiftExpression {
    /**
     * constructor
     */
    public UnsignedShiftRightExpression(long where, Expression left, Expression right) {
	super(URSHIFT, where, left, right);
    }

    /**
     * Evaluate
     */
    Expression eval(int a, int b) {
	return new IntExpression(where, a >>> b);
    }
    Expression eval(long a, long b) {
	return new LongExpression(where, a >>> b);
    }

    /**
     * Simplify
     */
    Expression simplify() {
	if (right.equals(0)) {
	    return left;
	}
	if (left.equals(0)) {
	    return new CommaExpression(where, right, left).simplify();
	}
	return this;
    }

    /**
     * Code
     */
    void codeOperation(Environment env, Context ctx, Assembler asm) {
	asm.add(where, opc_iushr + type.getTypeCodeOffset());
    }
}
