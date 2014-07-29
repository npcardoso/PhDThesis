package io.crowbar.instrumentation.passes;


import io.crowbar.instrumentation.passes.matchers.ActionTaker;
import io.crowbar.instrumentation.runtime.HitVector.ProbeGroup.Probe;
import io.crowbar.instrumentation.runtime.ProbeType;
import io.crowbar.instrumentation.runtime.Tree.RegistrationException;
import io.crowbar.instrumentation.runtime.Tree.Node;

import javassist.CtClass;
import javassist.CtMethod;

import java.util.Arrays;
import java.util.List;
import java.util.LinkedList;

public final class TestWrapperPass extends AbstractPass {
    private final List<ActionTaker> actionTakers = new LinkedList<ActionTaker> ();


    public TestWrapperPass (ActionTaker... actionTakers) {
        this.actionTakers.addAll(Arrays.asList(actionTakers));
    }

    public TestWrapperPass (List<ActionTaker> actionTakers) {
        this.actionTakers.addAll(actionTakers);
    }

    @Override
    public Outcome transform (CtClass c) throws Exception {
        for (CtMethod m : c.getDeclaredMethods()) {
            for (ActionTaker at : actionTakers) {
                ActionTaker.Action ret = at.getAction(c,m);

                switch (ret) {
                case ACCEPT:
                    Node n = getNode(c, m);
                    m.insertBefore(getInstrumentationCode(c, n, ProbeType.TRANSACTION_START, false));
                    m.insertAfter(getInstrumentationCode(c, n, ProbeType.TRANSACTION_END, true),
                                  true /* asFinally */);
                    break;

                case REJECT:
                    return Outcome.CONTINUE;

                case NEXT:
                default:
                    continue;
                }
            }
        }

        return Outcome.CONTINUE;
    }

    private String getInstrumentationCode (CtClass c,
                                           Node n,
                                           ProbeType type,
                                           boolean hitFirst) throws RegistrationException {
        Probe p = registerProbe(c, n, type);
        String ret = "Collector c = Collector.getDefault();";
        String hit = "c.getHitVector().hit(" + p.getGlobalId() + ");";


        if (hitFirst)
            ret += hit;

        ret += "c." + type.getMethodName() + "(" + p.getGlobalId() + ");";

        if (!hitFirst)
            ret += hit;

        return ret;
    }
}