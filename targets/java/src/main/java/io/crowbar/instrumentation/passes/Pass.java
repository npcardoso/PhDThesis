package io.crowbar.instrumentation.passes;

import javassist.CtClass;
import javassist.CtMethod;


public interface Pass {
    public static enum Outcome {CONTINUE, ABORT, RETURN};

    public Outcome transform (CtClass c) throws Exception;
}