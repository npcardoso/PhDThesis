package io.crowbar.instrumentation.passes;

import javassist.CtClass;

public interface Pass {
    public static enum Outcome {CONTINUE, ABORT, RETURN};

    Outcome transform (CtClass c) throws Exception;
}