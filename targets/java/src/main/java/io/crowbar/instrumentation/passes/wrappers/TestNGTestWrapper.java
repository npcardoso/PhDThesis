package io.crowbar.instrumentation.passes.wrappers;

import io.crowbar.instrumentation.passes.matchers.ActionTaker;
import io.crowbar.instrumentation.passes.matchers.AnnotationMatcher;
import io.crowbar.instrumentation.passes.matchers.WhiteList;
import io.crowbar.instrumentation.runtime.Node;
import io.crowbar.instrumentation.runtime.Probe;
import io.crowbar.instrumentation.runtime.Collector;

import java.lang.reflect.Method;
import java.util.regex.Pattern;

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
    public final String getOracleCode (CtClass c,
                                       CtMethod m,
                                       Node n,
                                       Probe p,
                                       String collectorVar,
                                       String exceptionVar) {
        Class[] expected = null;
        String expectedMsgRegex = null;

        try {
            Object annotation = m.getAnnotation(Class.forName("org.testng.annotations.Test"));
            Method method = annotation.getClass().getMethod("expectedExceptions");
            expected = (Class[])method.invoke(annotation);
            method = annotation.getClass().getMethod("expectedExceptionsMessageRegExp");
            expectedMsgRegex = (String) method.invoke(annotation);

            // expectedStr = expected.getName();
        }
        catch (Throwable e) {}

        if (expectedMsgRegex == null)
            expectedMsgRegex = ".*";

        StringBuilder code = new StringBuilder();
        code.append(getClass().getName() + ".isPass(");
        code.append(collectorVar + ", ");
        code.append(p.getId() + ", ");
        code.append(exceptionVar + ", ");
        
        if(expected.length == 0) {
        	code.append("null, ");
        }
        else {
        	code.append("new String[]{");
        	boolean first = true;

        	for (Class cls : expected) {
            	if (!first)
                	code.append(", ");

            	code.append("\"" + cls.getName() + "\"");
            	first = false;
        	}

        	code.append("}, ");
        }
        code.append("\"" + expectedMsgRegex + "\")");
        return "if(" + code.toString() + ") throw " + exceptionVar + ";";
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
                                        String[] expected,
                                        String expectedMsgRegex) {
        if(expected != null)
        	for (String cls : expected)
        		if (isSameType(e, cls))
        			if (Pattern.matches(expectedMsgRegex, e.getMessage()))
        				return true;
        
        return false;
    }
}