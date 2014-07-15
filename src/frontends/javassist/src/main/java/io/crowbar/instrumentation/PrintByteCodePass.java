package io.crowbar.instrumentation;

import javassist.*;


public class PrintByteCodePass extends Pass {
    @Override
    public void transform(ClassPool cp,
                          CtClass c) throws Exception {
        System.out.println(c);
        for(CtMethod m : c.getDeclaredMethods()) {
            System.out.println(m);
        }
    }
}
