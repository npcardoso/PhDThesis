package io.crowbar.instrumentation.passes.wrappers;

import io.crowbar.instrumentation.passes.matchers.ActionTaker;
import io.crowbar.instrumentation.runtime.Node;
import io.crowbar.instrumentation.runtime.Probe;

import java.util.Set;

import javassist.CtClass;
import javassist.CtMethod;


public class ActionTakerToTestWrapper implements TestWrapper {
    private final ActionTaker actionTaker;

    public ActionTakerToTestWrapper (ActionTaker actionTaker) {
        this.actionTaker = actionTaker;
    }

    @Override
    public final Action getAction (CtClass c) {
        return actionTaker.getAction(c);
    }

    @Override
    public final Action getAction (CtClass c,
                                   CtMethod m) {
        return actionTaker.getAction(c, m);
    }

    @Override
    public final String getOracleCode (CtClass c,
                                       CtMethod m,
                                       Node n,
                                       Probe p,
                                       String collectorVar,
                                       String exceptionVar) {
        return null;
    }
}