package io.crowbar.instrumentation.passes.matchers;

import javassist.CtClass;
import javassist.CtMethod;

public interface ActionTaker {
    public static enum Action {
        ACCEPT, NEXT, REJECT
    }

    Action getAction (CtClass c);
    Action getAction (CtClass c, CtMethod m);
}