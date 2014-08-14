package io.crowbar.instrumentation.passes.matchers;

import javassist.CtClass;
import javassist.CtMethod;
import javassist.NotFoundException;

public class ReturnTypeMatcher implements Matcher {
    private final String returnType;

    public ReturnTypeMatcher (String returnType) {
        this.returnType = returnType;
    }

    @Override
    public final boolean matches (CtClass c) {
        return false;
    }

    @Override
    public final boolean matches (CtClass c, CtMethod m) {
        try {
            return returnType.equals(m.getReturnType().getName());
        }
        catch (NotFoundException e) {
            return false;
        }
    }
}