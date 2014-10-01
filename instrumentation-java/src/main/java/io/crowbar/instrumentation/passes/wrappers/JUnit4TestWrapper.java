package io.crowbar.instrumentation.passes.wrappers;

import io.crowbar.diagnostic.spectrum.Node;
import io.crowbar.instrumentation.passes.matchers.ActionTaker;
import io.crowbar.instrumentation.passes.matchers.AndMatcher;
import io.crowbar.instrumentation.passes.matchers.OrMatcher;
import io.crowbar.instrumentation.passes.matchers.MethodAnnotationMatcher;
import io.crowbar.instrumentation.passes.matchers.ReturnTypeMatcher;
import io.crowbar.instrumentation.passes.matchers.WhiteList;
import io.crowbar.instrumentation.runtime.Collector;


import java.lang.reflect.Method;

import javassist.CtClass;
import javassist.CtMethod;
import java.lang.annotation.Annotation;


public final class JUnit4TestWrapper
extends AbstractTestWrapper {
    private static final String ANNOTATION_CLASS = "org.junit.Test";
    private static final String EXPECTED_NONE_CLASS = ANNOTATION_CLASS + "$None";
    private static final ActionTaker ACTION_TAKER =
        new WhiteList(
            new AndMatcher(
                new OrMatcher(
                    new MethodAnnotationMatcher(ANNOTATION_CLASS),
                    new MethodAnnotationMatcher("org.junit.experimental.theories.Theory")),
                new ReturnTypeMatcher("void")));


    private static Class getExpected (Method m) {
        try {
            Annotation an = m.getAnnotation((Class< ? extends Annotation> )Class.forName(ANNOTATION_CLASS));
            Method method = an.getClass().getMethod("expected");
            return (Class) method.invoke(an);
        }
        catch (Throwable e) {
            e.printStackTrace();
        }

        return null;
    }

    public static boolean isPass (Class cls,
                                  String methodName) {
        try {
            Method method = cls.getMethod(methodName);
            Class c = getExpected(method);
            return (c == null || c.getName() == EXPECTED_NONE_CLASS);
        }
        catch (NoSuchMethodException e) {}
        return true;
    }

    public static boolean isPass (Class cls,
                                  String methodName,
                                  Throwable e) {
        if (e == null)
            return true;

        if (isSameType(e, "org.junit.Assume$AssumptionViolatedException"))
            return true;

        if (isSameType(e, "org.junit.internal.AssumptionViolatedException"))
            return true;

        try {
            Method method = cls.getMethod(methodName);
            Class expected = getExpected(method);
            return isSameType(e, expected);
        }
        catch (NoSuchMethodException ex) {}

        return false;
    }

    @Override
    protected ActionTaker getActionTaker () {
        return ACTION_TAKER;
    }
}