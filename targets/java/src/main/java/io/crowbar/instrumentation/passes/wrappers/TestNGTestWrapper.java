package io.crowbar.instrumentation.passes.wrappers;

import io.crowbar.instrumentation.passes.matchers.ActionTaker;
import io.crowbar.instrumentation.passes.matchers.AnnotationMatcher;
import io.crowbar.instrumentation.passes.matchers.WhiteList;

import java.util.Set;

import javassist.CtClass;
import javassist.CtMethod;


public class TestNGTestWrapper implements TestWrapper {
    private static final ActionTaker ACTION_TAKER =
        new WhiteList(
            new AnnotationMatcher("org.testng.annotations.Test"));

    @Override
    public final Action getAction (CtClass c) {
        return ACTION_TAKER.getAction(c);
    }

    @Override
    public final Action getAction (CtClass c,
                                   CtMethod m) {
        return ACTION_TAKER.getAction(c, m);
    }

    @Override
    public final Set<String> validExceptions (CtClass c,
                                              CtMethod m) {
        return null;
    }
}