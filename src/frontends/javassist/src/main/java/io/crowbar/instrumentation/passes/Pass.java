package io.crowbar.instrumentation.passes;

import java.util.logging.Logger;
import javassist.ClassPool;
import javassist.CtClass;



public abstract class Pass {
    public static class IgnoreClassException extends Exception {};

    public abstract void transform(CtClass c) throws Exception;
}
