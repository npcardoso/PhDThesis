package io.crowbar.instrumentation.passes.wrappers;

import io.crowbar.diagnostic.spectrum.Node;
import io.crowbar.instrumentation.passes.matchers.ActionTaker;
import io.crowbar.instrumentation.passes.matchers.AndMatcher;
import io.crowbar.instrumentation.passes.matchers.OrMatcher;
import io.crowbar.instrumentation.passes.matchers.AnnotationMatcher;
import io.crowbar.instrumentation.passes.matchers.ReturnTypeMatcher;
import io.crowbar.instrumentation.passes.matchers.WhiteList;
import io.crowbar.instrumentation.runtime.Collector;

import java.lang.reflect.Method;

import javassist.CtClass;
import javassist.CtMethod;


public final class JUnit4TestWrapper implements TestWrapper {
    private static final ActionTaker ACTION_TAKER =
        new WhiteList(
            new AndMatcher(
                new OrMatcher(
                    new AnnotationMatcher("org.junit.Test"),
                    new AnnotationMatcher("org.junit.experimental.theories.Theory")),
                new ReturnTypeMatcher("void")));

    private static boolean isSameType (Object o,
                                       String type) {
        try {
            Class< ? > cls = Class.forName(type);
            return cls.isAssignableFrom(o.getClass());
        }
        catch (ClassNotFoundException e) {
            return false;
        }
    }

    public static final boolean isPass (Throwable e,
                                        String expected) {
        if (e == null)
            return true;

        if (isSameType(e, "org.junit.Assume$AssumptionViolatedException"))
            return true;

        if (isSameType(e, "org.junit.internal.AssumptionViolatedException"))
            return true;

        if (isSameType(e, expected))
            return true;

        return false;
    }

    @Override
    public Action getAction (CtClass c) {
        return ACTION_TAKER.getAction(c);
    }

    @Override
    public Action getAction (CtClass c,
                             CtMethod m) {
        return ACTION_TAKER.getAction(c, m);
    }

    private String getExpectedExceptionName (CtMethod m) {
        String expectedStr = null;


        try {
            Object annotation = m.getAnnotation(Class.forName("org.junit.Test"));
            Method method = annotation.getClass().getMethod("expected");
            Class< ? > expected = (Class< ? > )method.invoke(annotation);
            expectedStr = expected.getName();
        }
        catch (Throwable e) {}

        return expectedStr;
    }

    @Override
    public String getOracleCode (CtClass c,
                                 CtMethod m,
                                 Node n,
                                 int probeId,
                                 String collectorVar,
                                 String exceptionVar) {
        String expectedStr = getExpectedExceptionName(m);


        if (expectedStr == null)
            expectedStr = "\"\"";
        else
            expectedStr = "\"" + expectedStr + "\"";

        String oracleCall =
            getClass().getName() + ".isPass(" +
            exceptionVar + ", " +
            expectedStr + ")";
        return "if(" + oracleCall + ") throw " + exceptionVar + ";";
    }

    @Override
    public boolean isDefaultPass (CtClass c,
                                  CtMethod m) {
        String expectedStr = getExpectedExceptionName(m);


        return expectedStr == null;
    }
}