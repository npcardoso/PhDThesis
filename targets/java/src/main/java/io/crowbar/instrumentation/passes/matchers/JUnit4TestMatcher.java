package io.crowbar.instrumentation.passes.matchers;

import javassist.CtClass;
import javassist.CtMethod;


public class JUnit4TestMatcher implements Matcher {
    private static final Matcher matcher = new AnnotationMatcher("org.junit.Test");

    @Override
    public final boolean matches (CtClass c) {
        return false;
    }

    @Override
    public final boolean matches (CtMethod m) {
        return matcher.matches(m);
    }
}