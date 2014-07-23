package io.crowbar.instrumentation.passes;


import io.crowbar.instrumentation.passes.wrappers.Wrapper;
import io.crowbar.instrumentation.runtime.HitVector.ProbeGroup.Probe;
import io.crowbar.instrumentation.runtime.ProbeType;
import io.crowbar.instrumentation.runtime.Tree;
import io.crowbar.instrumentation.runtime.Tree.RegistrationException;
import io.crowbar.instrumentation.runtime.Tree.Node;

import javassist.*;
import javassist.bytecode.*;

import java.util.*;

public class TestWrapperPass extends Pass {
    @Override
    public void transform (CtClass c) throws Exception {
        for (CtMethod m : c.getDeclaredMethods()) {
            for (Wrapper w : wrappers) {
                if (w.matches(m)) {
                    Node n = getNode(c, m);
                    m.insertBefore(getInstrumentationCode(c, n, ProbeType.TRANSACTION_START, false));
                    m.insertAfter(getInstrumentationCode(c, n, ProbeType.TRANSACTION_END, true),
                                  true /* asFinally */);
                    break;
                }
            }
        }
    }

    protected String getInstrumentationCode (CtClass c,
                                             Node n,
                                             ProbeType type,
                                             boolean hit_first) throws RegistrationException {
        Probe p = registerProbe(c, n, type);
        String ret = "Collector c = Collector.getDefault();";
        String hit = "c.getHitVector().hit(" + p.getGlobalId() + ");";


        if (hit_first)
            ret += hit;

        ret += "c." + type.method_name + "(" + p.getGlobalId() + ");";

        if (!hit_first)
            ret += hit;

        return ret;
    }

    public List<Wrapper> wrappers = new LinkedList<Wrapper> ();
}