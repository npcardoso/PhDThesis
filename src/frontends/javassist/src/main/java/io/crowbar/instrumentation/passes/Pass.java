package io.crowbar.instrumentation.passes;

import io.crowbar.instrumentation.runtime.*;

import java.util.logging.Logger;
import javassist.ClassPool;
import javassist.CtClass;



public abstract class Pass {
    public static class IgnoreClassException extends Exception {};

    public abstract void transform(CtClass c,
                                   ProbeSet ps) throws Exception;
}
