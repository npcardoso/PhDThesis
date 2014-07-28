package io.crowbar.instrumentation.passes.matchers;

import javassist.CtClass;
import javassist.CtMethod;

public interface ActionTaker {
    public static enum Action {
        ACCEPT, NEXT, REJECT
    }

    public Action getAction (CtClass c);
    public Action getAction (CtMethod c);
}