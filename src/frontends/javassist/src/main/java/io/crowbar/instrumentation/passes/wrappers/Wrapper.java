package io.crowbar.instrumentation.passes.wrappers;

import javassist.CtMethod;

public interface Wrapper {
    public boolean matches(CtMethod m);
}
