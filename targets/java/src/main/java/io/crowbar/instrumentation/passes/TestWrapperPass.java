package io.crowbar.instrumentation.passes;


import io.crowbar.instrumentation.passes.matchers.ActionTaker;
import io.crowbar.instrumentation.runtime.Node;
import io.crowbar.instrumentation.runtime.ProbeGroup.HitProbe;
import io.crowbar.instrumentation.runtime.ProbeType;
import io.crowbar.instrumentation.runtime.Tree.RegistrationException;

import javassist.ClassPool;
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
                ActionTaker.Action ret = at.getAction(c, m);

                switch (ret) {
                case ACCEPT:
                    Node n = getNode(c, m);
                    m.insertBefore(getTransactionCode(c, n, ProbeType.TRANSACTION_START, false));
                    m.insertAfter(getTransactionCode(c, n, ProbeType.TRANSACTION_END, true),
                                  false /* asFinally */);
                    m.addCatch(getOracleCode(c, n, "e"),
                               ClassPool.getDefault().get("java.lang.Throwable"),
                               "e");
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

    private String getOracleCode (CtClass c,
                                  Node n,
                                  String exVariable) throws RegistrationException {
        HitProbe p = registerProbe(c, n, ProbeType.ORACLE);
        String ret = "{{Collector c = Collector.instance();";


        ret += "c.hit(" + p.getId() + ");";
        ret += "c." + ProbeType.ORACLE.getMethodName() + "(" + p.getId() + ", 1d, 1d);}";
        ret += "{" + getTransactionCode(c, n, ProbeType.TRANSACTION_END, true) + "}";
        ret += "throw " + exVariable + ";}";
        return ret;
    }

    private String getTransactionCode (CtClass c,
                                       Node n,
                                       ProbeType type,
                                       boolean hitFirst) throws RegistrationException {
        HitProbe p = registerProbe(c, n, type);
        String ret = "Collector c = Collector.instance();";
        String hit = "c.hit(" + p.getId() + ");";


        if (hitFirst)
            ret += hit;

        ret += "c." + type.getMethodName() + "(" + p.getId() + ");";

        if (!hitFirst)
            ret += hit;

        return ret;
    }
}