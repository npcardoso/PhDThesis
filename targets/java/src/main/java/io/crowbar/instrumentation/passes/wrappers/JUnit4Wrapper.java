package io.crowbar.instrumentation.passes.wrappers;

import javassist.*;

public class JUnit4Wrapper implements Wrapper {
    @Override
    public boolean matches (CtMethod m) {
        try {
            return m.hasAnnotation(org.junit.Test.class);
        }
        catch (Exception e) {}
        return false;
    }
}