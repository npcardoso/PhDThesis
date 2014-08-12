package io.crowbar.instrumentation.passes;

import javassist.CtClass;
import javassist.CtMethod;
import javassist.bytecode.MethodInfo;
import javassist.bytecode.CodeAttribute;

public class StackSizePass extends AbstractPass {
    @Override
    public final Outcome transform (CtClass c) throws Exception {
        for (CtMethod m : c.getDeclaredMethods()) {
            MethodInfo info = m.getMethodInfo();
            CodeAttribute ca = info.getCodeAttribute();

            if (ca == null)
                continue;

            int ss = ca.computeMaxStack();
            ca.setMaxStack(ss);
        }

        return Outcome.CONTINUE;
    }
}