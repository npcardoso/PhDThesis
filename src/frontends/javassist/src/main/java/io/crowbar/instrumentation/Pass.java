package io.crowbar.instrumentation;

import java.util.logging.Logger;
import javassist.ClassPool;
import javassist.CtClass;



public abstract class Pass {
    protected static Logger log = Logger.getLogger("io.crowbar.instrumentation");

    public abstract void transform(ClassPool cp,
                                   CtClass c) throws Exception;
}
