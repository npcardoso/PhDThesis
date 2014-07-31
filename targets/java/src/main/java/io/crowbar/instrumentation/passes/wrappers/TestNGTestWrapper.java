package io.crowbar.instrumentation.passes.wrappers;

import io.crowbar.instrumentation.passes.matchers.ActionTaker;
import io.crowbar.instrumentation.passes.matchers.AnnotationMatcher;
import io.crowbar.instrumentation.passes.matchers.WhiteList;

import java.util.Set;

import javassist.CtClass;
import javassist.CtMethod;


public class TestNGTestWrapper implements TestWrapper {
    private static final ActionTaker actionTaker =
        new WhiteList(
            new AnnotationMatcher("org.testng.annotations.Test"));

    @Override
    public Action getAction (CtClass c) {
        return actionTaker.getAction(c);
    }

    @Override
    public Action getAction (CtClass c, CtMethod m) {
        return actionTaker.getAction(c, m);
    }

    @Override
    public Set<String> validExceptions (CtClass c,
                                        CtMethod m) {
        return null;
    }
}