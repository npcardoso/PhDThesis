package io.crowbar.instrumentation.passes.wrappers;

import io.crowbar.instrumentation.passes.matchers.ActionTaker;

import java.util.Set;

import javassist.CtClass;
import javassist.CtMethod;


public class ActionTakerToTestWrapper implements TestWrapper {
    private final ActionTaker actionTaker;

    public ActionTakerToTestWrapper (ActionTaker actionTaker) {
        this.actionTaker = actionTaker;
    }

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