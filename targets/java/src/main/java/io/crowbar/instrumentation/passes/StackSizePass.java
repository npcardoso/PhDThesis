package io.crowbar.instrumentation.passes;

import javassist.CtClass;
import javassist.CtMethod;
import javassist.CtField;
import javassist.bytecode.MethodInfo;
import javassist.bytecode.CodeAttribute;
import javassist.bytecode.CodeIterator;
import javassist.bytecode.Bytecode;
import javassist.bytecode.ConstPool;
import javassist.bytecode.Opcode;

public class StackSizePass extends AbstractPass {
    @Override
    public final Outcome transform (CtClass c) throws Exception {
        for (CtMethod m : c.getDeclaredMethods()) {
            MethodInfo info = m.getMethodInfo();
            CodeAttribute ca = info.getCodeAttribute();

            if (ca == null)
                continue;

            ca.setMaxStack(ca.computeMaxStack());
        }

        return Outcome.CONTINUE;
    }
}