package io.crowbar.instrumentation.passes.matchers;

import javassist.CtClass;
import javassist.CtMethod;


public interface Matcher {
    public boolean matches (CtClass c);
    public boolean matches (CtMethod c);
}