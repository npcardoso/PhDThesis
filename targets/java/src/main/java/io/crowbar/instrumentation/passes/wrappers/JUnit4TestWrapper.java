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
    private static final ActionTaker actionTaker =
        new WhiteList(new AnnotationMatcher("org.junit.Test"));

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
        Set<String> ret = new HashSet<String> ();
        ret.add("org.junit.Assume$AssumptionViolatedException");

        try {
            Object annotation = m.getAnnotation(Class.forName("org.junit.Test"));
            Method method = annotation.getClass().getMethod("expected");
            Class expected = (Class) method.invoke(annotation);

            ret.add(expected.getName());
        }
        catch (Throwable e) {
            e.printStackTrace();
        }

        return ret;
    }
}