package io.crowbar.instrumentation.passes.wrappers;

import io.crowbar.instrumentation.passes.matchers.ActionTaker;
import io.crowbar.instrumentation.passes.matchers.AndMatcher;
import io.crowbar.instrumentation.passes.matchers.AnnotationMatcher;
import io.crowbar.instrumentation.passes.matchers.ReturnTypeMatcher;
import io.crowbar.instrumentation.passes.matchers.WhiteList;
import io.crowbar.instrumentation.runtime.Collector;
import io.crowbar.instrumentation.runtime.Node;
import io.crowbar.instrumentation.runtime.Probe;

import java.lang.reflect.Method;

import javassist.CtClass;
import javassist.CtMethod;


public class JUnit4TestWrapper implements TestWrapper {
    private static final ActionTaker ACTION_TAKER =
        new WhiteList(
        	new AndMatcher(
        		new AnnotationMatcher("org.junit.Test"),
        		new ReturnTypeMatcher("void")));

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
    public final String getOracleCode (CtClass c,
                                       CtMethod m,
                                       Node n,
                                       Probe p,
                                       String collectorVar,
                                       String exceptionVar) {
        String expectedStr = null;


        try {
            Object annotation = m.getAnnotation(Class.forName("org.junit.Test"));
            Method method = annotation.getClass().getMethod("expected");
            Class expected = (Class) method.invoke(annotation);
            expectedStr = expected.getName();
        }
        catch (Throwable e) {}

        if (expectedStr == null)
            expectedStr = "\"\"";
        else
            expectedStr = "\"" + expectedStr + "\"";

        String oracleCall =
            getClass().getName() + ".isPass(" +
            collectorVar + ", " +
            p.getId() + ", " +
            exceptionVar + ", " +
            expectedStr + ")";
        return "if(" + oracleCall + ") throw " + exceptionVar + ";";
    }

    private static boolean isSameType (Object o,
                                       String type) {
        try {
            Class cls = Class.forName(type);
            return cls.isAssignableFrom(o.getClass());
        }
        catch (ClassNotFoundException e) {
            return false;
        }
    }

    public static final boolean isPass (Collector c,
                                        int probeId,
                                        Throwable e,
                                        String expected) {
        if (isSameType(e, "org.junit.Assume$AssumptionViolatedException"))
            return true;

        if (isSameType(e, expected))
            return true;

        return false;
    }
}