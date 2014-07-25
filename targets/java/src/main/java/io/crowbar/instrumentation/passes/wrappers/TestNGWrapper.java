package io.crowbar.instrumentation.passes.wrappers;

import javassist.CtMethod;

public class TestNGWrapper implements Wrapper {
    @Override
    public boolean matches (CtMethod m) {
        try {
            return m.hasAnnotation(Class.forName("org.testng.annotations.Test"));
        }
        catch (Exception e) {}
        return false;
    }
}