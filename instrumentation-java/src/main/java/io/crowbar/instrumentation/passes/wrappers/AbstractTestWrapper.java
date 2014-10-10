package io.crowbar.instrumentation.passes.wrappers;

import io.crowbar.diagnostic.spectrum.Node;
import io.crowbar.instrumentation.passes.matchers.ActionTaker;

import javassist.CtClass;
import javassist.CtMethod;

/**
 * @brief This class provides a basic implementation for a TestWrapper
 * The extending class must implement two static methods:
 * - boolean isPass (Class cls, String methodName);
 * - boolean isPass (Class cls, String methodName, Throwable e);
 */

public abstract class AbstractTestWrapper implements TestWrapper {
    protected final static boolean isSameType (Object o,
                                               String type) {
        try {
            Class< ? > cls = Class.forName(type);
            return cls.isAssignableFrom(o.getClass());
        }
        catch (ClassNotFoundException e) {
            return false;
        }
    }

    protected final static boolean isSameType (Object o,
                                               Class cls) {
        if (cls == null || o == null)
            return false;

        System.out.println("Object: " + o + ", Class: " + cls + ", res: " + cls.isAssignableFrom(o.getClass()));
        return cls.isAssignableFrom(o.getClass());
    }

    protected abstract ActionTaker getActionTaker ();

    @Override
    public Action getAction (CtClass c) {
        return getActionTaker().getAction(c);
    }

    @Override
    public Action getAction (CtClass c,
                             CtMethod m) {
        return getActionTaker().getAction(c, m);
    }

    @Override
    public final String getOracleCode (CtClass c,
                                       CtMethod m,
                                       Node n,
                                       int probeId,
                                       String collectorVar,
                                       String exceptionVar) {
        String oracleCall =
            getClass().getName() + ".isPass(" + c.getName() + ".class, \"" +
            m.getName() + "\"," +
            exceptionVar + ")";


        return "if(" + oracleCall + ") throw " + exceptionVar + ";";
    }

    @Override
    public final String getOracleCode (CtClass c,
                                       CtMethod m,
                                       Node n,
                                       int probeId,
                                       String collectorVar) {
        String oracleCall =
            getClass().getName() + ".isPass(" + c.getName() + ".class, \"" +
            m.getName() + "\")";


        return "if(!" + oracleCall + "){" +
               collectorVar + ".hit(" + probeId + ");" +
               collectorVar + ".oracle(" + probeId + ",1.0,1.0);}";
    }
}