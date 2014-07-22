package io.crowbar.instrumentation.passes;


import io.crowbar.instrumentation.runtime.*;
import io.crowbar.instrumentation.passes.wrappers.*;
import io.crowbar.sandbox.Tree;
import io.crowbar.sandbox.Tree.Node;

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
                    m.insertBefore(getInstrumentationCode(c, n, ProbeType.TRANSACTION_START));
                    m.insertAfter(getInstrumentationCode(c, n, ProbeType.TRANSACTION_END),
                                  true /* asFinally */);
                    break;
                }
            }
        }
    }

    protected String getInstrumentationCode (CtClass c,
                                             Node n,
                                             ProbeType type) {
        int id = registerProbe(c, n, type);


        return "Collector.getDefault()." + type.method_name + "(" + id + ");";
    }

    public List<Wrapper> wrappers = new LinkedList<Wrapper> ();
}