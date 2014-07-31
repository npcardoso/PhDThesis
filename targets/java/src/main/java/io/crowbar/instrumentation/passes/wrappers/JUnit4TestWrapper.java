package io.crowbar.instrumentation.passes.wrappers;

import io.crowbar.instrumentation.passes.matchers.ActionTaker;
import io.crowbar.instrumentation.passes.matchers.AnnotationMatcher;
import io.crowbar.instrumentation.passes.matchers.WhiteList;

import java.lang.reflect.Method;
import java.util.Set;
import java.util.HashSet;

import javassist.CtClass;
import javassist.CtMethod;


public class JUnit4TestWrapper implements TestWrapper {
    private static final ActionTaker ACTION_TAKER =
        new WhiteList(new AnnotationMatcher("org.junit.Test"));

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
        Set<String> ret = new HashSet<String> ();
        ret.add("org.junit.Assume$AssumptionViolatedException");

        try {
            Object annotation = m.getAnnotation(Class.forName("org.junit.Test"));
            Method method = annotation.getClass().getMethod("expected");
            Class expected = (Class) method.invoke(annotation);
            ret.add(expected.getName());
        }
        catch (Throwable e) {}

        return ret;
    }
}